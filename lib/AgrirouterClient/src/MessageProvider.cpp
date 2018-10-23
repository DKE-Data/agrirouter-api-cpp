#include <MessageProvider.h>

#include <AgrirouterMessageUtils.h>
#include <math.h>

#include <list>
#include <map>
#include <string>

MessageProvider::MessageProvider(Settings *settings) { m_settings = settings; }

MessageProvider::~MessageProvider() {}

AgrirouterMessage MessageProvider::getAgrirouterMessage(std::string *messageId, int32_t seqNo, std::string technicalMessageType, std::string typeUrl, const std::string &teamSetContextId, Message *message) {
  Addressing addressing;
  addressing.mode = RequestEnvelope::PUBLISH;

  return this->getAgrirouterMessage(messageId, seqNo, addressing, technicalMessageType, typeUrl, teamSetContextId, message);
}

AgrirouterMessage MessageProvider::getAgrirouterMessage(std::string *messageId, int32_t seqNo, Addressing addressing, std::string technicalMessageType, std::string typeUrl, const std::string &teamSetContextId, Message *message) {
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

  AgrirouterMessage agrirouterMsg = AgrirouterMessage(request);

  return agrirouterMsg;
}

AgrirouterMessage MessageProvider::getAgrirouterMessage(std::string *messageId, int32_t seqNo, Addressing addressing, std::string technicalMessageType, std::string typeUrl, const std::string &teamSetContextId, char *message, int size) {
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

  AgrirouterMessage agrirouterMsg = AgrirouterMessage(request);

  return agrirouterMsg;
}

AgrirouterMessage MessageProvider::getCapabilityMessage(std::string *messageId, int32_t seqNo, const std::string &teamSetContextId, CapabilitySpecification *capabilites) {
  return getAgrirouterMessage(messageId, seqNo, "dke:capabilities", "types.agrirouter.com/agrirouter.request.payload.endpoint.CapabilitySpecification", teamSetContextId, capabilites);
}

AgrirouterMessage MessageProvider::getSubscriptionMessage(std::string *messageId, int32_t seqNo, const std::string &teamSetContextId, Subscription *subscription) {
  return getAgrirouterMessage(messageId, seqNo, "dke:subscription", "types.agrirouter.com/agrirouter.request.payload.endpoint.Subscription", teamSetContextId, subscription);
}

AgrirouterMessage MessageProvider::getListEndpointsMessage(std::string *messageId, int32_t seqNo, const std::string &teamSetContextId, ListEndpointsQuery *listEndpointsQuery) {
  return getAgrirouterMessage(messageId, seqNo, "dke:list_endpoints", "types.agrirouter.com/agrirouter.request.payload.account.ListEndpointsQuery", teamSetContextId, listEndpointsQuery);
}

AgrirouterMessage MessageProvider::getListEndpointsUnfilteredMessage(std::string *messageId, int32_t seqNo, const std::string &teamSetContextId, ListEndpointsQuery *listEndpointsQuery) {
  return getAgrirouterMessage(messageId, seqNo, "dke:list_endpoints_unfiltered", "types.agrirouter.com/agrirouter.request.payload.account.ListEndpointsQuery", teamSetContextId, listEndpointsQuery);
}

AgrirouterMessage MessageProvider::getQueryMessage(std::string *messageId, int32_t seqNo, const std::string &teamSetContextId, MessageQuery *messageQuery) {
  return getAgrirouterMessage(messageId, seqNo, "dke:feed_message_query", "types.agrirouter.com/agrirouter.feed.request.MessageQuery", teamSetContextId, messageQuery);
}

AgrirouterMessage MessageProvider::getQueryHeaderMessage(std::string *messageId, int32_t seqNo, const std::string &teamSetContextId, MessageQuery *messageQuery) {
  return getAgrirouterMessage(messageId, seqNo, "dke:feed_header_query", "types.agrirouter.com/agrirouter.feed.request.MessageQuery", teamSetContextId, messageQuery);
}

AgrirouterMessage MessageProvider::getConfirmMessage(std::string *messageId, int32_t seqNo, const std::string &teamSetContextId, MessageConfirm *messageConfirm) {
  return getAgrirouterMessage(messageId, seqNo, "dke:feed_confirm", "types.agrirouter.com/agrirouter.feed.request.MessageConfirm", teamSetContextId, messageConfirm);
}

AgrirouterMessage MessageProvider::getDeleteMessage(std::string *messageId, int32_t seqNo, const std::string &teamSetContextId, MessageDelete *messageDelete) {
  return getAgrirouterMessage(messageId, seqNo, "dke:feed_delete", "types.agrirouter.com/agrirouter.feed.request.MessageDelete", teamSetContextId, messageDelete);
}

/*AgrirouterMessage MessageProvider::getDeviceDescriptionMessage(std::string *messageId, int32_t seqNo, Addressing addressing, const std::string &teamSetContextId, ISO11783_TaskData *taskData) {
  return getAgrirouterMessage(messageId, seqNo, addressing, "iso:11783:-10:device_description:protobuf", "types.agrirouter.com/efdi.ISO11783_TaskData", teamSetContextId, taskData);
}*/

/*AgrirouterMessage MessageProvider::getTimelogMessage(std::string *messageId, int32_t seqNo, Addressing addressing, const std::string &teamSetContextId, TimeLog *timelog) {
  return getAgrirouterMessage(messageId, seqNo, addressing, "iso:11783:-10:time_log:protobuf", "types.agrirouter.com/efdi.TimeLog", teamSetContextId, timelog);
}*/

std::list<AgrirouterMessage> MessageProvider::getImageMessage(std::string *messageId, int32_t seqNo, Addressing addressing, const std::string &teamSetContextId, char *image, int size) {
  return getChunkedMessages(messageId, seqNo, addressing, teamSetContextId, image, size, "img:png");
}

std::list<AgrirouterMessage> MessageProvider::getTaskdataZipMessage(std::string *messageId, int32_t seqNo, Addressing addressing, const std::string &teamSetContextId, char *taskdataZip, int size) {
  return getChunkedMessages(messageId, seqNo, addressing, teamSetContextId, taskdataZip, size, "iso:11783:-10:taskdata:zip");
}

std::list<AgrirouterMessage> MessageProvider::getChunkedMessages(std::string *messageId, int32_t seqNo, Addressing addressing, const std::string &teamSetContextId, char *unchunkedData, int size, std::string technicalMessageType) {
  int chunkSize = 500000;
  // Do we have to chunk? 500kB is the size when we chunk

  std::list<AgrirouterMessage> list;

  if (size > chunkSize) {
    int count = 0;
    int chunks = getNumberOfChunks(size, chunkSize);

    std::string contextId = createUuid();
    std::string data(unchunkedData, size);
    for (int i = 0; i < chunks; i++) {
      count++;
      std::string applicationMessageId;

      if (messageId->empty()) {
        applicationMessageId = createUuid();
      } else {
        applicationMessageId = getModifiedUuid(*messageId, count);
      }

      Request request;
      request.envelope = createRequestHeader(applicationMessageId, seqNo, technicalMessageType, addressing, teamSetContextId);
      ChunkComponent *chunk = request.envelope.mutable_chunk_info();
      // Start counting at 1
      chunk->set_context_id(contextId);
      chunk->set_current(count);
      chunk->set_total(chunks);
      chunk->set_total_size(size);

      std::string chunkedData = "";
      if (i <= (chunks - 1)) {
        chunkedData = data.substr(i * chunkSize, chunkSize);
      } else {
        chunkedData = data.substr(i * chunkSize);
      }

      google::protobuf::Any *payload = request.payloadWrapper.mutable_details();
      payload->set_value(chunkedData);
      list.push_back(AgrirouterMessage(request));
    }
  } else {
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
    list.push_back(AgrirouterMessage(request));
  }
  return list;
}
