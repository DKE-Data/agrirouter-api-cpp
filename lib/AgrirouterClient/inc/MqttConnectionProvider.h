#ifndef LIB_AGRIROUTERCLIENT_SRC_CONNECTIONPROVIDER_MQTTCONNECTIONPROVIDER_H_
#define LIB_AGRIROUTERCLIENT_SRC_CONNECTIONPROVIDER_MQTTCONNECTIONPROVIDER_H_

#include "ConnectionProvider.h"
#include "MqttClient.h"

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
            char *memory;
            size_t size;
        } MemoryStruct;

        static size_t requestMqttCallback(char *topic, void *payload, int payloadlen, void *member);

        void sendMessage(MessageParameters messageParameters);
        void sendMessageWithChunkedResponse(MessageParameters messageParameters);
        static size_t chunkedResponseCallback(char *content, size_t size, size_t nmemb, void *member);

        void getMessages(void);
        static size_t getMessagesCallback(char *content, size_t size, size_t nmemb, void *member);

        void onboard(MessageParameters messageParameters);

    private:
        bool m_polling;
        MqttClient *m_mqttClient;
        MessageParameters m_messageParameters;

        void init();

        void setCurlUrl(CURL *hnd);
        curl_slist *setCurlHeaders(CURL *hnd, curl_slist *slist);
        void setCurlBody(CURL *hnd);
        void setChunkedCurlCallback(CURL *hnd, MemoryStruct *chunk);
        void setCurlSSL(CURL *hnd);
        void executeChunkedCurl(CURL *hnd, MemoryStruct *chunk, MessageParameters messageParameters);
        void cleanupChunkedCurl(CURL *hnd, curl_slist *slist, MemoryStruct *chunk);
};

#endif  // LIB_AGRIROUTERCLIENT_SRC_CONNECTIONPROVIDER_MQTTCONNECTIONPROVIDER_H_
