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

void Registration::sendOnboard(const std::string& registrationCode, const AgrirouterSettings& agrirouterSettings)
{
    m_registrationCode = registrationCode;

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

    ConnectionParameters parameters;
    if (containsError(message) || !self->parseParametersAndCertificates(message, parameters))
    {
        self->m_settings->callOnLog(MG_LFL_ERR, "Invalid onboard response: " + message);

        MessageParameters messageParameters;
        messageParameters.member = static_cast<void *>(self);
        self->m_settings->callOnError(0, MG_ERROR_INVALID_ONBOARD_RESPONSE, "Registration: Invalid onboard response", messageParameters, message);
        self->m_callback(false, self->m_member);

        return realsize;
    }

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
    ConnectionParameters parameters;
    self->parseParametersAndCertificates(message, parameters);
    return parameters;
}

bool Registration::parseParametersAndCertificates(const std::string& message, ConnectionParameters& parameters)
{
    cJSON *root = cJSON_Parse(message.c_str());
    cJSON *connectionCriteria = cJSON_GetObjectItem(root, "connectionCriteria");
    cJSON *authentication = cJSON_GetObjectItem(root, "authentication");

    bool valid = getJsonString(root, "deviceAlternateId", parameters.deviceAlternateId) &&
                 getJsonString(root, "capabilityAlternateId", parameters.capabilityAlternateId) &&
                 getJsonString(root, "sensorAlternateId", parameters.sensorAlternateId) &&
                 getJsonString(authentication, "secret", parameters.secret) &&
                 getJsonString(authentication, "type", parameters.certificateType) &&
                 getJsonString(connectionCriteria, "measures", parameters.measuresUrl) &&
                 getJsonString(connectionCriteria, "commands", parameters.commandsUrl) &&
                 getJsonString(connectionCriteria, "gatewayId", parameters.gatewayId);

    // Check for MQTT (gatewayId "2")
    if (valid && (parameters.gatewayId == "2"))
    {
        cJSON *port = cJSON_GetObjectItem(connectionCriteria, "port");
        valid = getJsonString(connectionCriteria, "host", parameters.host) &&
                getJsonString(connectionCriteria, "clientId", parameters.clientId) &&
                (port != nullptr) && cJSON_IsNumber(port);
        if (valid)
        {
            parameters.port = port->valueint;
        }
    }

    std::string cert;
    valid = valid && getJsonString(authentication, "certificate", cert);

    cJSON_Delete(root);

    if (!valid)
    {
        return false;
    }

    size_t certIndex = cert.find("-----BEGIN CERTIFICATE-----");
    if (certIndex == std::string::npos)
    {
        return false;
    }

    std::string privKey = cert.substr(0, certIndex);
    std::string certificate = cert.substr(certIndex);

    m_settings->setCertificate(certificate);
    m_settings->setPrivateKey(privKey);

    return true;
}

bool Registration::getJsonString(cJSON *object, const char *name, std::string& value)
{
    cJSON *item = cJSON_GetObjectItem(object, name);
    if ((item == nullptr) || (item->valuestring == nullptr))
    {
        return false;
    }

    value = item->valuestring;
    return true;
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
