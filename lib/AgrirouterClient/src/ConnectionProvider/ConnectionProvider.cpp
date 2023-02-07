#include "ConnectionProvider.h"

#include <string>
#include <vector>

void ConnectionProvider::setSettings(Settings* settings)
{
    this->m_settings = settings;
}

Settings* ConnectionProvider::getSettings() { return this->m_settings; }

void ConnectionProvider::setBody(std::string body) { this->m_body = body; }

std::string ConnectionProvider::getBody() { return this->m_body; }

void ConnectionProvider::setUrl(std::string url) { this->m_url = url; }

std::string ConnectionProvider::getUrl() { return this->m_url; }

void ConnectionProvider::setHeaders(std::vector<std::string> headers)
{
    this->m_headers = headers;
}

std::vector<std::string> ConnectionProvider::getHeaders()
{
    return this->m_headers;
}

void ConnectionProvider::setCallback(ConnectionProvider::Callback callback)
{
    this->m_callback = callback;
}

ConnectionProvider::Callback ConnectionProvider::getCallback()
{
    return this->m_callback;
}
void ConnectionProvider::setMember(void* member) { this->m_member = member; }

void* ConnectionProvider::getMember() { return this->m_member; }
