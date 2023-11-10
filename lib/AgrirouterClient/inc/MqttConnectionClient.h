#ifndef LIB_AGRIROUTERCLIENT_INC_MQTTCONNECTIONCLIENT_H_
#define LIB_AGRIROUTERCLIENT_INC_MQTTCONNECTIONCLIENT_H_

#include "Definitions.h"
#include "Settings.h"
#include "third_party/mosquitto/mosquitto.h"

#include <mutex>

class MqttConnectionClient {

public:
    MqttConnectionClient(const std::string& clientId, const std::string& address, int port, Settings *settings);
    ~MqttConnectionClient();

    // Function pointer for callback functions
    typedef void (*MqttCallback)(char *topic, void *payload, int payloadlen, void *member);
    typedef void (*MqttErrorCallback)(int errorCode, std::string message, std::string content, void *member);
    typedef int (*pw_callback)(char *buf, int size, int rwflag, void *userdata);

    int init();
    void subscribe(const std::string& topic, int qos);
    void publish(const std::string& topic, const std::string& payload, int qos);
    void publish(const std::string& topic, char *payload, int size, int qos);
    void publish(const std::string& topic, char *payload, int size, int qos, bool retain);

    void setMqttCallback(MqttCallback callback);
    MqttCallback getMqttCallback();
    void setMqttErrorCallback(MqttErrorCallback errorCallback);
    MqttErrorCallback getMqttErrorCallback();
    void setMember(void* member);
    void* getMember();

    static std::string getStaticSecret();
    static void setStaticSecret(std::string secret);

    bool isConnected();

private:
    struct mosquitto *m_mosq = nullptr;
    std::string m_host = "";
    int m_port = 0;
    std::string m_clientId = "";

    int m_messageId = 1;
    bool m_connected = false;
    void *m_member = nullptr;
    Settings *m_settings = nullptr;

    MqttCallback m_mqttCallback;
    MqttErrorCallback m_mqttErrorCallback;

    static std::string globalSecret;
    static std::mutex mutexSecret;

    static int onPWCallback(char *buf, int size, int rwflag, void *userdata);
    static void connectCallback(struct mosquitto *mosq, void *obj, int result);
    static void disconnectCallback(struct mosquitto *mosq, void *obj, int result);
    static void publishCallback(struct mosquitto *mosq, void *obj, int messageId);
    static void loggingCallback(struct mosquitto *mosq, void *obj, int level, const char *message);
    static void subscribeCallback(struct mosquitto *mosq, void *obj, int messageId, int qosCount, const int *grantedQos);
    static void unsubscribeCallback(struct mosquitto *mosq, void *obj, int messageId);
    static void messageCallback(struct mosquitto *mosq, void *obj, const struct mosquitto_message *message);
};

#endif // LIB_AGRIROUTERCLIENT_INC_MQTTCONNECTIONCLIENT_H_
