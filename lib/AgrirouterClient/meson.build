
subdir('src/Protobuf')

agrirouter_lib_sources = [
    'src/ConnectionProvider/MqttConnectionClient.cpp',
    'src/ConnectionProvider/ConnectionProvider.cpp',
    'src/ConnectionProvider/CurlConnectionProvider.cpp',
    'src/ConnectionProvider/MqttConnectionProvider.cpp',
    'src/AgrirouterClient.cpp',
    'src/AgrirouterMessage.cpp',
    'src/MessageProvider.cpp',
    'src/Registration.cpp',
    'src/Settings.cpp',
    'third_party/cJSON/cJSON.c'
]

libagrirouter_include_dir = include_directories('./inc')

proto_dep = dependency('protobuf')
curl_dep = dependency('libcurl')
ssl_dep = dependency('libssl')
crypto_dep = dependency('libcrypto')

libagrirouter = static_library(
    'agrirouter',
    sources: agrirouter_lib_sources,
    include_directories: libagrirouter_include_dir,
    dependencies: [agrirouter_protobuf_dep, curl_dep, proto_dep, ssl_dep, crypto_dep]
)

include_dirs = [
    include_directories('.'),
    libagrirouter_include_dir,
    agrirouter_proto_lib_include_dir
]

libagrirouter_dep = declare_dependency(
  link_with: libagrirouter,
  include_directories: include_dirs
)