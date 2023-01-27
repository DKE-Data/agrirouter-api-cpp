
subdir('src/Protobuf')

agrirouter_lib_sources = [
    'src/ConnectionProvider/ConnectionProvider.cpp',
    'src/ConnectionProvider/CurlConnectionProvider.cpp',
    'src/AgrirouterClient.cpp',
    'src/AgrirouterMessage.cpp',
    'src/MessageProvider.cpp',
    'src/Registration.cpp',
    'src/Settings.cpp',
    'third_party/cJSON/cJSON.c'
]

libagrirouter_include_dir = include_directories('./inc')

libagrirouter = static_library(
    'agrirouter',
    sources: agrirouter_lib_sources,
    include_directories: libagrirouter_include_dir,
    dependencies: [agrirouter_protobuf_dep]
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