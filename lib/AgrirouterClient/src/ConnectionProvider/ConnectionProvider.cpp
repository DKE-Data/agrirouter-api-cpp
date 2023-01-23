#include "../../inc/ConnectionProvider.h"

#include <string>
#include <vector>

void ConnectionProvider::setSettings(Settings* settings) {
  this->settings = settings;
}

Settings* ConnectionProvider::getSettings() {
  return this->settings;
}

void ConnectionProvider::setBody(std::string body) {
  this->body = body;
}

std::string ConnectionProvider::getBody() {
  return this->body;
}

void ConnectionProvider::setUrl(std::string url) {
  this->url = url;
}

std::string ConnectionProvider::getUrl() {
  return this->url;
}

void ConnectionProvider::setHeaders(std::vector<std::string> headers) {
  this->headers = headers;
}

std::vector<std::string> ConnectionProvider::getHeaders() {
  return this->headers;
}

void ConnectionProvider::setCallback(ConnectionProvider::Callback callback) {
  this->callback = callback;
}

ConnectionProvider::Callback ConnectionProvider::getCallback() {
  return this->callback;
}
void ConnectionProvider::setMember(void* member) {
  this->member = member;
}

void* ConnectionProvider::getMember() {
  return this->member;
}
