#ifndef LIB_AGRIROUTERCLIENT_INC_DEFINITIONS_H_
#define LIB_AGRIROUTERCLIENT_INC_DEFINITIONS_H_

#include "commons/chunk.pb.h"
#include "commons/message.pb.h"
#include "messaging/request/request.pb.h"
#include "messaging/response/response.pb.h"
#include "messaging/request/payload/account/endpoints.pb.h"
#include "messaging/request/payload/efdi/grpc-efdi.pb.h"
#include "messaging/request/payload/endpoint/capabilities.pb.h"
#include "messaging/request/payload/endpoint/subscription.pb.h"
#include "messaging/request/payload/feed/feed-requests.pb.h"
#include "messaging/response/payload/account/endpoints.pb.h"
#include "messaging/response/payload/feed/feed-response.pb.h"
#include "messaging/response/payload/feed/push-notification.pb.h"

#include <google/protobuf/io/coded_stream.h>
#include <google/protobuf/io/zero_copy_stream.h>
#include <google/protobuf/io/zero_copy_stream_impl.h>
#include <google/protobuf/util/json_util.h>

#include <list>
#include <string>

#define CAPABILITIES 1
#define SUBSCRIPTIONS 2
#define DEVICE_DESCRIPTION 3
#define LIST_ENDPOINTS_FILTERED 4
#define LIST_ENDPOINTS_UNFILTERED 5
#define QUERY_HEADERS 6
#define QUERY_MESSAGES 8
#define NON_TELEMETRY 9
#define MESSAGE_CONFIRM 10
#define MESSAGE_DELETE 11
#define GET_MESSAGES 12
#define TIMELOG 13

#define MG_PARAMETER_BASE 100
#define MG_PARAMETER_AUTH_USERNAME (MG_PARAMETER_BASE + 1)
#define MG_PARAMETER_AUTH_PASSWORD (MG_PARAMETER_BASE + 2)
#define MG_PARAMETER_ACCOUNT_ID (MG_PARAMETER_BASE + 3)
#define MG_PARAMETER_APPLICATION_ID (MG_PARAMETER_BASE + 4)
#define MG_PARAMETER_CERTIFICATION_VERSION_ID (MG_PARAMETER_BASE + 5)
#define MG_PARAMETER_ONBOARD_ID (MG_PARAMETER_BASE + 6)
#define MG_PARAMETER_GATEWAY_ID (MG_PARAMETER_BASE + 7)
#define MG_PARAMETER_CERTIFICATE_TYPE (MG_PARAMETER_BASE + 8)
#define MG_PARAMETER_CERTIFICATE (MG_PARAMETER_BASE + 9)
#define MG_PARAMETER_CERTIFICATE_PATH (MG_PARAMETER_BASE + 10)
#define MG_PARAMETER_PRIVATE_KEY (MG_PARAMETER_BASE + 11)
#define MG_PARAMETER_PRIVATE_KEY_PATH (MG_PARAMETER_BASE + 12)
#define MG_PARAMETER_PRIVATE_KEY_PASSPHRASE (MG_PARAMETER_BASE + 13)
#define MG_PARAMETER_ENCODING_TYPE (MG_PARAMETER_BASE + 14)
#define MG_PARAMETER_CONNECTION_PARAMETERS (MG_PARAMETER_BASE + 15)
#define MG_PARAMETER_CONNECTION_PARAMETERS_PATH (MG_PARAMETER_BASE + 16)
#define MG_PARAMETER_CONNECTION_TYPE (MG_PARAMETER_BASE + 17)
#define MG_PARAMETER_POLLING_INTERVAL (MG_PARAMETER_BASE + 18)
#define MG_PARAMETER_POLLING_MAX_TIME (MG_PARAMETER_BASE + 19)
#define MG_PARAMETER_MQTT_KEEP_ALIVE_TIME (MG_PARAMETER_BASE + 20)

#define MG_EV_BASE 200
#define MG_EV_CAPABILITIES (MG_EV_BASE + CAPABILITIES)
#define MG_EV_SUBSCRIPTIONS (MG_EV_BASE + SUBSCRIPTIONS)
#define MG_EV_DEVICE_DESCRIPTION (MG_EV_BASE + DEVICE_DESCRIPTION)
#define MG_EV_LIST_ENDPOINTS_FILTERED (MG_EV_BASE + LIST_ENDPOINTS_FILTERED)
#define MG_EV_LIST_ENDPOINTS_UNFILTERED (MG_EV_BASE + LIST_ENDPOINTS_UNFILTERED)
#define MG_EV_QUERY_HEADERS (MG_EV_BASE + QUERY_HEADERS)
#define MG_EV_QUERY_MESSAGES (MG_EV_BASE + QUERY_MESSAGES)
#define MG_EV_NON_TELEMETRY (MG_EV_BASE + NON_TELEMETRY)
#define MG_EV_MESSAGE_CONFIRM (MG_EV_BASE + MESSAGE_CONFIRM)
#define MG_EV_MESSAGE_DELETE (MG_EV_BASE + MESSAGE_DELETE)
#define MG_EV_GET_MESSAGES (MG_EV_BASE + GET_MESSAGES)
#define MG_EV_TIMELOG (MG_EV_BASE + TIMELOG)

// error messages
#define MG_ERROR_BASE 300
#define MG_ERROR_MISSING_ENDPOINT (MG_ERROR_BASE + 1)
#define MG_ERROR_MISSING_OR_EXPIRED_CERTIFICATE (MG_ERROR_BASE + 2)
#define MG_ERROR_MISSING_OR_EXPIRED_CA_CERTIFICATE (MG_ERROR_BASE + 3)
#define MG_ERROR_NOT_VALID_TOPIC (MG_ERROR_BASE + 4)

// message types
#define MESSAGE_TYPE_TASK_DATA "iso:11783:-10:taskdata:zip"
#define MESSAGE_TYPE_TIME_LOG "iso:11783:-10:time_log:protobuf"
#define MESSAGE_TYPE_DEVICE_DESCRIPTION "iso:11783:-10:device_description:protobuf"

// loglevel
#define MG_LFL_CRI 1  // critical  
#define MG_LFL_ERR 2  // error  
#define MG_LFL_WRN 3  // warning  
#define MG_LFL_MSG 4  // message  
#define MG_LFL_NTC 5  // notice  
#define MG_LFL_TRC 6  // trace  
#define MG_LFL_DBG 7  // debug  

// Other definitions
#define DEFAULT_CHUNK_SIZE 300000 // 0,3 MB
#define DEFAULT_KEEP_ALIVE_TIME 240 // 240 s

// Protobuf typedefs
typedef agrirouter::request::RequestEnvelope RequestEnvelope;
typedef agrirouter::request::RequestPayloadWrapper RequestPayloadWrapper;

typedef agrirouter::response::ResponseEnvelope ResponseEnvelope;
typedef agrirouter::response::ResponsePayloadWrapper ResponsePayloadWrapper;

typedef agrirouter::request::payload::endpoint::CapabilitySpecification CapabilitySpecification;
typedef agrirouter::request::payload::endpoint::CapabilitySpecification_Capability Capability;
typedef agrirouter::request::payload::endpoint::Subscription Subscription;
typedef agrirouter::request::payload::endpoint::CapabilitySpecification_Direction CapabilitySpecification_Direction;
typedef agrirouter::request::payload::endpoint::Subscription_MessageTypeSubscriptionItem MessageTypeSubscriptionItem;
typedef agrirouter::request::payload::account::ListEndpointsQuery ListEndpointsQuery;
typedef agrirouter::request::payload::account::ListEndpointsQuery_Direction ListEndpointsQuery_Direction;

typedef agrirouter::response::payload::account::ListEndpointsResponse ListEndpointsResponse;
typedef agrirouter::response::payload::account::ListEndpointsResponse_MessageType MessageType;
typedef agrirouter::response::payload::account::ListEndpointsResponse_Endpoint Endpoint;
typedef agrirouter::response::payload::account::ListEndpointsResponse_Direction Direction;

typedef agrirouter::feed::request::MessageConfirm MessageConfirm;
typedef agrirouter::feed::request::MessageDelete MessageDelete;
typedef agrirouter::feed::request::MessageQuery MessageQuery;
typedef agrirouter::feed::request::ValidityPeriod ValidityPeriod;

typedef agrirouter::feed::response::QueryMetrics QueryMetrics;
typedef agrirouter::feed::response::HeaderQueryResponse HeaderQueryResponse;
typedef agrirouter::feed::response::MessageQueryResponse MessageQueryResponse;
typedef agrirouter::feed::response::FailedMessageQueryResponse FailedMessageQueryResponse;

typedef agrirouter::feed::push::notification::PushNotification PushNotification;

typedef agrirouter::commons::Message CommonsMessage;
typedef agrirouter::commons::Messages CommonsMessages;
typedef agrirouter::commons::ChunkComponent ChunkComponent;

typedef efdi::ISO11783_TaskData ISO11783_TaskData;
typedef efdi::TimeLog TimeLog;
typedef efdi::Time Time;
typedef efdi::Task Task;
typedef efdi::DataLogValue DataLogValue;
typedef efdi::Position Position;

typedef google::protobuf::Any Any;
typedef google::protobuf::Timestamp Timestamp;
typedef google::protobuf::Message Message;
typedef google::protobuf::MessageLite MessageLite;

typedef google::protobuf::io::ZeroCopyInputStream ZeroCopyInputStream;
typedef google::protobuf::io::ZeroCopyOutputStream ZeroCopyOutputStream;
typedef google::protobuf::io::CodedInputStream CodedInputStream;
typedef google::protobuf::io::CodedOutputStream CodedOutputStream;
typedef google::protobuf::io::StringOutputStream StringOutputStream;

// Error struct for callback functions
typedef struct Error
{
    std::string code = "";
    std::string message = "";
    std::string applicationMessageId = "";
} Error;

// Struct for connection parameters received in onboard reponse
typedef struct ConnectionParameters
{
    std::string deviceAlternateId = "";
    std::string capabilityAlternateId = "";
    std::string sensorAlternateId = "";
    std::string certificateType = "";
    std::string secret = "";
    std::string measuresUrl = "";
    std::string commandsUrl = "";
    std::string gatewayId = "";
    std::string host = "";
    uint16_t port = 0;
    std::string clientId = "";
} ConnectionParameters;

// Struct for message parameters used for communication
typedef struct MessageParameters
{
    int event = 0;
    std::string applicationMessageId = "";
    void *member = nullptr;
} MessageParameters;

// Struct for evaluating the mode. DIRECT: with recipients, PUBLISH: without
// recipients, PUBLISH_WITH_DIRECT: with recipients
typedef struct Addressing
{
    RequestEnvelope::Mode mode = RequestEnvelope::Mode::RequestEnvelope_Mode_DIRECT;
    std::list<std::string> recipients = std::list<std::string>();
} Addressing;

// Struct for application settings
typedef struct ApplicationSettings
{
    std::string applicationId = "";
    std::string certificationVersionId = "";
    std::string externalId = "";
    std::string locationCertsAndIds = "";
    std::string teamsetContextId = "";
    std::string connectionType = "";
    bool acceptSelfSignedCertificate = false;
} ApplicationSettings;

// Struct for agrirouter settings
typedef struct AgrirouterSettings
{
    std::string registrationServicePath = "";
    std::string landscape = "";
    std::string cfHost = "";
    std::string apiPrefix = "";
    std::string registrationUrl = "";
} AgrirouterSettings;

// Struct for handling binary data
typedef struct BinaryData
{
    unsigned char* data = nullptr;
    int size = 0;
} BinaryData;

// Function pointer for callback functions
typedef size_t (*TokenCallback) (std::string token, Error error);
typedef size_t (*GetMessagesCallback) ( google::protobuf::RepeatedPtrField<CommonsMessage>, Error error);

const static std::string logLevelTexts[8] = {"", "CRITICAL", "ERROR   ", "WARNING ", "MESSAGE ", "NOTICE  ", "TRACE   ", "DEBUG   "};
static inline std::string getLogLevelText(int logLevel)
{
    return logLevelTexts[logLevel];
}

#endif  // LIB_AGRIROUTERCLIENT_INC_DEFINITIONS_H_
