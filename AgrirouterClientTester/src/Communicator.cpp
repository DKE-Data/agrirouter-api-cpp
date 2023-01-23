#include <Communicator.h>

#include <AgrirouterClient.h>
#include <AgrirouterMessageUtils.h>
#include <Registration.h>
#include <Utils.h>

#include <algorithm>
#include <iostream>
#include <list>
#include <map>
#include <string>
#include <vector>

Communicator::Communicator() { }

Communicator::Communicator(Settings *settings, AgrirouterClient *agrirouterClient, AgrirouterSettings agrirouterSettings) {
  this->m_settings = settings;
  this->m_agrirouterClient = agrirouterClient;
  this->m_agrirouterSettings = agrirouterSettings;
}

Communicator::~Communicator() { }

void Communicator::onboard(std::string registrationCode, std::string externalId) {
  m_settings->setExternalId(externalId);
  if (m_settings->getConnectionType() == Settings::HTTP) {
    m_settings->setGatewayId("3");
  } else if (m_settings->getConnectionType() == Settings::MQTT) {
    // ToDo: MQTT needs to be implemented
    m_settings->setGatewayId("2");
    m_settings->setConnectionType(Settings::HTTP);
  }
  m_settings->setCertificateType("PEM");

  m_agrirouterClient->registerDeviceWithRegCode(registrationCode, this->m_agrirouterSettings);
}

void Communicator::sendCapabilities() {
  CapabilitySpecification capabilitySpecification = CapabilitySpecification();
  capabilitySpecification.set_app_certification_id(m_settings->getApplicationId());
  capabilitySpecification.set_app_certification_version_id(m_settings->getCertificationVersionId());

  Capability *taskdata = capabilitySpecification.add_capabilities();
  taskdata->set_technical_message_type("iso:11783:-10:taskdata:zip");
  taskdata->set_direction(CapabilitySpecification::SEND_RECEIVE);

  std::string cap = messageToJson(&capabilitySpecification);
  printf("Capabilities: %s\n", cap.c_str());

  std::string messageId;
  m_agrirouterClient->sendCapabilities(&messageId, &capabilitySpecification);
}

void Communicator::sendSubscription() {
  Subscription subscription = Subscription();

  MessageTypeSubscriptionItem *taskdata = subscription.add_technical_message_types();
  taskdata->set_technical_message_type("iso:11783:-10:taskdata:zip");

  /*MessageTypeSubscriptionItem *efdiTimeLog = subscription.add_technical_message_types();
  efdiTimeLog->set_technical_message_type("iso:11783:-10:time_log:protobuf");
  efdiTimeLog->set_position(true);
  for (int i = 1; i < 528; i++) {
    efdiTimeLog->add_ddis(i);
  }*/

  std::string subs = messageToJson(&subscription);
  printf("Subscriptions: %s\n", subs.c_str());

  std::string messageId;
  m_agrirouterClient->sendSubscription(&messageId, &subscription);
}

int Communicator::sendMessageQuery() {
  // For the last 29 days
  ValidityPeriod *validityPeriod = getValidityPeriodForLastMinutes(60 * 24 * 29);

  MessageQuery messageQuery = MessageQuery();
  ValidityPeriod *v = messageQuery.mutable_validity_period();
  *v = *validityPeriod;

  std::string val = messageToJson(&messageQuery);
  printf("MessageQuery: %s\n", val.c_str());

  std::string messageId;
  m_agrirouterClient->sendQueryHeaderMessages(&messageId, &messageQuery);

  return EXIT_SUCCESS;
}

void Communicator::sendMessageQueryWithId(std::list<std::string> messageIds) {
  // For the last 29 days
  ValidityPeriod *validityPeriod = getValidityPeriodForLastMinutes(60 * 24 * 29);

  MessageQuery messageQuery = MessageQuery();
  ValidityPeriod *v = messageQuery.mutable_validity_period();
  *v = *validityPeriod;

  for (std::list<std::string>::iterator it = messageIds.begin(); it != messageIds.end(); ++it) {
    std::string id = (std::string)*it;
    messageQuery.add_message_ids(id);
  }

  std::string messageId;
  m_agrirouterClient->sendQueryMessages(&messageId, &messageQuery);
}

void Communicator::sendMessageQueryWithSender(std::list<std::string> senders) {
  // For the last 29 days
  ValidityPeriod *validityPeriod = getValidityPeriodForLastMinutes(60 * 24 * 29);

  MessageQuery messageQuery = MessageQuery();
  ValidityPeriod *v = messageQuery.mutable_validity_period();
  *v = *validityPeriod;

  for (std::list<std::string>::iterator it = senders.begin(); it != senders.end(); ++it) {
    std::string sender = (std::string)*it;
    messageQuery.add_senders(sender);
  }

  std::string messageId;
  m_agrirouterClient->sendQueryMessages(&messageId, &messageQuery);
}

void Communicator::sendMessageQueryWithValidityPeriod(ValidityPeriod *validityPeriod) {
  MessageQuery messageQuery = MessageQuery();
  ValidityPeriod *v = messageQuery.mutable_validity_period();
  *v = *validityPeriod;

  std::string message = messageToJson(&messageQuery);
  printf("MessageQuery: %s\n", message.c_str());

  std::string messageId;
  m_agrirouterClient->sendQueryMessages(&messageId, &messageQuery);
}

void Communicator::getListEndpointsFiltered() {
  ListEndpointsQuery listEndpointsQuery = ListEndpointsQuery();
  listEndpointsQuery.set_technical_message_type("iso:11783:-10:taskdata:zip");
  listEndpointsQuery.set_direction(ListEndpointsQuery::SEND_RECEIVE);

  std::string query = messageToJson(&listEndpointsQuery);
  printf("ListEndpointsQuery: %s\n", query.c_str());

  std::string messageId;
  m_agrirouterClient->sendListEndpointsFiltered(&messageId, &listEndpointsQuery);
}

void Communicator::getListEndpointsUnfiltered() {
  ListEndpointsQuery listEndpointsQuery = ListEndpointsQuery();
  listEndpointsQuery.set_technical_message_type("iso:11783:-10:taskdata:zip");
  listEndpointsQuery.set_direction(ListEndpointsQuery::SEND_RECEIVE);

  std::string query = messageToJson(&listEndpointsQuery);
  printf("ListEndpointsQuery: %s\n", query.c_str());

  std::string messageId;
  m_agrirouterClient->sendListEndpointsUnfiltered(&messageId, &listEndpointsQuery);
}

void Communicator::getMessages() {
  m_agrirouterClient->requestMessages();
}
