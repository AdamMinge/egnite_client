/* ----------------------------------- Local -------------------------------- */
#include "egnite/rest/rest_client.h"

#include "egnite/rest/detail/rest_client_p.h"
#include "egnite/rest/rest_api.h"
/* -------------------------------------------------------------------------- */

namespace egnite::rest {

/* ------------------------------- RestClient ------------------------------- */

RestClient::RestClient(QObject* parent)
    : RestClient(
          *new detail::RestClientPrivate(QUrl{}, QVersionNumber{},
                                         RestClient::Headers{}, QUrlQuery{}),
          parent) {}

RestClient::RestClient(detail::RestClientPrivate& impl, QObject* parent)
    : QObject(impl, parent) {}

RestClient::~RestClient() = default;

RestApi* RestClient::createApi(const QString& path, QObject* parent) {
  return new RestApi(this, path, parent);
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

void RestClient::setGlobalHeaders(const Headers& headers) {
  Q_D(detail::RestClient);
  if (d->getGlobalHeaders() == headers) return;

  d->setGlobalHeaders(headers);
  Q_EMIT globalHeadersChanged(headers);
}

RestClient::Headers RestClient::getGlobalHeaders() const {
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

/* ---------------------------- RestClientPrivate --------------------------- */

namespace detail {

RestClientPrivate::RestClientPrivate(const QUrl& url,
                                     const QVersionNumber& version,
                                     const RestClient::Headers& headers,
                                     const QUrlQuery& parameters)
    : m_base_url(url),
      m_version(version),
      m_headers(headers),
      m_parameters(parameters) {}

void RestClientPrivate::setBaseUrl(const QUrl& url) { m_base_url = url; }

QUrl RestClientPrivate::getBaseUrl() const { return m_base_url; }

void RestClientPrivate::setVersion(const QVersionNumber& version) {
  m_version = version;
}

QVersionNumber RestClientPrivate::getVersion() const { return m_version; }

void RestClientPrivate::setGlobalHeaders(const RestClient::Headers& headers) {
  m_headers = headers;
}

RestClient::Headers RestClientPrivate::getGlobalHeaders() const {
  return m_headers;
}

void RestClientPrivate::setGlobalParameters(const QUrlQuery& parameters) {
  m_parameters = parameters;
}

QUrlQuery RestClientPrivate::getGlobalParameters() const {
  return m_parameters;
}

}  // namespace detail

}  // namespace egnite::rest

#include "egnite/rest/moc_rest_client.cpp"
