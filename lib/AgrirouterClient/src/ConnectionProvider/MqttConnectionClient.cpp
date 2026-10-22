#include "MqttConnectionClient.h"

#include <algorithm>
#include <chrono>
#include <cstring>
#include <pthread.h>
#include <thread>

std::map<const struct mosquitto *, std::string> MqttConnectionClient::s_secrets;
std::mutex MqttConnectionClient::s_secretsMutex;
std::once_flag MqttConnectionClient::s_libInitFlag;

namespace
{
    // Time to wait for the DISCONNECT to be sent before the mosquitto thread is stopped
    const int DISCONNECT_TIMEOUT_MS = 1000;
    const int DISCONNECT_POLL_INTERVAL_MS = 10;

    // Delay before the ended mosquitto thread is restarted, doubled until it connects again
    const int LOOP_RESTART_DELAY_MIN_S = 2;
    const int LOOP_RESTART_DELAY_MAX_S = 120;

    // Number of calls into libmosquitto on this thread from outside of its own thread
    thread_local int t_applicationCallDepth = 0;

    // Set around calls into libmosquitto from the application thread.
    // libmosquitto 1.x also calls callbacks (e.g. the log callback) from the calling thread, these threads must
    // not be watched as mosquitto thread.
    class ApplicationCallScope
    {
    public:
        ApplicationCallScope()
        {
            ++t_applicationCallDepth;
        }

        ~ApplicationCallScope()
        {
            --t_applicationCallDepth;
        }

        ApplicationCallScope(const ApplicationCallScope &) = delete;
        ApplicationCallScope &operator=(const ApplicationCallScope &) = delete;
    };

    // Set at the start of every mosquitto callback.
    // The destructor stops the mosquitto thread with pthread_cancel, because libmosquitto 1.x sleeps up to the maximum
    // reconnect delay without being interruptible. Cancelling inside a callback could leave mutexes of the application
    // or of this library locked, so cancellation is deferred until the callback has returned.
    class CallbackScope {
    public:
        CallbackScope()
        {
            pthread_setcancelstate(PTHREAD_CANCEL_DISABLE, &m_oldCancelState);
        }

        ~CallbackScope()
        {
            pthread_setcancelstate(m_oldCancelState, nullptr);
        }

        CallbackScope(const CallbackScope &) = delete;
        CallbackScope &operator=(const CallbackScope &) = delete;

    private:
        int m_oldCancelState = PTHREAD_CANCEL_ENABLE;
    };
} // namespace

// The thread_local watcher is destroyed when the mosquitto thread ends
struct MqttConnectionClient::LoopThreadWatcher
{
    MqttConnectionClient *client = nullptr;

    ~LoopThreadWatcher()
    {
        if (client != nullptr)
        {
            client->onLoopThreadEnded();
        }
    }
};

thread_local MqttConnectionClient::LoopThreadWatcher MqttConnectionClient::s_loopThreadWatcher;

MqttConnectionClient::MqttConnectionClient(const std::string& clientId, const std::string& host, int port, Settings *settings)
{
    m_clientId = clientId;
    m_port = port;
    m_host = host;
    m_settings = settings;
    m_secret = m_settings->getConnectionParameters().secret;
}

MqttConnectionClient::~MqttConnectionClient()
{
    // Stop the supervisor first, it must not restart the mosquitto thread while it is stopped
    {
        std::lock_guard<std::mutex> lock(m_supervisorMutex);
        m_stopping = true;
    }

    m_supervisorCondition.notify_all();
    if (m_supervisor.joinable())
    {
        m_supervisor.join();
    }

    if(m_mosq != nullptr)
    {
        ApplicationCallScope applicationCallScope;
        if (mosquitto_disconnect(m_mosq) == MOSQ_ERR_SUCCESS)
        {
            // The mosquitto thread sends the DISCONNECT, give it the chance to do so before it is cancelled
            for (int waited = 0; m_connected && (waited < DISCONNECT_TIMEOUT_MS); waited += DISCONNECT_POLL_INTERVAL_MS)
            {
                std::this_thread::sleep_for(std::chrono::milliseconds(DISCONNECT_POLL_INTERVAL_MS));
            }
        }

        // Cancelling is safe, it is deferred while a callback is running (see CallbackScope)
        mosquitto_loop_stop(m_mosq, true);

        {
            std::lock_guard<std::mutex> lock(s_secretsMutex);
            s_secrets.erase(m_mosq);
        }

        mosquitto_destroy(m_mosq);
        m_mosq = nullptr;
    }
}

int MqttConnectionClient::init()
{
    // The bundled header is used for the API, the library is linked by the parent project
    int major = 0;
    int minor = 0;
    int revision = 0;
    mosquitto_lib_version(&major, &minor, &revision);
    std::string linkedVersion = std::to_string(major) + "." + std::to_string(minor) + "." + std::to_string(revision);
    if (major != LIBMOSQUITTO_MAJOR)
    {
        m_settings->callOnLog(MG_LFL_ERR, "MqttConnectionClient: libmosquitto " + linkedVersion + " is linked, but major version " + std::to_string(LIBMOSQUITTO_MAJOR) + " is required");
        return EXIT_FAILURE;
    }

    m_settings->callOnLog(MG_LFL_NTC, "MqttConnectionClient: using libmosquitto " + linkedVersion);

    std::call_once(s_libInitFlag, mosquitto_lib_init);

    m_mosq = mosquitto_new(m_clientId.c_str(), false, (void *) this);

    if (m_mosq == nullptr)
    {
        m_settings->callOnLog(MG_LFL_ERR, "MqttConnectionClient: not connect to broker");
        return EXIT_FAILURE;
    }

    {
        std::lock_guard<std::mutex> lock(s_secretsMutex);
        s_secrets[m_mosq] = m_secret;
    }

    mosquitto_connect_callback_set(m_mosq, connectCallback);
    mosquitto_disconnect_callback_set(m_mosq, disconnectCallback);
    mosquitto_publish_callback_set(m_mosq, publishCallback);
    mosquitto_log_callback_set(m_mosq, loggingCallback);
    mosquitto_subscribe_callback_set(m_mosq, subscribeCallback);
    mosquitto_unsubscribe_callback_set(m_mosq, unsubscribeCallback);
    mosquitto_message_callback_set(m_mosq, messageCallback);
    mosquitto_reconnect_delay_set(m_mosq, 2, 120, true);

    int tlsInsecure = mosquitto_tls_insecure_set(m_mosq, false);
    if(tlsInsecure == MOSQ_ERR_SUCCESS)
    {
        m_settings->callOnLog(MG_LFL_NTC, "MqttConnectionClient: tlsInsecure set successful - " + std::to_string(tlsInsecure) + ": " + mosquitto_strerror(tlsInsecure));
    }
    else
    {
        std::string errorMessage = "MqttConnectionClient: tlsInsecure set failed " + std::to_string(tlsInsecure) + ": " + mosquitto_strerror(tlsInsecure);
        m_settings->callOnLog(MG_LFL_ERR, errorMessage);
        (m_mqttErrorCallback) (tlsInsecure, errorMessage, "", m_member);
        return EXIT_FAILURE;
    }

    int tlsOptsSet = mosquitto_tls_opts_set(m_mosq, 1, NULL, NULL);
    if(tlsOptsSet == MOSQ_ERR_SUCCESS)
    {
        m_settings->callOnLog(MG_LFL_NTC, "MqttConnectionClient: tlsOptsSet set successful - " + std::to_string(tlsOptsSet) + ": " + mosquitto_strerror(tlsOptsSet));
    }
    else
    {
        std::string errorMessage = "MqttConnectionClient: tlsOptsSet set failed " + std::to_string(tlsOptsSet) + ": " + mosquitto_strerror(tlsOptsSet);
        m_settings->callOnLog(MG_LFL_ERR, errorMessage);
        (m_mqttErrorCallback) (tlsOptsSet, errorMessage, "", m_member);
        return EXIT_FAILURE;
    }

    std::string caCerts = m_settings->getCertificateCaPath();
    std::string client_certificate = m_settings->getCertificatePath();
    std::string client_key = m_settings->getPrivateKeyPath();

    int tlsSet = mosquitto_tls_set(m_mosq, nullptr, caCerts.c_str(), client_certificate.c_str(), client_key.c_str(), onPWCallback);
    if(tlsSet == MOSQ_ERR_SUCCESS)
    {
        m_settings->callOnLog(MG_LFL_NTC, "MqttConnectionClient: tlsSet set successful - " + std::to_string(tlsSet) + ": " + mosquitto_strerror(tlsSet));
    }
    else
    {
        std::string errorMessage = "MqttConnectionClient: tlsSet set failed " + std::to_string(tlsSet) + ": " + mosquitto_strerror(tlsSet);
        m_settings->callOnLog(MG_LFL_ERR, errorMessage);
        std::string errorJSON = "{\"error\":{\"code\":\""+ std::to_string(MG_ERROR_MISSING_OR_EXPIRED_CERTIFICATE) + "\",\"message\":\"" + errorMessage + "\",\"target\":\"agrirouter-api-cpp\",\"details\":[]}}";
        (m_mqttErrorCallback) (MG_ERROR_MISSING_OR_EXPIRED_CERTIFICATE, "MqttConnectionClient: MQTT TLS Failed", errorJSON, m_member);
        return EXIT_FAILURE;
    }

    int keepAliveTime = m_settings->getMqttKeepAliveTime();
    if(keepAliveTime == 0)
    {
        keepAliveTime = DEFAULT_KEEP_ALIVE_TIME;
    }

    // Connect before the mosquitto thread is started. The thread reconnects by itself, if it runs already, both threads
    // connect at the same time on the same mosquitto instance (two connections, crashes in OpenSSL).
    int connect = MOSQ_ERR_SUCCESS;
    {
        ApplicationCallScope applicationCallScope;
        connect = mosquitto_connect_async(m_mosq, m_host.c_str(), m_port, keepAliveTime);
    }

    if(connect == MOSQ_ERR_SUCCESS)
    {
        m_settings->callOnLog(MG_LFL_NTC, "MqttConnectionClient: connect set successful - " + std::to_string(connect) + ": " + mosquitto_strerror(connect));
    }
    else
    {
        std::string errorMessage = "MqttConnectionClient: connect set failed " + std::to_string(connect) + ": " + mosquitto_strerror(connect);
        m_settings->callOnLog(MG_LFL_ERR, errorMessage);
        (m_mqttErrorCallback) (connect, errorMessage, "", m_member);
        // not return fail, the mosquitto_loop trigger the connect until it is connected
    }

    int loop = mosquitto_loop_start(m_mosq);
    if(loop == MOSQ_ERR_SUCCESS)
    {
        m_settings->callOnLog(MG_LFL_NTC, "MqttConnectionClient: loop set successful - " + std::to_string(loop) + ": " + mosquitto_strerror(loop));
    }
    else
    {
        std::string errorMessage = "MqttConnectionClient: loop start failed " + std::to_string(loop) + ": " + mosquitto_strerror(loop);
        m_settings->callOnLog(MG_LFL_ERR, errorMessage);
        (m_mqttErrorCallback) (loop, errorMessage, "", m_member);
        return EXIT_FAILURE;
    }

    m_supervisor = std::thread(&MqttConnectionClient::superviseLoopThread, this);

    m_settings->callOnLog(MG_LFL_NTC, "MqttConnectionClient: init success");
    return EXIT_SUCCESS;
}

void MqttConnectionClient::setMqttCallback(MqttConnectionClient::MqttCallback callback)
{
    m_mqttCallback = callback;
}

MqttConnectionClient::MqttCallback MqttConnectionClient::getMqttCallback()
{
    return m_mqttCallback;
}

void MqttConnectionClient::setMqttErrorCallback(MqttConnectionClient::MqttErrorCallback errorCallback)
{
    m_mqttErrorCallback = errorCallback;
}

MqttConnectionClient::MqttErrorCallback MqttConnectionClient::getMqttErrorCallback()
{
    return m_mqttErrorCallback;
}

void MqttConnectionClient::setMember(void* member) { m_member = member; }

void* MqttConnectionClient::getMember() { return m_member; }

int MqttConnectionClient::onPWCallback(char *buf, int size, int rwflag, void *userdata)
{
    // libmosquitto passes its instance as userdata of the OpenSSL password callback
    std::string secret;
    {
        std::lock_guard<std::mutex> lock(s_secretsMutex);
        std::map<const struct mosquitto *, std::string>::const_iterator it = s_secrets.find(static_cast<const struct mosquitto *>(userdata));
        if (it != s_secrets.end())
        {
            secret = it->second;
        }
    }

    strncpy(buf, secret.c_str(), size);
    buf[size-1] = '\0';
    return strlen(buf);
}

void MqttConnectionClient::connectCallback(struct mosquitto *mosq, void *obj, int reasonCode)
{
    MqttConnectionClient *self = static_cast<MqttConnectionClient *>(obj);
    CallbackScope scope;
    self->watchLoopThread();
    self->m_settings->callOnLog(MG_LFL_NTC, "MqttConnectionClient: connect callback with result: " + std::to_string(reasonCode));

    self->m_connected = (reasonCode == 0);
    if(reasonCode == 0)
    {
        self->m_connectedSinceRestart = true;
        self->m_settings->callOnLog(MG_LFL_MSG, "MqttConnectionClient: Connected to MQTT Broker (" + self->m_host + ":" + std::to_string(self->m_port) + ")");

        // Subscribe on every connect, a subscription before the connection is established would be lost
        if (!self->m_subscriptionTopic.empty())
        {
            self->subscribe(self->m_subscriptionTopic, self->m_subscriptionQos);
        }
    }
    else
    {
        std::string errorMessage = "MqttConnectionClient: Failed Connected to MQTT Broker " + std::to_string(reasonCode) + ": " + mosquitto_connack_string(reasonCode);
        self->m_settings->callOnLog(MG_LFL_ERR, errorMessage);
        (self->m_mqttErrorCallback) (reasonCode, errorMessage, "", self->m_member);
    }
}

void MqttConnectionClient::disconnectCallback(struct mosquitto *mosq, void *obj, int reasonCode)
{
    MqttConnectionClient *self = static_cast<MqttConnectionClient *>(obj);
    CallbackScope scope;
    self->watchLoopThread();
    self->m_settings->callOnLog(MG_LFL_NTC, "MqttConnectionClient: disconnect callback with result: '" + std::to_string(reasonCode) + ":" + mosquitto_strerror(reasonCode) + "'");
    self->m_connected = false;

    // reasonCode 0 disconnect is called by client, so no reconnect on destruct mqtt client
    if(reasonCode > 0)
    {
        std::string errorMessage = "MqttConnectionClient: disconnect unexpected " + std::to_string(reasonCode) + ": " + mosquitto_strerror(reasonCode);
        self->m_settings->callOnLog(MG_LFL_ERR, errorMessage);
        (self->m_mqttErrorCallback) (reasonCode, errorMessage, "", self->m_member);
    }
}

void MqttConnectionClient::publishCallback(struct mosquitto *mosq, void *obj, int messageId)
{
    MqttConnectionClient *self = static_cast<MqttConnectionClient *>(obj);
    CallbackScope scope;
    self->watchLoopThread();
    self->m_settings->callOnLog(MG_LFL_NTC, "MqttConnectionClient: [MsgId: " + std::to_string(messageId) + "] publish OK");
}

void MqttConnectionClient::loggingCallback(struct mosquitto *mosq, void *obj, int level, const char *message)
{
    MqttConnectionClient *self = static_cast<MqttConnectionClient *>(obj);
    CallbackScope scope;
    self->watchLoopThread();
    self->m_settings->callOnLog(MG_LFL_NTC, "MqttConnectionClient: logging callback with message: '" + std::string(message) + "' and Log level: '" + std::to_string(level) + "'");

    if(std::string(message).find("tls_process_server_certificate:certificate verify failed") != std::string::npos)
    {
        self->m_settings->callOnLog(MG_LFL_ERR, message);
        std::string errorJSON = "{\"error\":{\"code\":\""+ std::to_string(MG_ERROR_MISSING_OR_EXPIRED_CA_CERTIFICATE) + "\",\"message\":\"" + message + "\",\"target\":\"agrirouter-api-cpp\",\"details\":[]}}";
        (self->m_mqttErrorCallback) (MG_ERROR_MISSING_OR_EXPIRED_CA_CERTIFICATE, "MqttConnectionClient: MQTT TLS verify server certificate Failed", errorJSON, self->m_member);
    }
}

void MqttConnectionClient::subscribeCallback(struct mosquitto *mosq, void *obj, int messageId, int qosCount, const int *grantedQos)
{
    MqttConnectionClient *self = static_cast<MqttConnectionClient *>(obj);
    CallbackScope scope;
    self->watchLoopThread();
    self->m_settings->callOnLog(MG_LFL_NTC, "MqttConnectionClient: [MsgId: " + std::to_string(messageId) + "] subscribe callback");

    // 0x80 (128) is the only failure code of SUBACK, a lower QoS than requested is a valid grant
    const int subscribeFailure = 0x80;
    for (int i = 0; i < qosCount; i++)
    {
        if (grantedQos[i] == subscribeFailure)
        {
            std::string errorMessage = "MqttConnectionClient: subscribe callback count " + std::to_string(i) + " failed with " + std::to_string(grantedQos[i]);
            self->m_settings->callOnLog(MG_LFL_ERR, errorMessage);
            (self->m_mqttErrorCallback) (grantedQos[i], errorMessage, "", self->m_member);
        }
        else if (grantedQos[i] < self->m_subscriptionQos)
        {
            self->m_settings->callOnLog(MG_LFL_WRN, "MqttConnectionClient: subscribe callback count " + std::to_string(i) + " granted qos " + std::to_string(grantedQos[i]) + " instead of " + std::to_string(self->m_subscriptionQos));
        }
        else
        {
            self->m_settings->callOnLog(MG_LFL_NTC, "MqttConnectionClient: subscribe callback count " + std::to_string(i) + " OK");
        }
    }
}

void MqttConnectionClient::unsubscribeCallback(struct mosquitto *mosq, void *obj, int messageId)
{
    MqttConnectionClient *self = static_cast<MqttConnectionClient *>(obj);
    CallbackScope scope;
    self->watchLoopThread();
    self->m_settings->callOnLog(MG_LFL_NTC, "MqttConnectionClient: unsubscribe callback");
}

void MqttConnectionClient::messageCallback(struct mosquitto *mosq, void *obj, const struct mosquitto_message *message)
{
    MqttConnectionClient *self = static_cast<MqttConnectionClient *>(obj);
    CallbackScope scope;
    self->watchLoopThread();
    self->m_settings->callOnLog(MG_LFL_NTC, "MqttConnectionClient: [MsgId: " + std::to_string(message->mid) + "] messageCallback on topic " + message->topic + " with qos " + std::to_string(message->qos));

    (self->m_mqttCallback)(message->topic, message->payload, message->payloadlen, self->m_member);
}

void MqttConnectionClient::subscribe(const std::string& topic, int qos)
{
    int messageId = 0;
    int result = MOSQ_ERR_SUCCESS;
    {
        ApplicationCallScope applicationCallScope;
        result = mosquitto_subscribe(m_mosq, &messageId, topic.c_str(), qos);
    }

    m_settings->callOnLog(MG_LFL_NTC, "MqttConnectionClient: [MsgId: " + std::to_string(messageId) + "] subscribing on topic " + topic + " with qos " + std::to_string(qos) + ": " + mosquitto_strerror(result));
}

void MqttConnectionClient::setSubscription(const std::string& topic, int qos)
{
    m_subscriptionTopic = topic;
    m_subscriptionQos = qos;
}

int MqttConnectionClient::publish(const std::string& topic, const std::string& payload, int qos)
{
    return publish(topic, const_cast<char *>(payload.data()), static_cast<int>(payload.size()), qos, false);
}

int MqttConnectionClient::publish(const std::string& topic, char *payload, int size, int qos)
{
    return publish(topic, payload, size, qos, false);
}

int MqttConnectionClient::publish(const std::string& topic, char *payload, int size, int qos, bool retain)
{
    // mosquitto_publish copies the payload
    int messageId = 0;
    int result = MOSQ_ERR_SUCCESS;
    {
        ApplicationCallScope applicationCallScope;
        result = mosquitto_publish(m_mosq, &messageId, topic.c_str(), size, payload, qos, retain);
    }

    m_settings->callOnLog(MG_LFL_NTC, "MqttConnectionClient: [MsgId: " + std::to_string(messageId) + "] publishing on topic " + topic + " with qos " + std::to_string(qos) + ", size " + std::to_string(size) + " and retain " + std::to_string(retain) + ": " + mosquitto_strerror(result));
    return result;
}

bool MqttConnectionClient::isConnected()
{
    return m_connected;
}

void MqttConnectionClient::watchLoopThread()
{
    // Callbacks outside of an ApplicationCallScope run on the mosquitto thread
    if (t_applicationCallDepth == 0)
    {
        s_loopThreadWatcher.client = this;
    }
}

void MqttConnectionClient::onLoopThreadEnded()
{
    {
        std::lock_guard<std::mutex> lock(m_supervisorMutex);
        m_loopThreadEnded = true;
    }

    m_supervisorCondition.notify_all();
}

void MqttConnectionClient::superviseLoopThread()
{
    int restartDelay = LOOP_RESTART_DELAY_MIN_S;
    std::unique_lock<std::mutex> lock(m_supervisorMutex);
    while (!m_stopping)
    {
        m_supervisorCondition.wait(lock, [this] { return m_stopping || m_loopThreadEnded; });
        if (m_stopping)
        {
            break;
        }

        if (m_connectedSinceRestart.exchange(false))
        {
            restartDelay = LOOP_RESTART_DELAY_MIN_S;
        }

        // Do not call the application with the lock held
        lock.unlock();
        m_settings->callOnLog(MG_LFL_WRN, "MqttConnectionClient: mosquitto thread ended, restart in " + std::to_string(restartDelay) + " s");
        lock.lock();

        if (m_supervisorCondition.wait_for(lock, std::chrono::seconds(restartDelay), [this] { return m_stopping; }))
        {
            break;
        }

        m_loopThreadEnded = false;
        lock.unlock();

        // The thread has ended, so loop_stop returns immediately. It resets the thread state of libmosquitto,
        // the new thread reconnects by itself.
        mosquitto_loop_stop(m_mosq, false);
        int loop = mosquitto_loop_start(m_mosq);
        m_settings->callOnLog((loop == MOSQ_ERR_SUCCESS) ? MG_LFL_MSG : MG_LFL_ERR, "MqttConnectionClient: mosquitto thread restarted: " + std::to_string(loop) + ": " + mosquitto_strerror(loop));

        lock.lock();
        restartDelay = std::min(restartDelay * 2, LOOP_RESTART_DELAY_MAX_S);
    }
}
