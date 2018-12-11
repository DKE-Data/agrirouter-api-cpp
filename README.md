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

### Steps to start program

1. `cmake CMakeLists.txt`
2. `make`
3. `./AgrirouterClientTest --help` <br>
(Use one of the command line arguments)

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
        "url": "https://agrirouter-registration-service.cfapps.eu1.hana.ondemand.com/api/v1.0/registration/onboard"
    }
}
```

### Other hints

The project is devided into 2 parts: the [lib](lib/AgrirouterClient) and an [example application](AgrirouterClientTester) that is using the compiled lib.

Before creating a pull request, please execute the lint program in folder [cpplint](cpplint) and remove all occuring errors.

## How to install Protobuf C++ Compiler <a name="compiler"></a>

The following instructions are based on the [Google Protobuf installation guide](https://github.com/google/protobuf/blob/master/src/README.md) and are optimized for Unix based systems. For Windows it is recommended to install [Cygwin](https://www.cygwin.com/) and do the same steps as if you were on a Unix based system.

#### Download packages

You can download protoc and protobuf packages from [Google Protobuf Releases](https://github.com/google/protobuf/releases).
Make sure you download **the same versions** of protoc and protobuf otherwise the installation won't work.

E.g. *protoc-3.5.0-linux-x86_64.zip* and *protobuf-cpp-3.5.0.zip*

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

#### Cross compiling

If you want to use cross compiling you have to use `--host` option when executing `./configure`. <br>
E.g. `./configure --host=powerpc-linux CC=powerpc-linux-gnu-gcc CXX=powerpc-linux-gnu-g++  --with-protoc=/usr/bin/protoc` <br>

Also see [Cross-compiling hints](https://github.com/eurotech/edc-examples/wiki/Cross-compiling-protobuf-for-ARM-architecture).

