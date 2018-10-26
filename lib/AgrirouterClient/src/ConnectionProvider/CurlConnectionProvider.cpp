#include <CurlConnectionProvider.h>

#include <AgrirouterClient.h>
#include <Utils.h>

#include <string>

CurlConnectionProvider::CurlConnectionProvider(Settings *settings) {
  this->m_polling = false;
  this->settings = settings;
  this->body = "";
  this->url = "";
}

CurlConnectionProvider::~CurlConnectionProvider() {}

void CurlConnectionProvider::sendMessage(MessageParameters messageParameters) {
  this->m_polling = false;

  this->sendMessageWithChunkedResponse(messageParameters);
}

void CurlConnectionProvider::sendMessageWithChunkedResponse(MessageParameters messageParameters) {
  // Initializations
  CURL *hnd;

  struct MemoryStruct chunk;
  // Will be grown as needed by the realloc above
  chunk.memory = reinterpret_cast<char*>(malloc(1));
  // No data at this point
  chunk.size = 0;

  struct curl_slist *slist = NULL;
  hnd = curl_easy_init();

  this->setCurlUrl(hnd);
  slist = this->setCurlHeaders(hnd, slist);
  this->setCurlBody(hnd);
  this->setCurlSSL(hnd);
  this->setChunkedCurlCallback(hnd, &chunk);
  this->executeChunkedCurl(hnd, &chunk, messageParameters);
  this->cleanupChunkedCurl(hnd, slist, &chunk);
}

void CurlConnectionProvider::onboard(MessageParameters messageParameters) {
  // Initializations
  CURL *hnd;

  struct MemoryStruct chunk;
  // Will be grown as needed by the realloc above
  chunk.memory = reinterpret_cast<char*>(malloc(1));
  // No data at this point
  chunk.size = 0;

  struct curl_slist *slist = NULL;
  hnd = curl_easy_init();

  this->setCurlUrl(hnd);
  slist = this->setCurlHeaders(hnd, slist);
  this->setCurlBody(hnd);
  this->setChunkedCurlCallback(hnd, &chunk);
  this->executeChunkedCurl(hnd, &chunk, messageParameters);
  this->cleanupChunkedCurl(hnd, slist, &chunk);
}

void CurlConnectionProvider::getMessages(void) {
  this->m_polling = true;

  // Initializations
  CURL *hnd;

  struct MemoryStruct chunk;
  // Will be grown as needed by the realloc above
  chunk.memory = reinterpret_cast<char*>(malloc(1));
  // No data at this point
  chunk.size = 0;

  struct curl_slist *slist = NULL;
  hnd = curl_easy_init();

  this->setCurlUrl(hnd);
  slist = this->setCurlHeaders(hnd, slist);
  this->setCurlBody(hnd);
  this->setCurlSSL(hnd);
  this->setChunkedCurlCallback(hnd, &chunk);

  MessageParameters messageParameters;
  this->executeChunkedCurl(hnd, &chunk, messageParameters);
  this->cleanupChunkedCurl(hnd, slist, &chunk);
}

size_t CurlConnectionProvider::getMessagesCallback(char *content, size_t size, size_t nmemb, void *member) {
  size_t realsize = size * nmemb;
  static int pollCount = 1;

  CurlConnectionProvider *self = reinterpret_cast<CurlConnectionProvider*>(member);

  // Convert char* to string including some necessary modifications
  char *tmp = new char[realsize];
  strncpy(tmp, content, realsize);
  tmp[realsize] = '\0';
  std::string message(tmp);

  struct timeval tv;

  int currentPollingTime = pollCount * self->settings->getPollingInterval();
  if (currentPollingTime >= self->settings->getPollingMaxTime()) {
    self->callback(content, realsize, 1, self->member);
  } else if (message == "[]") {
    if (self->settings->getPollingInterval() == 0) {
      // No polling required

      // Call callback of caller with member=this->member
      return realsize;
    }

    // Poll for messages depending on intervall
    tv.tv_sec = self->settings->getPollingInterval();
    tv.tv_usec = 0;
    select(0, NULL, NULL, NULL, &tv);

    pollCount++;
    self->getMessages();
  } else {
    // Received entire message: call callback
    self->callback(content, realsize, 1, self->member);
  }

  delete [] tmp;

  // Has to be returned for successful curl communication
  return realsize;
}

// For chunked curl responses see: https://curl.haxx.se/libcurl/c/getinmemory.html
size_t CurlConnectionProvider::chunkedResponseCallback(char *content, size_t size, size_t nmemb, void *userp) {
  size_t realsize = size * nmemb;
  MemoryStruct *mem = reinterpret_cast<MemoryStruct*>(userp);

  int memorySize = mem->size + realsize + 1;
  mem->memory = reinterpret_cast<char*>(realloc(mem->memory, memorySize));

  if (mem->memory == NULL) {
    /* out of memory! */
    // printf("not enough memory (realloc returned NULL)\n");
    return 0;
  }

  // Add to MemoryStruct
  memcpy(&(mem->memory[mem->size]), content, realsize);
  mem->size += realsize;
  mem->memory[mem->size] = 0;

  // Has to be returned for successful curl communication
  return realsize;
}

void CurlConnectionProvider::setCurlUrl(CURL *hnd) {
  if (this->url != "") {
    curl_easy_setopt(hnd, CURLOPT_URL, this->url.c_str());
  }
}

struct curl_slist *CurlConnectionProvider::setCurlHeaders(CURL *hnd, struct curl_slist *slist) {
  slist = NULL;
  for (size_t i = 0; i < this->headers.size(); i++) {
    slist = curl_slist_append(slist, this->headers[i].c_str());
  }

  // Only set header if header array contains elements
  if (this->headers.size() > 0) {
    curl_easy_setopt(hnd, CURLOPT_HTTPHEADER, slist);
  }

  return slist;
}

void CurlConnectionProvider::setCurlBody(CURL *hnd) {
  if (this->body != "") {
    curl_easy_setopt(hnd, CURLOPT_POSTFIELDS, this->body.c_str());
  }
}

// For chunked curl responses see: https://curl.haxx.se/libcurl/c/getinmemory.html
void CurlConnectionProvider::setChunkedCurlCallback(CURL *hnd, MemoryStruct *chunk) {
  /* send all data to this function  */
  curl_easy_setopt(hnd, CURLOPT_WRITEFUNCTION, chunkedResponseCallback);

  /* we pass our 'chunk' struct to the callback function */
  curl_easy_setopt(hnd, CURLOPT_WRITEDATA, reinterpret_cast<void*>(chunk));
}

void CurlConnectionProvider::setCurlSSL(CURL *hnd) {
  if (this->settings->getCertificatePath() != "") {
    curl_easy_setopt(hnd, CURLOPT_SSLCERT, this->settings->getCertificatePath().c_str());
  }

  // Set private key
  if (this->settings->getPrivateKeyPath() != "") {
    curl_easy_setopt(hnd, CURLOPT_SSLKEY, this->settings->getPrivateKeyPath().c_str());
  }

  if (this->settings->getConnectionParameters().secret != "") {
    curl_easy_setopt(hnd, CURLOPT_KEYPASSWD, this->settings->getConnectionParameters().secret.c_str());
  }
}

// For chunked curl responses see: https://curl.haxx.se/libcurl/c/getinmemory.html
void CurlConnectionProvider::executeChunkedCurl(CURL *hnd, MemoryStruct *chunk, MessageParameters messageParameters) {
  CURLcode res;

  // For logging purposes
  curl_easy_setopt(hnd, CURLOPT_VERBOSE, 1L);

  res = curl_easy_perform(hnd);

  /* check for errors */
  if (res != CURLE_OK) {
    int32_t httpCode = 0;
    curl_easy_getinfo(hnd, CURLINFO_RESPONSE_CODE, &httpCode);
    this->settings->callOnError(httpCode, std::string(curl_easy_strerror(res)), messageParameters);

  } else {
    /*
    * Now, our chunk.memory points to a memory block that is chunk.size
    * bytes big and contains the remote file.
    *
    * Do something nice with it!
    */

    int32_t httpCode = 0;
    curl_easy_getinfo(hnd, CURLINFO_RESPONSE_CODE, &httpCode);
    // printf("HTTP Response Code %li\n", httpCode);

    if ((httpCode != 200) && (httpCode != 201)) {
      // Something went wrong
      this->settings->callOnError(httpCode, std::string(curl_easy_strerror(res)), messageParameters);
      return;
    }

    if (this->m_polling) {
      // Poll for new messages
      this->getMessagesCallback(chunk->memory, chunk->size, 1, this);
    } else {
      // No polling, call callback instead
      (this->callback)(chunk->memory, chunk->size, 1, this->member);
    }
  }
}

// For chunked curl responses see: https://curl.haxx.se/libcurl/c/getinmemory.html
void CurlConnectionProvider::cleanupChunkedCurl(CURL *hnd, struct curl_slist *slist, MemoryStruct *chunk) {
  curl_easy_cleanup(hnd);
  hnd = NULL;
  curl_slist_free_all(slist);
  slist = NULL;

  free(chunk->memory);

  /* we're done with libcurl, so clean it up */
}
