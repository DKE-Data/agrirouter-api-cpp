#include "Settings.h"

#include <string>

Settings::Settings() 
  : m_acceptSelfSignedCertificate(false) 
{}

Settings::~Settings() {}

void Settings::callOnParameterChange(int event, void *data)
{
  this->onParameter(event, data, this->callbackCallee);
}

void Settings::callOnMessage(Response *response, MessageParameters messageParameters)
{
  this->onMessage(messageParameters.event, response,
                  messageParameters.applicationMessageId, this->callbackCallee);
}

void Settings::callOnError(int statusCode, int connectionProviderErrorCode, std::string curlMessage, 
    MessageParameters messageParameters, std::string content)
{
  this->onError(statusCode, connectionProviderErrorCode, curlMessage, 
                  messageParameters.applicationMessageId, content, this->callbackCallee);
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
  this->onParameter = onParameter;
}

Settings::onParameterChangeCallback Settings::getOnParameterChangeCallback()
{
  return this->onParameter;
}

void Settings::setOnMessageCallback(onMessageCallback onMessage)
{
  this->onMessage = onMessage;
}

Settings::onMessageCallback Settings::getOnMessageCallback()
{
  return this->onMessage;
}

void Settings::setOnErrorCallback(onErrorCallback onError)
{
  this->onError = onError;
}

Settings::onErrorCallback Settings::getOnErrorCallback()
{
  return this->onError;
}

void Settings::setCallbackCallee(void *callbackCallee)
{
  this->callbackCallee = callbackCallee;
}

void *Settings::getCallbackCallee() { return this->callbackCallee; }

void Settings::setAuthUsername(std::string authUsername)
{
  this->authUsername = authUsername;
  this->onParameter(MG_PARAMETER_AUTH_USERNAME,
                    static_cast<void *>(&authUsername),
                    this->callbackCallee);
}

std::string Settings::getAuthUsername() { return this->authUsername; }

void Settings::setAuthPassword(std::string authPassword)
{
  this->authPassword = authPassword;
  this->onParameter(MG_PARAMETER_AUTH_PASSWORD,
                    static_cast<void *>(&authPassword),
                    this->callbackCallee);
}

std::string Settings::getAuthPassword() { return this->authPassword; }

void Settings::setAccountId(std::string accountId)
{
  this->accountId = accountId;
  this->onParameter(MG_PARAMETER_ACCOUNT_ID,
                    static_cast<void *>(&accountId),
                    this->callbackCallee);
}

std::string Settings::getAccountId() { return this->accountId; }

void Settings::setApplicationId(std::string applicationId)
{
  this->applicationId = applicationId;
  this->onParameter(MG_PARAMETER_APPLICATION_ID,
                    static_cast<void *>(&applicationId),
                    this->callbackCallee);
}

std::string Settings::getApplicationId() { return this->applicationId; }

void Settings::setCertificationVersionId(std::string certificationVersionId)
{
  this->certificationVersionId = certificationVersionId;
  this->onParameter(MG_PARAMETER_CERTIFICATION_VERSION_ID,
                    static_cast<void *>(&certificationVersionId),
                    this->callbackCallee);
}

std::string Settings::getCertificationVersionId()
{
  return this->certificationVersionId;
}

void Settings::setOnboardId(std::string onboardId)
{
  this->onboardId = onboardId;
  this->onParameter(MG_PARAMETER_ONBOARD_ID,
                    static_cast<void *>(&onboardId),
                    this->callbackCallee);
}

std::string Settings::getOnboardId() { return this->onboardId; }

void Settings::setGatewayId(std::string gatewayId)
{
  this->gatewayId = gatewayId;
  this->onParameter(MG_PARAMETER_GATEWAY_ID,
                    static_cast<void *>(&gatewayId),
                    this->callbackCallee);
}

std::string Settings::getGatewayId() { return this->gatewayId; }

void Settings::setCertificateType(std::string certificateType)
{
  this->certificateType = certificateType;
  this->onParameter(MG_PARAMETER_CERTIFICATE_TYPE,
                    static_cast<void *>(&certificateType),
                    this->callbackCallee);
}

std::string Settings::getCertificateType() { return this->certificateType; }

void Settings::setCertificate(std::string certificate)
{
  this->certificate = certificate;
  this->onParameter(MG_PARAMETER_CERTIFICATE,
                    static_cast<void *>(&certificate),
                    this->callbackCallee);
}

std::string& Settings::getCertificate() { return this->certificate; }

void Settings::setCertificatePath(std::string certificatePath)
{
  this->certificatePath = certificatePath;
  this->onParameter(MG_PARAMETER_CERTIFICATE_PATH,
                    static_cast<void *>(&certificatePath),
                    this->callbackCallee);
}

std::string& Settings::getCaFilePath() { return this->caFilePath; }

void Settings::setCaFilePath(std::string caFilePath)
{
  this->caFilePath = caFilePath;
  this->onParameter(MG_PARAMETER_CERTIFICATE_PATH,
                    static_cast<void *>(&caFilePath),
                    this->callbackCallee);
}

std::string& Settings::getCertificatePath() { return this->certificatePath; }

void Settings::setPrivateKey(std::string privateKey)
{
  this->privateKey = privateKey;
  this->onParameter(MG_PARAMETER_PRIVATE_KEY,
                    static_cast<void *>(&privateKey),
                    this->callbackCallee);
}

std::string& Settings::getPrivateKey() { return this->privateKey; }

void Settings::setPrivateKeyPath(std::string privateKeyPath)
{
  this->privateKeyPath = privateKeyPath;
  this->onParameter(MG_PARAMETER_PRIVATE_KEY_PATH,
                    static_cast<void *>(&privateKeyPath),
                    this->callbackCallee);
}

std::string& Settings::getPrivateKeyPath() { return this->privateKeyPath; }

void Settings::setEncodingType(std::string encodingType)
{
  this->encodingType = encodingType;
  this->onParameter(MG_PARAMETER_ENCODING_TYPE,
                    static_cast<void *>(&encodingType),
                    this->callbackCallee);
}

std::string& Settings::getEncodingType() { return this->encodingType; }

void Settings::setConnectionType(ConnectionType connectionType)
{
  this->connectionType = connectionType;
  this->onParameter(MG_PARAMETER_CONNECTION_TYPE,
                    static_cast<void *>(&connectionType),
                    this->callbackCallee);
}

Settings::ConnectionType Settings::getConnectionType()
{
  return this->connectionType;
}

void Settings::setConnectionParameters(
    ConnectionParameters connectionParameters)
{
  this->connectionParameters = connectionParameters;
  this->onParameter(MG_PARAMETER_CONNECTION_PARAMETERS,
                    static_cast<void *>(&connectionParameters),
                    this->callbackCallee);
}

ConnectionParameters& Settings::getConnectionParameters()
{
  return this->connectionParameters;
}

void Settings::setConnectionParametersPath(std::string connectionParametersPath)
{
  this->connectionParametersPath = connectionParametersPath;
  this->onParameter(MG_PARAMETER_CONNECTION_PARAMETERS_PATH,
                    static_cast<void *>(&connectionParametersPath),
                    this->callbackCallee);
}

std::string& Settings::getConnectionParametersPath()
{
  return this->connectionParametersPath;
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
  this->pollingInterval = pollingInterval;
  this->onParameter(MG_PARAMETER_POLLING_INTERVAL,
                    static_cast<void *>(&pollingInterval),
                    this->callbackCallee);
}

int Settings::getPollingInterval() { return this->pollingInterval; }

void Settings::setPollingMaxTime(int pollingMaxTime)
{
  this->pollingMaxTime = pollingMaxTime;
  this->onParameter(MG_PARAMETER_POLLING_MAX_TIME,
                    static_cast<void *>(&pollingMaxTime),
                    this->callbackCallee);
}

int Settings::getPollingMaxTime() { return this->pollingMaxTime; }
