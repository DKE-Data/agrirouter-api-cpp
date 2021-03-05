#ifndef LIB_AGRIROUTERCLIENT_INC_AGRIROUTERCLIENT_H_
#define LIB_AGRIROUTERCLIENT_INC_AGRIROUTERCLIENT_H_

#include <AgrirouterMessage.h>
#include <ConnectionProvider.h>
#include <Definitions.h>
#include <MessageProvider.h>
#include <Settings.h>

#include <list>
#include <string>

class AgrirouterClient {
 public:
  AgrirouterClient();
  explicit AgrirouterClient(Settings *settings);
  ~AgrirouterClient();
  void terminate();

  void registerDeviceWithRegCode(std::string registrationCode, AgrirouterSettings agrirouterSettings);

  // Messages without specific recipients
  void sendCapabilities(std::string *messageId, CapabilitySpecification *capabilities);
  void sendSubscription(std::string *messageId, Subscription *subscription);
  void sendListEndpointsFiltered(std::string *messageId, ListEndpointsQuery *listEndpointsQuery);
  void sendListEndpointsUnfiltered(std::string *messageId, ListEndpointsQuery *listEndpointsQuery);
  void sendQueryMessages(std::string *messageId, MessageQuery *messageQuery);
  void sendQueryHeaderMessages(std::string *messageId, MessageQuery *messageQuery);
  void sendMessagesConfirm(std::string *messageId, MessageConfirm *messageConfirm);
  void sendMessagesDelete(std::string *messageId, MessageDelete *messageDelete);

  // Messages with specific recipients
  // void sendDeviceDescription(Addressing addressing, std::string *messageId, ISO11783_TaskData *taskdata);
  // void sendTimelog(Addressing addressing, std::string *messageId, TimeLog *timelog);
  void sendImage(Addressing addressing, std::string *messageId, char *image, int size);
  void sendTaskdataZip(Addressing addressing, std::string *messageId, char *taskdataZip, int size);

  // Request messages
  void requestMessages();
  static size_t requestMessagesCallback(char *content, size_t size, size_t nmemb, void *member);

  // Getter and Setter
  int32_t getNextSeqNo();
  std::vector<int32_t> AgrirouterClient::getNextChunkedSeqNos(int32_t size);
  std::string getContextId() const;
  void setContextId(const std::string &contextId);

 private:
  void init(Settings *settings);

  int sendMessage(AgrirouterMessage agrirouterMessage, int event, std::string *messageId);
  void getMessages(AgrirouterClient *self, ConnectionProvider::Callback callback);

  void callbackHandler(char *content, size_t size, MessageParameters messageParameters);

  static size_t messageCallback(char *content, size_t size, size_t nmemb, void *member);

  MessageProvider *m_messageProvider;
  ConnectionProvider *m_connectionProvider;
  Settings *m_settings;

  std::string m_contextId;
  int32_t m_seqNo;

  std::string m_xsuaaToken;
};

#endif  // LIB_AGRIROUTERCLIENT_INC_AGRIROUTERCLIENT_H_
