/* ------------------------------------ Qt ---------------------------------- */
#include <QJsonDocument>
/* ----------------------------------- Local -------------------------------- */
#include "egnite/egnite/web/web_client.h"
/* -------------------------------------------------------------------------- */

namespace egnite::web {

/* -------------------------------- WebHeaders ---------------------------- */

WebHeaders::WebHeaders() = default;

WebHeaders::~WebHeaders() = default;

QNetworkRequest WebHeaders::createRequest(const QUrl &url) const {
  auto request = m_request;
  request.setUrl(url);
  return request;
}

void WebHeaders::setHeader(QNetworkRequest::KnownHeaders header, const QVariant &value) {
  m_request.setHeader(header, value);
}

void WebHeaders::setRawHeader(const QByteArray &headerName, const QByteArray &headerValue) {
  m_request.setRawHeader(headerName, headerValue);
}

QVariant WebHeaders::header(QNetworkRequest::KnownHeaders header) const {
  return m_request.header(header);
}

QByteArray WebHeaders::rawHeader(const QByteArray &headerName) const {
  return m_request.rawHeader(headerName);
}

QList<QByteArray> WebHeaders::rawHeaderList() const { return m_request.rawHeaderList(); }

bool WebHeaders::hasRawHeader(const QByteArray &headerName) const {
  return m_request.hasRawHeader(headerName);
}

bool WebHeaders::operator==(const WebHeaders &other) { return m_request == other.m_request; }

bool WebHeaders::operator!=(const WebHeaders &other) { return m_request != other.m_request; }

/* --------------------------------- WebClient ---------------------------- */

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

QNetworkReply *WebClient::get(const QUrl &url) {
  return m_network_access_manager.get(m_headers.createRequest(url));
}

QNetworkReply *WebClient::post(const QUrl &url) {
  return m_network_access_manager.sendCustomRequest(m_headers.createRequest(url), "POST");
}

QNetworkReply *WebClient::post(const QUrl &url, const QJsonObject &data) {

  return m_network_access_manager.post(m_headers.createRequest(url), QJsonDocument(data).toJson());
}

QNetworkReply *WebClient::put(const QUrl &url) {
  return m_network_access_manager.sendCustomRequest(m_headers.createRequest(url), "PUT");
}

QNetworkReply *WebClient::put(const QUrl &url, const QJsonObject &data) {
  return m_network_access_manager.put(m_headers.createRequest(url), QJsonDocument(data).toJson());
}

QNetworkReply *WebClient::patch(const QUrl &url) {
  return m_network_access_manager.sendCustomRequest(m_headers.createRequest(url), "PATCH");
}

QNetworkReply *WebClient::patch(const QUrl &url, const QJsonObject &data) {
  return m_network_access_manager.sendCustomRequest(m_headers.createRequest(url), "PATCH",
                                                    QJsonDocument(data).toJson());
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
