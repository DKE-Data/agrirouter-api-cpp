#include "MqttConnectionClient.h"

#include "third_party/mosquitto/mosquitto_internal.h"

MqttConnectionClient::MqttConnectionClient(std::string &clientId, std::string &host, int port, Settings *settings)
{
    this->m_clientId = clientId;
    this->m_port = port;
    this->m_mosq = NULL;
    this->m_host = host;
    this->m_messageId = 1;
    this->m_connected = false;
    this->m_settings = settings;
}

MqttConnectionClient::~MqttConnectionClient()
{
    if(this->m_mosq != NULL)
    {
        mosquitto_disconnect(this->m_mosq);
        mosquitto_destroy(this->m_mosq);
    }
    mosquitto_lib_cleanup();
}

int MqttConnectionClient::init()
{
    mosquitto_lib_init();
    this->m_mosq = mosquitto_new(this->m_clientId.c_str(), false, (void *) this);

    if (this->m_mosq != NULL)
    {
        // set this to mosq for the pw_callback 
        // the function mosquitto_user_data_set(..) not working for this callback
        // the direct set to struct, not the best solution but it works
        this->m_mosq->userdata = this;

        mosquitto_connect_callback_set(this->m_mosq, connectCallback);
        mosquitto_disconnect_callback_set(this->m_mosq, disconnectCallback);
        mosquitto_publish_callback_set(this->m_mosq, publishCallback);
        mosquitto_log_callback_set(this->m_mosq, loggingCallback);
        mosquitto_subscribe_callback_set(this->m_mosq, subscribeCallback);
        mosquitto_unsubscribe_callback_set(this->m_mosq, unsubscribeCallback);
        mosquitto_message_callback_set(this->m_mosq, messageCallback);

        int tlsInsecure = mosquitto_tls_insecure_set(this->m_mosq, false);
        if(tlsInsecure == MOSQ_ERR_SUCCESS)
        {
            this->m_settings->callOnLog(MG_LFL_NTC, "MqttConnectionClient: tlsInsecure set successful - " + std::to_string(tlsInsecure) + ": " + mosquitto_strerror(tlsInsecure));
        }
        else
        {
            std::string errorMessage = "MqttConnectionClient: tlsInsecure set failed " + std::to_string(tlsInsecure) + ": " + mosquitto_strerror(tlsInsecure);
            this->m_settings->callOnLog(MG_LFL_ERR, errorMessage);
            (this->m_mqttErrorCallback) (tlsInsecure, errorMessage, "", this->m_member);
            return EXIT_FAILURE;
        }

        int tlsOptsSet = mosquitto_tls_opts_set(this->m_mosq, 1, NULL, NULL);
        if(tlsOptsSet == MOSQ_ERR_SUCCESS)
        {
            this->m_settings->callOnLog(MG_LFL_NTC, "MqttConnectionClient: tlsOptsSet set successful - " + std::to_string(tlsOptsSet) + ": " + mosquitto_strerror(tlsOptsSet));
        }
        else
        {
            std::string errorMessage = "MqttConnectionClient: tlsOptsSet set failed " + std::to_string(tlsOptsSet) + ": " + mosquitto_strerror(tlsOptsSet);
            this->m_settings->callOnLog(MG_LFL_ERR, errorMessage);
            (this->m_mqttErrorCallback) (tlsOptsSet, errorMessage, "", this->m_member);
            return EXIT_FAILURE;
        }

        std::string caCerts = this->m_settings->getCertificateCaPath();
        std::string client_certificate = this->m_settings->getCertificatePath();
        std::string client_key = this->m_settings->getPrivateKeyPath();

        int tlsSet = mosquitto_tls_set(this->m_mosq, nullptr, caCerts.c_str(), client_certificate.c_str(), client_key.c_str(), onPWCallback);
        if(tlsSet == MOSQ_ERR_SUCCESS)
        {
            this->m_settings->callOnLog(MG_LFL_NTC, "MqttConnectionClient: tlsSet set successful - " + std::to_string(tlsSet) + ": " + mosquitto_strerror(tlsSet));
        }
        else
        {
            std::string errorMessage = "MqttConnectionClient: tlsSet set failed " + std::to_string(tlsSet) + ": " + mosquitto_strerror(tlsSet);
            this->m_settings->callOnLog(MG_LFL_ERR, errorMessage);
            std::string errorJSON = "{\"error\":{\"code\":\""+ std::to_string(MG_ERROR_MISSING_OR_EXPIRED_CERTIFICATE) + "\",\"message\":\"" + errorMessage + "\",\"target\":\"agrirouter-api-cpp\",\"details\":[]}}";
            (this->m_mqttErrorCallback) (MG_ERROR_MISSING_OR_EXPIRED_CERTIFICATE, "MqttConnectionClient: MQTT TLS Failed", errorJSON, this->m_member);
            return EXIT_FAILURE;
        }

        int connect = mosquitto_connect_async(this->m_mosq, this->m_host.c_str(), this->m_port, 20);
        if(connect == MOSQ_ERR_SUCCESS)
        {
            this->m_settings->callOnLog(MG_LFL_NTC, "MqttConnectionClient: connect set successful - " + std::to_string(connect) + ": " + mosquitto_strerror(connect));
        }
        else
        {
            std::string errorMessage = "MqttConnectionClient: connect set failed " + std::to_string(connect) + ": " + mosquitto_strerror(connect);
            this->m_settings->callOnLog(MG_LFL_ERR, errorMessage);
            (this->m_mqttErrorCallback) (connect, errorMessage, "", this->m_member);
            return EXIT_FAILURE;
        }

        int loop = mosquitto_loop_start(this->m_mosq);
        if(loop == MOSQ_ERR_SUCCESS)
        {
            this->m_settings->callOnLog(MG_LFL_NTC, "MqttConnectionClient: loop set successful - " + std::to_string(loop) + ": " + mosquitto_strerror(loop));
        }
        else
        {
            std::string errorMessage = "MqttConnectionClient: loop start failed " + std::to_string(loop) + ": " + mosquitto_strerror(loop);
            this->m_settings->callOnLog(MG_LFL_ERR, errorMessage);
            (this->m_mqttErrorCallback) (loop, errorMessage, "", this->m_member);
            return EXIT_FAILURE;
        }
        
        if (connect == 0 && loop == 0)
        {
            return EXIT_SUCCESS;
        }
    }
    else
    {
        this->m_settings->callOnLog(MG_LFL_ERR, "MqttConnectionClient not connect to broker");
    }
    return EXIT_FAILURE;
}

void MqttConnectionClient::setMqttCallback(MqttConnectionClient::MqttCallback callback)
{
    this->m_mqttCallback = callback;
}

MqttConnectionClient::MqttCallback MqttConnectionClient::getMqttCallback()
{
    return this->m_mqttCallback;
}

void MqttConnectionClient::setMqttErrorCallback(MqttConnectionClient::MqttErrorCallback errorCallback)
{
    this->m_mqttErrorCallback = errorCallback;
}

MqttConnectionClient::MqttErrorCallback MqttConnectionClient::getMqttErrorCallback()
{
    return this->m_mqttErrorCallback;
}

void MqttConnectionClient::setMember(void* member) { this->m_member = member; }

void* MqttConnectionClient::getMember() { return this->m_member; }

int MqttConnectionClient::onPWCallback(char *buf, int size, int rwflag, void *userdata)
{
    struct mosquitto *mosq = static_cast<struct mosquitto *>(userdata);
    MqttConnectionClient *self = static_cast<MqttConnectionClient *>(mosq->userdata);
    std::string secret = self->m_settings->getConnectionParameters().secret.c_str();

    strncpy(buf, secret.c_str(), size);
    buf[size-1] = '\0';
    return strlen(buf);
}

void MqttConnectionClient::connectCallback(struct mosquitto *mosq, void *obj, int reasonCode)
{
    MqttConnectionClient *self = static_cast<MqttConnectionClient *>(obj);
    self->m_settings->callOnLog(MG_LFL_NTC, "MqttConnectionClient: connect callback with result: " + std::to_string(reasonCode));

    self->m_connected = true;
    if(reasonCode == 0)
    {
        self->m_settings->callOnLog(MG_LFL_MSG, "MqttConnectionClient: Connected to MQTT Broker (" + self->m_host + ":" + std::to_string(self->m_port));
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
    self->m_settings->callOnLog(MG_LFL_NTC, "MqttConnectionClient: disconnect callback with result: '" + std::to_string(reasonCode) + ":" + mosquitto_strerror(reasonCode) + "'");
    self->m_connected = false;

    // reasonCode 0 disconnect is called by client, so no reconnect on destruct mqtt client 
    if(reasonCode > 0)
    {
        std::string errorMessage = "MqttConnectionClient: disconnect unexpected " + std::to_string(reasonCode) + ": " + mosquitto_connack_string(reasonCode);
        self->m_settings->callOnLog(MG_LFL_ERR, errorMessage);
        (self->m_mqttErrorCallback) (reasonCode, errorMessage, "", self->m_member);
    

        // try to reconnect
        int reconn = mosquitto_reconnect(self->m_mosq);
        if (reconn == MOSQ_ERR_SUCCESS)
        {
            self->m_settings->callOnLog(MG_LFL_MSG, "MqttConnectionClient: Reconnected");
        }
        else
        {
            std::string errorMessage = "MqttConnectionClient: reconnect failed " + std::to_string(reconn) + ": " + mosquitto_strerror(reconn);
            self->m_settings->callOnLog(MG_LFL_ERR, errorMessage);
            (self->m_mqttErrorCallback) (reconn, errorMessage, "", self->m_member);
        }
    }
}

void MqttConnectionClient::publishCallback(struct mosquitto *mosq, void *obj, int messageId)
{
    MqttConnectionClient *self = static_cast<MqttConnectionClient *>(obj);
    self->m_settings->callOnLog(MG_LFL_NTC, "MqttConnectionClient: [MsgId: " + std::to_string(messageId) + "] publish OK");
}

void MqttConnectionClient::loggingCallback(struct mosquitto *mosq, void *obj, int level, const char *message)
{
    MqttConnectionClient *self = static_cast<MqttConnectionClient *>(obj);
    self->m_settings->callOnLog(MG_LFL_NTC, "MqttConnectionClient: logging callback with message: '" + std::string(message) + 
            "' and Log level: '" + std::to_string(level) + "'");

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
    self->m_settings->callOnLog(MG_LFL_NTC, "MqttConnectionClient: [MsgId: " + std::to_string(messageId) + "] subscribe callback");

    for(int8_t i = 0; i < qosCount; i++)
    {
        if(grantedQos[i] == 2)
        {
            self->m_settings->callOnLog(MG_LFL_NTC, "MqttConnectionClient: subscribe callback count " + std::to_string(i) + " OK");
        }
        else
        {
            std::string errorMessage = "MqttConnectionClient: subscribe callback count " + std::to_string(i) + " failed with " + std::to_string(grantedQos[i]);           
            self->m_settings->callOnLog(MG_LFL_ERR, errorMessage);
            (self->m_mqttErrorCallback) (grantedQos[i], errorMessage, "", self->m_member);
        }
    }
}

void MqttConnectionClient::unsubscribeCallback(struct mosquitto *mosq, void *obj, int messageId)
{
    MqttConnectionClient *self = static_cast<MqttConnectionClient *>(obj);
    self->m_settings->callOnLog(MG_LFL_NTC, "MqttConnectionClient: unsubscribe callback");
}

void MqttConnectionClient::messageCallback(struct mosquitto *mosq, void *obj, const struct mosquitto_message *message)
{
    MqttConnectionClient *self = static_cast<MqttConnectionClient *>(obj);
    self->m_settings->callOnLog(MG_LFL_NTC, "MqttConnectionClient: [MsgId: " + std::to_string(message->mid) + "] messageCallback on topic " + 
                    message->topic + " with qos " + std::to_string(message->qos));

    (self->m_mqttCallback)(message->topic, message->payload, message->payloadlen, self->m_member);
}

void MqttConnectionClient::subscribe(std::string &topic, int qos)
{
    this->m_settings->callOnLog(MG_LFL_NTC, "MqttConnectionClient: [MsgId: " + std::to_string(this->m_messageId) + "] subscribing on topic " + 
                   topic.c_str() + " with qos " + std::to_string(qos));

    mosquitto_subscribe(this->m_mosq, &(this->m_messageId), topic.c_str(), qos);
    ++this->m_messageId;
}

void MqttConnectionClient::publish(std::string &topic, std::string &payload, int qos)
{
    this->m_settings->callOnLog(MG_LFL_NTC, "MqttConnectionClient: [MsgId: " + std::to_string(this->m_messageId) + "] publishing on topic " + 
                   topic.c_str() + " with qos " + std::to_string(qos) + " and payload-length " + std::to_string(payload.length()));

    mosquitto_publish(this->m_mosq, &(this->m_messageId), topic.c_str(), strlen(payload.c_str()), payload.c_str(), qos, 0);
    ++this->m_messageId;
}

void MqttConnectionClient::publish(std::string &topic, char *payload, int size, int qos)
{
    this->m_settings->callOnLog(MG_LFL_NTC, "MqttConnectionClient: [MsgId: " + std::to_string(this->m_messageId) + "] publishing on topic " + 
                   topic.c_str() + " with qos " + std::to_string(qos) + " and size " + std::to_string(size));

    mosquitto_publish(this->m_mosq, &(this->m_messageId), topic.c_str(), size, payload, qos, false);
    ++this->m_messageId;
}

void MqttConnectionClient::publish(std::string &topic, char *payload, int size, int qos, bool retain)
{
    this->m_settings->callOnLog(MG_LFL_NTC, "MqttConnectionClient: [MsgId: " + std::to_string(this->m_messageId) + "] publishing on topic " + 
                   topic.c_str() + " with qos " + std::to_string(qos) + " and size " + std::to_string(size) + " and retain " + std::to_string(retain));

    mosquitto_publish(this->m_mosq, &(this->m_messageId), topic.c_str(), size, payload, qos, retain);
    ++this->m_messageId;
}

bool MqttConnectionClient::isConnected()
{
    return this->m_connected;
}
