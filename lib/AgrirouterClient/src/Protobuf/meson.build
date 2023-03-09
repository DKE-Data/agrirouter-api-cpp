python3 = find_program('python3')
protoc = find_program('protoc')

res = run_command(python3, '-c', 'import os; print(os.environ["PKG_CONFIG_SYSROOT_DIR"])')
if res.returncode() == 0
    sdk_var = res.stdout().strip() + '/usr/include/'
else 
    sdk_var = meson.current_source_dir()
endif

protobuf_generator = generator(
    protoc,
    output    : ['@BASENAME@.pb.cc', '@BASENAME@.pb.h'],
    arguments : [
        '-I=' + meson.current_source_dir(),
        '-I=' + sdk_var,
        '--cpp_out=@BUILD_DIR@',
        '@INPUT@'
    ]
)

agrirouter_protobuf_files = protobuf_generator.process(
    join_paths([meson.current_source_dir(), 'commons/message.proto']),
    join_paths([meson.current_source_dir(), 'commons/chunk.proto']),
    join_paths([meson.current_source_dir(), 'cloud-provider-integration/cloud-virtualized-app-registration.proto']),
    join_paths([meson.current_source_dir(), 'messaging/request/request.proto']),
    join_paths([meson.current_source_dir(), 'messaging/request/payload/account/endpoints.proto']),
    join_paths([meson.current_source_dir(), 'messaging/request/payload/efdi/grpc-efdi.proto']),
    join_paths([meson.current_source_dir(), 'messaging/request/payload/endpoint/capabilities.proto']),
    join_paths([meson.current_source_dir(), 'messaging/request/payload/endpoint/subscription.proto']),
    join_paths([meson.current_source_dir(), 'messaging/request/payload/feed/feed-requests.proto']),
    join_paths([meson.current_source_dir(), 'messaging/response/response.proto']),
    join_paths([meson.current_source_dir(), 'messaging/response/payload/account/endpoints.proto']),
    join_paths([meson.current_source_dir(), 'messaging/response/payload/endpoint/capability.proto']),
    join_paths([meson.current_source_dir(), 'messaging/response/payload/feed/feed-response.proto']),
    join_paths([meson.current_source_dir(), 'messaging/response/payload/feed/push-notification.proto']),
    join_paths([meson.current_source_dir(), 'settings/dh-settings.proto']),
    preserve_path_from : meson.current_source_dir()
)

agrirouter_proto_lib = static_library(
    'agrirouter_proto',
    agrirouter_protobuf_files
)

agrirouter_proto_lib_include_dir = agrirouter_proto_lib.private_dir_include()

agrirouter_protobuf_dep = declare_dependency(
    link_with: agrirouter_proto_lib,
    include_directories: agrirouter_proto_lib_include_dir
)

