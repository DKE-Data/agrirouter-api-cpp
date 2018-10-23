#ifndef LIB_AGRIROUTERCLIENT_INC_REGISTRATION_H_
#define LIB_AGRIROUTERCLIENT_INC_REGISTRATION_H_

#include <ConnectionProvider.h>
#include <Definitions.h>
#include <Settings.h>

#include <string>
#include <vector>

class Registration {
 public:
  Registration(ConnectionProvider *connectionProvider, Settings *settings);
  ~Registration();

  /**
   * Onboard device with registration code.
   */
  void registerToAgrirouterWithRegCode(std::string registrationCode, AgrirouterSettings agrirouterSettings);

  /**
   * Send request to finish onboarding/registration process.
   */
  void sendOnboard(AgrirouterSettings agrirouterSettings);

  /**
   * Write callback of get endpoint registration code.
   *
   * @return The size of the incoming data.
      */
  static size_t sendOnboardCallback(char *content, size_t size, size_t nmemb, void *member);

  /**
   * Checks if string contains an error message.
   *
   * @return Contains error or not.
      */
  static bool containsError(std::string message);

  /**
   * To get key and pem outof onboard message.
      */
  void parseCertificates(std::string message, void *member);

 private:
  ConnectionProvider *m_connectionProvider;
  Settings *m_settings;

  std::string m_registrationCode;
};

#endif  //  LIB_AGRIROUTERCLIENT_INC_REGISTRATION_H_
