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

const QByteArray IApi::GetVerb = QByteArray{"GET"};
const QByteArray IApi::PostVerb = QByteArray{"POST"};
const QByteArray IApi::PutVerb = QByteArray{"PUT"};
const QByteArray IApi::PatchVerb = QByteArray{"PATCH"};
const QByteArray IApi::DeleteVerb = QByteArray{"DELETE"};
const QByteArray IApi::HeadVerb = QByteArray{"HEAD"};

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

IPagingDataFactory* Api::getPagingDataFactory() const {
  Q_D(const detail::Api);
  return d->getPagingDataFactory();
}

void Api::setGlobalHeaders(const Headers& headers) {
  Q_D(detail::Api);
  d->setGlobalHeaders(headers);
}

Headers Api::getGlobalHeaders() const {
  Q_D(const detail::Api);
  return d->getGlobalHeaders();
}

void Api::setGlobalParameters(const QUrlQuery& parameters) {
  Q_D(detail::Api);
  d->setGlobalParameters(parameters);
}

QUrlQuery Api::getGlobalParameters() const {
  Q_D(const detail::Api);
  return d->getGlobalParameters();
}

DataSerializer::Format Api::getRequestDataFormat(const Headers& headers) const {
  Q_D(const detail::Api);
  return d->getRequestDataFormat(headers);
}

IApi* Api::createSubApi(const QString& path, QObject* parent) {
  Q_D(const detail::Api);
  return d->getClient()->createApi(QString("%1/%2").arg(d->getPath(), path),
                                   parent);
}

IReply* Api::callRaw(const QByteArray& verb, const QString& path,
                     const QUrlQuery& parameters, const Headers& headers,
                     QObject* parent) const {
  Q_D(const detail::Api);
  auto reply = new Reply(const_cast<Api*>(this),
                         d->create(verb, path, parameters, headers), parent);

  return getReplyDecorator()->decorate(reply);
}

IReply* Api::callRaw(const QByteArray& verb, const QString& path,
                     const QJsonValue& data, const QUrlQuery& parameters,
                     const Headers& headers, QObject* parent) const {
  Q_D(const detail::Api);
  auto reply =
      new Reply(const_cast<Api*>(this),
                d->create(verb, path, data, parameters, headers), parent);

  return getReplyDecorator()->decorate(reply);
}

IReply* Api::callRaw(const QByteArray& verb, const QString& path,
                     const QCborValue& data, const QUrlQuery& parameters,
                     const Headers& headers, QObject* parent) const {
  Q_D(const detail::Api);
  auto reply =
      new Reply(const_cast<Api*>(this),
                d->create(verb, path, data, parameters, headers), parent);

  return getReplyDecorator()->decorate(reply);
}

IReply* Api::callRaw(const QByteArray& verb, const QUrl& relative_url,
                     const QUrlQuery& parameters, const Headers& headers,
                     QObject* parent) const {
  Q_D(const detail::Api);
  auto reply =
      new Reply(const_cast<Api*>(this),
                d->create(verb, relative_url, parameters, headers), parent);

  return getReplyDecorator()->decorate(reply);
}

IReply* Api::callRaw(const QByteArray& verb, const QUrl& relative_url,
                     const QJsonValue& data, const QUrlQuery& parameters,
                     const Headers& headers, QObject* parent) const {
  Q_D(const detail::Api);
  auto reply = new Reply(
      const_cast<Api*>(this),
      d->create(verb, relative_url, data, parameters, headers), parent);

  return getReplyDecorator()->decorate(reply);
}

IReply* Api::callRaw(const QByteArray& verb, const QUrl& relative_url,
                     const QCborValue& data, const QUrlQuery& parameters,
                     const Headers& headers, QObject* parent) const {
  Q_D(const detail::Api);
  auto reply = new Reply(
      const_cast<Api*>(this),
      d->create(verb, relative_url, data, parameters, headers), parent);

  return getReplyDecorator()->decorate(reply);
}

/* ------------------------------- ApiPrivate ------------------------------- */

namespace detail {
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

IPagingDataFactory* ApiPrivate::getPagingDataFactory() const {
  return m_client->getPagingDataFactory();
}

void ApiPrivate::setGlobalHeaders(const Headers& headers) {
  Q_Q(Api);
  if (m_headers == headers) return;

  m_headers = headers;
  Q_EMIT q->globalHeadersChanged(m_headers);
}

Headers ApiPrivate::getGlobalHeaders() const { return m_headers; }

void ApiPrivate::setGlobalParameters(const QUrlQuery& parameters) {
  Q_Q(Api);
  if (m_parameters == parameters) return;

  m_parameters = parameters;
  Q_EMIT q->globalParametersChanged(m_parameters);
}

QUrlQuery ApiPrivate::getGlobalParameters() const { return m_parameters; }

QString ApiPrivate::getPath() const { return m_path; }

[[nodiscard]] DataSerializer::Format ApiPrivate::getRequestDataFormat(
    const Headers& headers) const {
  auto request = getRequestBuilder().addHeaders(headers).build();
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
  auto request = getRequestBuilder()
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
      getRequestBuilder()
          .addPath(path)
          .addParameters(parameters)
          .addHeaders(headers)
          .addHeader(ReplyPrivate::ContentType, ReplyPrivate::ContentTypeJson)
          .addHeader(ReplyPrivate::Accept, ReplyPrivate::ContentTypeJson)
          .build();

  return ReplyPrivate::send(m_manager, request, verb,
                            convertDataToByteArray(data));
}

QNetworkReply* ApiPrivate::create(const QByteArray& verb, const QString& path,
                                  const QCborValue& data,
                                  const QUrlQuery& parameters,
                                  const Headers& headers) const {
  auto request =
      getRequestBuilder()
          .addPath(path)
          .addParameters(parameters)
          .addHeaders(headers)
          .addHeader(ReplyPrivate::ContentType, ReplyPrivate::ContentTypeCbor)
          .addHeader(ReplyPrivate::Accept, ReplyPrivate::ContentTypeCbor)
          .build();

  return ReplyPrivate::send(m_manager, request, verb,
                            convertDataToByteArray(data));
}

QNetworkReply* ApiPrivate::create(const QByteArray& verb,
                                  const QUrl& relative_url,
                                  const QUrlQuery& parameters,
                                  const Headers& headers) const {
  auto request = getRequestBuilder()
                     .updateFromRelativeUrl(relative_url, true)
                     .addParameters(parameters)
                     .addHeaders(headers)
                     .build();

  return ReplyPrivate::send(m_manager, request, verb, {});
}

QNetworkReply* ApiPrivate::create(const QByteArray& verb,
                                  const QUrl& relative_url,
                                  const QJsonValue& data,
                                  const QUrlQuery& parameters,
                                  const Headers& headers) const {
  auto request =
      getRequestBuilder()
          .updateFromRelativeUrl(relative_url, true)
          .addParameters(parameters)
          .addHeaders(headers)
          .addHeader(ReplyPrivate::ContentType, ReplyPrivate::ContentTypeJson)
          .addHeader(ReplyPrivate::Accept, ReplyPrivate::ContentTypeJson)
          .build();

  return ReplyPrivate::send(m_manager, request, verb,
                            convertDataToByteArray(data));
}

QNetworkReply* ApiPrivate::create(const QByteArray& verb,
                                  const QUrl& relative_url,
                                  const QCborValue& data,
                                  const QUrlQuery& parameters,
                                  const Headers& headers) const {
  auto request =
      getRequestBuilder()
          .updateFromRelativeUrl(relative_url, true)
          .addParameters(parameters)
          .addHeaders(headers)
          .addHeader(ReplyPrivate::ContentType, ReplyPrivate::ContentTypeCbor)
          .addHeader(ReplyPrivate::Accept, ReplyPrivate::ContentTypeCbor)
          .build();

  return ReplyPrivate::send(m_manager, request, verb,
                            convertDataToByteArray(data));
}

RequestBuilder ApiPrivate::getRequestBuilder() const {
  return m_client->getRequestBuilder()
      .addParameters(m_client->getGlobalParameters())
      .addHeaders(m_client->getGlobalHeaders())
      .addPath(m_path)
      .addParameters(m_parameters)
      .addHeaders(m_headers);
}

}  // namespace detail

}  // namespace egnite::rest

#include "egnite/rest/moc_api.cpp"