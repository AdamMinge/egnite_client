/* ----------------------------------- Local -------------------------------- */
#include "egnite/rest/rest_client.h"

#include "egnite/rest/private/rest_client_p.h"
#include "egnite/rest/rest_api.h"
/* -------------------------------------------------------------------------- */

namespace egnite::rest {

/* ------------------------------- RestClient ------------------------------- */

RestClient::RestClient(QObject* parent)
    : RestClient(*new RestClientPrivate(QUrl{}, QVersionNumber{}), parent) {}

RestClient::RestClient(RestClientPrivate& impl, QObject* parent)
    : QObject(impl, parent) {}

RestClient::~RestClient() = default;

RestApi* RestClient::createApi(const QString& path) {
  return new RestApi(this, path);
}

void RestClient::setBaseUrl(const QUrl& url) {
  Q_D(RestClient);
  if (d->base_url == url) return;

  d->base_url = url;
  Q_EMIT baseUrlChanged(d->base_url);
}

QUrl RestClient::getBaseUrl() const {
  Q_D(const RestClient);
  return d->base_url;
}

void RestClient::setVersion(const QVersionNumber& version) {
  Q_D(RestClient);
  if (d->version == version) return;

  d->version = version;
  Q_EMIT versionChanged(d->version);
}

QVersionNumber RestClient::getVersion() const {
  Q_D(const RestClient);
  return d->version;
}

/* ---------------------------- RestClientPrivate --------------------------- */

RestClientPrivate::RestClientPrivate(const QUrl& url,
                                     const QVersionNumber& version)
    : base_url(url), version(version) {}

}  // namespace egnite::rest
