#ifndef LIB_AGRIROUTERCLIENT_INC_UTILS_H_
#define LIB_AGRIROUTERCLIENT_INC_UTILS_H_

#include <Definitions.h>

#include <stdio.h>
#include <sys/time.h>
#include <uuid/uuid.h>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include <iterator>

#include "../third_party/cJSON/cJSON.h"

inline std::string createUuid() {
  uuid_t id;
  uuid_generate(id);
  char *str = new char[100];
  uuid_unparse(id, str);

  std::string uuid = std::string(str);
  delete [] str;

  return uuid;
}

inline int32_t getId() {
  static int32_t s_id;
  return ++s_id;
}

inline std::string uint32ToString(uint32_t number) {
  char num[32];
  snprintf(num, sizeof(num), "%i", number);

  return std::string(num);
}

inline int stringToInt(std::string s) {
  return static_cast<int>(strtol(s.c_str(), NULL, 10));
}

inline int getNumberOfChunks(int sizeOfFile, int chunkSize) {
  int chunks = sizeOfFile / chunkSize;
  // printf("%i = %i / %i\n", chunks, sizeOfFile, chunkSize);
  if ((sizeOfFile % chunkSize) != 0) {
    // printf("%i \% %i = %i\n", sizeOfFile, chunkSize, sizeOfFile % chunkSize);
    chunks++;
  }
  // printf("return %i\n", chunks);
  return chunks;
}

inline std::string getModifiedUuid(std::string uuid, int number) {
  int numberOfDigits = 1;
  if (number >= 10) {
    numberOfDigits = 2;
  }
  if (number >= 100) {
    numberOfDigits = 3;
  }
  if (number >= 1000) {
    numberOfDigits = 4;
  }

  std::string tmpUuid = uuid.substr(0, uuid.size() - numberOfDigits);
  return tmpUuid + uint32ToString(number);
}

// Automatically correct the type_url to match SAPs syntax
inline void fillAnyMessage(Any *any, Message *message) {
  size_t size = message->ByteSizeLong();
  char msg[size];

  if (message->SerializeToArray(msg, size)) {
    // // printf("Serialization was successful\n");

    any->set_value(msg, size);
  } else {
    // // printf("Serialization failed\n");
  }

}

inline std::vector<std::string> split(const std::string &s, char seperator) {
  std::vector<std::string> output;

  std::string::size_type prev_pos = 0, pos = 0;

  while ((pos = s.find(seperator, pos)) != std::string::npos) {
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

static const std::string base64_chars = "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
  "abcdefghijklmnopqrstuvwxyz"
  "0123456789+/";

inline bool isBase64(BYTE c) {
  return (isalnum(c) || (c == '+') || (c == '/'));
}

inline std::string encodeBase64(BYTE const *buf, unsigned int bufLen) {
  std::string ret;
  int i = 0;
  int j = 0;
  BYTE char_array_3[3];
  BYTE char_array_4[4];

  while (bufLen--) {
    char_array_3[i++] = *(buf++);
    if (i == 3) {
      char_array_4[0] = (char_array_3[0] & 0xfc) >> 2;
      char_array_4[1] = ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4);
      char_array_4[2] = ((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6);
      char_array_4[3] = char_array_3[2] & 0x3f;

      for (i = 0; (i < 4); i++) {
        ret += base64_chars[char_array_4[i]];
      }
      i = 0;
    }
  }

  if (i) {
    for (j = i; j < 3; j++) {
      char_array_3[j] = '\0';
    }

    char_array_4[0] = (char_array_3[0] & 0xfc) >> 2;
    char_array_4[1] = ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4);
    char_array_4[2] =  ((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6);
    char_array_4[3] = char_array_3[2] & 0x3f;

    for (j = 0; (j < i + 1); j++) {
      ret += base64_chars[char_array_4[j]];
    }

    while ((i++ < 3)) {
      ret += '=';
    }
  }

  return ret;
}

inline std::vector<BYTE> decodeBase64(std::string const &encoded_string) {
  int in_len = encoded_string.size();
  int i = 0;
  int j = 0;
  int in_ = 0;
  BYTE char_array_4[4], char_array_3[3];
  std::vector<BYTE> ret;

  while (in_len-- && (encoded_string[in_] != '=') && isBase64(encoded_string[in_])) {
    char_array_4[i++] = encoded_string[in_];
    in_++;
    if (i == 4) {
      for (i = 0; i < 4; i++) {
        char_array_4[i] = base64_chars.find(char_array_4[i]);
      }

      char_array_3[0] = (char_array_4[0] << 2) + ((char_array_4[1] & 0x30) >> 4);
      char_array_3[1] = ((char_array_4[1] & 0xf) << 4) + ((char_array_4[2] & 0x3c) >> 2);
      char_array_3[2] = ((char_array_4[2] & 0x3) << 6) + char_array_4[3];

      for (i = 0; (i < 3); i++) {
        ret.push_back(char_array_3[i]);
      }

      i = 0;
    }
  }

  if (i) {
    for (j = i; j < 4; j++) {
      char_array_4[j] = 0;
    }

    for (j = 0; j < 4; j++) {
      char_array_4[j] = base64_chars.find(char_array_4[j]);
    }

    char_array_3[0] = (char_array_4[0] << 2) + ((char_array_4[1] & 0x30) >> 4);
    char_array_3[1] = ((char_array_4[1] & 0xf) << 4) + ((char_array_4[2] & 0x3c) >> 2);
    char_array_3[2] = ((char_array_4[2] & 0x3) << 6) + char_array_4[3];

    for (j = 0; (j < i - 1); j++) {
      ret.push_back(char_array_3[j]);
    }
  }

  return ret;
}

/*
 * File operations
 */

inline bool fileExists(std::string name) {
  if (FILE *file = fopen(name.c_str(), "r")) {
    fclose(file);
    return true;
  } else {
    return false;
  }
}

inline std::string readFile(std::string absolutePath) {
  std::string line = "";
  std::string data = "";

  if (fileExists(absolutePath)) {
    // Read file at given path
    FILE *file;
    file = fopen(absolutePath.c_str(), "r");
    if (file != NULL) {
      while (1) {
        char *line;
        char buf[4096];
        line = fgets(buf, sizeof buf, file);
        if (line == NULL) {
          break;
        }
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

inline void writeFile(const char *data, int size, std::string absolutePath) {
  std::ofstream file;

  try {
    file.open(absolutePath.c_str());
    file << data;
    file.close();
    // printf("Wrote to file \"%s\"\n", absolutePath.c_str());

    FILE *file = NULL;
    file = fopen(absolutePath.c_str(), "rb");
    fseek(file, 0, SEEK_END);
    fclose(file);
  } catch (std::ios_base::failure &e) {
    // printf("File \"%s\" could not be written", absolutePath.c_str());
  }
}

inline void writeFile(const std::string &data, std::string absolutePath) {
  std::ofstream file;

  try {
    file.open(absolutePath.c_str());
    file << data.c_str();
    file.close();
    // printf("Wrote to file \"%s\"\n", absolutePath.c_str());
  } catch (std::ios_base::failure &e) {
    // printf("File \"%s\" could not be written", absolutePath.c_str());
  }
}

inline std::vector<unsigned char> readBinaryFile(std::string absolutePath) {
  std::ifstream input(absolutePath.c_str(), std::ios::binary);

  // copies all data into buffer
  std::vector<unsigned char> buffer((std::istreambuf_iterator<char>(input)), (std::istreambuf_iterator<char>()));

  return buffer;
}

inline void writeBinaryFile(std::vector<unsigned char> buffer, std::string absolutePath) {
  std::ofstream output(absolutePath.c_str(), std::ios::binary);
  output.write((const char *)&buffer[0], buffer.size());
}

inline std::string readBinaryFileAndBase64(std::string absolutePath) {
  std::vector<unsigned char> binaryFile = readBinaryFile(absolutePath);
  unsigned char *file = new unsigned char[binaryFile.size()];
  std::copy(binaryFile.begin(), binaryFile.end(), file);
  unsigned int fileLength = binaryFile.size();
  binaryFile.clear();
  std::string encodedData = encodeBase64(file, fileLength);
  delete [] file;
  return encodedData;
}

inline void writeBase64EncodedBinaryFile(std::string file, std::string absolutePath) {
  std::vector<unsigned char> decoded = decodeBase64(file);
  writeBinaryFile(decoded, absolutePath);
}

inline ConnectionParameters getSavedConnectionParameters(std::string absolutePath) {
  ConnectionParameters parameters;

  if (fileExists(absolutePath)) {
    std::string params = readFile(absolutePath);
    // printf("parameters: %s\n", params.c_str());

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
    if (parameters.gatewayId == "2") {
      parameters.host = cJSON_GetObjectItem(root, "host")->valuestring,
      parameters.port = cJSON_GetObjectItem(root, "port")->valuestring;
      parameters.clientId = cJSON_GetObjectItem(root, "clientId")->valuestring;
    }
  }

  return parameters;
}

inline void saveConnectionParameters(ConnectionParameters *parameters, std::string absolutePath) {
  std::string ids = "{";

  ids += "\n\t\"deviceAlternateId\": \"";
  ids += parameters->deviceAlternateId;
  ids += "\",\n\t\"capabilityAlternateId\": \"";
  ids += parameters->capabilityAlternateId;
  ids += "\",\n\t\"sensorAlternateId\": \"";
  ids += parameters->sensorAlternateId;
  ids += "\",\n\t\"certificateType\": \"";
  ids += parameters->certificateType;
  ids += "\",\n\t\"secret\": \"";
  ids += parameters->secret;
  ids += "\",\n\t\"measures\": \"";
  ids += parameters->measuresUrl;
  ids += "\",\n\t\"commands\": \"";
  ids += parameters->commandsUrl;
  ids += "\",\n\t\"gatewayId\": \"";
  ids += parameters->gatewayId;

  // Check for MQTT (gatewayId "2")
  if (parameters->gatewayId == "2") {
    ids += "\",\n\t\"host\": \"";
    ids += parameters->host;
    ids += "\",\n\t\"port\": ";
    ids += parameters->port;
    ids += ",\n\t\"clientId\": \"";
    ids += parameters->clientId;
  }

  ids += "\"\n}";

  writeFile(ids, absolutePath);
}

inline ApplicationSettings getApplicationSettings(std::string absolutePath) {
  ApplicationSettings settings;

  if (fileExists(absolutePath)) {
    std::string allSettings = readFile(absolutePath);

    cJSON *root = cJSON_Parse(allSettings.c_str());
    cJSON *item = cJSON_GetObjectItem(root, "applicationSettings");

    settings.applicationId = cJSON_GetObjectItem(item, "applicationId")->valuestring;
    settings.certificationVersionId = cJSON_GetObjectItem(item, "certificationVersionId")->valuestring;
    settings.externalId = cJSON_GetObjectItem(item, "externalId")->valuestring;
    settings.locationCertsAndIds = cJSON_GetObjectItem(item, "locationCertsAndIds")->valuestring;
    settings.teamsetContextId = cJSON_GetObjectItem(item, "teamsetContextId")->valuestring;
    settings.connectionType = cJSON_GetObjectItem(item, "connectionType")->valuestring;
  }

  return settings;
}

inline AgrirouterSettings getAgrirouterSettings(std::string absolutePath) {
  std::string allSettings = readFile(absolutePath);

  cJSON *root = cJSON_Parse(allSettings.c_str());
  cJSON *item = cJSON_GetObjectItem(root, "agrirouterSettings");

  AgrirouterSettings settings;
  settings.registrationUrl = cJSON_GetObjectItem(item, "url")->valuestring;

  printf("registration %s\n", settings.registrationUrl.c_str());

  return settings;
}

/*
 * Timestamp operations
 */

inline struct timeval getTimestamp() {
  struct timeval tp;
  gettimeofday(&tp, NULL);

  return tp;
}

inline google::protobuf::Timestamp getTimestampFromTimeval(struct timeval tv) {
  google::protobuf::Timestamp timestamp = google::protobuf::Timestamp();
  timestamp.set_seconds(tv.tv_sec);
  timestamp.set_nanos(tv.tv_usec / 1000);

  return timestamp;
}

inline google::protobuf::Timestamp getCurrentTimestamp() {
  struct timeval tv = getTimestamp();

  return getTimestampFromTimeval(tv);
}

inline ValidityPeriod *getValidityPeriodForLastMinutes(int minutes) {
  ValidityPeriod *validityPeriod = new ValidityPeriod();

  google::protobuf::Timestamp to = getCurrentTimestamp();
  google::protobuf::Timestamp *t = validityPeriod->mutable_sentto();
  *t = to;

  struct timeval tv = getTimestamp();
  tv.tv_sec -= minutes * 60;

  google::protobuf::Timestamp from = getTimestampFromTimeval(tv);
  google::protobuf::Timestamp *f = validityPeriod->mutable_sentfrom();
  *f = from;

  return validityPeriod;
}

#endif // LIB_AGRIROUTERCLIENT_INC_UTILS_H_
