################################################################################
# AgrirouterClient files
################################################################################
file(GLOB INC_H
    "inc/*.h"
    "inc/Protobuf/*/*pb.h"
    "inc/Protobuf/messaging/*/*pb.h"
    "inc/Protobuf/messaging/*/payload/*/*pb.h"
    "third_party/cJSON/*.h"
    "third_party/mosquitto/*.h"
    "src/ConnectionProvider/*.h"
)

file(GLOB SRCS
    "src/*.cpp"
    "src/*.h"
    "src/Protobuf/*/*pb.cc"
    "src/Protobuf/messaging/*/*pb.cc"
    "src/Protobuf/messaging/*/payload/*/*pb.cc"
    "src/ConnectionProvider/*.cpp"
    "third_party/cJSON/*.c"
)

################################################################################
# Build static lib and install it
################################################################################
add_library(AgrirouterClient STATIC ${SRCS} ${INC_H})

target_include_directories(AgrirouterClient PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/inc>)
target_include_directories(AgrirouterClient PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src>)
target_include_directories(AgrirouterClient PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src/ConnectionProvider>)
target_include_directories(AgrirouterClient PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/inc/Protobuf>)

install(TARGETS AgrirouterClient DESTINATION lib/AgrirouterClient)
install(FILES ${INC_H} DESTINATION include/AgrirouterClient)

################################################################################
# Link protobuf lib
################################################################################
target_link_libraries(AgrirouterClient protobuf curl uuid mosquitto)
