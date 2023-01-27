#ifndef LIB_AGRIROUTERCLIENT_INC_AGRIROUTERMESSAGE_H_
#define LIB_AGRIROUTERCLIENT_INC_AGRIROUTERMESSAGE_H_

#include "Definitions.h"
#include "Utils.h"

#include <string>

struct Request 
{
    RequestEnvelope envelope;
    RequestPayloadWrapper payloadWrapper;

    Request()
    {
        envelope = RequestEnvelope::default_instance();
        payloadWrapper = RequestPayloadWrapper::default_instance();
    }

    Request(RequestEnvelope envelope, RequestPayloadWrapper payloadWrapper)
    {
        envelope = envelope;
        payloadWrapper = payloadWrapper;
    }

    void addRecipient(const std::string &recipient)
    {
        envelope.add_recipients(recipient);
    }

    void setPayload(Message *message)
    {
        fillAnyMessage(payloadWrapper.mutable_details(), message);
    }

    void clear()
    {
        envelope.Clear();
        payloadWrapper.Clear();
    }
};

struct Response
{
    ResponseEnvelope envelope;
    ResponsePayloadWrapper payloadWrapper;

    Response()
    {
        envelope = ResponseEnvelope::default_instance();
        payloadWrapper = ResponsePayloadWrapper::default_instance();
    }

    Response(const ResponseEnvelope &env, const ResponsePayloadWrapper &payload)
    {
        envelope = env;
        payloadWrapper = payload;
    }

    void setEnvelope(const std::string &appMesssageId, int32_t responseCode, ResponseEnvelope::ResponseBodyType type)
    {
        envelope.set_application_message_id(appMesssageId);
        envelope.set_response_code(responseCode);
        envelope.set_type(type);
    }

    void setPayload(Message *message)
    {
        fillAnyMessage(payloadWrapper.mutable_details(), message);
    }

    void clear()
    {
        envelope.Clear();
        payloadWrapper.Clear();
    }
};

class AgrirouterMessage {
    public:
        AgrirouterMessage();
        explicit AgrirouterMessage(const Request request);
        AgrirouterMessage(const RequestEnvelope envelope, const RequestPayloadWrapper payloadWrapper);
        ~AgrirouterMessage();

        Request request();
        Response response();

        void setResponse(Response response);

        std::string encodedRequest();
        std::string encodedResponse();

        std::string messageId();
        std::string messageType();

    private:
        Request m_request;
        Response m_response;
};

#endif  // LIB_AGRIROUTERCLIENT_INC_AGRIROUTERMESSAGE_H_
