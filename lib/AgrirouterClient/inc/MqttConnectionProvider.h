#ifndef LIB_AGRIROUTERCLIENT_SRC_CONNECTIONPROVIDER_MQTTCONNECTIONPROVIDER_H_
#define LIB_AGRIROUTERCLIENT_SRC_CONNECTIONPROVIDER_MQTTCONNECTIONPROVIDER_H_

#include "ConnectionProvider.h"
#include "MqttConnectionClient.h"

#include "Settings.h"
#include <curl/curl.h>
#include <mutex>
#include <string>

class MqttConnectionProvider : public ConnectionProvider
{
    public:
        explicit MqttConnectionProvider(Settings *settings);
        ~MqttConnectionProvider();

        void renewConnection() override;
        bool isConnected() override;

        // Struct to use curl chunked callbacks
        typedef struct MemoryStruct
        {
            char *memory = nullptr;
            size_t size = 0;
        } MemoryStruct;

        static void requestMqttCallback(char *topic, void *payload, int payloadlen, void *member);
        static void requestMqttErrorCallback(int errorCode, std::string message, std::string content, void *member);

        void sendMessage(MessageParameters messageParameters);
        void sendMessageWithChunkedResponse(MessageParameters messageParameters);

        void getMessages(void);

        void onboard(MessageParameters messageParameters);

        // Receives all incoming messages. Set it before connecting, messages (e.g. push notifications)
        // can arrive right after the connect, before anything was sent.
        void setReceiver(Callback callback, void *receiver);

    private:
        MqttConnectionClient *m_mqttClient = nullptr;
        Callback m_receiveCallback = nullptr;
        void *m_receiver = nullptr;
        // Written by the sending thread, read by the mosquitto thread
        MessageParameters m_messageParameters;
        std::mutex m_messageParametersMutex;

        void init();
        MessageParameters getMessageParameters();
};

#endif  // LIB_AGRIROUTERCLIENT_SRC_CONNECTIONPROVIDER_MQTTCONNECTIONPROVIDER_H_
