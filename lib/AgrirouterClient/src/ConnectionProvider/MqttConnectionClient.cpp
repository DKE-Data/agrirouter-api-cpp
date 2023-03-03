#include "MqttConnectionClient.h"

#include "third_party/mosquitto/mosquitto_internal.h"

#include <openssl/asn1.h>
#include <openssl/bio.h>
#include <openssl/conf.h>
#include <openssl/err.h>
#include <openssl/pem.h>
#include <openssl/x509.h>

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
    mosquitto_destroy(this->m_mosq);
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
        // the direct to struct, not the best solution but it works
        this->m_mosq->userdata = this;

        mosquitto_connect_callback_set(this->m_mosq, connectCallback);
        mosquitto_disconnect_callback_set(this->m_mosq, disconnectCallback);
        mosquitto_publish_callback_set(this->m_mosq, publishCallback);
        mosquitto_log_callback_set(this->m_mosq, loggingCallback);
        mosquitto_subscribe_callback_set(this->m_mosq, subscribeCallback);
        mosquitto_unsubscribe_callback_set(this->m_mosq, unsubscribeCallback);
        mosquitto_message_callback_set(this->m_mosq, messageCallback);

        int tlsInsecure = mosquitto_tls_insecure_set (this->m_mosq, false);
        if(tlsInsecure == MOSQ_ERR_SUCCESS)
        {
            printf("MqttConnectionClient - tlsInsecure set successful - %i: %s\n", tlsInsecure, mosquitto_strerror(tlsInsecure));
        }
        else
        {
            printf("MqttConnectionClient - tlsInsecure set failed - %i: %s\n", tlsInsecure, mosquitto_strerror(tlsInsecure));
        }

        int tlsOptsSet = mosquitto_tls_opts_set     (this->m_mosq, 1, NULL, NULL);
        if(tlsOptsSet == MOSQ_ERR_SUCCESS)
        {
            printf("MqttConnectionClient - tlsOptsSet set successful - %i: %s\n", tlsOptsSet, mosquitto_strerror(tlsOptsSet));
        }
        else
        {
            printf("MqttConnectionClient - tlsOptsSet set failed - %i: %s\n", tlsOptsSet, mosquitto_strerror(tlsOptsSet));
        }

        std::string caCerts = "/etc/ssl/certs/";
        std::string client_certificate = this->m_settings->getCertificatePath();
        std::string client_key = this->m_settings->getPrivateKeyPath();

        int tlsSet = mosquitto_tls_set (this->m_mosq, nullptr, caCerts.c_str(), client_certificate.c_str(), client_key.c_str(), onPWCallback);
        if(tlsSet == MOSQ_ERR_SUCCESS)
        {
            printf("MqttConnectionClient - tlsSet set successful - %i: %s\n", tlsSet, mosquitto_strerror(tlsSet));
        }
        else
        {
            printf("MqttConnectionClient - tlsSet set failed - %i: %s\n", tlsSet, mosquitto_strerror(tlsSet));
        }

        int connect = mosquitto_connect_async(this->m_mosq, this->m_host.c_str(), this->m_port, 20);
        if(connect == MOSQ_ERR_SUCCESS)
        {
            printf("MqttConnectionClient - connect set successful - %i: %s\n", connect, mosquitto_strerror(connect));
        }
        else
        {
            printf("MqttConnectionClient - connect set failed - %i: %s\n", connect, mosquitto_strerror(connect));
        }

        int loop = mosquitto_loop_start(this->m_mosq);
        if(loop == MOSQ_ERR_SUCCESS)
        {
            printf("MqttConnectionClient - loop start successful - %i: %s\n", loop, mosquitto_strerror(loop));
        }
        else
        {
            printf("MqttConnectionClient - loop start failed - %i: %s\n", loop, mosquitto_strerror(loop));
        }
        
        if (connect == 0 && loop == 0)
        {
            return EXIT_SUCCESS;
        }
    }
    else
    {
        printf("MqttConnectionClient not connect to broker\n");
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

void MqttConnectionClient::connectCallback(struct mosquitto *mosq, void *obj, int messageId)
{
    printf("MqttConnectionClient - connect callback from message: '%i'\n", messageId);
    MqttConnectionClient *self = static_cast<MqttConnectionClient *>(obj);
    self->m_connected = true;
    printf("MqttConnectionClient - Connected to MQTT Broker (%s:%i)\n", self->m_host.c_str(), self->m_port);
}

void MqttConnectionClient::disconnectCallback(struct mosquitto *mosq, void *obj, int messageId)
{
    printf("MqttConnectionClient - disconnect callback from message: '%i'\n", messageId);
    MqttConnectionClient *self = static_cast<MqttConnectionClient *>(obj);
    self->m_connected = false;

    // try to reconnect
    int reconn = mosquitto_reconnect(self->m_mosq);
    if (reconn == MOSQ_ERR_SUCCESS)
    {
        printf("MqttConnectionClient - Reconnected\n");
    }
    else
    {
        printf("MqttConnectionClient - reconnect failed - %i: %s\n", reconn, mosquitto_strerror(reconn));
    }
}

void MqttConnectionClient::publishCallback(struct mosquitto *mosq, void *obj, int messageId)
{
    printf("MqttConnectionClient - [MsgId: %d] publish OK\n", messageId);
}

void MqttConnectionClient::loggingCallback(struct mosquitto *mosq, void *obj, int level, const char *message)
{
    printf("MqttConnectionClient - logging callback with message: '%s' and level: '%i'\n", message, level);
}

void MqttConnectionClient::subscribeCallback(struct mosquitto *mosq, void *obj, int messageId, int qosCount, const int *grantedQos)
{
    printf("MqttConnectionClient - [MsgId: %d] subscribe OK\n", messageId);
}

void MqttConnectionClient::unsubscribeCallback(struct mosquitto *mosq, void *obj, int messageId)
{
    printf("MqttConnectionClient - unsubscribe callback\n");
}

void MqttConnectionClient::messageCallback(struct mosquitto *mosq, void *obj, const struct mosquitto_message *message)
{
    printf("MqttConnectionClient - [MsgId: %d] messageCallback on topic %s with qos %i\n", message->mid, message->topic, message->qos);
    MqttConnectionClient *self = static_cast<MqttConnectionClient *>(obj);
    (self->m_mqttCallback)(message->topic, message->payload, message->payloadlen, self->m_member);
}

void MqttConnectionClient::subscribe(std::string &topic, int qos)
{
    printf("MqttConnectionClient - [MsgId: %d] subscribing on topic %s with qos %i\n", this->m_messageId, topic.c_str(), qos);
    mosquitto_subscribe(this->m_mosq, &(this->m_messageId), topic.c_str(), qos);
    ++this->m_messageId;
}

void MqttConnectionClient::publish(std::string &topic, std::string &payload, int qos)
{
    printf("MqttConnectionClient - [MsgId: %d] publishing on topic %s with qos %i and payload-length %i\n", this->m_messageId, topic.c_str(), qos, payload.length());
    mosquitto_publish(this->m_mosq, &(this->m_messageId), topic.c_str(), strlen(payload.c_str()), payload.c_str(), qos, 0);
    ++this->m_messageId;
}

void MqttConnectionClient::publish(std::string &topic, char *payload, int size, int qos)
{
    printf("MqttConnectionClient - [MsgId: %d] publishing on topic %s with qos %i and payload-length %i\n", this->m_messageId, topic.c_str(), qos, size);
    mosquitto_publish(this->m_mosq, &(this->m_messageId), topic.c_str(), size, payload, qos, false);
    ++this->m_messageId;
}

void MqttConnectionClient::publish(std::string &topic, char *payload, int size, int qos, bool retain)
{
    printf("MqttConnectionClient - [MsgId: %d] publishing on topic %s with qos %i and payload-length %i\n", this->m_messageId, topic.c_str(), qos, size);
    mosquitto_publish(this->m_mosq, &(this->m_messageId), topic.c_str(), size, payload, qos, retain);
    ++this->m_messageId;
}

bool MqttConnectionClient::isConnected()
{
    return this->m_connected;
}