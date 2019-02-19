#include <AgrirouterClient.h>

#include <AgrirouterMessageUtils.h>
#include <ConnectionProvider/CurlConnectionProvider.h>
#include <Definitions.h>
#include <Registration.h>

#include <google/protobuf/io/zero_copy_stream.h>
#include <list>
#include <string>
#include <vector>

AgrirouterClient::AgrirouterClient() {
  // HTTP is default ConnectionProvider
  Settings *settings = new Settings();
  settings->setConnectionType(Settings::HTTP);
  this->init(settings);
}

AgrirouterClient::AgrirouterClient(Settings *settings) {
  this->init(settings);
}

void AgrirouterClient::init(Settings *settings) {
  m_seqNo = 1;
  m_contextId = createUuid();
  m_settings = settings;
  m_messageProvider = new MessageProvider(settings);

  if (settings->getConnectionType() == Settings::HTTP) {
    m_connectionProvider = new CurlConnectionProvider(settings);
  } else if (settings->getConnectionType() == Settings::MQTT) {
    // ToDo: MQTT needs to be implemented
  }
}

AgrirouterClient::~AgrirouterClient() {
  this->terminate();
}

void AgrirouterClient::terminate() {
  if (this->m_messageProvider != NULL) {
    delete this->m_messageProvider;
    this->m_messageProvider = NULL;
  }

  if (this->m_connectionProvider != NULL) {
    delete this->m_connectionProvider;
    this->m_connectionProvider = NULL;
  }
}

void AgrirouterClient::registerDeviceWithRegCode(std::string registrationCode, AgrirouterSettings agrirouterSettings) {
  Registration registration = Registration(m_connectionProvider, m_settings);
  registration.registerToAgrirouterWithRegCode(registrationCode, agrirouterSettings);
}

int32_t AgrirouterClient::getNextSeqNo() {
  // Overflow check
  if ((++m_seqNo) >= 2147483647) {
    m_seqNo = 1;
  }

  return m_seqNo;
}

void AgrirouterClient::sendCapabilities(std::string *messageId, CapabilitySpecification *capabilities) {
  AgrirouterMessage message = m_messageProvider->getCapabilityMessage(messageId, getNextSeqNo(), getContextId(), capabilities);
  sendMessage(message, MG_EV_CAPABILITIES, messageId);
}

void AgrirouterClient::sendSubscription(std::string *messageId, Subscription *subscription) {
  AgrirouterMessage message = m_messageProvider->getSubscriptionMessage(messageId, getNextSeqNo(), getContextId(), subscription);

  sendMessage(message, MG_EV_SUBSCRIPTIONS, messageId);
}

void AgrirouterClient::sendListEndpointsFiltered(std::string *messageId, ListEndpointsQuery *listEndpointsQuery) {
  AgrirouterMessage message = m_messageProvider->getListEndpointsMessage(messageId, getNextSeqNo(), getContextId(), listEndpointsQuery);

  sendMessage(message, MG_EV_LIST_ENDPOINTS_FILTERED, messageId);
}

void AgrirouterClient::sendListEndpointsUnfiltered(std::string *messageId, ListEndpointsQuery *listEndpointsQuery) {
  AgrirouterMessage message = m_messageProvider->getListEndpointsUnfilteredMessage(messageId, getNextSeqNo(), getContextId(), listEndpointsQuery);

  sendMessage(message, MG_EV_LIST_ENDPOINTS_UNFILTERED, messageId);
}

void AgrirouterClient::sendQueryMessages(std::string *messageId, MessageQuery *messageQuery) {
  AgrirouterMessage message = m_messageProvider->getQueryMessage(messageId, getNextSeqNo(), getContextId(), messageQuery);

  sendMessage(message, MG_EV_QUERY_MESSAGES, messageId);
}

void AgrirouterClient::sendQueryHeaderMessages(std::string *messageId, MessageQuery *messageQuery) {
  AgrirouterMessage message = m_messageProvider->getQueryHeaderMessage(messageId, getNextSeqNo(), getContextId(), messageQuery);

  sendMessage(message, MG_EV_QUERY_HEADERS, messageId);
}

void AgrirouterClient::sendMessagesConfirm(std::string *messageId, MessageConfirm *messageConfirm) {
  AgrirouterMessage message = m_messageProvider->getConfirmMessage(messageId, getNextSeqNo(), getContextId(), messageConfirm);

  sendMessage(message, MG_EV_MESSAGE_CONFIRM, messageId);
}

void AgrirouterClient::sendMessagesDelete(std::string *messageId, MessageDelete *messageDelete) {
  AgrirouterMessage message = m_messageProvider->getDeleteMessage(messageId, getNextSeqNo(), getContextId(), messageDelete);

  sendMessage(message, MG_EV_MESSAGE_DELETE, messageId);
}

/*void AgrirouterClient::sendDeviceDescription(Addressing addressing, std::string *messageId, ISO11783_TaskData *taskdata) {
  AgrirouterMessage message = m_messageProvider->getDeviceDescriptionMessage(messageId, getNextSeqNo(), addressing, getContextId(), taskdata);

  sendMessage(message, MG_EV_DEVICE_DESCRIPTION, messageId);
}*/

/*void AgrirouterClient::sendTimelog(Addressing addressing, std::string *messageId, TimeLog *timelog) {
  AgrirouterMessage message = m_messageProvider->getTimelogMessage(messageId, getNextSeqNo(), addressing, getContextId(), timelog);

  sendMessage(message, MG_EV_TIMELOG, messageId);
}*/

void AgrirouterClient::sendImage(Addressing addressing, std::string *messageId, unsigned char const* image, int size) {
  std::list<AgrirouterMessage> messages = m_messageProvider->getImageMessage(messageId, getNextSeqNo(), addressing, getContextId(), image, size);

  for (std::list<AgrirouterMessage>::iterator it = messages.begin();
       it != messages.end(); ++it) {
    AgrirouterMessage message = (AgrirouterMessage)*it;

    sendMessage(message, MG_EV_NON_TELEMETRY, messageId);
  }
}

void AgrirouterClient::sendTaskdataZip(Addressing addressing, std::string *messageId, unsigned char const* taskdataZip, int size, std::list<std::string>& a_ref_l_usedApplicationMsgId) {
  std::list<AgrirouterMessage> messages = m_messageProvider->getTaskdataZipMessage(messageId, getNextSeqNo(), addressing, getContextId(), taskdataZip, size);

  for (std::list<AgrirouterMessage>::iterator it = messages.begin(); it != messages.end(); ++it) {
    AgrirouterMessage message = (AgrirouterMessage)*it;

    sendMessage(message, MG_EV_NON_TELEMETRY, messageId);

    a_ref_l_usedApplicationMsgId.push_back(message.messageId());
  }
}

void AgrirouterClient::requestMessages() {
  // Create headers
  std::vector<std::string> headers;
  headers.push_back("Content-type: application/json");
  headers.push_back("Accept: application/json");

  m_connectionProvider->setBody("");
  m_connectionProvider->setUrl(this->m_settings->getConnectionParameters().commandsUrl);
  m_connectionProvider->setHeaders(headers);
  m_connectionProvider->setCallback(requestMessagesCallback);
  m_connectionProvider->setMember(reinterpret_cast<void*>(this));

  m_connectionProvider->getMessages();
}

size_t AgrirouterClient::requestMessagesCallback(char *content, size_t size, size_t nmemb, void *member) {
  size_t realsize = size * nmemb;

  AgrirouterClient *self = reinterpret_cast<AgrirouterClient*>(member);

  // Convert to string including some necessary modifications
  char *tmp = new char[realsize + 1];
  strncpy(tmp, content, realsize);
  tmp[realsize] = '\0';
  std::string message = std::string(tmp);

  std::list<Response> responseList = getResponsesFromMessage(message);

  for (std::list<Response>::iterator it = responseList.begin(); it != responseList.end(); ++it) {
    Response response = (Response) *it;

    // Confirm messages after retrieving MessageQueryResponse
    if (response.payloadWrapper.details().type_url() == "types.agrirouter.com/agrirouter.feed.response.MessageQueryResponse") {
      std::string envelope = messageToJson(&response.envelope);

      if (!response.envelope.message_id().empty()) {
        MessageConfirm messageConfirm = MessageConfirm();
        messageConfirm.add_message_ids(response.envelope.message_id());

        MessageQueryResponse qr = MessageQueryResponse();
        qr.ParseFromString(response.payloadWrapper.details().value());

        if (qr.messages_size() > 0) {
          printf("Confirm message with id %s\n", response.envelope.message_id().c_str());
          std::string messageId;
          self->sendMessagesConfirm(&messageId, &messageConfirm);
        }
      }
    }

    // Forward recieved message to application
    MessageParameters messageParameters;
    messageParameters.event = MG_EV_GET_MESSAGES;
    self->m_settings->callOnMessage(&response, messageParameters);
  }

  delete [] tmp;
  return realsize;
}

/*
 * Getter and Setter
 */

std::string AgrirouterClient::getContextId() const {
  return m_contextId;
}

void AgrirouterClient::setContextId(const std::string &contextId) {
  m_contextId = contextId;
}

/*
 * Private methods
 */

void AgrirouterClient::callbackHandler(char *content, size_t size, MessageParameters messageParameters) {
  if (size == 0) {
    Response response = Response();
    this->m_settings->callOnMessage(&response, messageParameters);
    this->getMessages(this, requestMessagesCallback);
  } else {
    // Convert to string including some necessary modifications
    char *tmp = new char[size + 1];
    strncpy(tmp, content, size);
    tmp[size] = '\0';
    std::list<Response> responses = getResponsesFromMessage(std::string(tmp));

    printf("%lu response messages\n", responses.size());
    for (std::list<Response>::iterator it = responses.begin(); it != responses.end(); ++it) {
      Response response = (Response)*it;
      messageParameters.event = MG_EV_GET_MESSAGES;
      this->m_settings->callOnMessage(&response, messageParameters);
    }

    delete [] tmp;
  }
}

size_t AgrirouterClient::messageCallback(char *content, size_t size, size_t nmemb, void *member) {
  printf("AgrirouterClient::messageCallback\n");
  MessageParameters *messageParameters = reinterpret_cast<MessageParameters*>(member);
  size_t realsize = size * nmemb;

  char *tmp = new char[realsize + 1];
  strncpy(tmp, content, realsize);
  tmp[realsize] = '\0';

  AgrirouterClient *self = reinterpret_cast<AgrirouterClient*>(messageParameters->member);
  self->callbackHandler(content, realsize, *messageParameters);

  delete [] tmp;
  return realsize;
}

int AgrirouterClient::sendMessage(AgrirouterMessage agrirouterMessage, int event, std::string *messageId) {
  MessageParameters messageParameters;
  messageParameters.member = reinterpret_cast<void*>(this);
  messageParameters.event = event;
  messageParameters.applicationMessageId = *messageId;

  struct timeval timestamp = getTimestamp();

  std::string message = "{\"sensorAlternateId\":\"" + this->m_settings->getConnectionParameters().sensorAlternateId +
      "\",\"capabilityAlternateId\":\"" + this->m_settings->getConnectionParameters().capabilityAlternateId +
      "\",\"measures\":[";
  message = message + "[\"" + agrirouterMessage.encodedRequest();
  message = message + "\", \"" + uint32ToString(timestamp.tv_sec) + "\"]";
  message += "]}";

  printf("%s\n", message.c_str());

  std::vector<std::string> headers;
  headers.push_back("Content-type: application/json");
  headers.push_back("Accept: application/json");

  m_connectionProvider->setBody(message);
  m_connectionProvider->setUrl(this->m_settings->getConnectionParameters().measuresUrl);
  m_connectionProvider->setHeaders(headers);
  m_connectionProvider->setCallback(messageCallback);
  m_connectionProvider->setMember(&messageParameters);

  m_connectionProvider->sendMessage(messageParameters);

  return EXIT_SUCCESS;
}

void AgrirouterClient::getMessages(AgrirouterClient *self, ConnectionProvider::Callback callback) {
  // Create headers
  std::vector<std::string> headers;
  headers.push_back("Content-type: application/json");
  headers.push_back("Accept: application/json");

  self->m_connectionProvider->setBody("");
  self->m_connectionProvider->setUrl(this->m_settings->getConnectionParameters().commandsUrl);
  self->m_connectionProvider->setHeaders(headers);
  self->m_connectionProvider->setCallback(callback);
  self->m_connectionProvider->setMember(reinterpret_cast<void*>(self));

  self->m_connectionProvider->getMessages();
}
