/* ----------------------------------- Local -------------------------------- */
#include "egnite/egnite/web/web_client.h"
/* -------------------------------------------------------------------------- */

namespace egnite::web {

WebClient::WebClient(QObject *parent) : QObject(parent) {
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
  Q_EMIT onBaseUrlChanged(m_base_url);
}

const WebHeaders &WebClient::getHeaders() const { return m_headers; }

void WebClient::setHeaders(const WebHeaders &headers) {
  if (m_headers == headers)
    return;

  m_headers = headers;
  Q_EMIT onHeadersChanged(m_headers);
}

WebSerializer *WebClient::getSerializer() const { return m_serializer; }

void WebClient::setSerializer(WebSerializer *serializer) {
  if (m_serializer == serializer)
    return;

  m_serializer = serializer;
  Q_EMIT onSerializerChanged(m_serializer);
}

QNetworkReply *WebClient::get(const QUrl &url) {
  return m_network_access_manager.get(m_headers.createRequest(url));
}

QNetworkReply *WebClient::post(const QUrl &url) {
  return m_network_access_manager.sendCustomRequest(m_headers.createRequest(url), "POST");
}

QNetworkReply *WebClient::post(const QUrl &url, const QByteArray &data) {

  return m_network_access_manager.post(m_headers.createRequest(url), data);
}

QNetworkReply *WebClient::put(const QUrl &url) {
  return m_network_access_manager.sendCustomRequest(m_headers.createRequest(url), "PUT");
}

QNetworkReply *WebClient::put(const QUrl &url, const QByteArray &data) {
  return m_network_access_manager.put(m_headers.createRequest(url), data);
}

QNetworkReply *WebClient::patch(const QUrl &url) {
  return m_network_access_manager.sendCustomRequest(m_headers.createRequest(url), "PATCH");
}

QNetworkReply *WebClient::patch(const QUrl &url, const QByteArray &data) {
  return m_network_access_manager.sendCustomRequest(m_headers.createRequest(url), "PATCH", data);
}

QNetworkReply *WebClient::deleteResource(const QUrl &url) {
  return m_network_access_manager.deleteResource(m_headers.createRequest(url));
}

QNetworkReply *WebClient::head(const QUrl &url) {
  return m_network_access_manager.head(m_headers.createRequest(url));
}

QNetworkReply *WebClient::options(const QUrl &url) {
  return m_network_access_manager.sendCustomRequest(m_headers.createRequest(url), "OPTIONS");
}

} // namespace egnite::web
