/* ----------------------------------- Local -------------------------------- */
#include "egnite/rest/rest_api.h"

#include "egnite/rest/detail/rest_api_p.h"
#include "egnite/rest/rest_client.h"
/* -------------------------------------------------------------------------- */

namespace egnite::rest {

/* -------------------------------- RestApi --------------------------------- */

RestApi::RestApi(RestClient* client, const QString& subpath, QObject* parent)
    : RestApi(*new detail::RestApiPrivate(client, subpath), parent) {}

RestApi::RestApi(detail::RestApiPrivate& impl, QObject* parent)
    : QObject(impl, parent) {}

RestApi::~RestApi() = default;

RestClient* RestApi::getClient() const {
  Q_D(const detail::RestApi);
  return d->getClient();
}

RestApi* RestApi::createSubApi(const QString& path, QObject* parent) {
  Q_D(const detail::RestApi);
  return d->getClient()->createApi(QString("%1/%2").arg(d->getPath(), path),
                                   parent);
}

RestReply* RestApi::get(const QString& path, const QUrlQuery& parameters,
                        const RestClient::Headers& headers) const {
  Q_D(const detail::RestApi);
  return new RestReply(
      d->create(detail::RestApiPrivate::GetVerb, path, parameters, headers),
      const_cast<RestApi*>(this));
}

RestReply* RestApi::head(const QString& path, const QUrlQuery& parameters,
                         const RestClient::Headers& headers) const {
  Q_D(const detail::RestApi);
  return new RestReply(
      d->create(detail::RestApiPrivate::HeadVerb, path, parameters, headers),
      const_cast<RestApi*>(this));
}

RestReply* RestApi::deleteResource(const QString& path,
                                   const QUrlQuery& parameters,
                                   const RestClient::Headers& headers) const {
  Q_D(const detail::RestApi);
  return new RestReply(
      d->create(detail::RestApiPrivate::DeleteVerb, path, parameters, headers),
      const_cast<RestApi*>(this));
}

RestReply* RestApi::post(const QString& path, const QUrlQuery& parameters,
                         const RestClient::Headers& headers) const {
  Q_D(const detail::RestApi);
  return new RestReply(
      d->create(detail::RestApiPrivate::PostVerb, path, parameters, headers),
      const_cast<RestApi*>(this));
}

RestReply* RestApi::post(const QString& path, const QJsonValue& data,
                         const QUrlQuery& parameters,
                         const RestClient::Headers& headers) const {
  Q_D(const detail::RestApi);
  return new RestReply(d->create(detail::RestApiPrivate::PostVerb, path, data,
                                 parameters, headers),
                       const_cast<RestApi*>(this));
}

RestReply* RestApi::post(const QString& path, const QCborValue& data,
                         const QUrlQuery& parameters,
                         const RestClient::Headers& headers) const {
  Q_D(const detail::RestApi);
  return new RestReply(d->create(detail::RestApiPrivate::PostVerb, path, data,
                                 parameters, headers),
                       const_cast<RestApi*>(this));
}

RestReply* RestApi::put(const QString& path, const QUrlQuery& parameters,
                        const RestClient::Headers& headers) const {
  Q_D(const detail::RestApi);
  return new RestReply(
      d->create(detail::RestApiPrivate::PutVerb, path, parameters, headers),
      const_cast<RestApi*>(this));
}

RestReply* RestApi::put(const QString& path, const QJsonValue& data,
                        const QUrlQuery& parameters,
                        const RestClient::Headers& headers) const {
  Q_D(const detail::RestApi);
  return new RestReply(d->create(detail::RestApiPrivate::PutVerb, path, data,
                                 parameters, headers),
                       const_cast<RestApi*>(this));
}

RestReply* RestApi::put(const QString& path, const QCborValue& data,
                        const QUrlQuery& parameters,
                        const RestClient::Headers& headers) const {
  Q_D(const detail::RestApi);
  return new RestReply(d->create(detail::RestApiPrivate::PutVerb, path, data,
                                 parameters, headers),
                       const_cast<RestApi*>(this));
}

RestReply* RestApi::patch(const QString& path, const QUrlQuery& parameters,
                          const RestClient::Headers& headers) const {
  Q_D(const detail::RestApi);
  return new RestReply(
      d->create(detail::RestApiPrivate::PatchVerb, path, parameters, headers),
      const_cast<RestApi*>(this));
}

RestReply* RestApi::patch(const QString& path, const QJsonValue& data,
                          const QUrlQuery& parameters,
                          const RestClient::Headers& headers) const {
  Q_D(const detail::RestApi);
  return new RestReply(d->create(detail::RestApiPrivate::PatchVerb, path, data,
                                 parameters, headers),
                       const_cast<RestApi*>(this));
}

RestReply* RestApi::patch(const QString& path, const QCborValue& data,
                          const QUrlQuery& parameters,
                          const RestClient::Headers& headers) const {
  Q_D(const detail::RestApi);
  return new RestReply(d->create(detail::RestApiPrivate::PatchVerb, path, data,
                                 parameters, headers),
                       const_cast<RestApi*>(this));
}

/* ----------------------------- RestApiPrivate ----------------------------- */

namespace detail {

const QByteArray RestApiPrivate::GetVerb = QByteArray{"GET"};
const QByteArray RestApiPrivate::PostVerb = QByteArray{"POST"};
const QByteArray RestApiPrivate::PutVerb = QByteArray{"PUT"};
const QByteArray RestApiPrivate::PatchVerb = QByteArray{"PATCH"};
const QByteArray RestApiPrivate::DeleteVerb = QByteArray{"DELETE"};
const QByteArray RestApiPrivate::HeadVerb = QByteArray{"HEAD"};

RestApiPrivate::RestApiPrivate(RestClient* client, const QString& path)
    : m_client(client), m_path(path) {}

RestClient* RestApiPrivate::getClient() const { return m_client; }

QString RestApiPrivate::getPath() const { return m_path; }

QNetworkReply* RestApiPrivate::create(
    const QByteArray& verb, const QString& path, const QUrlQuery& parameters,
    const RestClient::Headers& headers) const {}

QNetworkReply* RestApiPrivate::create(
    const QByteArray& verb, const QString& path, const QJsonValue& data,
    const QUrlQuery& parameters, const RestClient::Headers& headers) const {}

QNetworkReply* RestApiPrivate::create(
    const QByteArray& verb, const QString& path, const QCborValue& data,
    const QUrlQuery& parameters, const RestClient::Headers& headers) const {}

}  // namespace detail

}  // namespace egnite::rest

#include "egnite/rest/moc_rest_api.cpp"