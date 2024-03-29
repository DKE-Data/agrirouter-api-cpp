project(
    'agrirouter-api-cpp'
    , 'c'
    , 'cpp' 
    , meson_version : '>=0.47.2'
    , default_options : [
        'buildtype=minsize'
        , 'warning_level=1'
        , 'cpp_std=c++11'
    ]
)

subdir('lib/AgrirouterClient')

if get_option('prod')
    message('production build')
    linker_flags=['-s']
else 
    message('debug build')
    linker_flags=[]
endif

if get_option('buildTester')
    agrirouter_client_tester_include_dir = include_directories('./AgrirouterClientTester/inc')

    agrirouter_client_tester_sources = [
        './AgrirouterClientTester/src/Application.cpp',
        './AgrirouterClientTester/src/Communicator.cpp',
        './AgrirouterClientTester/src/main.cpp'
    ]

    uuid_dep = dependency('uuid')

    agrirouterClientTester_exe = executable(
        'agrirouterClientTester',
        sources: agrirouter_client_tester_sources,
        include_directories: agrirouter_client_tester_include_dir,
        dependencies: [libagrirouter_dep, uuid_dep],
        link_args : linker_flags,
        install : false
    )
endif
