#include "MqttConnectionProvider.h"

#include "AgrirouterClient.h"
#include "AgrirouterMessageUtils.h"
#include "Utils.h"

#include <stdio.h>
#include <stdlib.h>
#include <string>

MqttConnectionProvider::MqttConnectionProvider(Settings *settings)
{
    this->m_polling = false;
    this->m_settings = settings;
    this->m_body = "";
    this->m_url = "";
    this->init();
}

MqttConnectionProvider::~MqttConnectionProvider()
{
    if (this->m_mqttClient != NULL)
    {
        delete this->m_mqttClient;
    }
}

void MqttConnectionProvider::init()
{
    ConnectionParameters conn = this->m_settings->getConnectionParameters();
    this->m_mqttClient = new MqttClient(conn.clientId, conn.host, conn.port, this->m_settings);
    this->m_mqttClient->init();
    
    // set message callback
    this->m_mqttClient->setMember(this);
    this->m_mqttClient->setMqttCallback(requestMqttCallback);

    // subscribe to commands only subscribe when topic is valid (onboarding is done)
    if(conn.commandsUrl.length() > 0)
    {
        this->m_mqttClient->subscribe(conn.commandsUrl, 2);
    }
}

size_t MqttConnectionProvider::requestMqttCallback(char *topic, void *payload, int payloadlen, void *member)
{
    MqttConnectionProvider *self = static_cast<MqttConnectionProvider *>(member);
    char* msg = (char*) payload;
       
    // If msg starts with '{', it is not an array as it comes from curl, so add the square brackets
    if (strncmp(msg, "{", 1) == 0)
    {
        std::string message = std::string(msg, payloadlen);
        message = "[" + message + "]";
        payloadlen = message.length();
        msg = strdup(message.c_str());
    }

    (self->m_callback)(msg, payloadlen, 1, &self->m_messageParameters);

    return payloadlen;
}

void MqttConnectionProvider::sendMessage(MessageParameters messageParameters)
{
    this->m_polling = false;
    this->sendMessageWithChunkedResponse(messageParameters);
}

void MqttConnectionProvider::sendMessageWithChunkedResponse(MessageParameters messageParameters)
{
    printf("Send message mqtt with message: '%s' and application id: '%s' \n", this->m_body.c_str(), messageParameters.applicationMessageId.c_str());
    this->m_mqttClient->publish(this->m_settings->getConnectionParameters().measuresUrl, this->m_body, 2);
    this->m_messageParameters = messageParameters;
}

void MqttConnectionProvider::onboard(MessageParameters messageParameters)
{
    // Initializations
    CURL *hnd;

    MemoryStruct chunk;
    // Will be grown as needed by the realloc above
    chunk.memory = static_cast<char*>(malloc(1));
    // No data at this point
    chunk.size = 0;

    curl_slist *slist = NULL;
    hnd = curl_easy_init();

    this->setCurlUrl(hnd);
    slist = this->setCurlHeaders(hnd, slist);
    this->setCurlBody(hnd);
    this->setChunkedCurlCallback(hnd, &chunk);
    this->executeChunkedCurl(hnd, &chunk, messageParameters);
    this->cleanupChunkedCurl(hnd, slist, &chunk);
}

void MqttConnectionProvider::getMessages(void)
{
    this->m_polling = true;

    // Initializations
    CURL *hnd;

    MemoryStruct chunk;
    // Will be grown as needed by the realloc above
    chunk.memory = static_cast<char*>(malloc(1));
    // No data at this point
    chunk.size = 0;

    curl_slist *slist = NULL;
    hnd = curl_easy_init();

    this->setCurlUrl(hnd);
    slist = this->setCurlHeaders(hnd, slist);
    this->setCurlBody(hnd);
    this->setCurlSSL(hnd);
    this->setChunkedCurlCallback(hnd, &chunk);

    // ToDo: is there any application message id available?
    MessageParameters messageParameters;
    this->executeChunkedCurl(hnd, &chunk, messageParameters);
    this->cleanupChunkedCurl(hnd, slist, &chunk);
}

size_t MqttConnectionProvider::getMessagesCallback(char *content, size_t size, size_t nmemb, void *member)
{
    size_t realsize = size * nmemb;
    static int pollCount = 1;

    MqttConnectionProvider *self = static_cast<MqttConnectionProvider*>(member);
    std::string message(content, realsize);

    timeval tv;

    int currentPollingTime = pollCount * self->m_settings->getPollingInterval();
    if (currentPollingTime >= self->m_settings->getPollingMaxTime())
    {
        self->m_callback(content, realsize, 1, self->m_member);
    }
    else if (message == "[]")
    {
        if (self->m_settings->getPollingInterval() == 0)
        {
            // No polling required
            // Call callback of caller with member=this->member
            return realsize;
        }

        // Poll for messages depending on intervall
        tv.tv_sec = self->m_settings->getPollingInterval();
        tv.tv_usec = 0;
        select(0, NULL, NULL, NULL, &tv);

        pollCount++;
        self->getMessages();
    }
    else
    {
        // Received entire message: call callback
        self->m_callback(content, realsize, 1, self->m_member);
    }

    // Has to be returned for successful curl communication
    return realsize;
}

// For chunked curl responses see: https://curl.haxx.se/libcurl/c/getinmemory.html
size_t MqttConnectionProvider::chunkedResponseCallback(char *content,
        size_t size, size_t nmemb, void *userp)
{
    size_t realsize = size * nmemb;
    MemoryStruct *mem = static_cast<MemoryStruct*>(userp);

    int memorySize = mem->size + realsize + 1;
    mem->memory = static_cast<char*>(realloc(mem->memory, memorySize));

    if (mem->memory == NULL)
    {
        /* out of memory! */
        return 0;
    }

    // Add to MemoryStruct
    memcpy(&(mem->memory[mem->size]), content, realsize);
    mem->size += realsize;
    mem->memory[mem->size] = 0;

    // Has to be returned for successful curl communication
    return realsize;
}

void MqttConnectionProvider::setCurlUrl(CURL *hnd)
{
    if (this->m_url != "")
    {
        curl_easy_setopt(hnd, CURLOPT_URL, this->m_url.c_str());
    }
}

curl_slist *MqttConnectionProvider::setCurlHeaders(CURL *hnd, curl_slist *slist)
{
    slist = NULL;
    for (size_t i = 0; i < this->m_headers.size(); i++)
    {
        slist = curl_slist_append(slist, this->m_headers[i].c_str());
    }

    // Only set header if header array contains elements
    if (this->m_headers.size() > 0)
    {
        curl_easy_setopt(hnd, CURLOPT_HTTPHEADER, slist);
    }

    return slist;
}

void MqttConnectionProvider::setCurlBody(CURL *hnd)
{
    if (this->m_body != "")
    {
        curl_easy_setopt(hnd, CURLOPT_POSTFIELDS, this->m_body.c_str());
    }
}

// For chunked curl responses see: https://curl.haxx.se/libcurl/c/getinmemory.html
void MqttConnectionProvider::setChunkedCurlCallback(CURL *hnd, MemoryStruct *chunk)
{
    /* send all data to this function  */
    curl_easy_setopt(hnd, CURLOPT_WRITEFUNCTION, chunkedResponseCallback);

    /* we pass our 'chunk' struct to the callback function */
    curl_easy_setopt(hnd, CURLOPT_WRITEDATA, static_cast<void*>(chunk));
}

void MqttConnectionProvider::setCurlSSL(CURL *hnd)
{
    if (this->m_settings->getCertificatePath() != "")
    {
        curl_easy_setopt(hnd, CURLOPT_SSLCERT, this->m_settings->getCertificatePath().c_str());
    }

    // Set private key
    if (this->m_settings->getPrivateKeyPath() != "")
    {
        curl_easy_setopt(hnd, CURLOPT_SSLKEY, this->m_settings->getPrivateKeyPath().c_str());
    }

    if (this->m_settings->getConnectionParameters().secret != "")
    {
        curl_easy_setopt(hnd, CURLOPT_KEYPASSWD, this->m_settings->getConnectionParameters().secret.c_str());
    }

    if (this->m_settings->acceptSelfSignedCertificate())
    {
        curl_easy_setopt(hnd, CURLOPT_SSL_VERIFYPEER, 0);
        curl_easy_setopt(hnd, CURLOPT_SSL_VERIFYHOST, 0);
    }
}

// For chunked curl responses see: https://curl.haxx.se/libcurl/c/getinmemory.html
void MqttConnectionProvider::executeChunkedCurl(CURL *hnd, MemoryStruct *chunk, MessageParameters messageParameters)
{
    CURLcode curlCode;

    // Read error messages
    char errbuf[CURL_ERROR_SIZE];
    curl_easy_setopt(hnd, CURLOPT_ERRORBUFFER, errbuf);
    errbuf[0] = 0;

    // For logging purposes
    // curl_easy_setopt(hnd, CURLOPT_VERBOSE, 1L);

    curlCode = curl_easy_perform(hnd);

    int32_t httpCode = 0;
    curl_easy_getinfo(hnd, CURLINFO_RESPONSE_CODE, &httpCode);

    /* check for errors */
    if (curlCode != CURLE_OK)
    {
        std::string curlMessage = "";

        // get content if there is some content
        std::string content(chunk->memory, chunk->size);

        // get the curl message
        if(strlen(errbuf))
        {
            curlMessage = std::string(errbuf);
        }
        else
        {
            curlMessage = std::string(curl_easy_strerror(curlCode));
        }

        this->m_settings->callOnError(httpCode, curlCode, curlMessage, messageParameters, content);
    }
    else
    {
        /*
        * Now, our chunk.memory points to a memory block that is chunk.size
        * bytes big and contains the remote file.
        *
        * Do something nice with it!
        */

        if ((httpCode < 200) || (httpCode > 299))
        {
            // Something went wrong
            // get content if there is some content
            std::string content(chunk->memory, chunk->size);

            this->m_settings->callOnError(httpCode, curlCode, std::string(curl_easy_strerror(curlCode)), messageParameters, content);
            return;
        }

        if (this->m_polling)
        {
            // Poll for new messages
            this->getMessagesCallback(chunk->memory, chunk->size, 1, this);
        }
        else
        {
            // No polling, call callback instead
            (this->m_callback)(chunk->memory, chunk->size, 1, this->m_member);
        }
    }
}

// For chunked curl responses see: https://curl.haxx.se/libcurl/c/getinmemory.html
void MqttConnectionProvider::cleanupChunkedCurl(CURL *hnd, curl_slist *slist, MemoryStruct *chunk)
{
    curl_easy_cleanup(hnd);
    hnd = NULL;
    curl_slist_free_all(slist);
    slist = NULL;

    free(chunk->memory);
    /* we're done with libcurl, so clean it up */
}
