/* ----------------------------------- Local -------------------------------- */
#include "egnite/egnite/web/web_client.h"
/* -------------------------------------------------------------------------- */

namespace egnite::web {

WebClient::WebClient(QObject *parent) : QObject(parent), m_serializer(nullptr) {
  m_headers.setRawHeader("Content-Type", "application/json");
}

WebClient::~WebClient() = default;

const QUrl &WebClient::getBaseUrl() const { return m_base_url; }

QUrl WebClient::getUrl(const QUrl &url) const {
  return m_base_url.resolved(QLatin1String(".") + url.toString());
}

void WebClient::setBaseUrl(const QUrl &base_url) {
  if (m_base_url == base_url)
    return;

  m_base_url = base_url;
  Q_EMIT baseUrlChanged(m_base_url);
}

const WebHeaders &WebClient::getHeaders() const { return m_headers; }

void WebClient::setHeaders(const WebHeaders &headers) {
  if (m_headers == headers)
    return;

  m_headers = headers;
  Q_EMIT headersChanged(m_headers);
}

WebSerializer *WebClient::getSerializer() const { return m_serializer; }

void WebClient::setSerializer(WebSerializer *serializer) {
  if (m_serializer == serializer)
    return;

  m_serializer = serializer;
  Q_EMIT serializerChanged(m_serializer);
}

QNetworkReply *WebClient::get(const QUrl &url) {
  auto reply = m_network_access_manager.get(m_headers.createRequest(url));
  connectErrorListeners(reply);
  return reply;
}

QNetworkReply *WebClient::post(const QUrl &url) {
  auto reply = m_network_access_manager.sendCustomRequest(m_headers.createRequest(url), "POST");
  connectErrorListeners(reply);
  return reply;
}

QNetworkReply *WebClient::post(const QUrl &url, const QByteArray &data) {
  auto reply = m_network_access_manager.post(m_headers.createRequest(url), data);
  connectErrorListeners(reply);
  return reply;
}

QNetworkReply *WebClient::put(const QUrl &url) {
  auto reply = m_network_access_manager.sendCustomRequest(m_headers.createRequest(url), "PUT");
  connectErrorListeners(reply);
  return reply;
}

QNetworkReply *WebClient::put(const QUrl &url, const QByteArray &data) {
  auto reply = m_network_access_manager.put(m_headers.createRequest(url), data);
  connectErrorListeners(reply);
  return reply;
}

QNetworkReply *WebClient::patch(const QUrl &url) {
  auto reply = m_network_access_manager.sendCustomRequest(m_headers.createRequest(url), "PATCH");
  connectErrorListeners(reply);
  return reply;
}

QNetworkReply *WebClient::patch(const QUrl &url, const QByteArray &data) {
  auto reply
      = m_network_access_manager.sendCustomRequest(m_headers.createRequest(url), "PATCH", data);
  connectErrorListeners(reply);
  return reply;
}

QNetworkReply *WebClient::deleteResource(const QUrl &url) {
  auto reply = m_network_access_manager.deleteResource(m_headers.createRequest(url));
  connectErrorListeners(reply);
  return reply;
}

QNetworkReply *WebClient::head(const QUrl &url) {
  auto reply = m_network_access_manager.head(m_headers.createRequest(url));
  connectErrorListeners(reply);
  return reply;
}

QNetworkReply *WebClient::options(const QUrl &url) {
  auto reply = m_network_access_manager.sendCustomRequest(m_headers.createRequest(url), "OPTIONS");
  connectErrorListeners(reply);
  return reply;
}

void WebClient::connectErrorListeners(QNetworkReply *reply) {
  connect(reply, &QNetworkReply::errorOccurred, this, &WebClient::errorOccured);
  connect(reply, &QNetworkReply::sslErrors, this, &WebClient::sslErrorOccured);
}

} // namespace egnite::web
