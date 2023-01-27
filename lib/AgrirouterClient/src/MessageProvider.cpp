#include "MessageProvider.h"

#include "AgrirouterMessageUtils.h"
#include <list>
#include <map>
#include <string>
#include <math.h>

MessageProvider::MessageProvider(Settings *settings, uint32_t chunkSize) 
{
  this->m_settings = settings;
  this->m_chunkSize = chunkSize;
}

MessageProvider::~MessageProvider() {}

AgrirouterMessage MessageProvider::getAgrirouterMessage(std::string *messageId, int32_t seqNo, 
    std::string technicalMessageType, std::string typeUrl, const std::string &teamSetContextId, Message *message)
{
  Addressing addressing;
  addressing.mode = RequestEnvelope::PUBLISH;

  return this->getAgrirouterMessage(messageId, seqNo, addressing, technicalMessageType,
                                    typeUrl, teamSetContextId, message);
}

AgrirouterMessage MessageProvider::getAgrirouterMessage(std::string *messageId, int32_t seqNo, Addressing& addressing, 
    std::string technicalMessageType, std::string typeUrl, const std::string &teamSetContextId, Message *message)
{
  std::string applicationMessageId;
  if (messageId->empty()) {
    applicationMessageId = createUuid();
  } else {
    applicationMessageId = *messageId;
  }
  Request request;
  request.envelope = createRequestHeader(applicationMessageId, seqNo, technicalMessageType, addressing, teamSetContextId);
  request.payloadWrapper = createRequestBody(message);

  google::protobuf::Any *details = request.payloadWrapper.mutable_details();
  details->set_type_url(typeUrl);

  return AgrirouterMessage(request);
}

AgrirouterMessage MessageProvider::getAgrirouterMessage(std::string *messageId, int32_t seqNo, Addressing& addressing, 
    std::string technicalMessageType, std::string typeUrl, const std::string &teamSetContextId, char *message, int size)
{
  std::string applicationMessageId;
  if (messageId->empty()) {
    applicationMessageId = createUuid();
  } else {
    applicationMessageId = *messageId;
  }
  Request request;
  request.envelope = createRequestHeader(applicationMessageId, seqNo, technicalMessageType, addressing, teamSetContextId);

  google::protobuf::Any *payload = request.payloadWrapper.mutable_details();
  payload->set_value(message, size);
  payload->set_type_url(typeUrl);

  return AgrirouterMessage(request);
}

AgrirouterMessage MessageProvider::getCapabilityMessage(std::string *messageId, int32_t seqNo,
    const std::string &teamSetContextId, CapabilitySpecification *capabilities)
{
  return getAgrirouterMessage(messageId, seqNo, "dke:capabilities", "types.agrirouter.com/agrirouter.request.payload."
                              "endpoint.CapabilitySpecification", teamSetContextId, capabilities);
}

AgrirouterMessage MessageProvider::getSubscriptionMessage(std::string *messageId, int32_t seqNo,
    const std::string &teamSetContextId, Subscription *subscription)
{
  return getAgrirouterMessage(messageId, seqNo, "dke:subscription", "types.agrirouter.com/agrirouter.request.payload."
                              "endpoint.Subscription", teamSetContextId, subscription);
}

AgrirouterMessage MessageProvider::getListEndpointsMessage(std::string *messageId, int32_t seqNo, 
    const std::string &teamSetContextId, ListEndpointsQuery *listEndpointsQuery)
{
  return getAgrirouterMessage(messageId, seqNo, "dke:list_endpoints", "types.agrirouter.com/agrirouter.request.payload."
                              "account.ListEndpointsQuery", teamSetContextId, listEndpointsQuery);
}

AgrirouterMessage MessageProvider::getListEndpointsUnfilteredMessage(std::string *messageId, int32_t seqNo,
    const std::string &teamSetContextId, ListEndpointsQuery *listEndpointsQuery)
{
  return getAgrirouterMessage(messageId, seqNo, "dke:list_endpoints_unfiltered", "types.agrirouter.com/agrirouter."
                              "request.payload.account.ListEndpointsQuery", teamSetContextId, listEndpointsQuery);
}

AgrirouterMessage MessageProvider::getQueryMessage(std::string *messageId, int32_t seqNo,
    const std::string &teamSetContextId, MessageQuery *messageQuery)
{
  return getAgrirouterMessage(messageId, seqNo, "dke:feed_message_query", "types.agrirouter.com/agrirouter.feed."
                              "request.MessageQuery", teamSetContextId, messageQuery);
}

AgrirouterMessage MessageProvider::getQueryHeaderMessage(std::string *messageId, int32_t seqNo,
    const std::string &teamSetContextId, MessageQuery *messageQuery)
{
  return getAgrirouterMessage(messageId, seqNo, "dke:feed_header_query", "types.agrirouter.com/agrirouter.feed."
                              "request.MessageQuery", teamSetContextId, messageQuery);
}

AgrirouterMessage MessageProvider::getConfirmMessage(std::string *messageId, int32_t seqNo,
    const std::string &teamSetContextId, MessageConfirm *messageConfirm)
{
  return getAgrirouterMessage(messageId, seqNo, "dke:feed_confirm", "types.agrirouter.com/agrirouter.feed."
                              "request.MessageConfirm", teamSetContextId, messageConfirm);
}

AgrirouterMessage MessageProvider::getDeleteMessage(std::string *messageId, int32_t seqNo,
    const std::string &teamSetContextId, MessageDelete *messageDelete)
{
  return getAgrirouterMessage(messageId, seqNo, "dke:feed_delete", "types.agrirouter.com/agrirouter.feed."
                              "request.MessageDelete", teamSetContextId, messageDelete);
}

AgrirouterMessage MessageProvider::getDeviceDescriptionMessage(std::string *messageId, int32_t seqNo,
    Addressing& addressing, const std::string &teamSetContextId, ISO11783_TaskData *taskData)
{
  return getAgrirouterMessage(messageId, seqNo, addressing, MESSAGE_TYPE_DEVICE_DESCRIPTION,
                              "types.agrirouter.com/efdi.ISO11783_TaskData", teamSetContextId, taskData);
}

AgrirouterMessage MessageProvider::getTimelogMessage(std::string *messageId, int32_t seqNo, Addressing& addressing,
    const std::string &teamSetContextId, TimeLog *timelog)
{
  return getAgrirouterMessage(messageId, seqNo, addressing, MESSAGE_TYPE_TIME_LOG,
                              "types.agrirouter.com/efdi.TimeLog", teamSetContextId, timelog);
}

AgrirouterMessage MessageProvider::getImageMessage(std::string *messageId, Addressing& addressing, int32_t seqNo,
    const std::string &teamSetContextId, char *image, int size)
{
    std::string messageType = "img:png";
  return getMessage(messageId, addressing, seqNo, teamSetContextId, image, size, messageType);
}

AgrirouterMessage MessageProvider::getTaskdataZipMessage(std::string *messageId, Addressing& addressing, int32_t seqNo,
      const std::string &teamSetContextId, char *taskdataZip, int size)
{
    std::string messageType = MESSAGE_TYPE_TASK_DATA;
  return getMessage(messageId, addressing, seqNo, teamSetContextId, taskdataZip, size, messageType);
}

AgrirouterMessage MessageProvider::getMessage(std::string *messageId, Addressing& addressing, int32_t seqNo, 
  const std::string &teamSetContextId, char *unchunkedData, int size, std::string& technicalMessageType)
{
    AgrirouterMessage message;

    std::string applicationMessageId;
    if (messageId->empty()) {
        applicationMessageId = createUuid();
    } else {
        applicationMessageId = *messageId;
    }
    Request request;
    request.envelope = createRequestHeader(applicationMessageId, seqNo, technicalMessageType, addressing, teamSetContextId);
    google::protobuf::Any *payload = request.payloadWrapper.mutable_details();
    payload->set_value(unchunkedData);
    return AgrirouterMessage(request);
}

AgrirouterMessage MessageProvider::getChunkedMessage(std::string *messageId, Addressing& addressing, int32_t seqNo,
  uint16_t numberOfChunk, uint16_t numberOfChunks, const std::string &teamSetContextId, 
  const std::string &chunkContextId, std::string data, uint32_t size, std::string& technicalMessageType)
{
    AgrirouterMessage message;

  *messageId = createUuid();

  Request request;
  request.envelope = createRequestHeader(*messageId, seqNo, technicalMessageType, addressing, teamSetContextId);
  ChunkComponent *chunk = request.envelope.mutable_chunk_info();
  // Start counting at 1
  chunk->set_context_id(chunkContextId);
  chunk->set_current(numberOfChunk);
  chunk->set_total(numberOfChunks);
  chunk->set_total_size(size);

  google::protobuf::Any *payload = request.payloadWrapper.mutable_details();
  payload->set_value(data);

    return AgrirouterMessage(request);
}