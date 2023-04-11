#include "Settings.h"

#include <string>

Settings::Settings() 
    : m_certificateCaPath("/etc/ssl/certs/"),
      m_acceptSelfSignedCertificate(false)
{}

Settings::~Settings() {}

void Settings::callOnParameterChange(int event, void *data)
{
    m_onParameter(event, data, m_callbackCallee);
}

void Settings::callOnMessage(Response *response, MessageParameters messageParameters)
{
    m_onMessage(messageParameters.event, response,
                  messageParameters.applicationMessageId, m_callbackCallee);
}

void Settings::callOnError(int statusCode, int connectionProviderErrorCode, std::string errorMessage,
        MessageParameters messageParameters, std::string errorContent)
{
    m_onError(statusCode, connectionProviderErrorCode, errorMessage,
                  messageParameters.applicationMessageId, errorContent, m_callbackCallee);
}

void Settings::callOnLog(int logLevel, std::string logMessage)
{
    m_onLog(logLevel, logMessage, m_callbackCallee);
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
    m_onParameter = onParameter;
}

Settings::onParameterChangeCallback Settings::getOnParameterChangeCallback()
{
    return m_onParameter;
}

void Settings::setOnMessageCallback(onMessageCallback onMessage)
{
    m_onMessage = onMessage;
}

Settings::onMessageCallback Settings::getOnMessageCallback()
{
    return m_onMessage;
}

void Settings::setOnErrorCallback(onErrorCallback onError)
{
    m_onError = onError;
}

Settings::onErrorCallback Settings::getOnErrorCallback()
{
    return m_onError;
}

void Settings::setOnLoggingCallback(onLoggingCallback onLog)
{
    m_onLog = onLog;
}

Settings::onLoggingCallback Settings::getOnLoggingCallback()
{
    return m_onLog;
}

void Settings::setCallbackCallee(void *callbackCallee)
{
    m_callbackCallee = callbackCallee;
}

void *Settings::getCallbackCallee() { return m_callbackCallee; }

void Settings::setAuthUsername(std::string authUsername)
{
    m_authUsername = authUsername;
    m_onParameter(MG_PARAMETER_AUTH_USERNAME,
                    static_cast<void *>(&authUsername),
                    m_callbackCallee);
}

std::string Settings::getAuthUsername() { return m_authUsername; }

void Settings::setAuthPassword(std::string authPassword)
{
    m_authPassword = authPassword;
    m_onParameter(MG_PARAMETER_AUTH_PASSWORD,
                    static_cast<void *>(&authPassword),
                    m_callbackCallee);
}

std::string Settings::getAuthPassword() { return m_authPassword; }

void Settings::setAccountId(std::string accountId)
{
    m_accountId = accountId;
    m_onParameter(MG_PARAMETER_ACCOUNT_ID,
                    static_cast<void *>(&accountId),
                    m_callbackCallee);
}

std::string Settings::getAccountId() { return m_accountId; }

void Settings::setApplicationId(std::string applicationId)
{
    m_applicationId = applicationId;
    m_onParameter(MG_PARAMETER_APPLICATION_ID,
                    static_cast<void *>(&applicationId),
                    m_callbackCallee);
}

std::string Settings::getApplicationId() { return m_applicationId; }

void Settings::setCertificationVersionId(std::string certificationVersionId)
{
    m_certificationVersionId = certificationVersionId;
    m_onParameter(MG_PARAMETER_CERTIFICATION_VERSION_ID,
                    static_cast<void *>(&certificationVersionId),
                    m_callbackCallee);
}

std::string Settings::getCertificationVersionId()
{
    return m_certificationVersionId;
}

void Settings::setOnboardId(std::string onboardId)
{
    m_onboardId = onboardId;
    m_onParameter(MG_PARAMETER_ONBOARD_ID,
                    static_cast<void *>(&onboardId),
                    m_callbackCallee);
}

std::string Settings::getOnboardId() { return m_onboardId; }

void Settings::setGatewayId(std::string gatewayId)
{
    m_gatewayId = gatewayId;
    m_onParameter(MG_PARAMETER_GATEWAY_ID,
                    static_cast<void *>(&gatewayId),
                    m_callbackCallee);
}

std::string Settings::getGatewayId() { return m_gatewayId; }

void Settings::setCertificateType(std::string certificateType)
{
    m_certificateType = certificateType;
    m_onParameter(MG_PARAMETER_CERTIFICATE_TYPE,
                    static_cast<void *>(&certificateType),
                    m_callbackCallee);
}

std::string Settings::getCertificateType() { return m_certificateType; }

void Settings::setCertificate(std::string certificate)
{
    m_certificate = certificate;
    m_onParameter(MG_PARAMETER_CERTIFICATE,
                    static_cast<void *>(&certificate),
                    m_callbackCallee);
}

std::string& Settings::getCertificate() { return m_certificate; }

void Settings::setCertificatePath(std::string certificatePath)
{
    m_certificatePath = certificatePath;
    m_onParameter(MG_PARAMETER_CERTIFICATE_PATH,
                    static_cast<void *>(&certificatePath),
                    m_callbackCallee);
}

std::string& Settings::getCaFilePath() { return m_caFilePath; }

void Settings::setCaFilePath(std::string caFilePath)
{
    m_caFilePath = caFilePath;
    m_onParameter(MG_PARAMETER_CERTIFICATE_PATH,
                    static_cast<void *>(&caFilePath),
                    m_callbackCallee);
}

std::string& Settings::getCertificatePath() { return m_certificatePath; }

void Settings::setPrivateKey(std::string privateKey)
{
    m_privateKey = privateKey;
    m_onParameter(MG_PARAMETER_PRIVATE_KEY,
                    static_cast<void *>(&privateKey),
                    m_callbackCallee);
}

std::string& Settings::getPrivateKey() { return m_privateKey; }

void Settings::setPrivateKeyPath(std::string privateKeyPath)
{
    m_privateKeyPath = privateKeyPath;
    m_onParameter(MG_PARAMETER_PRIVATE_KEY_PATH,
                    static_cast<void *>(&privateKeyPath),
                    m_callbackCallee);
}

std::string& Settings::getPrivateKeyPath() { return m_privateKeyPath; }

void Settings::setEncodingType(std::string encodingType)
{
    m_encodingType = encodingType;
    m_onParameter(MG_PARAMETER_ENCODING_TYPE,
                    static_cast<void *>(&encodingType),
                    m_callbackCallee);
}

std::string& Settings::getEncodingType() { return m_encodingType; }

void Settings::setConnectionType(ConnectionType connectionType)
{
    m_connectionType = connectionType;
    m_onParameter(MG_PARAMETER_CONNECTION_TYPE,
                    static_cast<void *>(&connectionType),
                    m_callbackCallee);
}

Settings::ConnectionType Settings::getConnectionType()
{
  return m_connectionType;
}

void Settings::setConnectionParameters(ConnectionParameters connectionParameters, bool withCallback) // withCallback default is true
{
    m_connectionParameters = connectionParameters;
    if(withCallback)
    {
        m_onParameter(MG_PARAMETER_CONNECTION_PARAMETERS,
                    static_cast<void *>(&m_connectionParameters),
                    m_callbackCallee);
    }
}

ConnectionParameters& Settings::getConnectionParameters()
{
    return m_connectionParameters;
}

void Settings::setConnectionParametersPath(std::string connectionParametersPath)
{
    m_connectionParametersPath = connectionParametersPath;
    m_onParameter(MG_PARAMETER_CONNECTION_PARAMETERS_PATH,
                    static_cast<void *>(&connectionParametersPath),
                    m_callbackCallee);
}

std::string& Settings::getConnectionParametersPath()
{
    return m_connectionParametersPath;
}

void Settings::setCertificateCaPath(std::string certificateCaPath)
{
    m_certificateCaPath = certificateCaPath;
}

std::string& Settings::getCertificateCaPath()
{
    return m_certificateCaPath;
}

void Settings::setAcceptSelfSignedCertificate(bool a_accept)
{
    m_acceptSelfSignedCertificate = a_accept;
}

bool Settings::acceptSelfSignedCertificate()
{
    return m_acceptSelfSignedCertificate;
}

void Settings::setPollingInterval(int pollingInterval)
{
    m_pollingInterval = pollingInterval;
    m_onParameter(MG_PARAMETER_POLLING_INTERVAL,
                    static_cast<void *>(&pollingInterval),
                    m_callbackCallee);
}

int Settings::getPollingInterval() { return m_pollingInterval; }

void Settings::setPollingMaxTime(int pollingMaxTime)
{
    m_pollingMaxTime = pollingMaxTime;
    m_onParameter(MG_PARAMETER_POLLING_MAX_TIME,
                    static_cast<void *>(&pollingMaxTime),
                    m_callbackCallee);
}

int Settings::getPollingMaxTime() { return m_pollingMaxTime; }
