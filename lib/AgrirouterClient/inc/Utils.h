#ifndef LIB_AGRIROUTERCLIENT_INC_UTILS_H_
#define LIB_AGRIROUTERCLIENT_INC_UTILS_H_

#include "Definitions.h"
#include "third_party/cJSON/cJSON.h"

#include <stdio.h>
#include <sys/time.h>
#include <uuid/uuid.h>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <iterator>



inline std::string createUuid()
{
    uuid_t id;
    uuid_generate(id);
    char *str = new char[100];
    uuid_unparse(id, str);
    std::string uuidAsString = std::string(str);

    delete [] str;
    return uuidAsString;
}

inline int32_t getId()
{
    static int32_t s_id;
    return ++s_id;
}

inline std::string uint32ToString(uint32_t number)
{
    char num[32];
    snprintf(num, sizeof(num), "%i", number);
    std::string uint32AsString = std::string(num);

    return uint32AsString;
}

inline std::string int64ToString(int64_t number)
{
    char num[64];
    snprintf(num, sizeof(num), "%lli", number);
    std::string int64AsString = std::string(num);

    return std::string(int64AsString);
}

inline int stringToInt(std::string s)
{
    return static_cast<int>(strtol(s.c_str(), NULL, 10));
}

inline int getNumberOfChunks(int sizeOfFile, int chunkSize)
{
    int chunks = sizeOfFile / chunkSize;
    if ((sizeOfFile % chunkSize) != 0)
    {
        chunks++;
    }
    return chunks;
}

inline std::string getModifiedUuid(std::string& uuid, int number)
{
    int numberOfDigits = 1;
    if (number >= 10)
    {
        numberOfDigits = 2;
    }
    if (number >= 100)
    {
        numberOfDigits = 3;
    }
    if (number >= 1000)
    {
        numberOfDigits = 4;
    }

    std::string tmpUuid = uuid.substr(0, uuid.size() - numberOfDigits);
    std::string modifiedUuid = tmpUuid + uint32ToString(number);
    return modifiedUuid;
}

// Automatically correct the type_url to match SAPs syntax
inline void fillAnyMessage(Any *any, Message *message)
{
    size_t size = message->ByteSizeLong();
    char msg[size];

    if (message->SerializeToArray(msg, size))
    {
        any->set_value(msg, size);
    }
    else
    {
        // printf("Serialization failed");
    }
}

inline std::vector<std::string> split(const std::string &s, char seperator)
{
    std::vector<std::string> output;

    std::string::size_type prev_pos = 0, pos = 0;

    while ((pos = s.find(seperator, pos)) != std::string::npos)
    {
        std::string substring(s.substr(prev_pos, pos - prev_pos));

        output.push_back(substring);

        prev_pos = ++pos;
    }

    output.push_back(s.substr(prev_pos, pos - prev_pos)); // Last word

    return output;
}

/*
 * Base64 operations, copied from
 * https://stackoverflow.com/questions/180947/base64-decode-snippet-in-c
 */

typedef unsigned char BYTE;

static const std::string base64_chars = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

inline bool isBase64(BYTE c)
{
    return (isalnum(c) || (c == '+') || (c == '/'));
}

inline std::string encodeBase64(BYTE const *buf, unsigned int bufLen)
{
    std::string ret;
    int i = 0;
    int j = 0;
    BYTE char_array_3[3];
    BYTE char_array_4[4];

    while (bufLen--)
    {
        char_array_3[i++] = *(buf++);
        if (i == 3)
        {
            char_array_4[0] = (char_array_3[0] & 0xfc) >> 2;
            char_array_4[1] = ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4);
            char_array_4[2] = ((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6);
            char_array_4[3] = char_array_3[2] & 0x3f;

            for (i = 0; (i < 4); i++)
            {
                ret += base64_chars[char_array_4[i]];
            }
            i = 0;
        }
    }

    if (i)
    {
        for (j = i; j < 3; j++)
        {
            char_array_3[j] = '\0';
        }

        char_array_4[0] = (char_array_3[0] & 0xfc) >> 2;
        char_array_4[1] = ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4);
        char_array_4[2] = ((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6);
        char_array_4[3] = char_array_3[2] & 0x3f;

        for (j = 0; (j < i + 1); j++)
        {
            ret += base64_chars[char_array_4[j]];
        }

        while ((i++ < 3))
        {
            ret += '=';
        }
    }

    return ret;
}

inline std::vector<BYTE> decodeBase64(std::string const &encoded_string)
{
    int in_len = encoded_string.size();
    int i = 0;
    int j = 0;
    int in_ = 0;
    BYTE char_array_4[4], char_array_3[3];
    std::vector<BYTE> ret;

    while (in_len-- && (encoded_string[in_] != '=') && isBase64(encoded_string[in_]))
    {
        char_array_4[i++] = encoded_string[in_];
        in_++;
        if (i == 4)
        {
            for (i = 0; i < 4; i++)
            {
                char_array_4[i] = base64_chars.find(char_array_4[i]);
            }

            char_array_3[0] = (char_array_4[0] << 2) + ((char_array_4[1] & 0x30) >> 4);
            char_array_3[1] = ((char_array_4[1] & 0xf) << 4) + ((char_array_4[2] & 0x3c) >> 2);
            char_array_3[2] = ((char_array_4[2] & 0x3) << 6) + char_array_4[3];

            for (i = 0; (i < 3); i++)
            {
                ret.push_back(char_array_3[i]);
            }
            i = 0;
        }
    }

    if (i)
    {
        for (j = i; j < 4; j++)
        {
            char_array_4[j] = 0;
        }

        for (j = 0; j < 4; j++)
        {
            char_array_4[j] = base64_chars.find(char_array_4[j]);
        }

        char_array_3[0] = (char_array_4[0] << 2) + ((char_array_4[1] & 0x30) >> 4);
        char_array_3[1] = ((char_array_4[1] & 0xf) << 4) + ((char_array_4[2] & 0x3c) >> 2);
        char_array_3[2] = ((char_array_4[2] & 0x3) << 6) + char_array_4[3];

        for (j = 0; (j < i - 1); j++)
        {
            ret.push_back(char_array_3[j]);
        }
    }

    return ret;
}

/*
 * File operations
 */

inline bool fileExists(std::string& name)
{
    if (FILE *file = fopen(name.c_str(), "r"))
    {
        fclose(file);
        return true;
    }
    else
    {
        return false;
    }
}

inline std::string readFile(std::string& absolutePath)
{
    std::string line = "";
    std::string data = "";

    if (fileExists(absolutePath))
    {
        // Read file at given path
        FILE *file;
        file = fopen(absolutePath.c_str(), "r");
        if (file != NULL)
        {
            while (1)
            {
                char *line;
                char buf[4096];
                line = fgets(buf, sizeof buf, file);
                if (line == NULL)
                break;
                data += std::string(line);
            }
        }
        fclose(file);

        // Successful
        return data;
    }

    // Not Successful
    return "";
}

inline void writeFile(const char *data, std::string& absolutePath)
{
    std::ofstream file;

    try
    {
        file.open(absolutePath.c_str());
        file << data;
        file.close();

        FILE *file = NULL;
        file = fopen(absolutePath.c_str(), "rb");
        fseek(file, 0, SEEK_END);
        fclose(file);
    }
    catch (std::ios_base::failure &e)
    {
        // printf("File \"%s\" could not be written", absolutePath.c_str());
    }
}

inline void writeFile(const std::string &data, std::string& absolutePath)
{
    std::ofstream file;

    try
    {
        file.open(absolutePath.c_str());
        file << data.c_str();
        file.close();
    }
    catch (std::ios_base::failure &e)
    {
        // printf("File \"%s\" could not be written", absolutePath.c_str());
    }
}

inline std::vector<unsigned char> readBinaryFile(std::string& absolutePath)
{
    std::ifstream input(absolutePath.c_str(), std::ios::binary);

    // copies all data into buffer
    std::vector<unsigned char> buffer((std::istreambuf_iterator<char>(input)), (std::istreambuf_iterator<char>()));

    return buffer;
}

inline void writeBinaryFile(std::vector<unsigned char> buffer, std::string& absolutePath)
{
    std::ofstream output(absolutePath.c_str(), std::ios::binary);
    output.write((const char *)&buffer[0], buffer.size());
}

inline std::string readBinaryFileAndBase64(std::string& absolutePath)
{
    std::vector<unsigned char> binaryFile = readBinaryFile(absolutePath);
    unsigned char *file = new unsigned char[binaryFile.size()];
    std::copy(binaryFile.begin(), binaryFile.end(), file);
    binaryFile.clear();
    std::string base64 = encodeBase64(file, binaryFile.size());

    delete [] file;
    return base64;
}

inline void writeBase64EncodedBinaryFile(std::string& file, std::string& absolutePath)
{
    std::vector<unsigned char> decoded = decodeBase64(file);
    writeBinaryFile(decoded, absolutePath);
}

inline ConnectionParameters getConnectionParametersFromJSON(std::string params)
{
    ConnectionParameters parameters;

    cJSON *root = cJSON_Parse(params.c_str());
    parameters.deviceAlternateId = cJSON_GetObjectItem(root, "deviceAlternateId")->valuestring;
    parameters.capabilityAlternateId = cJSON_GetObjectItem(root, "capabilityAlternateId")->valuestring;
    parameters.sensorAlternateId = cJSON_GetObjectItem(root, "sensorAlternateId")->valuestring;
    parameters.certificateType = cJSON_GetObjectItem(root, "certificateType")->valuestring;
    parameters.secret = cJSON_GetObjectItem(root, "secret")->valuestring;
    parameters.measuresUrl = cJSON_GetObjectItem(root, "measures")->valuestring;
    parameters.commandsUrl = cJSON_GetObjectItem(root, "commands")->valuestring;
    parameters.gatewayId = cJSON_GetObjectItem(root, "gatewayId")->valuestring;

    // Check for MQTT (gatewayId "2")
    if (parameters.gatewayId == "2")
    {
        parameters.host = cJSON_GetObjectItem(root, "host")->valuestring;
        parameters.port = cJSON_GetObjectItem(root, "port")->valueint;
        parameters.clientId = cJSON_GetObjectItem(root, "clientId")->valuestring;
    }

    cJSON_Delete(root);

    return parameters;
}

inline ConnectionParameters getSavedConnectionParameters(std::string& absolutePath)
{
    if (fileExists(absolutePath))
    {
        std::string params = readFile(absolutePath);
        return getConnectionParametersFromJSON(params);
    }
    else
    {
        ConnectionParameters parameters;
        return parameters;
    }
}

inline std::string getConnectionParametersAsJSON(ConnectionParameters *parameters) 
{
    std::string credentials = "";
    cJSON *root;

    root = cJSON_CreateObject();
    cJSON_AddStringToObject(root, "deviceAlternateId", parameters->deviceAlternateId.c_str());
    cJSON_AddStringToObject(root, "capabilityAlternateId", parameters->capabilityAlternateId.c_str());
    cJSON_AddStringToObject(root, "sensorAlternateId", parameters->sensorAlternateId.c_str());
    cJSON_AddStringToObject(root, "certificateType", parameters->certificateType.c_str());
    cJSON_AddStringToObject(root, "secret", parameters->secret.c_str());
    cJSON_AddStringToObject(root, "measures", parameters->measuresUrl.c_str());
    cJSON_AddStringToObject(root, "commands", parameters->commandsUrl.c_str());
    cJSON_AddStringToObject(root, "gatewayId", parameters->gatewayId.c_str());

    // Check for MQTT
    if (parameters->gatewayId == "2")
    {
        cJSON_AddStringToObject(root, "host", parameters->host.c_str());
        cJSON_AddNumberToObject(root, "port", parameters->port);
        cJSON_AddStringToObject(root, "clientId", parameters->clientId.c_str());
    }

    credentials = cJSON_PrintUnformatted(root);

    cJSON_Delete(root);

    return credentials;
}

inline void saveConnectionParameters(ConnectionParameters *parameters, std::string& absolutePath)
{
    std::string credentials = getConnectionParametersAsJSON(parameters);
    writeFile(credentials, absolutePath);
}

inline ApplicationSettings getApplicationSettings(std::string& absolutePath)
{
    ApplicationSettings settings;

    if (fileExists(absolutePath))
    {
        std::string allSettings = readFile(absolutePath);

        cJSON *root = cJSON_Parse(allSettings.c_str());
        cJSON *item = cJSON_GetObjectItem(root, "applicationSettings");

        settings.applicationId = cJSON_GetObjectItem(item, "applicationId")->valuestring;
        settings.certificationVersionId = cJSON_GetObjectItem(item, "certificationVersionId")->valuestring;
        settings.externalId = cJSON_GetObjectItem(item, "externalId")->valuestring;
        settings.locationCertsAndIds = cJSON_GetObjectItem(item, "locationCertsAndIds")->valuestring;
        settings.teamsetContextId = cJSON_GetObjectItem(item, "teamsetContextId")->valuestring;
        settings.connectionType = cJSON_GetObjectItem(item, "connectionType")->valuestring;

        const cJSON* p_acceptSelfSignedCertificate = cJSON_GetObjectItem(item, "acceptSelfSignedCertificate");
        if((NULL != p_acceptSelfSignedCertificate) && (std::string(p_acceptSelfSignedCertificate->valuestring) == "true"))
        {
            settings.acceptSelfSignedCertificate = true;
        }
        else
        {
            settings.acceptSelfSignedCertificate = false;
        }

        cJSON_Delete(root);
    }

    return settings;
}

inline AgrirouterSettings getAgrirouterSettings(std::string& absolutePath)
{
    std::string allSettings = readFile(absolutePath);

    cJSON *root = cJSON_Parse(allSettings.c_str());
    cJSON *item = cJSON_GetObjectItem(root, "agrirouterSettings");

    AgrirouterSettings settings;
    settings.registrationServicePath = cJSON_GetObjectItem(item, "registrationServicePath")->valuestring;
    settings.landscape = cJSON_GetObjectItem(item, "landscape")->valuestring;
    settings.cfHost = cJSON_GetObjectItem(item, "cfHost")->valuestring;
    settings.apiPrefix = cJSON_GetObjectItem(item, "apiPrefix")->valuestring;
    settings.registrationUrl = "https://" + settings.registrationServicePath + "-" + settings.landscape + "." + settings.cfHost + "/" + settings.apiPrefix + "/registration/onboard";

    cJSON_Delete(root);

    return settings;
}

/*
 * Timestamp operations
 */

inline timeval getTimestamp()
{
    timeval tp;
    gettimeofday(&tp, NULL);

    return tp;
}

inline google::protobuf::Timestamp getTimestampFromTimeval(timeval tv)
{
    google::protobuf::Timestamp timestamp = google::protobuf::Timestamp();
    timestamp.set_seconds(tv.tv_sec);
    timestamp.set_nanos(tv.tv_usec / 1000);

    return timestamp;
}

inline google::protobuf::Timestamp getCurrentTimestamp()
{
    timeval tv = getTimestamp();

    return getTimestampFromTimeval(tv);
}

inline ValidityPeriod getValidityPeriodForLastMinutes(int minutes)
{
    ValidityPeriod validityPeriod = ValidityPeriod();

    google::protobuf::Timestamp to = getCurrentTimestamp();
    google::protobuf::Timestamp *t = validityPeriod.mutable_sentto();
    *t = to;

    timeval tv = getTimestamp();
    tv.tv_sec -= minutes * 60;

    google::protobuf::Timestamp from = getTimestampFromTimeval(tv);
    google::protobuf::Timestamp *f = validityPeriod.mutable_sentfrom();
    *f = from;

    return validityPeriod;
}

#endif // LIB_AGRIROUTERCLIENT_INC_UTILS_H_
