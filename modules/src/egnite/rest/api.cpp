/* ----------------------------------- Local -------------------------------- */
#include "egnite/rest/api.h"

#include "egnite/rest/client.h"
#include "egnite/rest/detail/api_p.h"
#include "egnite/rest/detail/reply_p.h"
#include "egnite/rest/reply_decorator.h"
/* ------------------------------------- Qt --------------------------------- */
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
/* -------------------------------------------------------------------------- */

namespace egnite::rest {

/* ---------------------------------- IApi ---------------------------------- */

IApi::IApi(QObject* parent) : QObject(parent) {}

IApi::IApi(QObjectPrivate& impl, QObject* parent) : QObject(impl, parent) {}

IApi::~IApi() = default;

/* ---------------------------------- Api ----------------------------------- */

Api::Api(IClient* client, const QString& subpath, QObject* parent)
    : Api(*new detail::ApiPrivate(client, client->getNetworkAccessManager(),
                                  subpath),
          parent) {}

Api::Api(detail::ApiPrivate& impl, QObject* parent) : IApi(impl, parent) {}

Api::~Api() = default;

IClient* Api::getClient() const {
  Q_D(const detail::Api);
  return d->getClient();
}

IReplyDecorator* Api::getReplyDecorator() const {
  Q_D(const detail::Api);
  return d->getReplyDecorator();
}

DataSerializer* Api::getDataSerializer() const {
  Q_D(const detail::Api);
  return d->getDataSerializer();
}

IApi* Api::createSubApi(const QString& path, QObject* parent) {
  Q_D(const detail::Api);
  return d->getClient()->createApi(QString("%1/%2").arg(d->getPath(), path),
                                   parent);
}

IReply* Api::get(const QString& path, const QUrlQuery& parameters,
                 const Headers& headers, QObject* parent) const {
  Q_D(const detail::Api);
  auto reply = new Reply(
      const_cast<Api*>(this),
      d->create(detail::ApiPrivate::GetVerb, path, parameters, headers),
      parent);

  return getReplyDecorator()->decorate(reply);
}

IReply* Api::head(const QString& path, const QUrlQuery& parameters,
                  const Headers& headers, QObject* parent) const {
  Q_D(const detail::Api);
  auto reply = new Reply(
      const_cast<Api*>(this),
      d->create(detail::ApiPrivate::HeadVerb, path, parameters, headers),
      parent);

  return getReplyDecorator()->decorate(reply);
}

IReply* Api::deleteResource(const QString& path, const QUrlQuery& parameters,
                            const Headers& headers, QObject* parent) const {
  Q_D(const detail::Api);
  auto reply = new Reply(
      const_cast<Api*>(this),
      d->create(detail::ApiPrivate::DeleteVerb, path, parameters, headers),
      parent);

  return getReplyDecorator()->decorate(reply);
}

IReply* Api::post(const QString& path, const QUrlQuery& parameters,
                  const Headers& headers, QObject* parent) const {
  Q_D(const detail::Api);
  auto reply = new Reply(
      const_cast<Api*>(this),
      d->create(detail::ApiPrivate::PostVerb, path, parameters, headers),
      parent);

  return getReplyDecorator()->decorate(reply);
}

IReply* Api::post(const QString& path, const QJsonValue& data,
                  const QUrlQuery& parameters, const Headers& headers,
                  QObject* parent) const {
  Q_D(const detail::Api);
  auto reply = new Reply(
      const_cast<Api*>(this),
      d->create(detail::ApiPrivate::PostVerb, path, data, parameters, headers),
      parent);

  return getReplyDecorator()->decorate(reply);
}

IReply* Api::post(const QString& path, const QCborValue& data,
                  const QUrlQuery& parameters, const Headers& headers,
                  QObject* parent) const {
  Q_D(const detail::Api);
  auto reply = new Reply(
      const_cast<Api*>(this),
      d->create(detail::ApiPrivate::PostVerb, path, data, parameters, headers),
      parent);

  return getReplyDecorator()->decorate(reply);
}

IReply* Api::put(const QString& path, const QUrlQuery& parameters,
                 const Headers& headers, QObject* parent) const {
  Q_D(const detail::Api);
  auto reply = new Reply(
      const_cast<Api*>(this),
      d->create(detail::ApiPrivate::PutVerb, path, parameters, headers),
      parent);

  return getReplyDecorator()->decorate(reply);
}

IReply* Api::put(const QString& path, const QJsonValue& data,
                 const QUrlQuery& parameters, const Headers& headers,
                 QObject* parent) const {
  Q_D(const detail::Api);
  auto reply = new Reply(
      const_cast<Api*>(this),
      d->create(detail::ApiPrivate::PutVerb, path, data, parameters, headers),
      parent);

  return getReplyDecorator()->decorate(reply);
}

IReply* Api::put(const QString& path, const QCborValue& data,
                 const QUrlQuery& parameters, const Headers& headers,
                 QObject* parent) const {
  Q_D(const detail::Api);
  auto reply = new Reply(
      const_cast<Api*>(this),
      d->create(detail::ApiPrivate::PutVerb, path, data, parameters, headers),
      parent);

  return getReplyDecorator()->decorate(reply);
}

IReply* Api::patch(const QString& path, const QUrlQuery& parameters,
                   const Headers& headers, QObject* parent) const {
  Q_D(const detail::Api);
  auto reply = new Reply(
      const_cast<Api*>(this),
      d->create(detail::ApiPrivate::PatchVerb, path, parameters, headers),
      parent);

  return getReplyDecorator()->decorate(reply);
}

IReply* Api::patch(const QString& path, const QJsonValue& data,
                   const QUrlQuery& parameters, const Headers& headers,
                   QObject* parent) const {
  Q_D(const detail::Api);
  auto reply = new Reply(
      const_cast<Api*>(this),
      d->create(detail::ApiPrivate::PatchVerb, path, data, parameters, headers),
      parent);

  return getReplyDecorator()->decorate(reply);
}

IReply* Api::patch(const QString& path, const QCborValue& data,
                   const QUrlQuery& parameters, const Headers& headers,
                   QObject* parent) const {
  Q_D(const detail::Api);
  auto reply = new Reply(
      const_cast<Api*>(this),
      d->create(detail::ApiPrivate::PatchVerb, path, data, parameters, headers),
      parent);

  return getReplyDecorator()->decorate(reply);
}

DataSerializer::Format Api::getRequestDataFormat(const Headers& headers) const {
  Q_D(const detail::Api);
  return d->getRequestDataFormat(headers);
}

/* ------------------------------- ApiPrivate ------------------------------- */

namespace detail {

const QByteArray ApiPrivate::GetVerb = QByteArray{"GET"};
const QByteArray ApiPrivate::PostVerb = QByteArray{"POST"};
const QByteArray ApiPrivate::PutVerb = QByteArray{"PUT"};
const QByteArray ApiPrivate::PatchVerb = QByteArray{"PATCH"};
const QByteArray ApiPrivate::DeleteVerb = QByteArray{"DELETE"};
const QByteArray ApiPrivate::HeadVerb = QByteArray{"HEAD"};

ApiPrivate::ApiPrivate(IClient* client, QNetworkAccessManager* manager,
                       const QString& path)
    : m_client(client), m_manager(manager), m_path(path) {}

IClient* ApiPrivate::getClient() const { return m_client; }

IReplyDecorator* ApiPrivate::getReplyDecorator() const {
  return m_client->getReplyDecorator();
}

DataSerializer* ApiPrivate::getDataSerializer() const {
  return m_client->getDataSerializer();
}

QString ApiPrivate::getPath() const { return m_path; }

[[nodiscard]] DataSerializer::Format ApiPrivate::getRequestDataFormat(
    const Headers& headers) const {
  auto request = m_client->getRequestBuilder().addHeaders(headers).build();
  auto content_type = request.header(QNetworkRequest::ContentTypeHeader)
                          .toByteArray()
                          .trimmed();

  if (const auto content_types = content_type.split(';');
      content_types.size() > 1) {
    content_type = content_types.first().trimmed();

    if (content_type == ReplyPrivate::ContentTypeJson)
      return DataSerializer::Format::Json;
    if (content_type == ReplyPrivate::ContentTypeCbor)
      return DataSerializer::Format::Cbor;
  }

  return DataSerializer::Format::Json;
}

QNetworkReply* ApiPrivate::create(const QByteArray& verb, const QString& path,
                                  const QUrlQuery& parameters,
                                  const Headers& headers) const {
  auto request = m_client->getRequestBuilder()
                     .addPath(m_path)
                     .addPath(path)
                     .addParameters(parameters)
                     .addHeaders(headers)
                     .build();

  return ReplyPrivate::send(m_manager, request, verb, {});
}

QNetworkReply* ApiPrivate::create(const QByteArray& verb, const QString& path,
                                  const QJsonValue& data,
                                  const QUrlQuery& parameters,
                                  const Headers& headers) const {
  auto request =
      m_client->getRequestBuilder()
          .addPath(m_path)
          .addPath(path)
          .addParameters(parameters)
          .addHeaders(headers)
          .addHeader(ReplyPrivate::ContentType, ReplyPrivate::ContentTypeJson)
          .addHeader(ReplyPrivate::Accept, ReplyPrivate::ContentTypeJson)
          .build();

  return ReplyPrivate::send(m_manager, request, verb, convertData(data));
}

QNetworkReply* ApiPrivate::create(const QByteArray& verb, const QString& path,
                                  const QCborValue& data,
                                  const QUrlQuery& parameters,
                                  const Headers& headers) const {
  auto request =
      m_client->getRequestBuilder()
          .addPath(m_path)
          .addPath(path)
          .addParameters(parameters)
          .addHeaders(headers)
          .addHeader(ReplyPrivate::ContentType, ReplyPrivate::ContentTypeCbor)
          .addHeader(ReplyPrivate::Accept, ReplyPrivate::ContentTypeCbor)
          .build();

  return ReplyPrivate::send(m_manager, request, verb, convertData(data));
}

QByteArray ApiPrivate::convertData(const Data& body) {
  return std::visit(
      core::utils::overloaded{
          [](std::nullopt_t) -> QByteArray { return QByteArray{}; },
          [](const QJsonValue& body) -> QByteArray {
            return ApiPrivate::convertData(body);
          },
          [](const QCborValue& body) -> QByteArray {
            return ApiPrivate::convertData(body);
          }},
      body);
}

QByteArray ApiPrivate::convertData(const QJsonValue& body) {
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

QByteArray ApiPrivate::convertData(const QCborValue& body) {
  return body.toCbor();
}

}  // namespace detail

}  // namespace egnite::rest

#include "egnite/rest/moc_api.cpp"