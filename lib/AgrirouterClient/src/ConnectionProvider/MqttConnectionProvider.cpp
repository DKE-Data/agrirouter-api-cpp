#include "MqttConnectionProvider.h"

#include "AgrirouterClient.h"
#include "AgrirouterMessageUtils.h"
#include "Utils.h"

#include <stdio.h>
#include <stdlib.h>
#include <string>

MqttConnectionProvider::MqttConnectionProvider(Settings *settings)
{
    this->m_polling = false;
    this->m_settings = settings;
    this->m_body = "";
    this->m_url = "";
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
    this->m_mqttClient->init();
    
    // set message callback
    this->m_mqttClient->setMember(this);
    this->m_mqttClient->setMqttCallback(requestMqttCallback);

    // subscribe to commands only subscribe when topic is valid (onboarding is done)
    if(conn.commandsUrl.length() > 0)
    {
        this->m_mqttClient->subscribe(conn.commandsUrl, 2);
    }
}

size_t MqttConnectionProvider::requestMqttCallback(char *topic, void *payload, int payloadlen, void *member)
{
    MqttConnectionProvider *self = static_cast<MqttConnectionProvider *>(member);
    char* msg = (char*) payload;
       
    // If msg starts with '{', it is not an array as it comes from curl, so add the square brackets
    if(msg)
    {
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

    return payloadlen;
}

void MqttConnectionProvider::sendMessage(MessageParameters messageParameters)
{
    this->sendMessageWithChunkedResponse(messageParameters);
}

void MqttConnectionProvider::sendMessageWithChunkedResponse(MessageParameters messageParameters)
{
    printf("Send message mqtt with message: '%s' and application id: '%s' \n", this->m_body.c_str(), messageParameters.applicationMessageId.c_str());
    this->m_mqttClient->publish(this->m_settings->getConnectionParameters().measuresUrl, this->m_body, 2);
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