#ifndef LIB_AGRIROUTERCLIENT_INC_MQTTCONNECTIONCLIENT_H_
#define LIB_AGRIROUTERCLIENT_INC_MQTTCONNECTIONCLIENT_H_

#include "Definitions.h"
#include "Settings.h"
#include "third_party/mosquitto/mosquitto.h"

#include <atomic>
#include <map>
#include <mutex>

class MqttConnectionClient {

public:
    MqttConnectionClient(const std::string& clientId, const std::string& address, int port, Settings *settings);
    ~MqttConnectionClient();

    typedef void (*MqttCallback)(char *topic, void *payload, int payloadlen, void *member);
    typedef void (*MqttErrorCallback)(int errorCode, std::string message, std::string content, void *member);
    typedef int (*pw_callback)(char *buf, int size, int rwflag, void *userdata);

    int init();
    void subscribe(const std::string& topic, int qos);
    int publish(const std::string& topic, const std::string& payload, int qos);
    int publish(const std::string& topic, char *payload, int size, int qos);
    int publish(const std::string& topic, char *payload, int size, int qos, bool retain);

    void setMqttCallback(MqttCallback callback);
    MqttCallback getMqttCallback();
    void setMqttErrorCallback(MqttErrorCallback errorCallback);
    MqttErrorCallback getMqttErrorCallback();
    void setMember(void* member);
    void* getMember();

    bool isConnected();

    void setSubscription(const std::string& topic, int qos);

private:
    struct mosquitto *m_mosq = nullptr;
    std::string m_host = "";
    int m_port = 0;
    std::string m_clientId = "";
    std::string m_secret = "";
    std::string m_subscriptionTopic = "";
    int m_subscriptionQos = 0;

    std::atomic<bool> m_connected{false}; // Set from the mosquitto thread
    void *m_member = nullptr;
    Settings *m_settings = nullptr;

    MqttCallback m_mqttCallback = NULL;
    MqttErrorCallback m_mqttErrorCallback = NULL;

    static int onPWCallback(char *buf, int size, int rwflag, void *userdata);
    static void connectCallback(struct mosquitto *mosq, void *obj, int result);
    static void disconnectCallback(struct mosquitto *mosq, void *obj, int result);
    static void publishCallback(struct mosquitto *mosq, void *obj, int messageId);
    static void loggingCallback(struct mosquitto *mosq, void *obj, int level, const char *message);
    static void subscribeCallback(struct mosquitto *mosq, void *obj, int messageId, int qosCount, const int *grantedQos);
    static void unsubscribeCallback(struct mosquitto *mosq, void *obj, int messageId);
    static void messageCallback(struct mosquitto *mosq, void *obj, const struct mosquitto_message *message);

    // The password callback only gets the mosquitto instance, so the secrets are stored per instance
    static std::map<const struct mosquitto *, std::string> s_secrets;
    static std::mutex s_secretsMutex;

    // mosquitto_lib_init() is global, it is called once for all clients
    static std::once_flag s_libInitFlag;
};

#endif // LIB_AGRIROUTERCLIENT_INC_MQTTCONNECTIONCLIENT_H_
