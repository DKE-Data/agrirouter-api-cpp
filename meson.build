project(
    'agrirouter-api-cpp'
    , 'c'
    , 'cpp' 
    , meson_version : '>=0.46.0'
    , default_options : [
        'buildtype=minsize'
        , 'warning_level=1'
        , 'cpp_std=c++11'
    ]
)

subdir('lib/AgrirouterClient')

agrirouter_client_tester_include_dir = include_directories('./AgrirouterClientTester/inc')

agrirouter_client_tester_sources = [
    './AgrirouterClientTester/src/Application.cpp',
    './AgrirouterClientTester/src/Communicator.cpp',
    './AgrirouterClientTester/src/main.cpp'
]

if get_option('prod')
    message('production build')
    linker_flags=['-s', '-lcurl', '-luuid']
else 
    message('debug build')
    linker_flags=['-lcurl', '-luuid']
endif

proto_dep = dependency('protobuf')

agrirouterClientTester_exe = executable(
    'agrirouterClientTester',
    sources: agrirouter_client_tester_sources,
    include_directories: agrirouter_client_tester_include_dir,
    dependencies: [libagrirouter_dep, proto_dep],
    link_args : linker_flags,
    install : false
)