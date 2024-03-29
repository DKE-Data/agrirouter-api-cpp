#include "AgrirouterClient.h"

#include "AgrirouterMessageUtils.h"
#include "CurlConnectionProvider.h"
#include "MqttConnectionProvider.h"
#include "Definitions.h"
#include "Registration.h"

#include <google/protobuf/io/zero_copy_stream.h>
#include <list>
#include <string>
#include <vector>

AgrirouterClient::AgrirouterClient()
{
    // HTTP is default ConnectionProvider
    Settings *settings = new Settings();
    settings->setConnectionType(Settings::HTTP);
    m_chunkSize = DEFAULT_CHUNK_SIZE;
    this->init(settings);
}

AgrirouterClient::AgrirouterClient(Settings *settings)
{
    m_chunkSize = DEFAULT_CHUNK_SIZE;
    this->init(settings);
}

AgrirouterClient::AgrirouterClient(Settings *settings, uint32_t chunkSize)
{
    m_chunkSize = chunkSize;
    this->init(settings);
}

void AgrirouterClient::init(Settings *settings)
{
    m_settings = settings;
    if(m_messageProvider != nullptr)
    {
        delete m_messageProvider;
        m_messageProvider = nullptr;
    }
    m_messageProvider = new MessageProvider(settings, m_chunkSize);

    if(m_connectionProvider != nullptr)
    {
        delete m_connectionProvider;
        m_connectionProvider = nullptr;
    }

    if (settings->getConnectionType() == Settings::HTTP)
    {        
        m_connectionProvider = new CurlConnectionProvider(settings);
    }
    else if (settings->getConnectionType() == Settings::MQTT)
    {
        m_connectionProvider = new MqttConnectionProvider(settings);
    }
}

AgrirouterClient::~AgrirouterClient()
{
    if (m_messageProvider != nullptr)
    {
        delete m_messageProvider;
        m_messageProvider = nullptr;
    }

    if (m_connectionProvider != nullptr)
    {
        delete m_connectionProvider;
        m_connectionProvider = nullptr;
    }
}

void AgrirouterClient::registerDeviceWithRegCode(const std::string& registrationCode, AgrirouterSettings& agrirouterSettings)
{
    Registration registration = Registration(m_connectionProvider, m_settings, this);
    registration.setCallback(registrationCallback);
    registration.sendOnboard(registrationCode, agrirouterSettings);
}

void AgrirouterClient::registrationCallback(bool success, void *member)
{
    AgrirouterClient *self = static_cast<AgrirouterClient*>(member);
    if(success) 
    {
        // reinit connection provider after onboard to new subscribe to the new topics
        if (self->m_connectionProvider != nullptr)
        {
            delete self->m_connectionProvider;
            self->m_connectionProvider = nullptr;
        }
        self->init(self->m_settings);
    }
    else
    {
        self->m_settings->callOnLog(MG_LFL_ERR, "Onboarding failed");
    }
}

int32_t AgrirouterClient::getNextSeqNo()
{
    // Overflow check
    if ((++m_seqNo) >= 2147483647)
    {
        m_seqNo = 1;
    }
    else if (m_seqNo == 0)
    {
        m_seqNo = 1;
    }

    return m_seqNo;
}

void AgrirouterClient::sendCapabilities(std::string *messageId, CapabilitySpecification *capabilities)
{
    AgrirouterMessage message = m_messageProvider->getCapabilityMessage(messageId, getNextSeqNo(), "", capabilities);
    sendMessage(&message, MG_EV_CAPABILITIES, messageId);
}

void AgrirouterClient::sendSubscription(std::string *messageId, Subscription *subscription)
{
    AgrirouterMessage message = m_messageProvider->getSubscriptionMessage(messageId, getNextSeqNo(), "", subscription);
    sendMessage(&message, MG_EV_SUBSCRIPTIONS, messageId);
}

void AgrirouterClient::sendListEndpointsFiltered(std::string *messageId, ListEndpointsQuery *listEndpointsQuery)
{
    AgrirouterMessage message = m_messageProvider->getListEndpointsMessage(messageId, getNextSeqNo(), "", listEndpointsQuery);
    sendMessage(&message, MG_EV_LIST_ENDPOINTS_FILTERED, messageId);
}

void AgrirouterClient::sendListEndpointsUnfiltered(std::string *messageId, ListEndpointsQuery *listEndpointsQuery)
{
    AgrirouterMessage message = m_messageProvider->getListEndpointsUnfilteredMessage(messageId, getNextSeqNo(), "", listEndpointsQuery);
    sendMessage(&message, MG_EV_LIST_ENDPOINTS_UNFILTERED, messageId);
}

void AgrirouterClient::sendQueryMessages(std::string *messageId, MessageQuery *messageQuery)
{
    AgrirouterMessage message = m_messageProvider->getQueryMessage(messageId, getNextSeqNo(), "", messageQuery);
    sendMessage(&message, MG_EV_QUERY_MESSAGES, messageId);
}

void AgrirouterClient::sendQueryHeaderMessages(std::string *messageId, MessageQuery *messageQuery)
{
    AgrirouterMessage message = m_messageProvider->getQueryHeaderMessage(messageId, getNextSeqNo(), "", messageQuery);
    sendMessage(&message, MG_EV_QUERY_HEADERS, messageId);
}

void AgrirouterClient::sendMessagesConfirm(std::string *messageId, MessageConfirm *messageConfirm)
{
    AgrirouterMessage message = m_messageProvider->getConfirmMessage(messageId, getNextSeqNo(), "", messageConfirm);
    sendMessage(&message, MG_EV_MESSAGE_CONFIRM, messageId);
}

void AgrirouterClient::sendMessagesDelete(std::string *messageId, MessageDelete *messageDelete)
{
    AgrirouterMessage message = m_messageProvider->getDeleteMessage(messageId, getNextSeqNo(), "", messageDelete);
    sendMessage(&message, MG_EV_MESSAGE_DELETE, messageId);
}

void AgrirouterClient::sendDeviceDescription(Addressing& addressing, std::string *messageId, const std::string& teamsetId, ISO11783_TaskData *taskdata, const std::string& fileName) // fileName default ""
{
    AgrirouterMessage message = m_messageProvider->getDeviceDescriptionMessage(messageId, getNextSeqNo(), addressing, teamsetId, taskdata, fileName);
    sendMessage(&message, MG_EV_DEVICE_DESCRIPTION, messageId);
}

void AgrirouterClient::sendTimelog(Addressing& addressing, std::string *messageId, const std::string& teamsetId, TimeLog *timelog, const std::string& fileName) // fileName default ""
{
    AgrirouterMessage message = m_messageProvider->getTimelogMessage(messageId, getNextSeqNo(), addressing, teamsetId, timelog, fileName);
    sendMessage(&message, MG_EV_TIMELOG, messageId);
}

void AgrirouterClient::sendChunk(AgrirouterMessage& message)
{
    // fileName already in AgrirouterMessage
    message.request().envelope.set_application_message_seq_no(getNextSeqNo());
    std::string applicationMessageId = message.request().envelope.application_message_id();
    sendMessage(&message, MG_EV_NON_TELEMETRY, &applicationMessageId);
}

void AgrirouterClient::sendImage(Addressing& addressing, std::string *messageId, const std::string& teamsetId, char *image, int size, const std::string& fileName) // fileName default ""
{
    AgrirouterMessage message = m_messageProvider->getImageMessage(messageId, addressing, getNextSeqNo(), teamsetId, image, size, fileName);
    sendMessage(&message, MG_EV_NON_TELEMETRY, messageId);
}

void AgrirouterClient::sendTaskdataZip(Addressing& addressing, std::string *messageId, const std::string& teamsetId, char *taskdataZip, int size, const std::string& fileName) // fileName default ""
{
    AgrirouterMessage message = m_messageProvider->getTaskdataZipMessage(messageId, addressing, getNextSeqNo(), teamsetId, taskdataZip, size, fileName);
    sendMessage(&message, MG_EV_NON_TELEMETRY, messageId);
}

AgrirouterMessage AgrirouterClient::createChunkMessage(std::string *messageId, Addressing& addressing, uint16_t numberOfChunk,
        uint16_t numberOfChunks, const std::string& teamSetContextId, const std::string& chunkContextId, const std::string& data,
        uint32_t size, const std::string& technicalMessageType, const std::string& fileName) // fileName default ""
{
    return m_messageProvider->getChunkedMessage(messageId, addressing, getNextSeqNo(), numberOfChunk, numberOfChunks,
                                                    teamSetContextId, chunkContextId, data, size, technicalMessageType, fileName);
}

void AgrirouterClient::requestMessages()
{
    // Create headers
    std::vector<std::string> headers;
    headers.push_back("Content-type: application/json");
    headers.push_back("Accept: application/json");

    m_connectionProvider->setBody("");
    m_connectionProvider->setUrl(m_settings->getConnectionParameters().commandsUrl);
    m_connectionProvider->setHeaders(headers);
    m_connectionProvider->setCallback(requestMessagesCallback);
    m_connectionProvider->setMember(this);

    m_connectionProvider->getMessages();
}

size_t AgrirouterClient::requestMessagesCallback(char *content, size_t size, size_t nmemb, void *member)
{
    size_t realsize = size * nmemb;

    AgrirouterClient *self = static_cast<AgrirouterClient *>(member);

    std::string message(content, realsize);

    std::list<Response> responseList;
    getResponsesFromMessage(&responseList, &message);

    for (std::list<Response>::iterator it = responseList.begin(); it != responseList.end(); ++it)
    {
        Response response = (Response) *it;

        // Confirm messages after retrieving MessageQueryResponse
        if (response.payloadWrapper.details().type_url() == "types.agrirouter.com/agrirouter.feed.response.MessageQueryResponse")
        {
            MessageConfirm messageConfirm = MessageConfirm();

            MessageQueryResponse qr = MessageQueryResponse();
            qr.ParseFromString(response.payloadWrapper.details().value());

            for (int num = 0; num < qr.messages_size(); num++)
            {
                messageConfirm.add_message_ids(qr.messages(num).header().message_id());
            }

            if (messageConfirm.message_ids().size() > 0)
            {
                std::string messageId;
                self->sendMessagesConfirm(&messageId, &messageConfirm);
            }
        }

        // Forward recieved message to application
        MessageParameters messageParameters;
        messageParameters.event = MG_EV_GET_MESSAGES;
        self->m_settings->callOnMessage(&response, messageParameters);
    }
    return realsize;
}

/*
 * Private methods
 */

void AgrirouterClient::callbackHandler(char *content, size_t size, MessageParameters& messageParameters)
{
    if (size == 0)
    {
        Response response = Response();
        m_settings->callOnMessage(&response, messageParameters);
        this->getMessages(this, requestMessagesCallback);
    }
    else
    {
        std::string message(content, size);
        std::list<Response> responseList;
        getResponsesFromMessage(&responseList, &message);

        for (std::list<Response>::iterator it = responseList.begin(); it != responseList.end(); ++it)
        {
            Response response = (Response)*it;
            messageParameters.event = MG_EV_GET_MESSAGES;
            m_settings->callOnMessage(&response, messageParameters);
        }
    }
}

size_t AgrirouterClient::messageCallback(char *content, size_t size, size_t nmemb, void *member)
{
    MessageParameters *messageParameters = static_cast<MessageParameters *>(member);
    size_t realsize = size * nmemb;

    AgrirouterClient *self = static_cast<AgrirouterClient *>(messageParameters->member);
    self->callbackHandler(content, realsize, *messageParameters);
    return realsize;
}

int AgrirouterClient::sendMessage(AgrirouterMessage *agrirouterMessage, int event, std::string *messageId)
{
    MessageParameters messageParameters;
    messageParameters.member = this;
    messageParameters.event = event;
    messageParameters.applicationMessageId = *messageId;

    timeval timestamp = getTimestamp();

    std::string message = "{\"sensorAlternateId\":\"" + m_settings->getConnectionParameters().sensorAlternateId +
        "\",\"capabilityAlternateId\":\"" + m_settings->getConnectionParameters().capabilityAlternateId +
        "\",\"measures\":[";

    message = message + "[\"" + agrirouterMessage->encodedRequest();
    message = message + "\", \"" + uint32ToString(timestamp.tv_sec) + "\"]";
    message += "]}";

    char num[32];
    snprintf(num, sizeof(num), "%li", timestamp.tv_sec);

    std::vector<std::string> headers;
    headers.push_back("Content-type: application/json");
    headers.push_back("Accept: application/json");

    m_connectionProvider->setBody(message);
    m_connectionProvider->setUrl(m_settings->getConnectionParameters().measuresUrl);
    m_connectionProvider->setHeaders(headers);
    m_connectionProvider->setCallback(messageCallback);
    m_connectionProvider->setMember(&messageParameters);

    m_connectionProvider->sendMessage(messageParameters);

    return EXIT_SUCCESS;
}

void AgrirouterClient::getMessages(AgrirouterClient *self, ConnectionProvider::Callback callback)
{
    // Create headers
    std::vector<std::string> headers;
    headers.push_back("Content-type: application/json");
    headers.push_back("Accept: application/json");

    self->m_connectionProvider->setBody("");
    self->m_connectionProvider->setUrl(m_settings->getConnectionParameters().commandsUrl);
    self->m_connectionProvider->setHeaders(headers);
    self->m_connectionProvider->setCallback(callback);
    self->m_connectionProvider->setMember(self);

    self->m_connectionProvider->getMessages();
}
