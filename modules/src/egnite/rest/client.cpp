/* ----------------------------------- Local -------------------------------- */
#include "egnite/rest/client.h"

#include "egnite/rest/api.h"
#include "egnite/rest/data_serializer.h"
#include "egnite/rest/detail/client_p.h"
#include "egnite/rest/pagination.h"
#include "egnite/rest/reply_decorator.h"
/* -------------------------------------------------------------------------- */

namespace egnite::rest {

/* --------------------------------- IClient -------------------------------- */

IClient::IClient(QObject* parent) : QObject(parent) {}

IClient::IClient(QObjectPrivate& impl, QObject* parent)
    : QObject(impl, parent) {}

IClient::~IClient() = default;

/* --------------------------------- Client --------------------------------- */

Client::Client(QObject* parent)
    : Client(*new detail::ClientPrivate(QUrl{}, QVersionNumber{}, Headers{},
                                        QUrlQuery{}),
             parent) {}

Client::Client(detail::ClientPrivate& impl, QObject* parent)
    : IClient(impl, parent) {}

Client::~Client() = default;

IApi* Client::createApi(const QString& path, QObject* parent) {
  Q_D(detail::Client);
  return new Api(this, path, parent);
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
  d->setGlobalHeaders(headers);
}

Headers Client::getGlobalHeaders() const {
  Q_D(const detail::Client);
  return d->getGlobalHeaders();
}

void Client::setGlobalParameters(const QUrlQuery& parameters) {
  Q_D(detail::Client);
  d->setGlobalParameters(parameters);
}

QUrlQuery Client::getGlobalParameters() const {
  Q_D(const detail::Client);
  return d->getGlobalParameters();
}

RequestBuilder Client::getRequestBuilder() const {
  Q_D(const detail::Client);
  return d->getRequestBuilder();
}

QNetworkAccessManager* Client::getNetworkAccessManager() const {
  Q_D(const detail::Client);
  return d->getNetworkAccessManager();
}

DataSerializer* Client::getDataSerializer() const {
  Q_D(const detail::Client);
  return d->getDataSerializer();
}

IReplyDecorator* Client::getReplyDecorator() const {
  Q_D(const detail::Client);
  return d->getReplyDecorator();
}

IPagingDataFactory* Client::getPagingDataFactory() const {
  Q_D(const detail::Client);
  return d->getPagingDataFactory();
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
      m_manager(new QNetworkAccessManager),
      m_data_serializer(new DataSerializer),
      m_reply_decorator(new ReplyDecorator),
      m_paging_data_factory(new StandardPagingDataFactory) {}

void ClientPrivate::setBaseUrl(const QUrl& url) { m_base_url = url; }

QUrl ClientPrivate::getBaseUrl() const { return m_base_url; }

void ClientPrivate::setVersion(const QVersionNumber& version) {
  m_version = version;
}

QVersionNumber ClientPrivate::getVersion() const { return m_version; }

void ClientPrivate::setGlobalHeaders(const Headers& headers) {
  Q_Q(Client);
  if (m_headers == headers) return;

  m_headers = headers;
  Q_EMIT q->globalHeadersChanged(m_headers);
}

Headers ClientPrivate::getGlobalHeaders() const { return m_headers; }

void ClientPrivate::setGlobalParameters(const QUrlQuery& parameters) {
  Q_Q(Client);
  if (m_parameters == parameters) return;

  m_parameters = parameters;
  Q_EMIT q->globalParametersChanged(m_parameters);
}

QUrlQuery ClientPrivate::getGlobalParameters() const { return m_parameters; }

QNetworkAccessManager* ClientPrivate::getNetworkAccessManager() const {
  return m_manager.get();
}

RequestBuilder ClientPrivate::getRequestBuilder() const {
  return RequestBuilder{}
      .setBaseUrl(m_base_url)
      .setVersion(m_version)
      .addParameters(m_parameters)
      .addHeaders(m_headers);
}

DataSerializer* ClientPrivate::getDataSerializer() const {
  return m_data_serializer.get();
}

IReplyDecorator* ClientPrivate::getReplyDecorator() const {
  return m_reply_decorator.get();
}

IPagingDataFactory* ClientPrivate::getPagingDataFactory() const {
  return m_paging_data_factory.get();
}

}  // namespace detail

}  // namespace egnite::rest

#include "egnite/rest/moc_client.cpp"
