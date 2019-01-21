#include "Registration.h"

#include <Utils.h>
#include <curl/curl.h>
#include <stdio.h>
#include <cstring>
#include <string>
#include <iostream>
#include <vector>

#include "../third_party/cJSON/cJSON.h"

Registration::Registration(ConnectionProvider *connectionProvider, Settings *settings) {
    m_settings = settings;
    m_connectionProvider = connectionProvider;
}

Registration::~Registration() {}

void Registration::registerToAgrirouterWithRegCode(
    std::string registrationCode, AgrirouterSettings agrirouterSettings) {
    this->m_registrationCode = registrationCode;

    this->sendOnboard(agrirouterSettings);
}

void Registration::sendOnboard(AgrirouterSettings agrirouterSettings) {
    // Set headers
    std::vector<std::string> headers;

    std::string authorization_header = "Authorization: Bearer " + this->m_registrationCode;
    headers.push_back(authorization_header);
    std::string content_type_header = "Content-Type: application/json";
    headers.push_back(content_type_header);

    std::string body = "{\"id\":\"" + m_settings->getExternalId() +
        "\",\"applicationId\":\"" + m_settings->getApplicationId() +
        "\",\"certificationVersionId\":\"" + m_settings->getCertificationVersionId() +
        "\",\"gatewayId\":\"" + m_settings->getGatewayId() +
        "\",\"certificateType\":\"" + "PEM" + "\"}";

    std::string url = agrirouterSettings.registrationUrl;
    m_connectionProvider->setBody(body);
    m_connectionProvider->setUrl(url);
    m_connectionProvider->setHeaders(headers);
    m_connectionProvider->setCallback(sendOnboardCallback);
    m_connectionProvider->setMember(reinterpret_cast<void*>(this));

    MessageParameters messageParameters;
    messageParameters.member = reinterpret_cast<void*>(this);

    m_connectionProvider->onboard(messageParameters);
}

size_t Registration::sendOnboardCallback(char *content, size_t size, size_t nmemb, void *member) {
    size_t realsize = size * nmemb;
    Registration *self = reinterpret_cast<Registration*>(member);

    // Convert to string including some necessary modifications
    char *tmp = new char[realsize + 1];
    strncpy(tmp, content, realsize);
    tmp[realsize] = '\0';
    std::string message(tmp);

    if (containsError(message)) {
        // printf("Received error: %s", message.c_str());
    }

    // Get key and pem
    self->parseCertificates(message, self);

    delete [] tmp;
    return realsize;
}

void Registration::parseCertificates(std::string message, void *member) {
    Registration *self = reinterpret_cast<Registration*>(member);

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

    // Check for MQTT (gatewayId "2"
    if (parameters.gatewayId == "2") {
        parameters.host = cJSON_GetObjectItem(connectionCriteria, "host")->valuestring, parameters.port = cJSON_GetObjectItem(connectionCriteria, "port")->valueint;
        parameters.clientId = cJSON_GetObjectItem(connectionCriteria, "clientId")->valuestring;
    }

    std::string cert = cJSON_GetObjectItem(authentication, "certificate")->valuestring;
    int certIndex = cert.find("-----BEGIN CERTIFICATE-----");

    std::string privKey = cert.substr(0, certIndex);
    std::string certificate = cert.substr(certIndex);

    self->m_settings->setCertificate(certificate);
    self->m_settings->setPrivateKey(privKey);

    self->m_settings->setConnectionParameters(parameters);
}

bool Registration::containsError(std::string message) {
    // Checks if string contains "statusCode" and "message" and returns the result
    // as a boolean
    return ((message.find("statusCode") != std::string::npos) && (message.find("message") != std::string::npos));
}
