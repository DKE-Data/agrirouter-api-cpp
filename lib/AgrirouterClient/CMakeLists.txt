
# Create protobuf pb.h and pb.cc
add_subdirectory(src/Protobuf)

################################################################################
# AgrirouterClient files
################################################################################

file(GLOB SRCS
    "src/*.cpp"
    "src/ConnectionProvider/*.cpp"
    "src/*.h"
    "inc/Protobuf/cloud-provider-integration/cloud-virtualized-app-registration.pb.cc"
    "inc/Protobuf/commons/chunk.pb.cc"
    "inc/Protobuf/commons/message.pb.cc"
    "inc/Protobuf/messaging/request/payload/account/endpoints.pb.cc"
    "inc/Protobuf/messaging/request/payload/efdi/grpc-efdi.pb.cc"
    "inc/Protobuf/messaging/request/payload/endpoint/capabilities.pb.cc"
    "inc/Protobuf/messaging/request/payload/endpoint/subscription.pb.cc"
    "inc/Protobuf/messaging/request/payload/feed/feed-requests.pb.cc"
    "inc/Protobuf/messaging/request/request.pb.cc"
    "inc/Protobuf/messaging/response/payload/account/endpoints.pb.cc"
    "inc/Protobuf/messaging/response/payload/endpoint/capability.pb.cc"
    "inc/Protobuf/messaging/response/payload/feed/feed-response.pb.cc"
    "inc/Protobuf/messaging/response/payload/feed/push-notification.pb.cc"
    "inc/Protobuf/messaging/response/response.pb.cc"
    "inc/Protobuf/settings/dh-settings.pb.cc"
    "third_party/cJSON/*.c"
    "third_party/mosquitto/*.h"
)

################################################################################
# Build static lib and install it
################################################################################
add_library(AgrirouterClient STATIC ${SRCS})

target_include_directories(AgrirouterClient PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/inc>)
target_include_directories(AgrirouterClient PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src>)
target_include_directories(AgrirouterClient PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src/ConnectionProvider>)
target_include_directories(AgrirouterClient PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/inc/Protobuf>)
target_include_directories(AgrirouterClient PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>)

install(TARGETS AgrirouterClient DESTINATION lib/AgrirouterClient)
install(FILES ${INC_H} DESTINATION include/AgrirouterClient)

################################################################################
# Link protobuf lib
################################################################################
target_link_libraries(AgrirouterClient protobuf curl uuid mosquitto)
