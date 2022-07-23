/* ----------------------------------- Local -------------------------------- */
#include "egnite/egnite/web/client.h"
/* -------------------------------------------------------------------------- */

namespace egnite::web {

Client::Client(QObject *parent) : QObject(parent), m_serializer(nullptr) {
  m_headers.setRawHeader("Content-Type", "application/json");
}

Client::~Client() = default;

const QUrl &Client::getBaseUrl() const { return m_base_url; }

QUrl Client::getUrl(const QUrl &url) const {
  return m_base_url.resolved(QLatin1String(".") + url.toString());
}

void Client::setBaseUrl(const QUrl &base_url) {
  if (m_base_url == base_url)
    return;

  m_base_url = base_url;
  Q_EMIT baseUrlChanged(m_base_url);
}

const Headers &Client::getHeaders() const { return m_headers; }

void Client::setHeaders(const Headers &headers) {
  if (m_headers == headers)
    return;

  m_headers = headers;
  Q_EMIT headersChanged(m_headers);
}

Serializer *Client::getSerializer() const { return m_serializer; }

void Client::setSerializer(Serializer *serializer) {
  if (m_serializer == serializer)
    return;

  m_serializer = serializer;
  Q_EMIT serializerChanged(m_serializer);
}

QNetworkReply *Client::get(const QUrl &url) {
  auto reply = m_network_access_manager.get(m_headers.createRequest(url));
  connectErrorListeners(reply);
  return reply;
}

QNetworkReply *Client::post(const QUrl &url) {
  auto reply = m_network_access_manager.sendCustomRequest(m_headers.createRequest(url), "POST");
  connectErrorListeners(reply);
  return reply;
}

QNetworkReply *Client::post(const QUrl &url, const QByteArray &data) {
  auto reply = m_network_access_manager.post(m_headers.createRequest(url), data);
  connectErrorListeners(reply);
  return reply;
}

QNetworkReply *Client::put(const QUrl &url) {
  auto reply = m_network_access_manager.sendCustomRequest(m_headers.createRequest(url), "PUT");
  connectErrorListeners(reply);
  return reply;
}

QNetworkReply *Client::put(const QUrl &url, const QByteArray &data) {
  auto reply = m_network_access_manager.put(m_headers.createRequest(url), data);
  connectErrorListeners(reply);
  return reply;
}

QNetworkReply *Client::patch(const QUrl &url) {
  auto reply = m_network_access_manager.sendCustomRequest(m_headers.createRequest(url), "PATCH");
  connectErrorListeners(reply);
  return reply;
}

QNetworkReply *Client::patch(const QUrl &url, const QByteArray &data) {
  auto reply
      = m_network_access_manager.sendCustomRequest(m_headers.createRequest(url), "PATCH", data);
  connectErrorListeners(reply);
  return reply;
}

QNetworkReply *Client::deleteResource(const QUrl &url) {
  auto reply = m_network_access_manager.deleteResource(m_headers.createRequest(url));
  connectErrorListeners(reply);
  return reply;
}

QNetworkReply *Client::head(const QUrl &url) {
  auto reply = m_network_access_manager.head(m_headers.createRequest(url));
  connectErrorListeners(reply);
  return reply;
}

QNetworkReply *Client::options(const QUrl &url) {
  auto reply = m_network_access_manager.sendCustomRequest(m_headers.createRequest(url), "OPTIONS");
  connectErrorListeners(reply);
  return reply;
}

void Client::connectErrorListeners(QNetworkReply *reply) {
  connect(reply, &QNetworkReply::errorOccurred, this, &Client::errorOccured);
  connect(reply, &QNetworkReply::sslErrors, this, &Client::sslErrorOccured);
}

} // namespace egnite::web
