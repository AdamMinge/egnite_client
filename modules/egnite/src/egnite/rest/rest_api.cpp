/* ----------------------------------- Local -------------------------------- */
#include "egnite/rest/rest_api.h"

#include "egnite/rest/detail/rest_api_p.h"
#include "egnite/rest/detail/rest_reply_p.h"
#include "egnite/rest/rest_client.h"
#include "egnite/rest/rest_reply.h"
/* ------------------------------------- Qt --------------------------------- */
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
/* -------------------------------------------------------------------------- */

namespace egnite::rest {

/* -------------------------------- RestApi --------------------------------- */

RestApi::RestApi(RestClient* client, QNetworkAccessManager* manager,
                 const QString& subpath, QObject* parent)
    : RestApi(*new detail::RestApiPrivate(client, manager, subpath), parent) {}

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
                        const RestHeaders& headers) const {
  Q_D(const detail::RestApi);
  return new RestReply(
      const_cast<RestApi*>(this),
      d->create(detail::RestApiPrivate::GetVerb, path, parameters, headers));
}

RestReply* RestApi::head(const QString& path, const QUrlQuery& parameters,
                         const RestHeaders& headers) const {
  Q_D(const detail::RestApi);
  return new RestReply(
      const_cast<RestApi*>(this),
      d->create(detail::RestApiPrivate::HeadVerb, path, parameters, headers));
}

RestReply* RestApi::deleteResource(const QString& path,
                                   const QUrlQuery& parameters,
                                   const RestHeaders& headers) const {
  Q_D(const detail::RestApi);
  return new RestReply(
      const_cast<RestApi*>(this),
      d->create(detail::RestApiPrivate::DeleteVerb, path, parameters, headers));
}

RestReply* RestApi::post(const QString& path, const QUrlQuery& parameters,
                         const RestHeaders& headers) const {
  Q_D(const detail::RestApi);
  return new RestReply(
      const_cast<RestApi*>(this),
      d->create(detail::RestApiPrivate::PostVerb, path, parameters, headers));
}

RestReply* RestApi::post(const QString& path, const QJsonValue& data,
                         const QUrlQuery& parameters,
                         const RestHeaders& headers) const {
  Q_D(const detail::RestApi);
  return new RestReply(const_cast<RestApi*>(this),
                       d->create(detail::RestApiPrivate::PostVerb, path, data,
                                 parameters, headers));
}

RestReply* RestApi::post(const QString& path, const QCborValue& data,
                         const QUrlQuery& parameters,
                         const RestHeaders& headers) const {
  Q_D(const detail::RestApi);
  return new RestReply(const_cast<RestApi*>(this),
                       d->create(detail::RestApiPrivate::PostVerb, path, data,
                                 parameters, headers));
}

RestReply* RestApi::put(const QString& path, const QUrlQuery& parameters,
                        const RestHeaders& headers) const {
  Q_D(const detail::RestApi);
  return new RestReply(
      const_cast<RestApi*>(this),
      d->create(detail::RestApiPrivate::PutVerb, path, parameters, headers));
}

RestReply* RestApi::put(const QString& path, const QJsonValue& data,
                        const QUrlQuery& parameters,
                        const RestHeaders& headers) const {
  Q_D(const detail::RestApi);
  return new RestReply(const_cast<RestApi*>(this),
                       d->create(detail::RestApiPrivate::PutVerb, path, data,
                                 parameters, headers));
}

RestReply* RestApi::put(const QString& path, const QCborValue& data,
                        const QUrlQuery& parameters,
                        const RestHeaders& headers) const {
  Q_D(const detail::RestApi);
  return new RestReply(const_cast<RestApi*>(this),
                       d->create(detail::RestApiPrivate::PutVerb, path, data,
                                 parameters, headers));
}

RestReply* RestApi::patch(const QString& path, const QUrlQuery& parameters,
                          const RestHeaders& headers) const {
  Q_D(const detail::RestApi);
  return new RestReply(
      const_cast<RestApi*>(this),
      d->create(detail::RestApiPrivate::PatchVerb, path, parameters, headers));
}

RestReply* RestApi::patch(const QString& path, const QJsonValue& data,
                          const QUrlQuery& parameters,
                          const RestHeaders& headers) const {
  Q_D(const detail::RestApi);
  return new RestReply(const_cast<RestApi*>(this),
                       d->create(detail::RestApiPrivate::PatchVerb, path, data,
                                 parameters, headers));
}

RestReply* RestApi::patch(const QString& path, const QCborValue& data,
                          const QUrlQuery& parameters,
                          const RestHeaders& headers) const {
  Q_D(const detail::RestApi);
  return new RestReply(const_cast<RestApi*>(this),
                       d->create(detail::RestApiPrivate::PatchVerb, path, data,
                                 parameters, headers));
}

/* ----------------------------- RestApiPrivate ----------------------------- */

namespace detail {

const QByteArray RestApiPrivate::GetVerb = QByteArray{"GET"};
const QByteArray RestApiPrivate::PostVerb = QByteArray{"POST"};
const QByteArray RestApiPrivate::PutVerb = QByteArray{"PUT"};
const QByteArray RestApiPrivate::PatchVerb = QByteArray{"PATCH"};
const QByteArray RestApiPrivate::DeleteVerb = QByteArray{"DELETE"};
const QByteArray RestApiPrivate::HeadVerb = QByteArray{"HEAD"};

RestApiPrivate::RestApiPrivate(RestClient* client,
                               QNetworkAccessManager* manager,
                               const QString& path)
    : m_client(client), m_manager(manager), m_path(path) {}

RestClient* RestApiPrivate::getClient() const { return m_client; }

QString RestApiPrivate::getPath() const { return m_path; }

QNetworkReply* RestApiPrivate::create(const QByteArray& verb,
                                      const QString& path,
                                      const QUrlQuery& parameters,
                                      const RestHeaders& headers) const {
  auto request = m_client->getRequestBuilder()
                     .addPath(path)
                     .addParameters(parameters)
                     .addHeaders(headers)
                     .build();

  return RestReplyPrivate::send(m_manager, request, verb, {});
}

QNetworkReply* RestApiPrivate::create(const QByteArray& verb,
                                      const QString& path,
                                      const QJsonValue& data,
                                      const QUrlQuery& parameters,
                                      const RestHeaders& headers) const {
  auto request = m_client->getRequestBuilder()
                     .addPath(path)
                     .addParameters(parameters)
                     .addHeaders(headers)
                     .addHeader(RestReplyPrivate::ContentType,
                                RestReplyPrivate::ContentTypeJson)
                     .addHeader(RestReplyPrivate::Accept,
                                RestReplyPrivate::ContentTypeJson)
                     .build();

  return RestReplyPrivate::send(m_manager, request, verb, convertData(data));
}

QNetworkReply* RestApiPrivate::create(const QByteArray& verb,
                                      const QString& path,
                                      const QCborValue& data,
                                      const QUrlQuery& parameters,
                                      const RestHeaders& headers) const {
  auto request = m_client->getRequestBuilder()
                     .addPath(path)
                     .addParameters(parameters)
                     .addHeaders(headers)
                     .addHeader(RestReplyPrivate::ContentType,
                                RestReplyPrivate::ContentTypeCbor)
                     .addHeader(RestReplyPrivate::Accept,
                                RestReplyPrivate::ContentTypeJson)
                     .build();

  return RestReplyPrivate::send(m_manager, request, verb, convertData(data));
}

QByteArray RestApiPrivate::convertData(const QJsonValue& body) const {
  switch (body.type()) {
    case QJsonValue::Array:
      return QJsonDocument(body.toArray()).toJson(QJsonDocument::Compact);
    case QJsonValue::Object:
      return QJsonDocument(body.toArray()).toJson(QJsonDocument::Compact);
    default:
      auto converted =
          QJsonDocument(QJsonArray{body}).toJson(QJsonDocument::Compact);
      converted = converted.mid(1, converted.size() - 2);
      return converted;
  }
}

QByteArray RestApiPrivate::convertData(const QCborValue& body) const {
  return body.toCbor();
}

}  // namespace detail

}  // namespace egnite::rest

#include "egnite/rest/moc_rest_api.cpp"