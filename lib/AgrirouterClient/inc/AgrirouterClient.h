#ifndef LIB_AGRIROUTERCLIENT_INC_AGRIROUTERCLIENT_H_
#define LIB_AGRIROUTERCLIENT_INC_AGRIROUTERCLIENT_H_

#include "AgrirouterMessage.h"
#include "ConnectionProvider.h"
#include "Definitions.h"
#include "MessageProvider.h"
#include "Settings.h"

#include <list>
#include <string>

class AgrirouterClient {
    public:
        AgrirouterClient();
        explicit AgrirouterClient(Settings *settings);
        explicit AgrirouterClient(Settings *settings, uint32_t chunkSize);
        ~AgrirouterClient();

        void registerDeviceWithRegCode(std::string& registrationCode, AgrirouterSettings& agrirouterSettings);

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
        void sendDeviceDescription(Addressing& addressing, std::string *messageId, std::string& teamsetId, ISO11783_TaskData *taskdata);
        void sendTimelog(Addressing& addressing, std::string *messageId, std::string& teamsetId, TimeLog *timelog);
        void sendImage(Addressing& addressing, std::string *messageId, std::string& teamsetId, char *image, int size);
        void sendTaskdataZip(Addressing& addressing, std::string *messageId, std::string& teamsetId, char *taskdataZip, int size);
        void sendChunk(AgrirouterMessage& message);

        AgrirouterMessage createChunkMessage(std::string *messageId, Addressing& addressing, uint16_t numberOfChunk, 
                                                uint16_t numberOfChunks, const std::string &teamSetContextId,
                                                const std::string &chunkContextId, std::string& data,
                                                uint32_t size, std::string& technicalMessageType);

        // Request messages
        void requestMessages();
        static size_t requestMessagesCallback(char *content, size_t size, size_t nmemb, void *member);

        // Getter and Setter
        int32_t getNextSeqNo();

    private:
        void init(Settings *settings);

        int sendMessage(AgrirouterMessage *agrirouterMessage, int event, std::string *messageId);
        void getMessages(AgrirouterClient *self, ConnectionProvider::Callback callback);

        void callbackHandler(char *content, size_t size, MessageParameters& messageParameters);

        static size_t messageCallback(char *content, size_t size, size_t nmemb, void *member);

        MessageProvider *m_messageProvider;
        ConnectionProvider *m_connectionProvider;
        Settings *m_settings;

        int32_t m_seqNo;
        int32_t m_chunkSize;
};

#endif  // LIB_AGRIROUTERCLIENT_INC_AGRIROUTERCLIENT_H_
