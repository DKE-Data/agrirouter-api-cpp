#ifndef LIB_AGRIROUTERCLIENT_INC_SETTINGS_H_
#define LIB_AGRIROUTERCLIENT_INC_SETTINGS_H_

#include <AgrirouterMessage.h>
#include <Definitions.h>
#include <google/protobuf/message.h>
#include <string>

class Settings {
 public:
  enum ConnectionType {
    HTTP = 1,
    MQTT = 2
  };

  typedef void (*onParameterChangeCallback)(int event, void *data, void *callbackCallee);
  typedef void (*onMessageCallback)(int event, Response *response, std::string applicationMessageId, void *callbackCallee);
  typedef void (*onErrorCallback)(int code, std::string message, std::string applicationMessageId, void *callbackCallee);

  Settings();
  ~Settings();

  void callOnParameterChange(int event, void *data);
  void callOnMessage(Response *response, MessageParameters messageParameters);
  void callOnError(int code, std::string message, MessageParameters messageParameters);

  // On parameter change callback
  void setOnParameterChangeCallback(onParameterChangeCallback onParameter, void* callbackCallee);
  onParameterChangeCallback getOnParameterChangeCallback();
  // On message change callback
  void setOnMessageCallback(onMessageCallback onMessage, void* callbackCallee);
  onMessageCallback getOnMessageCallback();
  // On error change callback
  void setOnErrorCallback(onErrorCallback onError, void* callbackCallee);
  onErrorCallback getOnErrorCallback();

  ConnectionParameters getConnectionParameters(std::string absolutePath);

  void setAuthUsername(std::string authUsername);
  std::string getAuthUsername();
  void setAuthPassword(std::string authPassword);
  std::string getAuthPassword();
  void setAccountId(std::string accountId);
  std::string getAccountId();
  void setApplicationId(std::string applicationId);
  std::string getApplicationId();
  void setCertificationVersionId(std::string certificationVersionId);
  std::string getCertificationVersionId();
  void setExternalId(std::string externalId);
  std::string getExternalId();
  void setGatewayId(std::string gatewayId);
  std::string getGatewayId();
  void setCertificateType(std::string certificateType);
  std::string getCertificateType();

  void setCertificate(std::string certificate);
  std::string getCertificate();
  void setCertificatePath(std::string certificatePath);
  std::string getCertificatePath();
  void setCaFilePath(std::string caFilePath);
  std::string getCaFilePath();
  void setPrivateKeyPath(std::string privateKeyPath);
  std::string getPrivateKeyPath();
  void setPrivateKey(std::string privateKey);
  std::string getPrivateKey();
  void setEncodingType(std::string encodingType);
  std::string getEncodingType();
  void setConnectionType(ConnectionType connectionType);
  ConnectionType getConnectionType();
  void setConnectionParameters(ConnectionParameters connectionParameters);
  ConnectionParameters getConnectionParameters();
  void setConnectionParametersPath(std::string connectionParametersPath);
  std::string getConnectionParametersPath();

  void setPollingInterval(int pollingInterval);
  int getPollingInterval();
  void setPollingMaxTime(int pollingMaxTime);
  int getPollingMaxTime();

 private:
  onParameterChangeCallback onParameter;
  onMessageCallback onMessage;
  onErrorCallback onError;

  void *parameterCallbackCallee;
  void *messageCallbackCallee;
  void *errorCallbackCallee;

  bool setOnParameter;
  bool setOnMessage;
  bool setOnError;

  // For onboarding
  std::string authUsername;
  std::string authPassword;
  std::string accountId;
  std::string applicationId;
  std::string certificationVersionId;
  std::string externalId;
  std::string gatewayId;
  std::string certificateType;

  // For sending and receiving messages
  std::string certificate;
  std::string certificatePath;
  std::string caFilePath;
  std::string privateKey;
  std::string privateKeyPath;
  std::string encodingType;
  ConnectionType connectionType;
  ConnectionParameters connectionParameters;
  std::string connectionParametersPath;

  // For general purposes
  int pollingInterval;
  int pollingMaxTime;
};

#endif  // LIB_AGRIROUTERCLIENT_INC_SETTINGS_H_
