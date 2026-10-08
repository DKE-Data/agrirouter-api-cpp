# Changelog

All notable changes to this project (since v3.0) are documented in this file.

## [Unreleased] (planned: v4.0)

> **⚠️ This release contains breaking changes.**
> Applications that fetch feed messages or use MQTT must be adapted.
> Read [Breaking changes](#breaking-changes) and the migration steps before updating.

### Breaking changes

#### Feed messages are no longer confirmed automatically

Until now, every `MessageQueryResponse` was confirmed via `sendMessagesConfirm()` before the
application received it. If the application crashed or could not process or store the message,
the message was lost: the agrirouter does not deliver confirmed messages again.

**Now:** The client does not confirm feed messages anymore. Until a message is confirmed, it is
delivered again on every query.

**Migration:**

- After processing each `MessageQueryResponse`, collect the IDs from
  `messages(n).header().message_id()` and confirm them with `sendMessagesConfirm()`.
- For chunked messages, confirm every chunk.
- Confirm only after the message has been processed or stored safely.
- `AgrirouterClientTester` (`Application::onMessageCallback`) shows an example.

Applications that do not fetch feed messages are not affected.

#### MQTT: no endless retry loop during client setup

Until now, `MqttConnectionProvider::init()` retried the MQTT client setup every 30 s until it
succeeded. If the setup failed, e.g. because of missing or invalid certificate files, the
`AgrirouterClient` constructor (with `ConnectionType MQTT`) and `renewConnection()` blocked
indefinitely.

**Now:**

- The setup is tried exactly once.
- If it fails, `onError` is called with the new error code `MG_ERROR_MQTT_CONNECT_FAILED`, and
  there is no automatic retry. The more specific error of the failing step, e.g.
  `MG_ERROR_MISSING_OR_EXPIRED_CERTIFICATE`, is still reported before it.
- If the setup succeeds but the connection cannot be established (e.g. no network), mosquitto
  keeps reconnecting by itself. This is unchanged.

**Migration:**

- Handle `MG_ERROR_MQTT_CONNECT_FAILED` in `onError` and retry with `renewConnection()`, e.g. with
  a timer every 30 s. This gives the previous behaviour without blocking.
- Check the new `isConnected()` before sending. The result of `mosquitto_publish` is not reported
  to the application.

HTTP applications are not affected.

### Added

- `AgrirouterClient::isConnected()`: the MQTT connection state. For HTTP it always returns `true`,
  because there is no persistent connection.
- `AgrirouterClient::sendRawMessage()`: sends a message with any technical message type and
  `typeUrl`. The payload is sent as binary data without chunking.
- `Settings::setHttpCaBundlePath()`: CA bundle for HTTPS, including onboarding. A file is used as
  `CURLOPT_CAINFO`, a directory as `CURLOPT_CAPATH`. If it is not set (the default), the CA store
  of libcurl or the system is used as before. `certificateCaPath` still applies to MQTT only.
- New error codes in `Definitions.h`:
  - `MG_ERROR_MQTT_CONNECT_FAILED` (`MG_ERROR_BASE + 5`)
  - `MG_ERROR_INVALID_ONBOARD_RESPONSE` (`MG_ERROR_BASE + 6`)
- `Registration::parseParametersAndCertificates(const std::string&, ConnectionParameters&)`:
  returns whether the onboarding response is valid. The existing overload is kept.
- `Settings::setHttpConnectTimeout()` (default 15 s) and `Settings::setHttpStallTimeout()`
  (default 30 s) for HTTPS requests. A request is aborted with `CURLE_OPERATION_TIMEDOUT` (28) if
  the connection cannot be established in time, or if less than 1 byte/s is transferred for the
  stall timeout. `0` restores the previous behavior (libcurl connect timeout of 300 s, no stall
  timeout). There is no total timeout, so slow uploads are not aborted.

### Changed

- The generated message ID is now returned to the caller. If an empty `messageId` is passed to
  any send method, the generated UUID is written back to it.
  **Note:** If you reuse the same string variable for several sends without clearing it, all of
  them are now sent with the same ID. Clear the string before each send.
- `getResponsesFromMessage()` returns `EXIT_FAILURE` if the content is not a JSON array or if
  entries without `command.message` were skipped. Previously it always returned `EXIT_SUCCESS`, or
  crashed in these cases.
- MQTT: the commands topic is subscribed after every successful connect, including automatic
  reconnects.
- CMake: `AgrirouterClient` links the targets `protobuf::libprotobuf`, `CURL::libcurl` (or
  `libcurl`) and `libmosquitto` if the parent project provides them, e.g. via `find_package`,
  FetchContent or CPM. Otherwise it links the plain library names as before. Helper targets named
  `curl` or `mosquitto` are no longer needed.
- HTTP: all requests of a `CurlConnectionProvider` share a libcurl share handle. Open
  connections, TLS sessions and DNS results are reused instead of a new TCP connection and full
  TLS handshake with client certificate for every request, which saves data volume. libcurl does
  not reuse connections that were idle for more than 118 s (`CURLOPT_MAXAGE_CONN`). TCP keepalive
  is enabled. `CurlConnectionProvider` is no longer copyable.

### Fixed

- Onboarding: crash or wrong success report for an unexpected response (missing fields, missing
  certificate). `onError` is now called with `MG_ERROR_INVALID_ONBOARD_RESPONSE`, and the
  registration callback reports failure. Connection parameters, certificate and private key are
  only stored if the response is valid.
- Crash on responses without `command.message`, e.g. JSON error responses (HTTP and MQTT).
- Stack overwrite when reading the HTTP status code (`long` expected by libcurl, `int32_t` passed).
- Binary payloads of `sendImage()` and `sendTaskdataZip()` were cut at the first NUL byte.
- libcurl in multithreaded processes: `CURLOPT_NOSIGNAL` is set on all handles, and
  `curl_global_init` is called once.
- HTTP polling: the poll counter was never reset. After `pollingMaxTime` had been reached once,
  every later polling cycle stopped after the first request.
- MQTT: memory leak for every incoming message.
- MQTT: the message parameters were set only after `publish()`, so a fast response could be
  delivered with the parameters of the previous message.
- MQTT: the connection state was set to connected even if the connect failed.
- MQTT: the subscription to the commands topic was lost if the first connect failed and was not
  repeated after a reconnect.
- MQTT: crash in `requestMessages()`.
- `decodeRequest()`/`decodeResponse()`: memory allocated with `new[]` was freed with `delete`.
  Length prefixes larger than the message are rejected.
- Stack overflow when serializing large messages (e.g. device descriptions or timelogs).
- HTTP: a request on a dead connection (e.g. after a mobile network handover) could block for
  many minutes, because no timeouts were set.

### Removed

- `getModifiedUuid()` from `Utils.h`. It was not used and created invalid UUIDs for numbers with
  more than one digit.

### Usage notes

- `AgrirouterClient` is not thread safe. Make all calls from one thread. MQTT callbacks are called
  from the mosquitto thread.
- Match responses via `response.envelope.application_message_id()`. The `applicationMessageId` in
  the callbacks only refers to the last sent message.
- `chunkSize` is not used for splitting messages. Split large messages yourself and send them with
  `createChunkMessage()` and `sendChunk()`, using a separate message ID per chunk.
- With `pollingInterval = 0`, `requestMessages()` returns after one request without polling again.
