#ifndef LIB_AGRIROUTERCLIENT_INC_REGISTRATION_H_
#define LIB_AGRIROUTERCLIENT_INC_REGISTRATION_H_

#include "ConnectionProvider.h"
#include "Definitions.h"
#include "Settings.h"

#include <string>
#include <vector>

struct cJSON;

class Registration
{
    public:
        Registration(ConnectionProvider *connectionProvider, Settings *settings, void *member);
        ~Registration();

        /**
        * Send request to finish onboarding/registration process.
        */
        void sendOnboard(const std::string& registrationCode, const AgrirouterSettings& agrirouterSettings);

        /**
        * Write callback of get endpoint registration code.
        *
        * @return The size of the incoming data.
        */
        static size_t sendOnboardCallback(char *content, size_t size, size_t nmemb, void *member);

        /**
        * Checks if string contains an error message.
        *
        * @return Contains error or not.
        */
        static bool containsError(const std::string& message);

        /**
        * To get key and pem outof onboard message.
        * 
        *  @return parsed ConnectionParameters from message
        */
        ConnectionParameters parseParametersAndCertificates(const std::string& message, void *member);

        /**
        * To get key and pem outof onboard message.
        * Certificate and private key are only stored in the settings if the message is valid.
        *
        *  @return true if all required fields are present and the certificate could be parsed
        */
        bool parseParametersAndCertificates(const std::string& message, ConnectionParameters& parameters);

        /**
        * Define registration callback
        */
        typedef void (*RegistrationCallback)(bool success, void *member);

        /**
        * Set a callback function for callback
        */
        void setCallback(RegistrationCallback registrationCallback);

    private:
        ConnectionProvider *m_connectionProvider = nullptr;
        Settings *m_settings = nullptr;
        void *m_member = nullptr;
        RegistrationCallback m_callback;

        std::string m_registrationCode = "";

        static bool getJsonString(cJSON *object, const char *name, std::string& value);
};

#endif //  LIB_AGRIROUTERCLIENT_INC_REGISTRATION_H_
