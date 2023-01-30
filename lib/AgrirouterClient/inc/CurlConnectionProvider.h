#ifndef LIB_AGRIROUTERCLIENT_SRC_CONNECTIONPROVIDER_CURLCONNECTIONPROVIDER_H_
#define LIB_AGRIROUTERCLIENT_SRC_CONNECTIONPROVIDER_CURLCONNECTIONPROVIDER_H_

#include "ConnectionProvider.h"

#include "Settings.h"
#include <curl/curl.h>
#include <string>

class CurlConnectionProvider : public ConnectionProvider
{
    public:
        explicit CurlConnectionProvider(Settings *settings);
        ~CurlConnectionProvider();

        // Struct to use curl chunked callbacks
        typedef struct MemoryStruct 
        {
            char *memory;
            size_t size;
        } MemoryStruct;

        void sendMessage(MessageParameters messageParameters);
        void sendMessageWithChunkedResponse(MessageParameters messageParameters);
        static size_t chunkedResponseCallback(char *content, size_t size, size_t nmemb, void *member);

        void getMessages(void);
        static size_t getMessagesCallback(char *content, size_t size, size_t nmemb, void *member);

        void onboard(MessageParameters messageParameters);

    private:
        bool m_polling;

        void setCurlUrl(CURL *hnd);
        curl_slist *setCurlHeaders(CURL *hnd, curl_slist *slist);
        void setCurlBody(CURL *hnd);
        void setChunkedCurlCallback(CURL *hnd, MemoryStruct *chunk);
        void setCurlSSL(CURL *hnd);
        void executeChunkedCurl(CURL *hnd, MemoryStruct *chunk, MessageParameters messageParameters);
        void cleanupChunkedCurl(CURL *hnd, curl_slist *slist, MemoryStruct *chunk);
};

#endif  // LIB_AGRIROUTERCLIENT_SRC_CONNECTIONPROVIDER_CURLCONNECTIONPROVIDER_H_
