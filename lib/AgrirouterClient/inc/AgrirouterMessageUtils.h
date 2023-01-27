#ifndef LIB_AGRIROUTERCLIENT_INC_AGRIROUTERMESSAGEUTILS_H_
#define LIB_AGRIROUTERCLIENT_INC_AGRIROUTERMESSAGEUTILS_H_

#include "AgrirouterMessage.h"

#include "Definitions.h"
#include "Utils.h"
#include <string>
#include <vector>
#include <list>

#include "third_party/cJSON/cJSON.h"

// Convert message to JSON
inline std::string messageToJson(const google::protobuf::Message *msg)
{
    std::string jsonString;
    google::protobuf::util::JsonPrintOptions options;
    options.add_whitespace = true;
    google::protobuf::util::MessageToJsonString( *(msg), &jsonString, options);

    return jsonString;
}

// Convert JSON to message
inline void jsonToMessage(const std::string &input, google::protobuf::Message * message)
{
    google::protobuf::util::JsonStringToMessage(input, message);
}

// Copied from https://stackoverflow.com/questions/2340730/are-there-c-equivalents-for-the-protocol-buffers-delimited-i-o-functions-in-ja/
inline bool writeDelimitedTo(const google::protobuf::MessageLite& message, google::protobuf::io::ZeroCopyOutputStream* rawOutput)
{
    // We create a new coded stream for each message.  Don't worry, this is fast.
    google::protobuf::io::CodedOutputStream output(rawOutput);

    // Write the size.
    const uint32_t size = message.ByteSizeLong();
    output.WriteVarint32(size);

    uint8_t* buffer = output.GetDirectBufferForNBytesAndAdvance(size);
    if (buffer != NULL)
    {
        // Optimization:  The message fits in one buffer, so use the faster
        // direct-to-array serialization path.
        message.SerializeWithCachedSizesToArray(buffer);
    }
    else
    {
        // Slightly-slower path when the message is multiple buffers.
        message.SerializeWithCachedSizes(&output);
        if (output.HadError()) return false;
    }

  return true;
}

// Copied from https://stackoverflow.com/questions/2340730/are-there-c-equivalents-for-the-protocol-buffers-delimited-i-o-functions-in-ja/
inline bool readDelimitedFrom(google::protobuf::io::ZeroCopyInputStream* rawInput, google::protobuf::MessageLite* message)
{
    // We create a new coded stream for each message.  Don't worry, this is fast,
    // and it makes sure the 64MB total size limit is imposed per-message rather
    // than on the whole stream.  (See the CodedInputStream interface for more
    // info on this limit.)
    google::protobuf::io::CodedInputStream input(rawInput);

    // Read the size.
    uint32_t size;
    if (!input.ReadVarint32(&size)) return false;

    // Tell the stream not to read beyond that size.
    google::protobuf::io::CodedInputStream::Limit limit =
        input.PushLimit(size);

    // Parse the message.
    if (!message->MergeFromCodedStream(&input)) return false;
    if (!input.ConsumedEntireMessage()) return false;

    // Release the limit.
    input.PopLimit(limit);

    return true;
}

inline RequestEnvelope createRequestHeader(const std::string &messageId, int64_t seqNo, const std::string &messageType,
        Addressing& addressing, const std::string &teamSetContextId)
{

    // Create request header
    RequestEnvelope header;
    header.set_application_message_id(messageId);
    header.set_application_message_seq_no(seqNo);
    header.set_technical_message_type(messageType);
    header.set_mode(addressing.mode);

    if ((addressing.mode == RequestEnvelope::DIRECT) || (addressing.mode == RequestEnvelope::PUBLISH_WITH_DIRECT))
    {
        // Add recipients
        for (std::list<std::string>::iterator it = addressing.recipients.begin(); it != addressing.recipients.end(); ++it)
        {
            std::string recipient = (std::string) *it;
            std::string *r = header.add_recipients();
            *r = recipient;
        }
    }

    // Get current timestamp
    struct timeval timestamp = getTimestamp();
    header.mutable_timestamp()->set_seconds(timestamp.tv_sec);
    header.mutable_timestamp()->set_nanos(timestamp.tv_usec * 1000);

    // Only add context id if it is present
    if (!teamSetContextId.empty())
    {
        header.set_team_set_context_id(teamSetContextId);
    }

    return header;
}

inline RequestPayloadWrapper createRequestBody(Message *message)
{

    // Check for validity
    if (!message)
    {
        return RequestPayloadWrapper::default_instance();
    }

    // Create payload and fill with content
    RequestPayloadWrapper body;
    fillAnyMessage(body.mutable_details(), message);

    return body;
}

inline std::string encodeRequest(Request& request)
{

    // Create string output strem to write request tp
    std::string result;
    StringOutputStream outputStream(&result);

    if (!writeDelimitedTo(request.envelope, &outputStream))
    {
        // Writing failed
        return "";
    }

    if (!writeDelimitedTo(request.payloadWrapper, &outputStream))
    {
        // Writing failed
        return "";
    }

    // Base64 encode the output stream/string
    std::string message = encodeBase64((unsigned char*) result.c_str(), result.size());

    return message;
}

inline std::string encodeResponse(Response& response)
{
    /*
     * ToDo
     */
    return "";
}

inline Request decodeRequest(std::string& encoded)
{
    Request request;

    // Base64 decoding and conversion to char*
    std::vector<unsigned char> decodedRequest = decodeBase64(encoded);
    char* decodedMsg = new char[decodedRequest.size()];
    std::copy(decodedRequest.begin(), decodedRequest.end(), decodedMsg);

    // Conversion to a coded input stream
    google::protobuf::io::ArrayInputStream inputStream((void*) decodedMsg, decodedRequest.size());
    google::protobuf::io::CodedInputStream codedStream(&inputStream);

    // Parse length of envelope
    uint32_t envelopeLength;
    if (!codedStream.ReadVarint32(&envelopeLength))
    {
        // Parsing failed
        delete decodedMsg;
        return Request();
    }

    // Parse envelope
    char *envelopeBuffer = new char[envelopeLength + 1];
    if (codedStream.ReadRaw(envelopeBuffer, envelopeLength))
    {
        request.envelope.ParseFromArray(envelopeBuffer, envelopeLength);
    }
    else
    {
        // Parsing failed
        delete decodedMsg;
        delete [] envelopeBuffer;
        return Request();
    }

    // Parse length of payload
    uint32_t payloadLength;
    if (!codedStream.ReadVarint32(&payloadLength))
    {
        // Parsing failed
        delete decodedMsg;
        delete [] envelopeBuffer;
        return Request();
    }

    // Parse payload
    char *payloadBuffer = new char[payloadLength + 1];
    if (codedStream.ReadRaw(payloadBuffer, payloadLength))
    {
        request.payloadWrapper.ParseFromArray(payloadBuffer, payloadLength);
    }
    else
    {
        // Parsing failed
        delete decodedMsg;
        delete [] envelopeBuffer;
        delete [] payloadBuffer;
        return request;
    }

    delete decodedMsg;
    delete [] envelopeBuffer;
    delete [] payloadBuffer;
    return request;
}

inline Response decodeResponse(std::string& encoded)
{
    Response response;

    // Base64 decoding and conversion to char*
    std::vector<unsigned char> decodedResponse = decodeBase64(encoded);
    char* decodedMsg = new char[decodedResponse.size()];
    std::copy(decodedResponse.begin(), decodedResponse.end(), decodedMsg);

    // Conversion to a coded input stream
    google::protobuf::io::ArrayInputStream inputStream((void*) decodedMsg, decodedResponse.size());
    google::protobuf::io::CodedInputStream codedStream(&inputStream);

    // Parse length of envelope
    uint32_t envelopeLength;
    if (!codedStream.ReadVarint32(&envelopeLength))
    {
        // Parsing failed
        delete decodedMsg;
        return Response();
    }

    // Parse envelope
    char *envelopeBuffer = new char[envelopeLength + 1];
    if (codedStream.ReadRaw(envelopeBuffer, envelopeLength))
    {
        response.envelope.ParseFromArray(envelopeBuffer, envelopeLength);
    }
    else
    {
        // Parsing failed
        delete decodedMsg;
        delete [] envelopeBuffer;
        return Response();
    }

    // Parse length of payload
    uint32_t payloadLength;
    if (!codedStream.ReadVarint32(&payloadLength))
    {
        // Parsing failed
        delete decodedMsg;
        delete [] envelopeBuffer;
        return response;
    }

    // Parse payload
    char *payloadBuffer = new char[payloadLength + 1];
    if (codedStream.ReadRaw(payloadBuffer, payloadLength))
    {
        response.payloadWrapper.ParseFromArray(payloadBuffer, payloadLength);
    }
    else
    {
        // Parsing failed
        delete decodedMsg;
        delete [] envelopeBuffer;
        delete [] payloadBuffer;
        return response;
    }

    delete decodedMsg;
    delete [] envelopeBuffer;
    delete [] payloadBuffer;

    return response;
}


inline int getResponsesFromMessage(std::list<Response> *list, std::string *message)
{
    // Iterate through message to get list of responses
    cJSON *root = cJSON_Parse(message->c_str());
    //cJSON *item = cJSON_GetObjectItem(root, "message");
    for (int i = 0 ; i < cJSON_GetArraySize(root) ; i++)
    {
        cJSON * subitem = cJSON_GetArrayItem(root, i);
        cJSON *command = cJSON_GetObjectItem(subitem, "command");
        cJSON *msg = cJSON_GetObjectItem(command, "message");
        std::string str = msg->valuestring;
        Response response = decodeResponse(str);
        list->push_back(response);
    }

    cJSON_Delete(root);

    return EXIT_SUCCESS;
}

#endif
