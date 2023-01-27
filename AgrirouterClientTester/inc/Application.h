#ifndef AGRIROUTERCLIENTTESTER_INC_APPLICATION_H_
#define AGRIROUTERCLIENTTESTER_INC_APPLICATION_H_

#include <stdint.h>
#include <string>
#include <list>

#include <AgrirouterClient.h>
#include <AgrirouterMessage.h>
#include <Settings.h>
#include <Communicator.h>

class Application {
 private:
  Settings *m_settings;
  AgrirouterClient *m_agrirouterClient;
  Communicator *m_communicator;

  AgrirouterSettings m_agrirouterSettings;
  ApplicationSettings m_applicationSettings;

  bool m_onboarding;
  Addressing m_addressing;

 public:
  Application();
  ~Application();

  int32_t run(int32_t argc, char *argv[]);

  void writeIds();
  std::string directory;
  std::string getCurrentWorkingDir();

  static void onParameterChangeCallback(int event, void *data, void *callbackCallee);
  static void onMessageCallback(int event, Response *response, std::string applicationMessageId, void *callbackCallee);
  static void onErrorCallback(int statusCode, int connectionProviderErrorCode, std::string curlMessage, std::string applicationMessageId, std::string content, void *callbackCallee);
};

#endif  // AGRIROUTERCLIENTTESTER_INC_APPLICATION_H_
