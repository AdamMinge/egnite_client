/* ----------------------------------- Local -------------------------------- */
#include "egnite/rest/client.h"

#include "egnite/rest/api.h"
#include "egnite/rest/data_serializer.h"
#include "egnite/rest/detail/client_p.h"
#include "egnite/rest/reply_decorator_manager.h"
/* -------------------------------------------------------------------------- */

namespace egnite::rest {

/* --------------------------------- Client --------------------------------- */

Client::Client(QObject* parent)
    : Client(*new detail::ClientPrivate(QUrl{}, QVersionNumber{}, Headers{},
                                        QUrlQuery{}),
             parent) {}

Client::Client(detail::ClientPrivate& impl, QObject* parent)
    : QObject(impl, parent) {}

Client::~Client() = default;

Api* Client::createApi(const QString& path, QObject* parent) {
  Q_D(detail::Client);
  return new Api(this, d->getNetworkAccessManager(), path, parent);
}

void Client::setBaseUrl(const QUrl& url) {
  Q_D(detail::Client);
  if (d->getBaseUrl() == url) return;

  d->setBaseUrl(url);
  Q_EMIT baseUrlChanged(url);
}

QUrl Client::getBaseUrl() const {
  Q_D(const detail::Client);
  return d->getBaseUrl();
}

void Client::setVersion(const QVersionNumber& version) {
  Q_D(detail::Client);
  if (d->getVersion() == version) return;

  d->setVersion(version);
  Q_EMIT versionChanged(version);
}

QVersionNumber Client::getVersion() const {
  Q_D(const detail::Client);
  return d->getVersion();
}

void Client::setGlobalHeaders(const Headers& headers) {
  Q_D(detail::Client);
  if (d->getGlobalHeaders() == headers) return;

  d->setGlobalHeaders(headers);
  Q_EMIT globalHeadersChanged(headers);
}

Headers Client::getGlobalHeaders() const {
  Q_D(const detail::Client);
  return d->getGlobalHeaders();
}

void Client::setGlobalParameters(const QUrlQuery& parameters) {
  Q_D(detail::Client);
  if (d->getGlobalParameters() == parameters) return;

  d->setGlobalParameters(parameters);
  Q_EMIT globalParametersChanged(parameters);
}

QUrlQuery Client::getGlobalParameters() const {
  Q_D(const detail::Client);
  return d->getGlobalParameters();
}

RequestBuilder Client::getRequestBuilder() const {
  Q_D(const detail::Client);
  return d->getRequestBuilder();
}

DataSerializer* Client::getDataSerializer() const {
  Q_D(const detail::Client);
  return d->getDataSerializer();
}

ReplyDecoratorManager* Client::getReplyDecoratorManager() const {
  Q_D(const detail::Client);
  return d->getReplyDecoratorManager();
}

/* ------------------------------ ClientPrivate ----------------------------- */

namespace detail {

ClientPrivate::ClientPrivate(const QUrl& url, const QVersionNumber& version,
                             const Headers& headers,
                             const QUrlQuery& parameters)
    : m_base_url(url),
      m_version(version),
      m_headers(headers),
      m_parameters(parameters),
      m_data_serializer(new DataSerializer),
      m_reply_decorator_manager(new ReplyDecoratorManager) {}

void ClientPrivate::setBaseUrl(const QUrl& url) { m_base_url = url; }

QUrl ClientPrivate::getBaseUrl() const { return m_base_url; }

void ClientPrivate::setVersion(const QVersionNumber& version) {
  m_version = version;
}

QVersionNumber ClientPrivate::getVersion() const { return m_version; }

void ClientPrivate::setGlobalHeaders(const Headers& headers) {
  m_headers = headers;
}

Headers ClientPrivate::getGlobalHeaders() const { return m_headers; }

void ClientPrivate::setGlobalParameters(const QUrlQuery& parameters) {
  m_parameters = parameters;
}

QUrlQuery ClientPrivate::getGlobalParameters() const { return m_parameters; }

RequestBuilder ClientPrivate::getRequestBuilder() const {
  return RequestBuilder{}
      .setBaseUrl(m_base_url)
      .setVersion(m_version)
      .addParameters(m_parameters)
      .addHeaders(m_headers);
}

QNetworkAccessManager* ClientPrivate::getNetworkAccessManager() const {
  return m_manager;
}

DataSerializer* ClientPrivate::getDataSerializer() const {
  return m_data_serializer.get();
}

ReplyDecoratorManager* ClientPrivate::getReplyDecoratorManager() const {
  return m_reply_decorator_manager.get();
}

}  // namespace detail

}  // namespace egnite::rest

#include "egnite/rest/moc_client.cpp"
