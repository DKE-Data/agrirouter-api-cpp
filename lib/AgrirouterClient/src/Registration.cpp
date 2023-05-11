#include "Registration.h"

#include "Utils.h"
#include "CurlConnectionProvider.h"
#include "../third_party/cJSON/cJSON.h"

#include <curl/curl.h>
#include <stdio.h>
#include <cstring>
#include <string>
#include <iostream>
#include <vector>

Registration::Registration(ConnectionProvider *connectionProvider, Settings *settings, void *member)
{
    m_settings = settings;
    m_connectionProvider = connectionProvider;
    m_member = member;
}

Registration::~Registration() {}

void Registration::registerToAgrirouterWithRegCode(const std::string& registrationCode, AgrirouterSettings& agrirouterSettings)
{
    m_registrationCode = registrationCode;

    this->sendOnboard(agrirouterSettings);
}

void Registration::sendOnboard(AgrirouterSettings& agrirouterSettings)
{
    // Set headers
    std::vector<std::string> headers;

    std::string authorization_header = "Authorization: Bearer " + m_registrationCode;
    headers.push_back(authorization_header);
    std::string content_type_header = "Content-Type: application/json";
    headers.push_back(content_type_header);

    std::string body = "{\"id\":\"" + m_settings->getOnboardId() + "\",\"applicationId\":\"" +
        m_settings->getApplicationId() + "\",\"certificationVersionId\":\"" +
        m_settings->getCertificationVersionId() + "\",\"gatewayId\":\"" +
        m_settings->getGatewayId() + "\",\"certificateType\":\"" + "PEM" + "\"}";

    std::string url = agrirouterSettings.registrationUrl;

    // change to curl connection provider, because onbarding is every time http
    CurlConnectionProvider connectionProvider = CurlConnectionProvider(m_settings);
    connectionProvider.setBody(body);
    connectionProvider.setUrl(url);
    connectionProvider.setHeaders(headers);
    connectionProvider.setCallback(sendOnboardCallback);
    connectionProvider.setMember(this);

    // ToDo: any application message id available?
    MessageParameters messageParameters;
    messageParameters.member = static_cast<void *>(this);

    connectionProvider.onboard(messageParameters);
}

size_t Registration::sendOnboardCallback(char *content, size_t size, size_t nmemb, void *member)
{
    size_t realsize = size * nmemb;
    Registration *self = static_cast<Registration *>(member);
    std::string message(content, realsize);

    if (containsError(message))
    {
        self->m_settings->callOnLog(MG_LFL_ERR, "Received error: " + message);
    }

    ConnectionParameters parameters = self->parseParametersAndCertificates(message, self);

    // set new secret and topic to can create mqtt connection before init agrirouterclient application
    self->m_settings->setConnectionParameters(parameters, false);
    self->m_callback(true, self->m_member);

    // set second time connection parameters with callback to agrirouterclient application
    self->m_settings->setConnectionParameters(parameters);

    return realsize;
}

ConnectionParameters Registration::parseParametersAndCertificates(const std::string& message, void *member)
{
    Registration *self = static_cast<Registration *>(member);

    cJSON *root = cJSON_Parse(message.c_str());
    cJSON *connectionCriteria = cJSON_GetObjectItem(root, "connectionCriteria");
    cJSON *authentication = cJSON_GetObjectItem(root, "authentication");

    ConnectionParameters parameters;

    parameters.deviceAlternateId = cJSON_GetObjectItem(root, "deviceAlternateId")->valuestring;
    parameters.capabilityAlternateId = cJSON_GetObjectItem(root, "capabilityAlternateId")->valuestring;
    parameters.sensorAlternateId = cJSON_GetObjectItem(root, "sensorAlternateId")->valuestring;

    parameters.secret = cJSON_GetObjectItem(authentication, "secret")->valuestring;
    parameters.certificateType = cJSON_GetObjectItem(authentication, "type")->valuestring;

    parameters.measuresUrl = cJSON_GetObjectItem(connectionCriteria, "measures")->valuestring;
    parameters.commandsUrl = cJSON_GetObjectItem(connectionCriteria, "commands")->valuestring;

    parameters.gatewayId = cJSON_GetObjectItem(connectionCriteria, "gatewayId")->valuestring;

    // Check for MQTT (gatewayId "2")
    if (parameters.gatewayId == "2")
    {
        parameters.host = cJSON_GetObjectItem(connectionCriteria, "host")->valuestring,
        parameters.port = cJSON_GetObjectItem(connectionCriteria, "port")->valueint;
        parameters.clientId = cJSON_GetObjectItem(connectionCriteria, "clientId")->valuestring;
    }

    std::string cert = cJSON_GetObjectItem(authentication, "certificate")->valuestring;
    int certIndex = cert.find("-----BEGIN CERTIFICATE-----");

    std::string privKey = cert.substr(0, certIndex);
    std::string certificate = cert.substr(certIndex);

    self->m_settings->setCertificate(certificate);
    self->m_settings->setPrivateKey(privKey);

    cJSON_Delete(root);

    return parameters;
}

bool Registration::containsError(const std::string& message)
{
    // Checks if string contains "statusCode" and "message" and returns the result
    // as a boolean
    return ((message.find("statusCode") != std::string::npos) && (message.find("message") != std::string::npos));
}

void Registration::setCallback(RegistrationCallback registrationCallback)
{
    m_callback = registrationCallback;
}
