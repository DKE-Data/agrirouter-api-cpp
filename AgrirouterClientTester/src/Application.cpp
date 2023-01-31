#include "Application.h"

#include <stdio.h>  /* defines FILENAME_MAX */
// #define WINDOWS  /* uncomment this line to get current working directory for windows.*/
#ifdef WINDOWS
#include <direct.h>
#define GetCurrentDir _getcwd
#else
#include <unistd.h>
#define GetCurrentDir getcwd
#endif

#include "AgrirouterClient.h"
#include "AgrirouterMessageUtils.h"
#include "Registration.h"
#include "Utils.h"

#include <algorithm>
#include <iostream>
#include <list>
#include <map>
#include <string>
#include <vector>

Application::Application()
{
    directory = getCurrentWorkingDir();
    m_settings = new Settings();

    m_onboarding = false;
    m_addressing.mode = RequestEnvelope::PUBLISH;
}

Application::~Application()
{
    if (this->m_communicator != NULL)
    {
        delete this->m_communicator;
        this->m_communicator = NULL;
    }

    if (this->m_agrirouterClient != NULL)
    {
        delete this->m_agrirouterClient;
        this->m_agrirouterClient = NULL;
    }

    if (this->m_settings != NULL)
    {
        delete this->m_settings;
        this->m_settings = NULL;
    }
}

std::string Application::getCurrentWorkingDir()
{
    char buff[FILENAME_MAX];
    char* res = GetCurrentDir(buff, FILENAME_MAX);
    std::string currentWorkingDir(res);
    currentWorkingDir += "/";
    return currentWorkingDir;
}

int32_t Application::run(int32_t argc, char *argv[])
{
    // Set callback settings
    m_settings->setOnParameterChangeCallback(onParameterChangeCallback);
    m_settings->setOnMessageCallback(onMessageCallback);
    m_settings->setOnErrorCallback(onErrorCallback);

    printf("Read file %sAgrirouterClientTesterConfig.json\n", directory.c_str());

    // Load settings from file
    std::string agrirouterSettings = directory + "AgrirouterClientTesterConfig.json";
    this->m_agrirouterSettings = getAgrirouterSettings(agrirouterSettings);
    printf("AgrirouterSettings: \n\t%s\n", this->m_agrirouterSettings.registrationUrl.c_str());

    std::string applicationSettings = directory + "AgrirouterClientTesterConfig.json";
    this->m_applicationSettings = getApplicationSettings(applicationSettings);
    printf("ApplicationSettings: \n\t%s \n\t%s \n\t%s \n\t%s \n\t%s\n", this->m_applicationSettings.applicationId.c_str(),
            this->m_applicationSettings.certificationVersionId.c_str(), this->m_applicationSettings.externalId.c_str(),
            this->m_applicationSettings.locationCertsAndIds.c_str(), this->m_applicationSettings.teamsetContextId.c_str());

    m_settings->setApplicationId(this->m_applicationSettings.applicationId);
    m_settings->setCertificationVersionId(this->m_applicationSettings.certificationVersionId);
    m_settings->setCertificatePath(this->m_applicationSettings.locationCertsAndIds + "/agrirouter_cert.pem");
    m_settings->setPrivateKeyPath(this->m_applicationSettings.locationCertsAndIds + "/agrirouter_key.pem");
    m_settings->setConnectionParametersPath(this->m_applicationSettings.locationCertsAndIds + "/agrirouter_ids.json");
    // m_settings->setCaFilePath("");

    ConnectionParameters conn = m_settings->getConnectionParameters(m_settings->getConnectionParametersPath());
    printf("ConnectionParameters: \n\tgatewayId %s \n\thost %s \n\tport %s\n\tclientId %s\n\tsecret %s\n", conn.gatewayId.c_str(), conn.host.c_str(), conn.port.c_str(), conn.clientId.c_str(), conn.secret.c_str());

    m_settings->setEncodingType("base64");
    if (this->m_applicationSettings.connectionType == "HTTP")
    {
        m_settings->setConnectionType(Settings::HTTP);
        m_settings->setPollingInterval(2);
        m_settings->setPollingMaxTime(6);
    }
    else if (this->m_applicationSettings.connectionType == "MQTT")
    {
        // ToDo: MQTT needs to be implemented
        m_settings->setConnectionType(Settings::MQTT);
    }

    // Initialize agrirouter client instance
    m_agrirouterClient = new AgrirouterClient(m_settings);
    m_communicator = new Communicator(m_settings, m_agrirouterClient, m_agrirouterSettings);

    std::vector<std::string> cmdLineArgs(argv, argv + argc);

    std::string arg = "";
    for (unsigned int i = 0; i < cmdLineArgs.size(); i++)
    {
        arg = cmdLineArgs[i];
        if (arg == "--help" || arg == "-help" || arg == "-h")
        {
            std::cout << "--------------------------------------------\n";
            std::cout << "These are the CLI commands wihout arguments:\n";
            std::cout << "--------------------------------------------\n";
            std::cout << "  -c\tsend capabilities\n";
            std::cout << "  -s\tsend subscription\n";
            std::cout << "  -g\tpoll for messages in outbox\n";
            std::cout << "  -q\tfeed header query\n";
            std::cout << "  -f\tfiltered list of endpoints\n";
            std::cout << "  -u\tunfiltered list of endpoints\n\n";
            std::cout << "--------------------------------------------\n";
            std::cout << "These are the CLI commands wiht arguments:\n";
            std::cout << "--------------------------------------------\n";
            std::cout << "  --onboard=<TAN>\tonboard CU with generated TAN/registration code\n";
            std::cout << "  \t\t\te.g. --onboard=1904a5f8-abd8-4f7d-bb20-27a704051904\n";
            std::cout << "  --id=<ID>\t\tset teamset context ID\n";
            std::cout << "  \t\t\te.g. --id=1904a5f8-abd8-4f7d-bb20-27a704051904\n";
            std::cout << "  --minutes=<MINUTES>\trequest messages of the last given minutes\n";
            std::cout << "  \t\t\te.g. --minutes=120\n";
            std::cout << "  --sender=<SENDER>\trequest messages of given sender\n";
            std::cout << "  \t\t\te.g. --sender=1904a5f8-abd8-4f7d-bb20-27a704051904\n";
            std::cout << "  --query-id=<ID>\trequest message with given ID\n";
            std::cout << "  \t\t\te.g. --query-id=1904a5f8-abd8-4f7d-bb20-27a704051904\n";
            std::cout << "  --confirm=<ID>\tconfirm message with given ID\n";
            std::cout << "  \t\t\te.g. --confirm=1904a5f8-abd8-4f7d-bb20-27a704051904\n";
            std::cout << "  --delete-sender=<SENDER>\tdelete messages of sender with given ID\n";
            std::cout << "  \t\t\te.g. --delete-sender=1904a5f8-abd8-4f7d-bb20-27a704051904\n";
            std::cout << "  --delete-id=<ID>\tdelete message with given ID\n";
            std::cout << "  \t\t\te.g. --delete-id=1904a5f8-abd8-4f7d-bb20-27a704051904\n";
            std::cout << "  --recipient=<RECIPIENT>\tadd recipient for messages\n";
            std::cout << "  \t\t\te.g. --recipient=1904a5f8-abd8-4f7d-bb20-27a704051904\n";
            std::cout << "  --url=<URL>\t\tchange URL to communicate to\n";
            std::cout << "  \t\t\te.g. --url=https://register-url.com\n";
            std::cout << "  --taskdata=<PATH>\t\tsend taskdata.zip file that is saved at given path\n";
            std::cout << "  \t\t\te.g. --taskdata=/home/TASKDATA.zip\n";
        }
        else if (arg == "-c")
        {
            this->m_communicator->sendCapabilities();
        }
        else if (arg == "-s")
        {
            this->m_communicator->sendSubscription();
        }
        else if (arg == "-g")
        {
            this->m_communicator->getMessages();
        }
        else if (arg == "-q")
        {
            this->m_communicator->sendMessageQuery();
        }
        else if (arg == "-f")
        {
            this->m_communicator->getListEndpointsFiltered();
        }
        else if (arg == "-u")
        {
            this->m_communicator->getListEndpointsUnfiltered();
        }
        else if (arg.find("--onboard") != std::string::npos)
        {
            size_t begin = arg.find("=");
            std::string message = arg.substr(begin + 1);

            m_onboarding = true;
            this->m_settings->setCallbackCallee(this);
            
            std::string externalId = this->m_applicationSettings.externalId;
            this->m_communicator->onboard(message, externalId);
        }
        else if (arg.find("--id") != std::string::npos)
        {
            size_t begin = arg.find("=");
            std::string message = arg.substr(begin + 1);
            // ToDo set context id on message send
            // m_agrirouterClient->setContextId(message);

        }
        else if (arg.find("--minutes") != std::string::npos)
        {
            size_t begin = arg.find("=");
            std::string message = arg.substr(begin + 1);

            int minutes = stringToInt(message);
            ValidityPeriod validityPeriod = getValidityPeriodForLastMinutes(minutes);

            this->m_communicator->sendMessageQueryWithValidityPeriod(&validityPeriod);
        }
        else if (arg.find("--sender") != std::string::npos)
        {
            size_t begin = arg.find("=");
            std::string message = arg.substr(begin + 1);

            std::list<std::string> senders;
            senders.push_back(message);
            this->m_communicator->sendMessageQueryWithSender(senders);
        }
        else if (arg.find("--query-id") != std::string::npos)
        {
            size_t begin = arg.find("=");
            std::string message = arg.substr(begin + 1);

            std::list<std::string> messageIds;
            messageIds.push_back(message);
            this->m_communicator->sendMessageQueryWithId(messageIds);
        }
        else if (arg.find("--confirm") != std::string::npos)
        {
            size_t begin = arg.find("=");
            std::string message = arg.substr(begin + 1);

            MessageConfirm messageConfirm = MessageConfirm();
            messageConfirm.add_message_ids(message);

            std::string messageId;
            this->m_agrirouterClient->sendMessagesConfirm(&messageId, &messageConfirm);
        }
        else if (arg.find("--delete-sender") != std::string::npos)
        {
            size_t begin = arg.find("=");
            std::string message = arg.substr(begin + 1);

            MessageDelete messageDelete = MessageDelete();
            messageDelete.add_senders(message);

            std::string messageId;
            this->m_agrirouterClient->sendMessagesDelete(&messageId, &messageDelete);
        }
        else if (arg.find("--delete-id") != std::string::npos)
        {
            size_t begin = arg.find("=");
            std::string message = arg.substr(begin + 1);

            MessageDelete messageDelete = MessageDelete();
            messageDelete.add_message_ids(message);

            std::string messageId;
            this->m_agrirouterClient->sendMessagesDelete(&messageId, &messageDelete);
        }
        else if (arg.find("--recipient") != std::string::npos)
        {
            size_t begin = arg.find("=");
            std::string message = arg.substr(begin + 1);

            m_addressing.mode = RequestEnvelope::PUBLISH_WITH_DIRECT;
            m_addressing.recipients.push_back(message);
        }
        else if (arg.find("--url") != std::string::npos)
        {
            size_t begin = arg.find("=");
            std::string url = arg.substr(begin + 1);

            this->m_agrirouterSettings.registrationUrl = url;
        }
        else if (arg.find("--taskdata") != std::string::npos)
        {
            size_t begin = arg.find("=");
            std::string path = arg.substr(begin + 1);

            std::string messageId;
            std::string message = readBinaryFileAndBase64(path);
            std::string teamsetContextId = this->m_applicationSettings.teamsetContextId;
            m_agrirouterClient->sendTaskdataZip(m_addressing, &messageId, teamsetContextId, const_cast<char *>(message.c_str()), message.size());
        }
    }

    timeval timeout;
    while (m_onboarding)
    {
        timeout.tv_sec = 1;
        select(0, NULL, NULL, NULL, &timeout);
    }

    return EXIT_SUCCESS;
}

void Application::onParameterChangeCallback(int event, void *data, void *callbackCallee)
{
    printf("Parameter change (event %d)\n", event);

    Application *self = reinterpret_cast<Application *>(callbackCallee);

    std::string *dataAsString;

    if (self->m_onboarding)
    {
        switch (event)
        {
            case MG_PARAMETER_CERTIFICATE:

                dataAsString = reinterpret_cast<std::string *>(data);
                printf("MG_PARAMETER_CERTIFICATE: %s\npath: %s\n", dataAsString->c_str(), self->m_settings->getCertificatePath().c_str());
                writeFile(*dataAsString, self->m_settings->getCertificatePath());

                break;

            case MG_PARAMETER_PRIVATE_KEY:

                dataAsString = reinterpret_cast<std::string *>(data);
                printf("MG_PARAMETER_PRIVATE_KEY: %s\npath: %s\n", dataAsString->c_str(), self->m_settings->getPrivateKeyPath().c_str());
                writeFile(dataAsString->c_str(), self->m_settings->getPrivateKeyPath());

                break;

            case MG_PARAMETER_CONNECTION_PARAMETERS:

                ConnectionParameters *parameters = reinterpret_cast<ConnectionParameters *>(data);
                printf("MG_PARAMETER_CONNECTION_PARAMETERS: path %s\n",  self->m_settings->getConnectionParametersPath().c_str());
                saveConnectionParameters(parameters, self->m_settings->getConnectionParametersPath());

                self->m_onboarding = false;
                break;
        }
    }
}

void Application::onMessageCallback(int event, Response *response, std::string applicationMessageId, void *callbackCallee)
{
    printf("\n");
    printf("Message callback (event %d)\n", event);
    Application *self = reinterpret_cast<Application *>(callbackCallee);
    std::string msg;
    std::list<std::string> messageIds;

    std::string envelope = messageToJson(&response->envelope);
    printf("Envelope (size %lu): %s\n", envelope.size(), envelope.c_str());

    if (((response->envelope.response_code() != 200) || (response->envelope.response_code() != 201)) && (response->payloadWrapper.details().value().size() != 0))
        {
        printf("Payload type_url: \"%s\", payload size: %lu\n", response->payloadWrapper.details().type_url().c_str(), response->payloadWrapper.details().value().size());

        if (response->payloadWrapper.details().type_url() == "types.agrirouter.com/agrirouter.feed.response.HeaderQueryResponse")
        {
            HeaderQueryResponse header = HeaderQueryResponse();
            header.ParseFromString(response->payloadWrapper.details().value());

            msg = messageToJson(&header);
            printf("HeaderQueryResponse: %s\n", msg.c_str());
        }
        else if (response->payloadWrapper.details().type_url() == "types.agrirouter.com/agrirouter.feed.response.MessageQueryResponse")
        {
            char *p = const_cast<char *>(response->payloadWrapper.details().value().c_str());

            MessageQueryResponse qr = MessageQueryResponse();
            qr.ParseFromArray(p, response->payloadWrapper.details().value().size());

            std::string s = messageToJson(&qr);
            printf("MessageQueryResponse: %s\n", s.c_str());

            google::protobuf::RepeatedPtrField<MessageQueryResponse::FeedMessage> *m = qr.mutable_messages();

            for (int i = 0; i < qr.messages_size(); i++)
            {
                MessageQueryResponse::FeedMessage *feedMessage = m->Mutable(i);
                MessageQueryResponse::Header *h = feedMessage->mutable_header();
                google::protobuf::Any *content = feedMessage->mutable_content();

                printf("type_url size %lu, size %lu\n", content->type_url().size(), content->value().size());
                printf("technical_message_type %s\n", h->technical_message_type().c_str());
                if (h->technical_message_type() == "iso:11783:-10:taskdata:zip") {
                std::string file = content->value();
                std::string path = self->directory + "received.zip";
                writeBase64EncodedBinaryFile(file, path);
                }
            }

        }
        else if (response->payloadWrapper.details().type_url() == "types.agrirouter.com/agrirouter.commons.Messages")
        {
            CommonsMessages commons = CommonsMessages();
            commons.ParseFromString(response->payloadWrapper.details().value());
            msg = messageToJson(&commons);
            printf("Messages, type_url: %s\nvalue: %s\n", response->payloadWrapper.details().type_url().c_str(), msg.c_str());
        }
        else if (response->payloadWrapper.details().type_url() == "types.agrirouter.com/agrirouter.response.payload.account.ListEndpointsResponse")
        {
            ListEndpointsResponse endpoints = ListEndpointsResponse();
            endpoints.ParseFromString(response->payloadWrapper.details().value());
            msg = messageToJson(&endpoints);
            printf("ListEndpointsResponse: %s\n", msg.c_str());
        }
    }
}

void Application::onErrorCallback(int statusCode, int connectionProviderErrorCode, std::string curlMessage,
                                    std::string applicationMessageId, std::string content, void *callbackCallee)
{
    printf("\n");
    printf("On Error callback: http code: %i, CURL code: %i, CURL message: %s, Application message id: %s, error page content: %s\n",
                statusCode, connectionProviderErrorCode, curlMessage.c_str(), applicationMessageId.c_str(), content.c_str());
}
