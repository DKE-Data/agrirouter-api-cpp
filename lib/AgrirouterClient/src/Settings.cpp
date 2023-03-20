#include "Settings.h"

#include <string>

Settings::Settings() 
    : m_certificateCaPath("/etc/ssl/certs/"),
      m_acceptSelfSignedCertificate(false)
{}

Settings::~Settings() {}

void Settings::callOnParameterChange(int event, void *data)
{
    this->m_onParameter(event, data, this->m_callbackCallee);
}

void Settings::callOnMessage(Response *response, MessageParameters messageParameters)
{
    this->m_onMessage(messageParameters.event, response,
                  messageParameters.applicationMessageId, this->m_callbackCallee);
}

void Settings::callOnError(int statusCode, int connectionProviderErrorCode, std::string errorMessage,
        MessageParameters messageParameters, std::string errorContent)
{
    this->m_onError(statusCode, connectionProviderErrorCode, errorMessage,
                  messageParameters.applicationMessageId, errorContent, this->m_callbackCallee);
}

void Settings::callOnLog(int logLevel, std::string logMessage)
{
    this->m_onLog(logLevel, logMessage, this->m_callbackCallee);
}

ConnectionParameters Settings::getConnectionParameters(std::string absolutePath)
{
    ConnectionParameters parameters = getSavedConnectionParameters(absolutePath);
    this->setConnectionParameters(parameters);

    return parameters;
}

/*
 * Getter and Setter
 */

void Settings::setOnParameterChangeCallback(onParameterChangeCallback onParameter)
{
    this->m_onParameter = onParameter;
}

Settings::onParameterChangeCallback Settings::getOnParameterChangeCallback()
{
    return this->m_onParameter;
}

void Settings::setOnMessageCallback(onMessageCallback onMessage)
{
    this->m_onMessage = onMessage;
}

Settings::onMessageCallback Settings::getOnMessageCallback()
{
    return this->m_onMessage;
}

void Settings::setOnErrorCallback(onErrorCallback onError)
{
    this->m_onError = onError;
}

Settings::onErrorCallback Settings::getOnErrorCallback()
{
    return this->m_onError;
}

void Settings::setOnLoggingCallback(onLoggingCallback onLog)
{
    this->m_onLog = onLog;
}

Settings::onLoggingCallback Settings::getOnLoggingCallback()
{
    return this->m_onLog;
}

void Settings::setCallbackCallee(void *callbackCallee)
{
    this->m_callbackCallee = callbackCallee;
}

void *Settings::getCallbackCallee() { return this->m_callbackCallee; }

void Settings::setAuthUsername(std::string authUsername)
{
    this->m_authUsername = authUsername;
    this->m_onParameter(MG_PARAMETER_AUTH_USERNAME,
                    static_cast<void *>(&authUsername),
                    this->m_callbackCallee);
}

std::string Settings::getAuthUsername() { return this->m_authUsername; }

void Settings::setAuthPassword(std::string authPassword)
{
    this->m_authPassword = authPassword;
    this->m_onParameter(MG_PARAMETER_AUTH_PASSWORD,
                    static_cast<void *>(&authPassword),
                    this->m_callbackCallee);
}

std::string Settings::getAuthPassword() { return this->m_authPassword; }

void Settings::setAccountId(std::string accountId)
{
    this->m_accountId = accountId;
    this->m_onParameter(MG_PARAMETER_ACCOUNT_ID,
                    static_cast<void *>(&accountId),
                    this->m_callbackCallee);
}

std::string Settings::getAccountId() { return this->m_accountId; }

void Settings::setApplicationId(std::string applicationId)
{
    this->m_applicationId = applicationId;
    this->m_onParameter(MG_PARAMETER_APPLICATION_ID,
                    static_cast<void *>(&applicationId),
                    this->m_callbackCallee);
}

std::string Settings::getApplicationId() { return this->m_applicationId; }

void Settings::setCertificationVersionId(std::string certificationVersionId)
{
    this->m_certificationVersionId = certificationVersionId;
    this->m_onParameter(MG_PARAMETER_CERTIFICATION_VERSION_ID,
                    static_cast<void *>(&certificationVersionId),
                    this->m_callbackCallee);
}

std::string Settings::getCertificationVersionId()
{
    return this->m_certificationVersionId;
}

void Settings::setOnboardId(std::string onboardId)
{
    this->m_onboardId = onboardId;
    this->m_onParameter(MG_PARAMETER_ONBOARD_ID,
                    static_cast<void *>(&onboardId),
                    this->m_callbackCallee);
}

std::string Settings::getOnboardId() { return this->m_onboardId; }

void Settings::setGatewayId(std::string gatewayId)
{
    this->m_gatewayId = gatewayId;
    this->m_onParameter(MG_PARAMETER_GATEWAY_ID,
                    static_cast<void *>(&gatewayId),
                    this->m_callbackCallee);
}

std::string Settings::getGatewayId() { return this->m_gatewayId; }

void Settings::setCertificateType(std::string certificateType)
{
    this->m_certificateType = certificateType;
    this->m_onParameter(MG_PARAMETER_CERTIFICATE_TYPE,
                    static_cast<void *>(&certificateType),
                    this->m_callbackCallee);
}

std::string Settings::getCertificateType() { return this->m_certificateType; }

void Settings::setCertificate(std::string certificate)
{
    this->m_certificate = certificate;
    this->m_onParameter(MG_PARAMETER_CERTIFICATE,
                    static_cast<void *>(&certificate),
                    this->m_callbackCallee);
}

std::string& Settings::getCertificate() { return this->m_certificate; }

void Settings::setCertificatePath(std::string certificatePath)
{
    this->m_certificatePath = certificatePath;
    this->m_onParameter(MG_PARAMETER_CERTIFICATE_PATH,
                    static_cast<void *>(&certificatePath),
                    this->m_callbackCallee);
}

std::string& Settings::getCaFilePath() { return this->m_caFilePath; }

void Settings::setCaFilePath(std::string caFilePath)
{
    this->m_caFilePath = caFilePath;
    this->m_onParameter(MG_PARAMETER_CERTIFICATE_PATH,
                    static_cast<void *>(&caFilePath),
                    this->m_callbackCallee);
}

std::string& Settings::getCertificatePath() { return this->m_certificatePath; }

void Settings::setPrivateKey(std::string privateKey)
{
    this->m_privateKey = privateKey;
    this->m_onParameter(MG_PARAMETER_PRIVATE_KEY,
                    static_cast<void *>(&privateKey),
                    this->m_callbackCallee);
}

std::string& Settings::getPrivateKey() { return this->m_privateKey; }

void Settings::setPrivateKeyPath(std::string privateKeyPath)
{
    this->m_privateKeyPath = privateKeyPath;
    this->m_onParameter(MG_PARAMETER_PRIVATE_KEY_PATH,
                    static_cast<void *>(&privateKeyPath),
                    this->m_callbackCallee);
}

std::string& Settings::getPrivateKeyPath() { return this->m_privateKeyPath; }

void Settings::setEncodingType(std::string encodingType)
{
    this->m_encodingType = encodingType;
    this->m_onParameter(MG_PARAMETER_ENCODING_TYPE,
                    static_cast<void *>(&encodingType),
                    this->m_callbackCallee);
}

std::string& Settings::getEncodingType() { return this->m_encodingType; }

void Settings::setConnectionType(ConnectionType connectionType)
{
    this->m_connectionType = connectionType;
    this->m_onParameter(MG_PARAMETER_CONNECTION_TYPE,
                    static_cast<void *>(&connectionType),
                    this->m_callbackCallee);
}

Settings::ConnectionType Settings::getConnectionType()
{
  return this->m_connectionType;
}

void Settings::setConnectionParameters(ConnectionParameters connectionParameters, bool withCallback) // withCallback default is true
{
    this->m_connectionParameters = connectionParameters;
    if(withCallback)
    {
        this->m_onParameter(MG_PARAMETER_CONNECTION_PARAMETERS,
                    static_cast<void *>(&this->m_connectionParameters),
                    this->m_callbackCallee);
    }
}

ConnectionParameters& Settings::getConnectionParameters()
{
    return this->m_connectionParameters;
}

void Settings::setConnectionParametersPath(std::string connectionParametersPath)
{
    this->m_connectionParametersPath = connectionParametersPath;
    this->m_onParameter(MG_PARAMETER_CONNECTION_PARAMETERS_PATH,
                    static_cast<void *>(&connectionParametersPath),
                    this->m_callbackCallee);
}

std::string& Settings::getConnectionParametersPath()
{
    return this->m_connectionParametersPath;
}

void Settings::setCertificateCaPath(std::string certificateCaPath)
{
    this->m_certificateCaPath = certificateCaPath;
}

std::string& Settings::getCertificateCaPath()
{
    return this->m_certificateCaPath;
}

void Settings::setAcceptSelfSignedCertificate(bool a_accept)
{
    this->m_acceptSelfSignedCertificate = a_accept;
}

bool Settings::acceptSelfSignedCertificate()
{
    return m_acceptSelfSignedCertificate;
}

void Settings::setPollingInterval(int pollingInterval)
{
    this->m_pollingInterval = pollingInterval;
    this->m_onParameter(MG_PARAMETER_POLLING_INTERVAL,
                    static_cast<void *>(&pollingInterval),
                    this->m_callbackCallee);
}

int Settings::getPollingInterval() { return this->m_pollingInterval; }

void Settings::setPollingMaxTime(int pollingMaxTime)
{
    this->m_pollingMaxTime = pollingMaxTime;
    this->m_onParameter(MG_PARAMETER_POLLING_MAX_TIME,
                    static_cast<void *>(&pollingMaxTime),
                    this->m_callbackCallee);
}

int Settings::getPollingMaxTime() { return this->m_pollingMaxTime; }
