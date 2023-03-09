#include "MqttConnectionProvider.h"

#include "AgrirouterClient.h"
#include "AgrirouterMessageUtils.h"
#include "Utils.h"

#include <stdio.h>
#include <stdlib.h>
#include <string>

MqttConnectionProvider::MqttConnectionProvider(Settings *settings)
{
    this->m_settings = settings;
    this->m_body = "";
    this->m_url = "";
    this->m_mqttClient = NULL;
    this->init();
}

MqttConnectionProvider::~MqttConnectionProvider()
{
    if (this->m_mqttClient != NULL)
    {
        delete this->m_mqttClient;
    }
}

void MqttConnectionProvider::init()
{
    ConnectionParameters conn = this->m_settings->getConnectionParameters();
    this->m_mqttClient = new MqttConnectionClient(conn.clientId, conn.host, conn.port, this->m_settings);

    // set message callback
    this->m_mqttClient->setMember(this);
    this->m_mqttClient->setMqttCallback(requestMqttCallback);
    this->m_mqttClient->setMqttErrorCallback(requestMqttErrorCallback);

    this->m_mqttClient->init();

    // subscribe to commands only subscribe when topic is valid (onboarding is done)
    if(conn.commandsUrl.length() > 0)
    {
        this->m_mqttClient->subscribe(conn.commandsUrl, 2);
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
    printf("Send message mqtt with message: '%s' and application id: '%s' \n", this->m_body.c_str(), messageParameters.applicationMessageId.c_str());
    if(this->m_url.find("http") != std::string::npos)
    {
        std::string errorJSON = "{\"error\":{\"code\":\""+ std::to_string(MG_ERROR_NOT_VALID_TOPIC) + "\",\"message\":\"" + this->m_url + "\",\"target\":\"agrirouter-api-cpp\",\"details\":[]}}";
        this->m_settings->callOnError(0, MG_ERROR_NOT_VALID_TOPIC, "Not a vaild MQTT topic. New Onboarding needed", this->m_messageParameters, errorJSON);
    }
    else
    {
        this->m_mqttClient->publish(this->m_url, this->m_body, 2);        
    }
    this->m_messageParameters = messageParameters;
}

void MqttConnectionProvider::onboard(MessageParameters messageParameters)
{
   printf("Onboarding with MQTT is not possible");
}

void MqttConnectionProvider::getMessages(void)
{
    MessageParameters messageParameters;
    messageParameters.applicationMessageId = createUuid();
    this->sendMessageWithChunkedResponse(messageParameters);
}