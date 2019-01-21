#include <Settings.h>

#include <string>

Settings::Settings()
    : m_acceptSelfSignedCertificate(false)
{
  this->setOnParameter = false;
  this->setOnMessage = false;
  this->setOnError = false;
}

Settings::~Settings() {}

void Settings::callOnParameterChange(int event, void *data) {
  if (this->setOnParameter) {
    this->onParameter(event, data, this->parameterCallbackCallee);
  }
}

void Settings::callOnMessage(Response *response, MessageParameters messageParameters) {
  if (this->setOnMessage) {
    this->onMessage(messageParameters.event, response, messageParameters.applicationMessageId, this->messageCallbackCallee);
  }
}

void Settings::callOnError(int code, std::string message, MessageParameters messageParameters) {
  if (this->setOnError) {
    this->onError(code, message, messageParameters.applicationMessageId, this->errorCallbackCallee);
  }
}

ConnectionParameters Settings::getConnectionParameters(std::string absolutePath) {
  ConnectionParameters parameters = getSavedConnectionParameters(absolutePath);
  this->setConnectionParameters(parameters);

  return parameters;
}

/*
 * Getter and Setter
 */

void Settings::setOnParameterChangeCallback(onParameterChangeCallback onParameter, void* callbackCallee) {
  this->setOnParameter = true;
  this->onParameter = onParameter;
  this->parameterCallbackCallee = callbackCallee;
}

Settings::onParameterChangeCallback Settings::getOnParameterChangeCallback() {
  return this->onParameter;
}

void Settings::setOnMessageCallback(onMessageCallback onMessage, void* callbackCallee) {
  this->setOnMessage = true;
  this->onMessage = onMessage;
  this->messageCallbackCallee = callbackCallee;
}

Settings::onMessageCallback Settings::getOnMessageCallback() {
  return this->onMessage;
}

void Settings::setOnErrorCallback(onErrorCallback onError, void* callbackCallee) {
  this->setOnError = true;
  this->onError = onError;
  this->errorCallbackCallee = callbackCallee;
}

Settings::onErrorCallback Settings::getOnErrorCallback() {
  return this->onError;
}

void Settings::setAuthUsername(std::string authUsername) {
  this->authUsername = authUsername;
  this->callOnParameterChange(MG_PARAMETER_AUTH_USERNAME, reinterpret_cast<void *>(&authUsername));
}

std::string Settings::getAuthUsername() {
  return this->authUsername;
}

void Settings::setAuthPassword(std::string authPassword) {
  this->authPassword = authPassword;
  this->callOnParameterChange(MG_PARAMETER_AUTH_PASSWORD, reinterpret_cast<void *>(&authPassword));
}

std::string Settings::getAuthPassword() {
  return this->authPassword;
}

void Settings::setAccountId(std::string accountId) {
  this->accountId = accountId;
  this->callOnParameterChange(MG_PARAMETER_ACCOUNT_ID, reinterpret_cast<void *>(&accountId));
}

std::string Settings::getAccountId() {
  return this->accountId;
}

void Settings::setApplicationId(std::string applicationId) {
  this->applicationId = applicationId;
  this->callOnParameterChange(MG_PARAMETER_APPLICATION_ID, reinterpret_cast<void *>(&applicationId));
}

std::string Settings::getApplicationId() {
  return this->applicationId;
}

void Settings::setCertificationVersionId(std::string certificationVersionId) {
  this->certificationVersionId = certificationVersionId;
  this->callOnParameterChange(MG_PARAMETER_CERTIFICATION_VERSION_ID, reinterpret_cast<void *>(&certificationVersionId));
}

std::string Settings::getCertificationVersionId() {
  return this->certificationVersionId;
}

void Settings::setExternalId(std::string externalId) {
  this->externalId = externalId;
  this->callOnParameterChange(MG_PARAMETER_EXTERNAL_ID, reinterpret_cast<void *>(&externalId));
}

std::string Settings::getExternalId() {
  return this->externalId;
}

void Settings::setGatewayId(std::string gatewayId) {
  this->gatewayId = gatewayId;
  this->callOnParameterChange(MG_PARAMETER_GATEWAY_ID, reinterpret_cast<void *>(&gatewayId));
}

std::string Settings::getGatewayId() {
  return this->gatewayId;
}

void Settings::setCertificateType(std::string certificateType) {
  this->certificateType = certificateType;
  this->callOnParameterChange(MG_PARAMETER_CERTIFICATE_TYPE, reinterpret_cast<void *>(&certificateType));
}

std::string Settings::getCertificateType() {
  return this->certificateType;
}

void Settings::setCertificate(std::string certificate) {
  this->certificate = certificate;
  this->callOnParameterChange(MG_PARAMETER_CERTIFICATE, reinterpret_cast<void *>(&certificate));
}

std::string Settings::getCertificate() {
  return this->certificate;
}

void Settings::setCertificatePath(std::string certificatePath) {
  this->certificatePath = certificatePath;
  this->callOnParameterChange(MG_PARAMETER_CERTIFICATE_PATH, reinterpret_cast<void *>(&certificatePath));
}

std::string Settings::getCaFilePath() {
  return this->caFilePath;
}

void Settings::setCaFilePath(std::string caFilePath) {
  this->caFilePath = caFilePath;
  this->callOnParameterChange(MG_PARAMETER_CERTIFICATE_PATH, reinterpret_cast<void *>(&caFilePath));
}

std::string Settings::getCertificatePath() {
  return this->certificatePath;
}

void Settings::setPrivateKey(std::string privateKey) {
  this->privateKey = privateKey;
  this->callOnParameterChange(MG_PARAMETER_PRIVATE_KEY, reinterpret_cast<void *>(&privateKey));
}

std::string Settings::getPrivateKey() {
  return this->privateKey;
}

void Settings::setPrivateKeyPath(std::string privateKeyPath) {
  this->privateKeyPath = privateKeyPath;
  this->callOnParameterChange(MG_PARAMETER_PRIVATE_KEY_PATH, reinterpret_cast<void *>(&privateKeyPath));
}

std::string Settings::getPrivateKeyPath() {
  return this->privateKeyPath;
}

void Settings::setEncodingType(std::string encodingType) {
  this->encodingType = encodingType;
  this->callOnParameterChange(MG_PARAMETER_ENCODING_TYPE, reinterpret_cast<void *>(&encodingType));
}

std::string Settings::getEncodingType() {
  return this->encodingType;
}

void Settings::setConnectionType(ConnectionType connectionType) {
  this->connectionType = connectionType;
  this->callOnParameterChange(MG_PARAMETER_CONNECTION_TYPE, reinterpret_cast<void *>(&connectionType));
}

Settings::ConnectionType Settings::getConnectionType() {
  return this->connectionType;
}

void Settings::setConnectionParameters(ConnectionParameters connectionParameters) {
  this->connectionParameters = connectionParameters;
  this->callOnParameterChange(MG_PARAMETER_CONNECTION_PARAMETERS, reinterpret_cast<void *>(&connectionParameters));
}

ConnectionParameters Settings::getConnectionParameters() {
  return this->connectionParameters;
}

void Settings::setConnectionParametersPath(std::string connectionParametersPath) {
  this->connectionParametersPath = connectionParametersPath;
  this->callOnParameterChange(MG_PARAMETER_CONNECTION_PARAMETERS_PATH, reinterpret_cast<void *>(&connectionParametersPath));
}

std::string Settings::getConnectionParametersPath() {
  return this->connectionParametersPath;
}

void Settings::setPollingInterval(int pollingInterval) {
  this->pollingInterval = pollingInterval;
  this->callOnParameterChange(MG_PARAMETER_POLLING_INTERVAL, reinterpret_cast<void *>(&pollingInterval));
}

int Settings::getPollingInterval() {
  return this->pollingInterval;
}

void Settings::setPollingMaxTime(int pollingMaxTime) {
  this->pollingMaxTime = pollingMaxTime;
  this->callOnParameterChange(MG_PARAMETER_POLLING_MAX_TIME, reinterpret_cast<void *>(&pollingMaxTime));
}

int Settings::getPollingMaxTime() {
  return this->pollingMaxTime;
}

void Settings::setAcceptSelfSignedCertificate(bool a_accept)
{
    m_acceptSelfSignedCertificate = a_accept;
}

bool Settings::acceptSelfSignedCertificate() const
{
    return m_acceptSelfSignedCertificate;
}

