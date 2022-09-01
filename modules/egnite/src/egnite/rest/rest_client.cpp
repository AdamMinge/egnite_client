/* ----------------------------------- Local -------------------------------- */
#include "egnite/rest/rest_client.h"

#include "egnite/rest/detail/rest_client_p.h"
#include "egnite/rest/rest_api.h"
#include "egnite/rest/rest_data_serializer.h"
#include "egnite/rest/rest_reply_decorator_manager.h"
/* -------------------------------------------------------------------------- */

namespace egnite::rest {

/* ------------------------------- RestClient ------------------------------- */

RestClient::RestClient(QObject* parent)
    : RestClient(*new detail::RestClientPrivate(
                     QUrl{}, QVersionNumber{}, RestHeaders{}, QUrlQuery{},
                     new RestDataSerializer(this),
                     new RestReplyDecoratorManager(this)),
                 parent) {}

RestClient::RestClient(detail::RestClientPrivate& impl, QObject* parent)
    : QObject(impl, parent) {}

RestClient::~RestClient() = default;

RestApi* RestClient::createApi(const QString& path, QObject* parent) {
  Q_D(detail::RestClient);
  return new RestApi(this, d->getNetworkAccessManager(), path, parent);
}

void RestClient::setBaseUrl(const QUrl& url) {
  Q_D(detail::RestClient);
  if (d->getBaseUrl() == url) return;

  d->setBaseUrl(url);
  Q_EMIT baseUrlChanged(url);
}

QUrl RestClient::getBaseUrl() const {
  Q_D(const detail::RestClient);
  return d->getBaseUrl();
}

void RestClient::setVersion(const QVersionNumber& version) {
  Q_D(detail::RestClient);
  if (d->getVersion() == version) return;

  d->setVersion(version);
  Q_EMIT versionChanged(version);
}

QVersionNumber RestClient::getVersion() const {
  Q_D(const detail::RestClient);
  return d->getVersion();
}

void RestClient::setGlobalHeaders(const RestHeaders& headers) {
  Q_D(detail::RestClient);
  if (d->getGlobalHeaders() == headers) return;

  d->setGlobalHeaders(headers);
  Q_EMIT globalHeadersChanged(headers);
}

RestHeaders RestClient::getGlobalHeaders() const {
  Q_D(const detail::RestClient);
  return d->getGlobalHeaders();
}

void RestClient::setGlobalParameters(const QUrlQuery& parameters) {
  Q_D(detail::RestClient);
  if (d->getGlobalParameters() == parameters) return;

  d->setGlobalParameters(parameters);
  Q_EMIT globalParametersChanged(parameters);
}

QUrlQuery RestClient::getGlobalParameters() const {
  Q_D(const detail::RestClient);
  return d->getGlobalParameters();
}

RestRequestBuilder RestClient::getRequestBuilder() const {
  Q_D(const detail::RestClient);
  return d->getRequestBuilder();
}

RestDataSerializer* RestClient::getDataSerializer() const {
  Q_D(const detail::RestClient);
  return d->getDataSerializer();
}

RestReplyDecoratorManager* RestClient::getReplyDecoratorManager() const {
  Q_D(const detail::RestClient);
  return d->getReplyDecoratorManager();
}

/* ---------------------------- RestClientPrivate --------------------------- */

namespace detail {

RestClientPrivate::RestClientPrivate(
    const QUrl& url, const QVersionNumber& version, const RestHeaders& headers,
    const QUrlQuery& parameters, RestDataSerializer* data_serializer,
    RestReplyDecoratorManager* reply_decorator_manager)
    : m_base_url(url),
      m_version(version),
      m_headers(headers),
      m_parameters(parameters),
      m_data_serializer(data_serializer),
      m_reply_decorator_manager(reply_decorator_manager) {}

void RestClientPrivate::setBaseUrl(const QUrl& url) { m_base_url = url; }

QUrl RestClientPrivate::getBaseUrl() const { return m_base_url; }

void RestClientPrivate::setVersion(const QVersionNumber& version) {
  m_version = version;
}

QVersionNumber RestClientPrivate::getVersion() const { return m_version; }

void RestClientPrivate::setGlobalHeaders(const RestHeaders& headers) {
  m_headers = headers;
}

RestHeaders RestClientPrivate::getGlobalHeaders() const { return m_headers; }

void RestClientPrivate::setGlobalParameters(const QUrlQuery& parameters) {
  m_parameters = parameters;
}

QUrlQuery RestClientPrivate::getGlobalParameters() const {
  return m_parameters;
}

RestRequestBuilder RestClientPrivate::getRequestBuilder() const {
  return RestRequestBuilder{}
      .setBaseUrl(m_base_url)
      .addParameters(m_parameters)
      .addHeaders(m_headers);
}

QNetworkAccessManager* RestClientPrivate::getNetworkAccessManager() const {
  return m_manager;
}

RestDataSerializer* RestClientPrivate::getDataSerializer() const {
  return m_data_serializer;
}

RestReplyDecoratorManager* RestClientPrivate::getReplyDecoratorManager() const {
  return m_reply_decorator_manager;
}

}  // namespace detail

}  // namespace egnite::rest

#include "egnite/rest/moc_rest_client.cpp"
