#include "CurlConnectionProvider.h"

#include "AgrirouterClient.h"
#include "Utils.h"

#include <mutex>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <sys/stat.h>

CurlConnectionProvider::CurlConnectionProvider(Settings *settings)
{
    m_settings = settings;

    // curl_global_init is not thread safe, so call it only once instead of implicitly in curl_easy_init
    static std::once_flag curlInitFlag;
    std::call_once(curlInitFlag, []() { curl_global_init(CURL_GLOBAL_DEFAULT); });

    // Reuse connections and TLS sessions, a full TLS handshake with client certificate for every request costs data volume
    m_share = curl_share_init();
    if (m_share != nullptr)
    {
        curl_share_setopt(m_share, CURLSHOPT_SHARE, CURL_LOCK_DATA_CONNECT);
        curl_share_setopt(m_share, CURLSHOPT_SHARE, CURL_LOCK_DATA_SSL_SESSION);
        curl_share_setopt(m_share, CURLSHOPT_SHARE, CURL_LOCK_DATA_DNS);
    }
}

CurlConnectionProvider::~CurlConnectionProvider()
{
    if (m_share != nullptr)
    {
        curl_share_cleanup(m_share);
    }
}

CURL *CurlConnectionProvider::createCurlHandle()
{
    CURL *hnd = curl_easy_init();

    // Do not use signals (e.g. SIGALRM for DNS timeouts), they are not safe in multithreaded processes
    curl_easy_setopt(hnd, CURLOPT_NOSIGNAL, 1L);

    if (m_share != nullptr)
    {
        curl_easy_setopt(hnd, CURLOPT_SHARE, m_share);
    }

    // Without timeouts a request on a dead connection (e.g. after a mobile network handover) blocks for minutes
    curl_easy_setopt(hnd, CURLOPT_CONNECTTIMEOUT, static_cast<long>(m_settings->getHttpConnectTimeout()));
    if (m_settings->getHttpStallTimeout() > 0)
    {
        // Abort if less than 1 byte/s is transferred for the stall timeout, a total timeout would abort slow uploads
        curl_easy_setopt(hnd, CURLOPT_LOW_SPEED_LIMIT, 1L);
        curl_easy_setopt(hnd, CURLOPT_LOW_SPEED_TIME, static_cast<long>(m_settings->getHttpStallTimeout()));
    }
    curl_easy_setopt(hnd, CURLOPT_TCP_KEEPALIVE, 1L);

    return hnd;
}

void CurlConnectionProvider::sendMessage(MessageParameters messageParameters)
{
    m_polling = false;
    this->sendMessageWithChunkedResponse(messageParameters);
}

void CurlConnectionProvider::sendMessageWithChunkedResponse(MessageParameters messageParameters)
{
    // Initializations
    CURL *hnd;

    MemoryStruct chunk;
    // Will be grown as needed by the realloc above
    chunk.memory = static_cast<char*>(malloc(1));
    // No data at this point
    chunk.size = 0;

    curl_slist *slist = NULL;
    hnd = this->createCurlHandle();

    this->setCurlUrl(hnd);
    slist = this->setCurlHeaders(hnd, slist);
    this->setCurlBody(hnd);
    this->setCurlSSL(hnd);
    this->setChunkedCurlCallback(hnd, &chunk);
    this->executeChunkedCurl(hnd, &chunk, messageParameters);
    this->cleanupChunkedCurl(hnd, slist, &chunk);
}

void CurlConnectionProvider::onboard(MessageParameters messageParameters)
{
    // Initializations
    CURL *hnd;

    MemoryStruct chunk;
    // Will be grown as needed by the realloc above
    chunk.memory = static_cast<char*>(malloc(1));
    // No data at this point
    chunk.size = 0;

    curl_slist *slist = NULL;
    hnd = this->createCurlHandle();

    this->setCurlUrl(hnd);
    slist = this->setCurlHeaders(hnd, slist);
    this->setCurlBody(hnd);
    this->setCurlCaBundle(hnd);
    this->setChunkedCurlCallback(hnd, &chunk);
    this->executeChunkedCurl(hnd, &chunk, messageParameters);
    this->cleanupChunkedCurl(hnd, slist, &chunk);
}

void CurlConnectionProvider::getMessages(void)
{
    // Start a new polling cycle
    m_pollCount = 1;
    this->pollMessages();
}

void CurlConnectionProvider::pollMessages(void)
{
    m_polling = true;

    // Initializations
    CURL *hnd;

    MemoryStruct chunk;
    // Will be grown as needed by the realloc above
    chunk.memory = static_cast<char*>(malloc(1));
    // No data at this point
    chunk.size = 0;

    curl_slist *slist = NULL;
    hnd = this->createCurlHandle();

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

size_t CurlConnectionProvider::getMessagesCallback(char *content, size_t size, size_t nmemb, void *member)
{
    size_t realsize = size * nmemb;

    CurlConnectionProvider *self = static_cast<CurlConnectionProvider*>(member);
    std::string message(content, realsize);

    timeval tv;

    int currentPollingTime = self->m_pollCount * self->m_settings->getPollingInterval();
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

        self->m_pollCount++;
        self->pollMessages();
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
size_t CurlConnectionProvider::chunkedResponseCallback(char *content,
        size_t size, size_t nmemb, void *userp)
{
    size_t realsize = size * nmemb;
    MemoryStruct *mem = static_cast<MemoryStruct *>(userp);

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

void CurlConnectionProvider::setCurlUrl(CURL *hnd)
{
    if (m_url != "")
    {
        curl_easy_setopt(hnd, CURLOPT_URL, m_url.c_str());
    }
}

curl_slist *CurlConnectionProvider::setCurlHeaders(CURL *hnd, curl_slist *slist)
{
    slist = NULL;
    for (size_t i = 0; i < m_headers.size(); i++)
    {
        slist = curl_slist_append(slist, m_headers[i].c_str());
    }

    // Only set header if header array contains elements
    if (m_headers.size() > 0)
    {
        curl_easy_setopt(hnd, CURLOPT_HTTPHEADER, slist);
    }

    return slist;
}

void CurlConnectionProvider::setCurlBody(CURL *hnd)
{
    if (m_body != "")
    {
        curl_easy_setopt(hnd, CURLOPT_POSTFIELDS, m_body.c_str());
    }
}

// For chunked curl responses see: https://curl.haxx.se/libcurl/c/getinmemory.html
void CurlConnectionProvider::setChunkedCurlCallback(CURL *hnd, MemoryStruct *chunk)
{
    /* send all data to this function  */
    curl_easy_setopt(hnd, CURLOPT_WRITEFUNCTION, chunkedResponseCallback);

    /* we pass our 'chunk' struct to the callback function */
    curl_easy_setopt(hnd, CURLOPT_WRITEDATA, static_cast<void *>(chunk));
}

void CurlConnectionProvider::setCurlCaBundle(CURL *hnd)
{
    const std::string& caBundlePath = m_settings->getHttpCaBundlePath();
    if (caBundlePath != "")
    {
        struct stat pathStat;
        if ((stat(caBundlePath.c_str(), &pathStat) == 0) && S_ISDIR(pathStat.st_mode))
        {
            curl_easy_setopt(hnd, CURLOPT_CAPATH, caBundlePath.c_str());
        }
        else
        {
            curl_easy_setopt(hnd, CURLOPT_CAINFO, caBundlePath.c_str());
        }
    }
}

void CurlConnectionProvider::setCurlSSL(CURL *hnd)
{
    this->setCurlCaBundle(hnd);

    if (m_settings->getCertificatePath() != "")
    {
        curl_easy_setopt(hnd, CURLOPT_SSLCERT, m_settings->getCertificatePath().c_str());
    }

    // Set private key
    if (m_settings->getPrivateKeyPath() != "")
    {
        curl_easy_setopt(hnd, CURLOPT_SSLKEY, m_settings->getPrivateKeyPath().c_str());
    }

    if (m_settings->getConnectionParameters().secret != "")
    {
        curl_easy_setopt(hnd, CURLOPT_KEYPASSWD, m_settings->getConnectionParameters().secret.c_str());
    }

    if (m_settings->acceptSelfSignedCertificate())
    {
        curl_easy_setopt(hnd, CURLOPT_SSL_VERIFYPEER, 0);
        curl_easy_setopt(hnd, CURLOPT_SSL_VERIFYHOST, 0);
    }
}

// For chunked curl responses see: https://curl.haxx.se/libcurl/c/getinmemory.html
void CurlConnectionProvider::executeChunkedCurl(CURL *hnd, MemoryStruct *chunk, MessageParameters messageParameters)
{
    CURLcode curlCode;

    // Read error messages
    char errbuf[CURL_ERROR_SIZE];
    curl_easy_setopt(hnd, CURLOPT_ERRORBUFFER, errbuf);
    errbuf[0] = 0;

    // For logging purposes
    // curl_easy_setopt(hnd, CURLOPT_VERBOSE, 1L);

    curlCode = curl_easy_perform(hnd);

    // CURLINFO_RESPONSE_CODE requires a long
    long httpCode = 0;
    curl_easy_getinfo(hnd, CURLINFO_RESPONSE_CODE, &httpCode);

    /* check for errors */
    if (curlCode != CURLE_OK)
    {
        std::string curlMessage = "";

        // get content if there is some content
        std::string errorContent(chunk->memory, chunk->size);

        // get the curl message
        if(strlen(errbuf))
        {
            curlMessage = std::string(errbuf);
        }
        else
        {
            curlMessage = std::string(curl_easy_strerror(curlCode));
        }

        m_settings->callOnError(static_cast<int>(httpCode), curlCode, curlMessage, messageParameters, errorContent);
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
            std::string errorContent(chunk->memory, chunk->size);

            m_settings->callOnError(static_cast<int>(httpCode), curlCode, std::string(curl_easy_strerror(curlCode)), messageParameters, errorContent);
            return;
        }

        if (m_polling)
        {
            // Poll for new messages
            this->getMessagesCallback(chunk->memory, chunk->size, 1, this);
        }
        else
        {
            // No polling, call callback instead
            (m_callback)(chunk->memory, chunk->size, 1, m_member);
        }
    }
}

// For chunked curl responses see: https://curl.haxx.se/libcurl/c/getinmemory.html
void CurlConnectionProvider::cleanupChunkedCurl(CURL *hnd, curl_slist *slist, MemoryStruct *chunk)
{
    curl_easy_cleanup(hnd);
    hnd = NULL;
    curl_slist_free_all(slist);
    slist = NULL;

    free(chunk->memory);
    /* we're done with libcurl, so clean it up */
}
