#ifndef LIB_AGRIROUTERCLIENT_INC_MQTTCLIENT_H_
#define LIB_AGRIROUTERCLIENT_INC_MQTTCLIENT_H_

#include "Definitions.h"
#include "Settings.h"
#include "third_party/mosquitto/mosquitto.h"

class MqttConnectionClient {

public:
    MqttConnectionClient(std::string &clientId, std::string &address, int port, Settings *settings);
    ~MqttConnectionClient();

    // Function pointer for callback functions
    typedef void (*MqttCallback)(char *topic, void *payload, int payloadlen, void *member);
    typedef void (*MqttErrorCallback)(int errorCode, std::string message, std::string content, void *member);
    typedef int (*pw_callback)(char *buf, int size, int rwflag, void *userdata);

    int init();
    void subscribe(std::string &topic, int qos);
    void publish(std::string &topic, std::string &payload, int qos);
    void publish(std::string &topic, char *payload, int size, int qos);
    void publish(std::string &topic, char *payload, int size, int qos, bool retain);

    void setMqttCallback(MqttCallback callback);
    MqttCallback getMqttCallback();
    void setMqttErrorCallback(MqttErrorCallback errorCallback);
    MqttErrorCallback getMqttErrorCallback();
    void setMember(void* member);
    void* getMember();

    bool isConnected();

    static MqttConnectionClient *mqttClient;

private:
    struct mosquitto *m_mosq;
    int m_port;
    std::string m_host;
    std::string m_clientId;
    int m_messageId;
    bool m_connected;
    Settings *m_settings;
    MqttCallback m_mqttCallback;
    MqttErrorCallback m_mqttErrorCallback;
    void *m_member;

    static int onPWCallback(char *buf, int size, int rwflag, void *userdata);
    static void connectCallback(struct mosquitto *mosq, void *obj, int result);
    static void disconnectCallback(struct mosquitto *mosq, void *obj, int result);
    static void publishCallback(struct mosquitto *mosq, void *obj, int messageId);
    static void loggingCallback(struct mosquitto *mosq, void *obj, int level, const char *message);
    static void subscribeCallback(struct mosquitto *mosq, void *obj, int messageId, int qosCount, const int *grantedQos);
    static void unsubscribeCallback(struct mosquitto *mosq, void *obj, int messageId);
    static void messageCallback(struct mosquitto *mosq, void *obj, const struct mosquitto_message *message);
};

#endif // LIB_AGRIROUTERCLIENT_INC_MQTTCLIENT_H_
