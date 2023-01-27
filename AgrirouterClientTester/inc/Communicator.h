#ifndef AGRIROUTERCLIENTTESTER_INC_COMMUNICATOR_H_
#define AGRIROUTERCLIENTTESTER_INC_COMMUNICATOR_H_

#include <stdint.h>
#include <string>
#include <list>

#include "AgrirouterClient.h"
#include "AgrirouterMessage.h"
#include "Settings.h"

class Communicator {
    private:
        Settings *m_settings;
        AgrirouterClient *m_agrirouterClient;
        AgrirouterSettings m_agrirouterSettings;

        Addressing m_addressing;

    public:
        Communicator();
        Communicator(Settings *m_settings, AgrirouterClient *m_agrirouterClient, AgrirouterSettings agrirouterSettings);
        ~Communicator();

        void onboard(std::string registrationCode, std::string externalId);
        void sendCapabilities();
        void sendSubscription();
        
        void getListEndpointsFiltered();
        void getListEndpointsUnfiltered();

        int sendMessageQuery();
        void sendMessageQueryWithId(std::list<std::string> messageIds);
        void sendMessageQueryWithSender(std::list<std::string> senders);
        void sendMessageQueryWithValidityPeriod(ValidityPeriod *validityPeriod);
        void getMessages();
};

#endif  // AGRIROUTERCLIENTTESTER_INC_COMMUNICATOR_H_
