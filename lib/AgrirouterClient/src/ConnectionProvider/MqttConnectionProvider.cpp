#include "MqttConnectionProvider.h"

#include "AgrirouterClient.h"
#include "AgrirouterMessageUtils.h"
#include "Utils.h"

#include <stdio.h>
#include <stdlib.h>
#include <string>

MqttConnectionProvider::MqttConnectionProvider(Settings *settings)
{
    m_settings = settings;
    this->init();
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

    m_mqttClient->init();

    // subscribe to commands only subscribe when topic is valid (onboarding is done)
    if(conn.commandsUrl.length() > 0)
    {
        m_mqttClient->subscribe(conn.commandsUrl, 2);
    }
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
    char* msg = (char*) payload;
       
    if(msg)
    {
        // If msg starts with '{', it is not an array as it comes from curl, so add the square brackets
        if (strncmp(msg, "{", 1) == 0)
        {
            std::string message = std::string(msg, payloadlen);
            message = "[" + message + "]";
            payloadlen = message.length();
            msg = strdup(message.c_str());
        }
    }
    else
    {
        std::string emptyMessage = "[]";
        msg = strdup(emptyMessage.c_str());
    }

    (self->m_callback)(msg, payloadlen, 1, &self->m_messageParameters);
}

void MqttConnectionProvider::sendMessage(MessageParameters messageParameters)
{
    this->sendMessageWithChunkedResponse(messageParameters);
}

void MqttConnectionProvider::sendMessageWithChunkedResponse(MessageParameters messageParameters)
{
    m_settings->callOnLog(MG_LFL_NTC, "Send message mqtt with application id: '" + messageParameters.applicationMessageId + "'");

    if(m_url.find("http") != std::string::npos)
    {
        std::string errorJSON = "{\"error\":{\"code\":\""+ std::to_string(MG_ERROR_NOT_VALID_TOPIC) + "\",\"message\":\"" + m_url + "\",\"target\":\"agrirouter-api-cpp\",\"details\":[]}}";
        m_settings->callOnError(0, MG_ERROR_NOT_VALID_TOPIC, "Not a valid MQTT topic. New Onboarding needed", messageParameters, errorJSON);
    }
    else
    {
        m_mqttClient->publish(m_url, m_body, 2); // Qos 2
    }
    m_messageParameters = messageParameters;
}

void MqttConnectionProvider::onboard(MessageParameters messageParameters)
{
    m_settings->callOnLog(MG_LFL_CRI, "Onboarding with MQTT is not possible");
}

void MqttConnectionProvider::getMessages(void)
{
    MessageParameters messageParameters;
    messageParameters.applicationMessageId = createUuid();
    this->sendMessageWithChunkedResponse(messageParameters);
}
