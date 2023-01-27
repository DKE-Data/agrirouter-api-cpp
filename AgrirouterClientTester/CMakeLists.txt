################################################################################
# AgrirouterClientTest files
################################################################################

file(GLOB AgrirouterClientTest_SRCS
    "src/*.cpp"
    "inc/*.h"
)

################################################################################
# Add AgrirouterClientTester to the project and install it
################################################################################
add_executable(AgrirouterClientTest ${AgrirouterClientTest_SRCS})
#target_include_directories(AgrirouterClientTester PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src>)
target_include_directories(AgrirouterClientTest PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/inc>)

target_link_libraries(AgrirouterClientTest AgrirouterClient)
