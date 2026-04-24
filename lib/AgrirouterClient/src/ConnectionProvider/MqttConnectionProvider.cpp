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

    int initReturnValue = EXIT_FAILURE;
    const int timeRetry = 1; // time in s to run loop and after it can be stop the app
    int retryReconnectCounter = 30 * timeRetry; // time in s to retry the mqtt init process
    int counter = 0;

    while (initReturnValue == EXIT_FAILURE)
    {
        if ((counter % retryReconnectCounter) == 0) // Call init() every 30s until it does not fail
        {
            initReturnValue = m_mqttClient->init();
            if(initReturnValue == EXIT_FAILURE)
            {
                this->m_settings->callOnLog(MG_LFL_ERR, "MqttConnectionClient: Init failed retry in " + std::to_string(retryReconnectCounter) + "s");
            }
            else if (initReturnValue == EXIT_SUCCESS)
            {
                break;
            }
        }

        if (counter < INT32_MAX)
        {
            counter++;
        }
        else
        {
            counter = 0;
        }

        timeval timeout;
        timeout.tv_sec = timeRetry;
        timeout.tv_usec = 0;

        int ret = select(0, nullptr, nullptr, nullptr, &timeout);

        if (ret == -1 && errno == EINTR) {
            // stop on exit application
            break;
        }
    }

    // subscribe to commands only subscribe when topic is valid (onboarding is done)
    if(conn.commandsUrl.length() > 0)
    {
        m_mqttClient->subscribe(conn.commandsUrl, 2);
    }
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
