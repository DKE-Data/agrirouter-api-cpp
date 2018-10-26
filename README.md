An implementation of the agrirouter interface in C++

### Prerequisite

The following packages have to be installed on your device

- protobuf: <br>
[protobuf installation guide](https://git.cci-net.de/cci/agrirouter_lib/wikis/install-protoc)
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

`{
    "applicationSettings": {
        "applicationId": "SOME_APPLICATION_ID",
        "certificationVersionId": "SOME_CERTIFICATION_VERSION_ID",
        "externalId": "SOME_EXTERNAL_ID",
        "locationCertsAndIds": "THE_LOCATION_WHERE_THE_KEY_AND_CERT_ARE_STORED",
        "teamsetContextId": "SOME_TEAMSET_CONTEXT_ID",
        "connectionType": "HTTP"
    },
    "agrirouterSettings": {
        "url": "https://agrirouter-registration-service-hubqa-eu1.cfapps.eu1.hana.ondemand.com/api/v1.0/registration/onboard"
    }
}`

### Other hints

The project is devided into 2 parts: the lib (folder lib/AgrirouterClient) and an example application that is using the compiled lib (folder AgrirouterClientTester).

Before creating a pull request, please execute the lint program in folder cpplint and remove all occuring errors.
 