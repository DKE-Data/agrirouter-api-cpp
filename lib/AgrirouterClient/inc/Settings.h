#ifndef LIB_AGRIROUTERCLIENT_INC_SETTINGS_H_
#define LIB_AGRIROUTERCLIENT_INC_SETTINGS_H_

#include "AgrirouterMessage.h"
#include "Definitions.h"
#include <google/protobuf/message.h>
#include <string>

/*
* ToDo: Define which parameters need a restart of the agrirouter client
*/

class Settings
{
    public:
        enum ConnectionType { HTTP = 1, MQTT = 2 };

        typedef void (*onParameterChangeCallback) (int event, void *data, void *callbackCallee);
        typedef void (*onMessageCallback) (int event, Response *response, std::string applicationMessageId, void *callbackCallee);
        typedef void (*onErrorCallback) (int statusCode, int connectionProviderErrorCode, std::string errorMessage, 
                                            std::string applicationMessageId, std::string errorContent, void *callbackCallee);

        Settings();
        ~Settings();

        void callOnParameterChange(int event, void *data);
        void callOnMessage(Response *response, MessageParameters messageParameters);
        void callOnError(int statusCode, int connectionProviderErrorCode, std::string errorMessage, MessageParameters messageParameters, std::string errorContent);

        // On parameter change callback
        void setOnParameterChangeCallback(onParameterChangeCallback onParameter);
        onParameterChangeCallback getOnParameterChangeCallback();
        // On message change callback
        void setOnMessageCallback(onMessageCallback onMessage);
        onMessageCallback getOnMessageCallback();
        // On error change callback
        void setOnErrorCallback(onErrorCallback onError);
        onErrorCallback getOnErrorCallback();
        // Callback callee
        void setCallbackCallee(void *callbackCallee);
        void *getCallbackCallee();

        ConnectionParameters getConnectionParameters(std::string absolutePath);

        void setAuthUsername(std::string authUsername);
        std::string getAuthUsername();
        void setAuthPassword(std::string authPassword);
        std::string getAuthPassword();
        void setAccountId(std::string accountId);
        std::string getAccountId();
        void setApplicationId(std::string applicationId);
        std::string getApplicationId();
        void setCertificationVersionId(std::string certificationVersionId);
        std::string getCertificationVersionId();
        void setOnboardId(std::string onboardId);
        std::string getOnboardId();
        void setGatewayId(std::string gatewayId);
        std::string getGatewayId();
        void setCertificateType(std::string certificateType);
        std::string getCertificateType();

        void setCertificate(std::string certificate);
        std::string& getCertificate();
        void setCertificatePath(std::string certificatePath);
        std::string& getCertificatePath();
        void setCaFilePath(std::string caFilePath);
        std::string& getCaFilePath();
        void setPrivateKeyPath(std::string privateKeyPath);
        std::string& getPrivateKeyPath();
        void setPrivateKey(std::string privateKey);
        std::string& getPrivateKey();
        void setEncodingType(std::string encodingType);
        std::string& getEncodingType();
        void setConnectionType(ConnectionType connectionType);
        ConnectionType getConnectionType();
        void setConnectionParameters(ConnectionParameters connectionParameters, bool withCallback = true);
        ConnectionParameters& getConnectionParameters();
        void setConnectionParametersPath(std::string connectionParametersPath);
        std::string& getConnectionParametersPath();

        // path to root certificate to check agrirouter certificate (Default set /etc/ssl/certs/)
        void setCertificateCaPath(std::string certificateCaPath);
        std::string& getCertificateCaPath();

        void setAcceptSelfSignedCertificate(bool a_accept);
        bool acceptSelfSignedCertificate();

        void setPollingInterval(int pollingInterval);
        int getPollingInterval();
        void setPollingMaxTime(int pollingMaxTime);
        int getPollingMaxTime();

    private:
        onParameterChangeCallback m_onParameter;
        onMessageCallback m_onMessage;
        onErrorCallback m_onError;
        void *m_callbackCallee;

        // For onboarding
        std::string m_authUsername;
        std::string m_authPassword;
        std::string m_accountId;
        std::string m_applicationId;
        std::string m_certificationVersionId;
        std::string m_onboardId;
        std::string m_gatewayId;
        std::string m_certificateType;

        // For sending and receiving messages
        std::string m_certificate;
        std::string m_certificatePath;
        std::string m_caFilePath;
        std::string m_privateKey;
        std::string m_privateKeyPath;
        std::string m_encodingType;
        ConnectionType m_connectionType;
        ConnectionParameters m_connectionParameters;
        std::string m_connectionParametersPath;
        std::string m_certificateCaPath;
        bool m_acceptSelfSignedCertificate;

        // For general purposes
        int m_pollingInterval;
        int m_pollingMaxTime;
};

#endif  // LIB_AGRIROUTERCLIENT_INC_SETTINGS_H_
