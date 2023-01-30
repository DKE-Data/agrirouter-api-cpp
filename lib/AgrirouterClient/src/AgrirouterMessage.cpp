#include "AgrirouterMessage.h"

#include "AgrirouterMessageUtils.h"

#include <string>

AgrirouterMessage::AgrirouterMessage() {}

AgrirouterMessage::AgrirouterMessage(Request request) { m_request = request; }

AgrirouterMessage::AgrirouterMessage(const RequestEnvelope envelope, const RequestPayloadWrapper payloadWrapper)
{
    m_request.envelope = envelope;
    m_request.payloadWrapper = payloadWrapper;
}

AgrirouterMessage::~AgrirouterMessage() {}

Request AgrirouterMessage::request() { return m_request; }

Response AgrirouterMessage::response() { return m_response; }

void AgrirouterMessage::setResponse(Response response)
{
    m_response = response;
}

std::string AgrirouterMessage::encodedRequest()
{
    std::string enc = encodeRequest(m_request);

    return enc;
}

std::string AgrirouterMessage::encodedResponse()
{
    return encodeResponse(m_response);
}

std::string AgrirouterMessage::messageId()
{
    return m_request.envelope.application_message_id();
}

std::string AgrirouterMessage::messageType()
{
    return m_request.envelope.technical_message_type();
}
