################################################################################
# Project settings and variables
################################################################################
cmake_minimum_required(VERSION 3.0.2)

project(AgrirouterClient)

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR})
set(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR})
# protobuf paths
set(PROTOBUF_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/lib/AgrirouterClient/inc/Protobuf")
# Install to ${PROJECT_DIR}/install
set(CMAKE_INSTALL_PREFIX ${CMAKE_CURRENT_SOURCE_DIR}/install)
# Enable verbose output from Makefile
set(CMAKE_VERBOSE_MAKEFILE ON)

# By setting this property, we reverse the include directory order.
# Now the local headers are prefered again.
set(CMAKE_INCLUDE_DIRECTORIES_BEFORE ON)

# Set the parent dir for protobuf lib and include paths
set(Protobuf_LIB_DIR /usr/local)

################################################################################
# Compiler-Settings
################################################################################
if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release)
endif()

#set(CMAKE_CXX_COMPILER /usr/bin/powerpc-linux-gnu-g++)

if(CMAKE_COMPILER_IS_GNUCXX)
  #Compiler flags for c
  set(CMAKE_C_FLAGS_DEBUG "-O0 -g3")
  set(CMAKE_C_FLAGS_RELEASE "-Os")
  set(CMAKE_C_FLAGS " ${CMAKE_C_FLAGS} -Wall -Wextra -fmessage-length=0  -Wno-unused-parameter -Wno-unused-function -Wno-unused-but-set-variable -fvisibility=hidden")

  #Compiler flags for c++
  set(CMAKE_CXX_FLAGS_DEBUG "-O0 -g3")
  set(CMAKE_CXX_FLAGS_RELEASE "-Os")
  set(CMAKE_CXX_FLAGS " ${CMAKE_CXX_FLAGS} -Wall -Wextra  -fmessage-length=0 -Wno-unused-parameter -Wno-unused-function -fvisibility=hidden")
endif()

################################################################################
# Include/link directories
################################################################################

#Add protobuf include/link directories
include_directories(
    ${Protobuf_LIB_DIR}/include # /usr/local/include
)
link_directories(
    ${Protobuf_LIB_DIR}/lib # /usr/local/lib
)

################################################################################
# sub directories
################################################################################

# Add AgrirouterClientTester
add_subdirectory(AgrirouterClientTester)

# Add AgrirouterClientLib
add_subdirectory(lib/AgrirouterClient)

################################################################################
# Add proto files
################################################################################
FILE(GLOB protoFiles
    "${CMAKE_CURRENT_SOURCE_DIR}/proto/compileProtos.sh"
    "${CMAKE_CURRENT_SOURCE_DIR}/proto/commons/*.proto"
    "${CMAKE_CURRENT_SOURCE_DIR}/proto/messaging/request/*.proto"
    "${CMAKE_CURRENT_SOURCE_DIR}/proto/messaging/request/payload/account/*.proto"
    "${CMAKE_CURRENT_SOURCE_DIR}/proto/messaging/request/payload/efdi/*.proto"
    "${CMAKE_CURRENT_SOURCE_DIR}/proto/messaging/request/payload/endpoint/*.proto"
    "${CMAKE_CURRENT_SOURCE_DIR}/proto/messaging/request/payload/feed/*.proto"
    "${CMAKE_CURRENT_SOURCE_DIR}/proto/messaging/response/*.proto"
    "${CMAKE_CURRENT_SOURCE_DIR}/proto/messaging/response/payload/account/*.proto"
    "${CMAKE_CURRENT_SOURCE_DIR}/proto/messaging/response/payload/efdi/*.proto"
    "${CMAKE_CURRENT_SOURCE_DIR}/proto/messaging/response/payload/endpoint/*.proto"
    "${CMAKE_CURRENT_SOURCE_DIR}/proto/messaging/response/payload/feed/*.proto"
    "${CMAKE_CURRENT_SOURCE_DIR}/proto/cloud-provider-integration/*.proto"
)
add_custom_target(protos SOURCES ${protoFiles})

################################################################################
# Readme.md
################################################################################
FILE(GLOB readme_md "${CMAKE_CURRENT_SOURCE_DIR}/README.md")
add_custom_target(readme SOURCES ${readme_md})
