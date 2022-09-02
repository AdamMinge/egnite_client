/* ----------------------------------- Local -------------------------------- */
#include "egnite/rest/rest_api.h"

#include "egnite/rest/detail/rest_api_p.h"
#include "egnite/rest/detail/rest_reply_p.h"
#include "egnite/rest/rest_client.h"
#include "egnite/rest/rest_reply_decorator_manager.h"
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

RestDataSerializer* RestApi::getDataSerializer() const {
  Q_D(const detail::RestApi);
  return d->getDataSerializer();
}

RestReplyDecoratorManager* RestApi::getReplyDecoratorManager() const {
  Q_D(const detail::RestApi);
  return d->getReplyDecoratorManager();
}

RestApi* RestApi::createSubApi(const QString& path, QObject* parent) {
  Q_D(const detail::RestApi);
  return d->getClient()->createApi(QString("%1/%2").arg(d->getPath(), path),
                                   parent);
}

RestReply* RestApi::get(const QString& path, const QUrlQuery& parameters,
                        const RestHeaders& headers, QObject* parent) const {
  Q_D(const detail::RestApi);
  return new RawRestReply(
      const_cast<RestApi*>(this),
      d->create(detail::RestApiPrivate::GetVerb, path, parameters, headers),
      parent);
}

RestReply* RestApi::head(const QString& path, const QUrlQuery& parameters,
                         const RestHeaders& headers, QObject* parent) const {
  Q_D(const detail::RestApi);
  return new RawRestReply(
      const_cast<RestApi*>(this),
      d->create(detail::RestApiPrivate::HeadVerb, path, parameters, headers),
      parent);
}

RestReply* RestApi::deleteResource(const QString& path,
                                   const QUrlQuery& parameters,
                                   const RestHeaders& headers,
                                   QObject* parent) const {
  Q_D(const detail::RestApi);
  return new RawRestReply(
      const_cast<RestApi*>(this),
      d->create(detail::RestApiPrivate::DeleteVerb, path, parameters, headers),
      parent);
}

RestReply* RestApi::post(const QString& path, const QUrlQuery& parameters,
                         const RestHeaders& headers, QObject* parent) const {
  Q_D(const detail::RestApi);
  return new RawRestReply(
      const_cast<RestApi*>(this),
      d->create(detail::RestApiPrivate::PostVerb, path, parameters, headers),
      parent);
}

RestReply* RestApi::post(const QString& path, const QJsonValue& data,
                         const QUrlQuery& parameters,
                         const RestHeaders& headers, QObject* parent) const {
  Q_D(const detail::RestApi);
  return new RawRestReply(const_cast<RestApi*>(this),
                          d->create(detail::RestApiPrivate::PostVerb, path,
                                    data, parameters, headers),
                          parent);
}

RestReply* RestApi::post(const QString& path, const QCborValue& data,
                         const QUrlQuery& parameters,
                         const RestHeaders& headers, QObject* parent) const {
  Q_D(const detail::RestApi);
  return new RawRestReply(const_cast<RestApi*>(this),
                          d->create(detail::RestApiPrivate::PostVerb, path,
                                    data, parameters, headers),
                          parent);
}

RestReply* RestApi::put(const QString& path, const QUrlQuery& parameters,
                        const RestHeaders& headers, QObject* parent) const {
  Q_D(const detail::RestApi);
  return new RawRestReply(
      const_cast<RestApi*>(this),
      d->create(detail::RestApiPrivate::PutVerb, path, parameters, headers),
      parent);
}

RestReply* RestApi::put(const QString& path, const QJsonValue& data,
                        const QUrlQuery& parameters, const RestHeaders& headers,
                        QObject* parent) const {
  Q_D(const detail::RestApi);
  return new RawRestReply(const_cast<RestApi*>(this),
                          d->create(detail::RestApiPrivate::PutVerb, path, data,
                                    parameters, headers),
                          parent);
}

RestReply* RestApi::put(const QString& path, const QCborValue& data,
                        const QUrlQuery& parameters, const RestHeaders& headers,
                        QObject* parent) const {
  Q_D(const detail::RestApi);
  return new RawRestReply(const_cast<RestApi*>(this),
                          d->create(detail::RestApiPrivate::PutVerb, path, data,
                                    parameters, headers),
                          parent);
}

RestReply* RestApi::patch(const QString& path, const QUrlQuery& parameters,
                          const RestHeaders& headers, QObject* parent) const {
  Q_D(const detail::RestApi);
  return new RawRestReply(
      const_cast<RestApi*>(this),
      d->create(detail::RestApiPrivate::PatchVerb, path, parameters, headers),
      parent);
}

RestReply* RestApi::patch(const QString& path, const QJsonValue& data,
                          const QUrlQuery& parameters,
                          const RestHeaders& headers, QObject* parent) const {
  Q_D(const detail::RestApi);
  return new RawRestReply(const_cast<RestApi*>(this),
                          d->create(detail::RestApiPrivate::PatchVerb, path,
                                    data, parameters, headers),
                          parent);
}

RestReply* RestApi::patch(const QString& path, const QCborValue& data,
                          const QUrlQuery& parameters,
                          const RestHeaders& headers, QObject* parent) const {
  Q_D(const detail::RestApi);
  return new RawRestReply(const_cast<RestApi*>(this),
                          d->create(detail::RestApiPrivate::PatchVerb, path,
                                    data, parameters, headers),
                          parent);
}

RestDataSerializer::Format RestApi::getRequestDataFormat(
    const RestHeaders& headers) const {
  Q_D(const detail::RestApi);
  return d->getRequestDataFormat(headers);
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

RestDataSerializer* RestApiPrivate::getDataSerializer() const {
  return m_client->getDataSerializer();
}

RestReplyDecoratorManager* RestApiPrivate::getReplyDecoratorManager() const {
  return m_client->getReplyDecoratorManager();
}

QString RestApiPrivate::getPath() const { return m_path; }

[[nodiscard]] RestDataSerializer::Format RestApiPrivate::getRequestDataFormat(
    const RestHeaders& headers) const {
  auto request = m_client->getRequestBuilder().addHeaders(headers).build();
  auto content_type = request.header(QNetworkRequest::ContentTypeHeader)
                          .toByteArray()
                          .trimmed();

  if (const auto content_types = content_type.split(';');
      content_types.size() > 1) {
    content_type = content_types.first().trimmed();

    if (content_type == RawRestReplyPrivate::ContentTypeJson)
      return RestDataSerializer::Format::Json;
    if (content_type == RawRestReplyPrivate::ContentTypeCbor)
      return RestDataSerializer::Format::Cbor;
  }

  return RestDataSerializer::Format::Json;
}

QNetworkReply* RestApiPrivate::create(const QByteArray& verb,
                                      const QString& path,
                                      const QUrlQuery& parameters,
                                      const RestHeaders& headers) const {
  auto request = m_client->getRequestBuilder()
                     .addPath(path)
                     .addParameters(parameters)
                     .addHeaders(headers)
                     .build();

  return RawRestReplyPrivate::send(m_manager, request, verb, {});
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
                     .addHeader(RawRestReplyPrivate::ContentType,
                                RawRestReplyPrivate::ContentTypeJson)
                     .addHeader(RawRestReplyPrivate::Accept,
                                RawRestReplyPrivate::ContentTypeJson)
                     .build();

  return RawRestReplyPrivate::send(m_manager, request, verb, convertData(data));
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
                     .addHeader(RawRestReplyPrivate::ContentType,
                                RawRestReplyPrivate::ContentTypeCbor)
                     .addHeader(RawRestReplyPrivate::Accept,
                                RawRestReplyPrivate::ContentTypeCbor)
                     .build();

  return RawRestReplyPrivate::send(m_manager, request, verb, convertData(data));
}

QByteArray RestApiPrivate::convertData(const RestData& body) {
  return std::visit(
      utils::overloaded{
          [&](std::nullopt_t) -> QByteArray { return QByteArray{}; },
          [&](const QJsonValue& body) -> QByteArray {
            return RestApiPrivate::convertData(body);
          },
          [&](const QCborValue& body) -> QByteArray {
            return RestApiPrivate::convertData(body);
          }},
      body);
}

QByteArray RestApiPrivate::convertData(const QJsonValue& body) {
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

QByteArray RestApiPrivate::convertData(const QCborValue& body) {
  return body.toCbor();
}

}  // namespace detail

}  // namespace egnite::rest

#include "egnite/rest/moc_rest_api.cpp"