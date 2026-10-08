#include "MqttConnectionProvider.h"

#include "AgrirouterClient.h"
#include "AgrirouterMessageUtils.h"
#include "Utils.h"

#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <limits>

MqttConnectionProvider::MqttConnectionProvider(Settings *settings)
{
    m_settings = settings;
}

MqttConnectionProvider::~MqttConnectionProvider()
{
    if (m_mqttClient != nullptr)
    {
        delete m_mqttClient;
    }
}

void MqttConnectionProvider::init()
{
    ConnectionParameters conn = m_settings->getConnectionParameters();
    if(m_mqttClient != nullptr)
    {
        delete m_mqttClient;
        m_mqttClient = nullptr;
    }
    m_mqttClient = new MqttConnectionClient(conn.clientId, conn.host, conn.port, m_settings);

    // set message callback
    m_mqttClient->setMember(this);
    m_mqttClient->setMqttCallback(requestMqttCallback);
    m_mqttClient->setMqttErrorCallback(requestMqttErrorCallback);

    // subscribe to commands only when topic is valid (onboarding is done), the client subscribes after every connect
    m_mqttClient->setSubscription(conn.commandsUrl, 2);

    // Try the setup only once. If it succeeds, mosquitto reconnects by itself (also if the first connect fails).
    // If it fails, there is no automatic retry, the application has to call renewConnection().
    if (m_mqttClient->init() != EXIT_SUCCESS)
    {
        std::string errorMessage = "MqttConnectionProvider: MQTT client setup failed, call renewConnection() to retry";
        m_settings->callOnLog(MG_LFL_ERR, errorMessage);
        std::string errorJSON = "{\"error\":{\"code\":\""+ std::to_string(MG_ERROR_MQTT_CONNECT_FAILED) + "\",\"message\":\"" + errorMessage + "\",\"target\":\"agrirouter-api-cpp\",\"details\":[]}}";
        m_settings->callOnError(0, MG_ERROR_MQTT_CONNECT_FAILED, errorMessage, MessageParameters(), errorJSON);
        return;
    }
}

bool MqttConnectionProvider::isConnected()
{
    return (m_mqttClient != nullptr) && m_mqttClient->isConnected();
}

void MqttConnectionProvider::renewConnection()
{
    this->init();
}

void MqttConnectionProvider::requestMqttErrorCallback(int errorCode, std::string message, std::string content, void *member)
{
    MqttConnectionProvider *self = static_cast<MqttConnectionProvider *>(member);
    // Qos 128 from mqtt client means that the endpoint is missing
    if(errorCode == 128)
    {
        errorCode = MG_ERROR_MISSING_ENDPOINT;
    }
    self->m_settings->callOnError(0, errorCode, message, self->m_messageParameters, content);
}

void MqttConnectionProvider::requestMqttCallback(char *topic, void *payload, int payloadlen, void *member)
{
    MqttConnectionProvider *self = static_cast<MqttConnectionProvider *>(member);
    std::string message;

    if(payload != nullptr)
    {
        message = std::string(static_cast<char *>(payload), payloadlen);
        // If msg starts with '{', it is not an array as it comes from curl, so add the square brackets
        if (strncmp(message.c_str(), "{", 1) == 0)
        {
            message = "[" + message + "]";
            payloadlen = message.length();
        }
    }
    else
    {
        message = "[]";
    }

    (self->m_callback)(&message[0], payloadlen, 1, &self->m_messageParameters);
}

void MqttConnectionProvider::sendMessage(MessageParameters messageParameters)
{
    this->sendMessageWithChunkedResponse(messageParameters);
}

void MqttConnectionProvider::sendMessageWithChunkedResponse(MessageParameters messageParameters)
{
    m_settings->callOnLog(MG_LFL_NTC, "Send message mqtt with application id: '" + messageParameters.applicationMessageId + "'");

    // Set before publishing, a fast response from the mosquitto thread would otherwise use the previous parameters
    m_messageParameters = messageParameters;

    if(m_url.find("http") != std::string::npos)
    {
        std::string errorJSON = "{\"error\":{\"code\":\""+ std::to_string(MG_ERROR_NOT_VALID_TOPIC) + "\",\"message\":\"" + m_url + "\",\"target\":\"agrirouter-api-cpp\",\"details\":[]}}";
        m_settings->callOnError(0, MG_ERROR_NOT_VALID_TOPIC, "Not a valid MQTT topic. New Onboarding needed", messageParameters, errorJSON);
    }
    else
    {
        m_mqttClient->publish(m_url, m_body, 2); // Qos 2
    }
}

void MqttConnectionProvider::onboard(MessageParameters messageParameters)
{
    m_settings->callOnLog(MG_LFL_CRI, "Onboarding with MQTT is not possible");
}

void MqttConnectionProvider::getMessages(void)
{
    MessageParameters messageParameters;
    messageParameters.applicationMessageId = createUuid();
    messageParameters.event = MG_EV_GET_MESSAGES;
    // The member is the AgrirouterClient, the callback gets these parameters as member
    messageParameters.member = m_member;
    this->sendMessageWithChunkedResponse(messageParameters);
}
