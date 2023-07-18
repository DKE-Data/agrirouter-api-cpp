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

        void registerDeviceWithRegCode(const std::string& registrationCode, AgrirouterSettings& agrirouterSettings);

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
        void sendDeviceDescription(Addressing& addressing, std::string *messageId, const std::string& teamsetId, ISO11783_TaskData *taskdata, const std::string& fileName = "");
        void sendTimelog(Addressing& addressing, std::string *messageId, const std::string& teamsetId, TimeLog *timelog, const std::string& fileName = "");
        void sendImage(Addressing& addressing, std::string *messageId, const std::string& teamsetId, char *image, int size, const std::string& fileName = "");
        void sendTaskdataZip(Addressing& addressing, std::string *messageId, const std::string& teamsetId, char *taskdataZip, int size, const std::string& fileName = "");
        void sendChunk(AgrirouterMessage& message);

        AgrirouterMessage createChunkMessage(std::string *messageId, Addressing& addressing, uint16_t numberOfChunk, uint16_t numberOfChunks, 
                                                const std::string& teamSetContextId, const std::string& chunkContextId, const std::string& data,
                                                uint32_t size, const std::string& technicalMessageType, const std::string& fileName = "");

        // Request messages
        void requestMessages();
        static size_t requestMessagesCallback(char *content, size_t size, size_t nmemb, void *member);
        static void registrationCallback(bool success, void *member);

        // Getter and Setter
        int32_t getNextSeqNo();

    private:
        void init(Settings *settings);

        int sendMessage(AgrirouterMessage *agrirouterMessage, int event, std::string *messageId);
        void getMessages(AgrirouterClient *self, ConnectionProvider::Callback callback);

        void callbackHandler(char *content, size_t size, MessageParameters& messageParameters);

        static size_t messageCallback(char *content, size_t size, size_t nmemb, void *member);

        MessageProvider *m_messageProvider = nullptr;
        ConnectionProvider *m_connectionProvider = nullptr;
        Settings *m_settings = nullptr;

        int32_t m_seqNo = 0;
        int32_t m_chunkSize = 0;
};

#endif  // LIB_AGRIROUTERCLIENT_INC_AGRIROUTERCLIENT_H_
