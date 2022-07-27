/* ----------------------------------- Local -------------------------------- */
#include "egnite/egnite/web/client.h"
/* --------------------------------- Standard ------------------------------- */
#include <functional>
/* -------------------------------------------------------------------------- */

namespace egnite::web {

Client::Client(QObject *parent) : QObject(parent), m_headers(nullptr), m_serializer(nullptr) {}

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

Headers *Client::getHeaders() const { return m_headers; }

void Client::setHeaders(Headers *headers) {
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

Reply Client::get(const QUrl &url) {
  Q_ASSERT(m_headers);
  auto reply = createReply(m_network_access_manager.get(m_headers->createRequest(url)));
  return reply;
}

Reply Client::post(const QUrl &url) {
  Q_ASSERT(m_headers);
  auto reply = createReply(
      m_network_access_manager.sendCustomRequest(m_headers->createRequest(url), "POST"));
  return reply;
}

Reply Client::post(const QUrl &url, const QByteArray &data) {
  Q_ASSERT(m_headers);
  auto reply = createReply(
      m_network_access_manager.sendCustomRequest(m_headers->createRequest(url), "POST", data));
  return reply;
}

Reply Client::put(const QUrl &url) {
  Q_ASSERT(m_headers);
  auto reply = createReply(
      m_network_access_manager.sendCustomRequest(m_headers->createRequest(url), "PUT"));
  return reply;
}

Reply Client::put(const QUrl &url, const QByteArray &data) {
  Q_ASSERT(m_headers);
  auto reply = createReply(
      m_network_access_manager.sendCustomRequest(m_headers->createRequest(url), "PUT", data));
  return reply;
}

Reply Client::patch(const QUrl &url) {
  Q_ASSERT(m_headers);
  auto reply = createReply(
      m_network_access_manager.sendCustomRequest(m_headers->createRequest(url), "PATCH"));
  return reply;
}

Reply Client::patch(const QUrl &url, const QByteArray &data) {
  Q_ASSERT(m_headers);
  auto reply = createReply(
      m_network_access_manager.sendCustomRequest(m_headers->createRequest(url), "PATCH", data));
  return reply;
}

Reply Client::deleteResource(const QUrl &url) {
  Q_ASSERT(m_headers);
  auto reply = createReply(m_network_access_manager.deleteResource(m_headers->createRequest(url)));
  return reply;
}

Reply Client::head(const QUrl &url) {
  Q_ASSERT(m_headers);
  auto reply = createReply(m_network_access_manager.head(m_headers->createRequest(url)));
  return reply;
}

Reply Client::options(const QUrl &url) {
  Q_ASSERT(m_headers);
  auto reply = createReply(
      m_network_access_manager.sendCustomRequest(m_headers->createRequest(url), "OPTIONS"));
  return reply;
}

Reply Client::createReply(QNetworkReply *reply) {
  auto client_reply = Reply{reply, m_serializer};

  client_reply.onError(std::bind(&Client::errorOccured, this, std::placeholders::_1))
      .onSslError(std::bind(&Client::sslErrorOccured, this, std::placeholders::_1))
      .onSuccess([reply]() { reply->deleteLater(); });

  return client_reply;
}

} // namespace egnite::web
