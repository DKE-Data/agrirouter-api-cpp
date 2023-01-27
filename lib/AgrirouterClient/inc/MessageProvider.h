#ifndef LIB_AGRIROUTERCLIENT_INC_MESSAGEPROVIDER_H_
#define LIB_AGRIROUTERCLIENT_INC_MESSAGEPROVIDER_H_

#include "AgrirouterMessage.h"
#include "Definitions.h"
#include "Settings.h"

#include <list>
#include <string>

class MessageProvider {
 public:
  explicit MessageProvider(Settings *settings, uint32_t chunkSize);
  ~MessageProvider();

  // Messages without specific recipients
  AgrirouterMessage getCapabilityMessage(std::string *messageId, int32_t seqNo, const std::string &teamSetContextId,
                                          CapabilitySpecification *capabilities);

  AgrirouterMessage getSubscriptionMessage(std::string *messageId, int32_t seqNo, const std::string &teamSetContextId,
                                            Subscription *subscription);

  AgrirouterMessage getListEndpointsMessage(std::string *messageId, int32_t seqNo, const std::string &teamSetContextId,
                                            ListEndpointsQuery *listEndpointsQuery);

  AgrirouterMessage getListEndpointsUnfilteredMessage(std::string *messageId, int32_t seqNo, const std::string &teamSetContextId,
                                            ListEndpointsQuery *listEndpointsQuery);

  AgrirouterMessage getQueryMessage(std::string *messageId, int32_t seqNo, const std::string &teamSetContextId,
                                    MessageQuery *messageQuery);

  AgrirouterMessage getQueryHeaderMessage(std::string *messageId, int32_t seqNo, const std::string &teamSetContextId,
                                           MessageQuery *messageQuery);

  AgrirouterMessage getConfirmMessage(std::string *messageId, int32_t seqNo, const std::string &teamSetContextId,
                                        MessageConfirm *messageConfirm);

  AgrirouterMessage getDeleteMessage(std::string *messageId, int32_t seqNo, const std::string &teamSetContextId,
                                        MessageDelete *messageDelete);

  // Messages with specific recipients
  AgrirouterMessage getDeviceDescriptionMessage(std::string *messageId, int32_t seqNo, Addressing& addressing, 
                                                    const std::string &teamSetContextId, ISO11783_TaskData *taskData);

  AgrirouterMessage getTimelogMessage(std::string *messageId, int32_t seqNo, Addressing& addressing,
                                        const std::string &teamSetContextId, TimeLog *timelog);

  AgrirouterMessage getImageMessage(std::string *messageId, Addressing& addressing, int32_t seqNo,
                                        const std::string &teamSetContextId, char *image, int size);

  AgrirouterMessage getTaskdataZipMessage(std::string *messageId, Addressing& addressing, int32_t seqNo,
                                            const std::string &teamSetContextId, char *taskdataZip, int size);

  AgrirouterMessage getChunkedMessage(std::string *messageId, Addressing& addressing, int32_t seqNo, uint16_t numberOfChunk,
                                        uint16_t numberOfChunks, const std::string &teamSetContextId, const std::string &chunkContextId, 
                                        std::string data, uint32_t size, std::string& technicalMessageType);

 private:
  Settings *m_settings;
  uint32_t m_chunkSize;
  
  AgrirouterMessage getAgrirouterMessage(std::string *messageId, int32_t seqNo, std::string technicalMessageType,
                                          std::string typeUrl, const std::string &teamSetContextId, Message *message);

  AgrirouterMessage getAgrirouterMessage(std::string *messageId, int32_t seqNo, Addressing& addressing,
                                          std::string technicalMessageType, std::string typeUrl,
                                          const std::string &teamSetContextId, Message *message);

  AgrirouterMessage getAgrirouterMessage(std::string *messageId, int32_t seqNo, Addressing& addressing,
                                          std::string technicalMessageType, std::string typeUrl,
                                          const std::string &teamSetContextId, char *message, int size);

  AgrirouterMessage getMessage(std::string *messageId, Addressing& addressing, int32_t seqNo, const std::string& teamSetContextId,
                                          char *unchunkedData, int size, std::string& technicalMessageType);
};

#endif  // LIB_AGRIROUTERCLIENT_INC_MESSAGEPROVIDER_H_
