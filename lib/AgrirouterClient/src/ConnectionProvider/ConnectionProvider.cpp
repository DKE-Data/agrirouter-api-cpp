#include "ConnectionProvider.h"

#include <string>
#include <vector>

void ConnectionProvider::setSettings(Settings* settings)
{
    m_settings = settings;
}

Settings* ConnectionProvider::getSettings() { return m_settings; }

void ConnectionProvider::setBody(std::string body) { m_body = body; }

std::string ConnectionProvider::getBody() { return m_body; }

void ConnectionProvider::setUrl(std::string url) { m_url = url; }

std::string ConnectionProvider::getUrl() { return m_url; }

void ConnectionProvider::setHeaders(std::vector<std::string> headers)
{
    m_headers = headers;
}

std::vector<std::string> ConnectionProvider::getHeaders()
{
    return m_headers;
}

void ConnectionProvider::setCallback(ConnectionProvider::Callback callback)
{
    m_callback = callback;
}

ConnectionProvider::Callback ConnectionProvider::getCallback()
{
    return m_callback;
}
void ConnectionProvider::setMember(void* member) { m_member = member; }

void* ConnectionProvider::getMember() { return m_member; }
