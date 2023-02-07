#ifndef LIB_AGRIROUTERCLIENT_INC_CONNECTIONPROVIDER_H_
#define LIB_AGRIROUTERCLIENT_INC_CONNECTIONPROVIDER_H_

#include "Settings.h"

#include <string>
#include <vector>

class ConnectionProvider
{
    public:
        // Send message and calls callback function when receiving a response
        virtual void sendMessage(MessageParameters messageParameters) = 0;
        // Get messages and calls callback function when receiving a response
        virtual void getMessages(void) = 0;

        // Used to onboard to agrirouter
        virtual void onboard(MessageParameters messageParameters) {}

        virtual ~ConnectionProvider() {}

        // Function pointer for callback functions
        typedef size_t (*Callback)(char *content, size_t size, size_t nmemb, void *member);

        // Setter and Getter
        void setSettings(Settings *settings);
        Settings *getSettings();

        void setBody(std::string body);
        std::string getBody();
        void setUrl(std::string url);
        std::string getUrl();
        void setHeaders(std::vector<std::string> headers);
        std::vector<std::string> getHeaders();
        void setCallback(Callback callback);
        Callback getCallback();
        void setMember(void *member);
        void *getMember();

    protected:
        Settings *m_settings;
        std::string m_body;
        std::string m_url;
        std::vector<std::string> m_headers;
        std::string m_applicationMessageId;
        Callback m_callback;
        void *m_member;
};

#endif  // LIB_AGRIROUTERCLIENT_INC_CONNECTIONPROVIDER_H_
