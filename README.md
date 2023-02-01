# agrirouter-api-cpp

1. [Abstract](#abstract)
2. [The current project you’re looking at](#project)
3. [How to install Protobuf C++ Compiler](#compiler)

## Abstract <a name="abstract"></a>

![alt text](https://my-agrirouter.com/fileadmin/template/assets/img/agrirouter_logo.svg "Logo Title Text 1")

The agrirouter is a universal data exchange platform for farmers and agricultural contractors that makes it possible to connect machinery and agricultural software, regardless of vendor or manufacturer. Agrirouter does not save data; it transfers data.
As a universal data exchange platform, agrirouter fills a gap on the way to Farming 4.0. Its underlying concept unites cross-vendor and discrimination-free data transfer. You retain full control over your data. Even data exchange with service providers (e.g. agricultural contractors) and other partners is uncomplicated: Data are very rapidly transferred via the online connection, and if you wish, is intelligently connected to other datasets.

## The current project you're looking at <a name="project"></a>

This project contains the C++ API for the communication with the agrirouter. Everything you need for the onboarding process, communication and much more.

### Prerequisite

The following packages have to be installed on your device

- protobuf: <br>
[protobuf installation guide](#compiler)
- curl: <br>
`sudo apt-get install curl`
- uuid and uuid-dev: <br>
`sudo apt-get install uuid uuid-dev`

### Steps to build program

1. Create build directory <br>
   `mkdir build`
2. Go to build directory <br>
   `cd build`
3. Create Makefiles <br>
   `cmake ..`
4. Build lib and tester <br>
   `make`

### Configuration

The configuration file `AgrirouterClientTesterConfig.json` looks like this

```
{
    "applicationSettings": {
        "applicationId": "SOME_APPLICATION_ID",
        "certificationVersionId": "SOME_CERTIFICATION_VERSION_ID",
        "externalId": "SOME_EXTERNAL_ID",
        "locationCertsAndIds": "THE_LOCATION_WHERE_THE_KEY_AND_CERT_ARE_STORED",
        "teamsetContextId": "SOME_TEAMSET_CONTEXT_ID",
        "connectionType": "HTTP"
    },
    "agrirouterSettings": {
        "registrationServicePath" : "onboard.my",
        "landscape" : "agrirouter",
        "cfHost" : "com",
        "apiPrefix" : "api/v1.0"
    }
}
```
### Start the agrirouter client tester
1. To use the tool change the applicationSettings in the configuration file. <br>
2. `./AgrirouterClientTest --help` (Use one of the command line arguments) <br>
3. For e.g. onboarding to get certificates`./AgrirouterClientTest --onboard=123456789`
### Other hints

The project is devided into 2 parts: the [lib](lib/AgrirouterClient) and an [example application](AgrirouterClientTester) that is using the compiled lib.

Before creating a pull request, please build the application and remove all occuring errors.

## How to install Protobuf C++ Compiler <a name="compiler"></a>

The following instructions are based on the [Google Protobuf installation guide](https://github.com/google/protobuf/blob/master/src/README.md) and are optimized for Unix based systems. For Windows it is recommended to install [Cygwin](https://www.cygwin.com/) and do the same steps as if you were on a Unix based system.

### Install with package manager
For example Debian/Ubuntu
1. Update sources <br>
   `sudo apt update`
2. Install protoc and protobuf <br>
`sudo apt install protobuf-compiler libprotobuf-dev libprotoc-dev`

### Install manuell
#### Download packages

You can download protoc and protobuf packages from [Google Protobuf Releases](https://github.com/google/protobuf/releases).
Make sure you download **the same versions** of protoc and protobuf otherwise the installation won't work.

E.g. *protoc-21.11-linux-x86_64.zip* and *protobuf-cpp-3.21.11.zip*

#### Prepare Protoc

1. Unzip package <br>
   `unzip protoc-3.* -d protoc3`
2. Move protoc to /usr/bin/ <br>
   `sudo mv protoc3/bin/protoc /usr/bin/`
3. Move included packages to /usr/include/ <br>
   `sudo mv protoc3/include/* /usr/include/`
4. Optional: make protoc executable for everyone <br>
   `sudo chmod +x /usr/bin/protoc`
5. Optional: make includes readable for everyone <br>
   `sudo chmod +r /usr/include/google/*`

#### Install Protobuf

1. Unzip package <br>
   `unzip protobuf-cpp-3.* -d protobuf3`
2. Go to folder *profobuf3* and make file configure executable <br>
   `chmod +x configure`
3. Execute following commands <br>
   `./configure` <br>
   `make` <br>
   `make check` <br>
   `sudo make install` <br>
   `sudo ldconfig` <br>
   Note: If "make check" fails, you can still install, but it is likely that some features of this library will not work correctly on your system. Proceed at your own risk.

#### Use Protoc

If you want to compile some proto files use `protoc [OPTIONS] PROTO_FILES` <br>

The `[OPTIONS]` part indicates the destination of compiled file. You can use relative and absolute paths. <br>
In the `PROTO_FILES` part you can define which protobuf files will be compiled.

E.g. the command `protoc --cpp_out=. ./*.proto` compiles every every proto file in this folder and places the compiled ones within this folder.

#### Generate cpp protobuf files
The files are generally generated with the build system.
If it is necessary to compile them without the build system, perform the following steps:

1. Go to protobuf folder <br>
   `cd ./lib/AgrirouterClient/src/Protobuf/`
2. Execute the compile protobuf script <br>
   `. ./compileProtos.sh` <br>

The generate files can be found under ./lib/AgrirouterClient/inc/Protobuf/

#### Cross compiling

If you want to use cross compiling you have to use `--host` option when executing `./configure`. <br>
E.g. `./configure --host=powerpc-linux CC=powerpc-linux-gnu-gcc CXX=powerpc-linux-gnu-g++  --with-protoc=/usr/bin/protoc` <br>

Also see [Cross-compiling hints](https://github.com/eurotech/edc-examples/wiki/Cross-compiling-protobuf-for-ARM-architecture).

