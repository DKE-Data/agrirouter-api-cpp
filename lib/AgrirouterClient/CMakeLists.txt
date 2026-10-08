
# Create protobuf pb.h and pb.cc
add_subdirectory(src/Protobuf)

# The EFDI types (package efdi) can be taken from the parent project instead of compiling the own copy of
# grpc-efdi.proto. Two copies of the same protobuf package in one process abort at startup
# ("Symbol name ... conflicts with the existing symbol"), e.g. if the application uses its own EFDI protos.
set(AGRIROUTER_EFDI_PROTO_TARGET "" CACHE STRING "Target providing the efdi protobuf types, empty: compile grpc-efdi.proto")
set(AGRIROUTER_EFDI_PROTO_HEADER "" CACHE STRING "Header of AGRIROUTER_EFDI_PROTO_TARGET with efdi::ISO11783_TaskData and efdi::TimeLog")

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
    "inc/Protobuf/messaging/request/payload/endpoint/capabilities.pb.cc"
    "inc/Protobuf/messaging/request/payload/endpoint/subscription.pb.cc"
    "inc/Protobuf/messaging/request/payload/feed/feed-requests.pb.cc"
    "inc/Protobuf/messaging/request/request.pb.cc"
    "inc/Protobuf/messaging/response/payload/account/endpoints.pb.cc"
    "inc/Protobuf/messaging/response/payload/endpoint/capability.pb.cc"
    "inc/Protobuf/messaging/response/payload/feed/feed-response.pb.cc"
    "inc/Protobuf/messaging/response/payload/feed/push-notification.pb.cc"
    "inc/Protobuf/messaging/response/response.pb.cc"
    "third_party/cJSON/*.c"
    "third_party/mosquitto/*.h"
)

if(AGRIROUTER_EFDI_PROTO_TARGET STREQUAL "")
    list(APPEND SRCS "inc/Protobuf/messaging/request/payload/efdi/grpc-efdi.pb.cc")
endif()

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
# Link libraries
################################################################################
# Prefer targets provided by the parent project (e.g. via find_package, FetchContent or CPM),
# they carry their own usage requirements. Otherwise link the plain library names as before.
if(TARGET protobuf::libprotobuf)
    set(AGRIROUTER_PROTOBUF_LIB protobuf::libprotobuf)
else()
    set(AGRIROUTER_PROTOBUF_LIB protobuf)
endif()

if(TARGET CURL::libcurl)
    set(AGRIROUTER_CURL_LIB CURL::libcurl)
elseif(TARGET libcurl)
    set(AGRIROUTER_CURL_LIB libcurl)
else()
    set(AGRIROUTER_CURL_LIB curl)
endif()

# libmosquitto is the library target of the mosquitto CMake build, mosquitto would be the broker executable
if(TARGET libmosquitto)
    set(AGRIROUTER_MOSQUITTO_LIB libmosquitto)
else()
    set(AGRIROUTER_MOSQUITTO_LIB mosquitto)
endif()

target_link_libraries(AgrirouterClient ${AGRIROUTER_PROTOBUF_LIB} ${AGRIROUTER_CURL_LIB} uuid ${AGRIROUTER_MOSQUITTO_LIB})

if(NOT AGRIROUTER_EFDI_PROTO_TARGET STREQUAL "")
    if(NOT TARGET ${AGRIROUTER_EFDI_PROTO_TARGET})
        message(FATAL_ERROR "AGRIROUTER_EFDI_PROTO_TARGET: target '${AGRIROUTER_EFDI_PROTO_TARGET}' does not exist")
    endif()
    if(AGRIROUTER_EFDI_PROTO_HEADER STREQUAL "")
        message(FATAL_ERROR "AGRIROUTER_EFDI_PROTO_HEADER must be set together with AGRIROUTER_EFDI_PROTO_TARGET")
    endif()
    message(STATUS "AgrirouterClient: using EFDI protobuf types of ${AGRIROUTER_EFDI_PROTO_TARGET}")
    # Public: Definitions.h includes the header, so every user of AgrirouterClient needs the definition
    target_compile_definitions(AgrirouterClient PUBLIC AGRIROUTER_EFDI_PROTO_HEADER="${AGRIROUTER_EFDI_PROTO_HEADER}")
    target_link_libraries(AgrirouterClient ${AGRIROUTER_EFDI_PROTO_TARGET})
endif()
