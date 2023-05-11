#ifndef LIB_AGRIROUTERCLIENT_SRC_CONNECTIONPROVIDER_MQTTCONNECTIONPROVIDER_H_
#define LIB_AGRIROUTERCLIENT_SRC_CONNECTIONPROVIDER_MQTTCONNECTIONPROVIDER_H_

#include "ConnectionProvider.h"
#include "MqttConnectionClient.h"

#include "Settings.h"
#include <curl/curl.h>
#include <string>

class MqttConnectionProvider : public ConnectionProvider
{
    public:
        explicit MqttConnectionProvider(Settings *settings);
        ~MqttConnectionProvider();

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

    private:
        MqttConnectionClient *m_mqttClient = nullptr;
        MessageParameters m_messageParameters;

        void init();
};

#endif  // LIB_AGRIROUTERCLIENT_SRC_CONNECTIONPROVIDER_MQTTCONNECTIONPROVIDER_H_
