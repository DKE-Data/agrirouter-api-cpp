#ifndef AGRIROUTERCLIENTTESTER_INC_APPLICATION_H_
#define AGRIROUTERCLIENTTESTER_INC_APPLICATION_H_

#include <stdint.h>
#include <string>
#include <list>

#include "AgrirouterClient.h"
#include "AgrirouterMessage.h"
#include "Settings.h"
#include "Communicator.h"

class Application {
    private:
        Settings *m_settings = nullptr;
        AgrirouterClient *m_agrirouterClient = nullptr;
        Communicator *m_communicator = nullptr;

        AgrirouterSettings m_agrirouterSettings;
        ApplicationSettings m_applicationSettings;

        bool m_onboarding = false;
        bool m_running = true;
        int8_t m_minLogLevel = MG_LFL_DBG; // set it to max loglevel
        Addressing m_addressing;

    public:
        Application();
        ~Application();

        std::string directory = "";

        int32_t run(int32_t argc, char *argv[]);

        void writeIds();
        
        std::string getCurrentWorkingDir();

        static void onParameterChangeCallback(int event, void *data, void *callbackCallee);
        static void onMessageCallback(int event, Response *response, std::string applicationMessageId, void *callbackCallee);
        static void onErrorCallback(int statusCode, int connectionProviderErrorCode, std::string errorMessage, std::string applicationMessageId, std::string errorContent, void *callbackCallee);
        static void onLogCallback(int logLevel, std::string logMessage, void *callbackCallee);
};

#endif  // AGRIROUTERCLIENTTESTER_INC_APPLICATION_H_
