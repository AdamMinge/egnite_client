/* ----------------------------------- Local -------------------------------- */
#include "egnite/rest/reply.h"

#include "egnite/rest/api.h"
#include "egnite/rest/client.h"
#include "egnite/rest/detail/reply_p.h"
/* ------------------------------------ Qt ---------------------------------- */
#include <QCborStreamReader>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
/* -------------------------------------------------------------------------- */

namespace egnite::rest {

/* ----------------------------------- IReply ------------------------------- */

IReply::IReply(QObject* parent) : QObject(parent) {}

IReply::IReply(QObjectPrivate& impl, QObject* parent) : QObject(impl, parent) {
  connect(this, &IReply::succeeded, this, &IReply::completed);
  connect(this, &IReply::failed, this, &IReply::completed);

  connect(this, &IReply::failed, this, &IReply::unsucceeded);
  connect(this, &IReply::error, this, &IReply::unsucceeded);
}

IReply::~IReply() = default;

/* ---------------------------------- Reply --------------------------------- */

Reply::Reply(IApi* api, QNetworkReply* network_reply, QObject* parent)
    : IReply(*new detail::ReplyPrivate(api, network_reply), parent) {
  Q_D(detail::Reply);
  d->connectReply();
}

Reply::~Reply() = default;

void Reply::abort() {
  Q_D(detail::Reply);
  d->abort();
}

void Reply::retry() {
  Q_D(detail::Reply);
  d->retry();
}

IApi* Reply::getApi() const {
  Q_D(const detail::Reply);
  return d->getApi();
}

IClient* Reply::getClient() const {
  Q_D(const detail::Reply);
  return d->getClient();
}

DataSerializer* Reply::getDataSerializer() const {
  Q_D(const detail::Reply);
  return d->getDataSerializer();
}

void Reply::setAutoDelete(bool enable) {
  Q_D(detail::Reply);
  d->setAutoDelete(enable);
}

bool Reply::isAutoDelete() const {
  Q_D(const detail::Reply);
  return d->isAutoDelete();
}

/* ------------------------------- WrappedReply ----------------------------- */

WrappedReply::WrappedReply(IReply* reply, QObject* parent)
    : IReply(*new detail::WrappedReplyPrivate(reply), parent) {
  Q_D(detail::WrappedReply);
  reply->setParent(this);
  d->connectReply();
}

WrappedReply::~WrappedReply() = default;

void WrappedReply::abort() {
  Q_D(detail::WrappedReply);
  d->abort();
}

void WrappedReply::retry() {
  Q_D(detail::WrappedReply);
  d->retry();
}

IApi* WrappedReply::getApi() const {
  Q_D(const detail::WrappedReply);
  return d->getApi();
}

IClient* WrappedReply::getClient() const {
  Q_D(const detail::WrappedReply);
  return d->getClient();
}

DataSerializer* WrappedReply::getDataSerializer() const {
  Q_D(const detail::WrappedReply);
  return d->getDataSerializer();
}

void WrappedReply::setAutoDelete(bool enable) {
  Q_D(detail::WrappedReply);
  d->setAutoDelete(enable);
}

bool WrappedReply::isAutoDelete() const {
  Q_D(const detail::WrappedReply);
  return d->isAutoDelete();
}

namespace detail {

/* ------------------------------- ReplyPrivate ----------------------------- */

static IReply::Error convertError(QNetworkReply::NetworkError error) {
  switch (error) {
    // clang-format off
    using enum QNetworkReply::NetworkError;

    case NoError:                             return IReply::Error::NoError;

    case ConnectionRefusedError:              return IReply::Error::ConnectionRefusedError;
    case RemoteHostClosedError:               return IReply::Error::RemoteHostClosedError;
    case HostNotFoundError:                   return IReply::Error::HostNotFoundError;
    case TimeoutError:                        return IReply::Error::TimeoutError;
    case OperationCanceledError:              return IReply::Error::OperationCanceledError;
    case SslHandshakeFailedError:             return IReply::Error::SslHandshakeFailedError;
    case TemporaryNetworkFailureError:        return IReply::Error::TemporaryNetworkFailureError;
    case NetworkSessionFailedError:           return IReply::Error::NetworkSessionFailedError;
    case BackgroundRequestNotAllowedError:    return IReply::Error::BackgroundRequestNotAllowedError;
    case TooManyRedirectsError:               return IReply::Error::TooManyRedirectsError;
    case InsecureRedirectError:               return IReply::Error::InsecureRedirectError;
    case UnknownNetworkError:                 return IReply::Error::UnknownNetworkError;

    case ProxyConnectionRefusedError:         return IReply::Error::ProxyConnectionRefusedError;
    case ProxyConnectionClosedError:          return IReply::Error::ProxyConnectionClosedError;
    case ProxyNotFoundError:                  return IReply::Error::ProxyNotFoundError;
    case ProxyTimeoutError:                   return IReply::Error::ProxyTimeoutError;
    case ProxyAuthenticationRequiredError:    return IReply::Error::ProxyAuthenticationRequiredError;
    case UnknownProxyError:                   return IReply::Error::UnknownProxyError;

    case ContentAccessDenied:                 return IReply::Error::ContentAccessDenied;
    case ContentOperationNotPermittedError:   return IReply::Error::ContentOperationNotPermittedError;
    case ContentNotFoundError:                return IReply::Error::ContentNotFoundError;
    case AuthenticationRequiredError:         return IReply::Error::AuthenticationRequiredError;
    case ContentReSendError:                  return IReply::Error::ContentReSendError;
    case ContentConflictError:                return IReply::Error::ContentConflictError;
    case ContentGoneError:                    return IReply::Error::ContentGoneError;
    case UnknownContentError:                 return IReply::Error::UnknownContentError;

    case ProtocolUnknownError:                return IReply::Error::ProtocolUnknownError;
    case ProtocolInvalidOperationError:       return IReply::Error::ProtocolInvalidOperationError;
    case ProtocolFailure:                     return IReply::Error::ProtocolFailure;

    case InternalServerError:                 return IReply::Error::InternalServerError;
    case OperationNotImplementedError:        return IReply::Error::OperationNotImplementedError;
    case ServiceUnavailableError:             return IReply::Error::ServiceUnavailableError;
    case UnknownServerError:                  return IReply::Error::UnknownServerError;
      // clang-format on

    default:
      Q_UNREACHABLE();
      break;
  }
}

static QNetworkReply::NetworkError convertError(IReply::Error error) {
  switch (error) {
    // clang-format off
    using enum IReply::Error;

    case NoError:                             return QNetworkReply::NetworkError::NoError;

    case ConnectionRefusedError:              return QNetworkReply::NetworkError::ConnectionRefusedError;
    case RemoteHostClosedError:               return QNetworkReply::NetworkError::RemoteHostClosedError;
    case HostNotFoundError:                   return QNetworkReply::NetworkError::HostNotFoundError;
    case TimeoutError:                        return QNetworkReply::NetworkError::TimeoutError;
    case OperationCanceledError:              return QNetworkReply::NetworkError::OperationCanceledError;
    case SslHandshakeFailedError:             return QNetworkReply::NetworkError::SslHandshakeFailedError;
    case TemporaryNetworkFailureError:        return QNetworkReply::NetworkError::TemporaryNetworkFailureError;
    case NetworkSessionFailedError:           return QNetworkReply::NetworkError::NetworkSessionFailedError;
    case BackgroundRequestNotAllowedError:    return QNetworkReply::NetworkError::BackgroundRequestNotAllowedError;
    case TooManyRedirectsError:               return QNetworkReply::NetworkError::TooManyRedirectsError;
    case InsecureRedirectError:               return QNetworkReply::NetworkError::InsecureRedirectError;
    case UnknownNetworkError:                 return QNetworkReply::NetworkError::UnknownNetworkError;

    case ProxyConnectionRefusedError:         return QNetworkReply::NetworkError::ProxyConnectionRefusedError;
    case ProxyConnectionClosedError:          return QNetworkReply::NetworkError::ProxyConnectionClosedError;
    case ProxyNotFoundError:                  return QNetworkReply::NetworkError::ProxyNotFoundError;
    case ProxyTimeoutError:                   return QNetworkReply::NetworkError::ProxyTimeoutError;
    case ProxyAuthenticationRequiredError:    return QNetworkReply::NetworkError::ProxyAuthenticationRequiredError;
    case UnknownProxyError:                   return QNetworkReply::NetworkError::UnknownProxyError;

    case ContentAccessDenied:                 return QNetworkReply::NetworkError::ContentAccessDenied;
    case ContentOperationNotPermittedError:   return QNetworkReply::NetworkError::ContentOperationNotPermittedError;
    case ContentNotFoundError:                return QNetworkReply::NetworkError::ContentNotFoundError;
    case AuthenticationRequiredError:         return QNetworkReply::NetworkError::AuthenticationRequiredError;
    case ContentReSendError:                  return QNetworkReply::NetworkError::ContentReSendError;
    case ContentConflictError:                return QNetworkReply::NetworkError::ContentConflictError;
    case ContentGoneError:                    return QNetworkReply::NetworkError::ContentGoneError;
    case ContentUnsupportedType:              return QNetworkReply::NetworkError::UnknownContentError;
    case ContentUnknownTypeDirective:         return QNetworkReply::NetworkError::UnknownContentError;
    case ContentUnsupportedCharset:           return QNetworkReply::NetworkError::UnknownContentError;
    case ContentJsonParseError:               return QNetworkReply::NetworkError::UnknownContentError;
    case ContentCborParseError:               return QNetworkReply::NetworkError::UnknownContentError;
    case UnknownContentError:                 return QNetworkReply::NetworkError::UnknownContentError;

    case ProtocolUnknownError:                return QNetworkReply::NetworkError::ProtocolUnknownError;
    case ProtocolInvalidOperationError:       return QNetworkReply::NetworkError::ProtocolInvalidOperationError;
    case ProtocolFailure:                     return QNetworkReply::NetworkError::ProtocolFailure;

    case InternalServerError:                 return QNetworkReply::NetworkError::InternalServerError;
    case OperationNotImplementedError:        return QNetworkReply::NetworkError::OperationNotImplementedError;
    case ServiceUnavailableError:             return QNetworkReply::NetworkError::ServiceUnavailableError;
    case UnknownServerError:                  return QNetworkReply::NetworkError::UnknownServerError;
      // clang-format on
    default:
      Q_UNREACHABLE();
      break;
  }
}

const QByteArray ReplyPrivate::PropertyBody =
    QByteArray{"__Egnite_Rest_ReplyPrivate_PropertyBody"};

const QByteArray ReplyPrivate::ContentType = QByteArray{"Content-Type"};
const QByteArray ReplyPrivate::Accept = QByteArray{"accept"};

const QByteArray ReplyPrivate::ContentTypeJson = QByteArray{"application/json"};
const QByteArray ReplyPrivate::ContentTypeCbor = QByteArray{"application/cbor"};

ReplyPrivate::ReplyPrivate(IApi* api, QNetworkReply* network_reply)
    : m_api(api), m_network_reply(network_reply), m_auto_delete(false) {}

ReplyPrivate::~ReplyPrivate() {
  if (m_network_reply) m_network_reply->deleteLater();
}

void ReplyPrivate::abort() { m_network_reply->abort(); }

void ReplyPrivate::retry() {
  auto manager = m_network_reply->manager();
  auto request = m_network_reply->request();
  auto verb = m_network_reply->attribute(QNetworkRequest::CustomVerbAttribute)
                  .toByteArray();
  auto body = m_network_reply->property(PropertyBody).toByteArray();

  m_network_reply->deleteLater();
  m_network_reply = send(manager, request, verb, body);
  connectReply();
}

IApi* ReplyPrivate::getApi() const { return m_api; }

IClient* ReplyPrivate::getClient() const { return m_api->getClient(); }

DataSerializer* ReplyPrivate::getDataSerializer() const {
  return m_api->getClient()->getDataSerializer();
}

void ReplyPrivate::setAutoDelete(bool enable) {
  Q_Q(Reply);
  if (m_auto_delete == enable) return;

  m_auto_delete = enable;

  if (m_auto_delete) {
    QObject::connect(q, &IReply::succeeded, q, &QObject::deleteLater);
    QObject::connect(q, &IReply::error, q, &QObject::deleteLater);
    QObject::connect(q, &IReply::failed, q, &QObject::deleteLater);
  } else {
    QObject::disconnect(q, &IReply::succeeded, q, &QObject::deleteLater);
    QObject::disconnect(q, &IReply::error, q, &QObject::deleteLater);
    QObject::disconnect(q, &IReply::failed, q, &QObject::deleteLater);
  }

  Q_EMIT q->autoDeleteChanged(m_auto_delete);
}

bool ReplyPrivate::isAutoDelete() const { return m_auto_delete; }

QNetworkReply* ReplyPrivate::send(QNetworkAccessManager* manager,
                                  const QNetworkRequest& request,
                                  const QByteArray& verb,
                                  const QByteArray& body) {
  QNetworkReply* reply = nullptr;
  if (body.isEmpty())
    reply = manager->sendCustomRequest(request, verb);
  else {
    reply = manager->sendCustomRequest(request, verb, body);
    if (reply) reply->setProperty(PropertyBody, body);
  }
  return reply;
}

void ReplyPrivate::replyFinished() {
  if (!m_network_reply) return;

  auto parse_error = ParseError{};
  auto content_type = parseContentType(parse_error);
  auto data =
      !parse_error ? parseData(content_type, parse_error) : Data{std::nullopt};

  processReply(data, parse_error);
}

void ReplyPrivate::connectReply() {
  Q_Q(Reply);

  QObject::connect(m_network_reply, &QNetworkReply::finished, q,
                   [this]() { replyFinished(); });
  QObject::connect(m_network_reply, &QNetworkReply::downloadProgress, q,
                   &IReply::downloadProgress);
  QObject::connect(m_network_reply, &QNetworkReply::uploadProgress, q,
                   &IReply::uploadProgress);
}

QByteArray ReplyPrivate::parseContentType(ParseError& parse_error) {
  auto content_type =
      m_network_reply->header(QNetworkRequest::ContentTypeHeader)
          .toByteArray()
          .trimmed();

  if (const auto content_types = content_type.split(';');
      content_types.size() > 1) {
    content_type = content_types.first().trimmed();
    for (auto i = 1; i < content_types.size(); ++i) {
      const auto args = content_types[i].trimmed().split('=');
      if (args.size() == 2 && args[0] == "charset") {
        if (args[1].toLower() != "utf-8")
          parse_error = std::make_pair(IReply::Error::ContentUnsupportedCharset,
                                       QStringLiteral("Unsupported charset: %1")
                                           .arg(QString::fromUtf8(args[1])));

      } else
        parse_error = std::make_pair(
            IReply::Error::ContentUnknownTypeDirective,
            QStringLiteral("Unknown content type directive: %1").arg(args[0]));
    }
  }

  return content_type;
}

Data ReplyPrivate::parseData(const QByteArray& content_type,
                             ParseError& parse_error) {
  if (content_type == ContentTypeJson) return parseJsonData(parse_error);
  if (content_type == ContentTypeCbor) return parseCborData(parse_error);

  parse_error = std::make_pair(IReply::Error::ContentUnsupportedType,
                               QStringLiteral("Unsupported content type: %1")
                                   .arg(QString::fromUtf8(content_type)));
  return std::nullopt;
}

Data ReplyPrivate::parseJsonData(ParseError& parse_error) {
  const auto read_data = m_network_reply->readAll();
  QJsonParseError error;
  auto json_doc = QJsonDocument::fromJson(read_data, &error);
  if (error.error != QJsonParseError::NoError) {
    auto error_str = error.errorString();
    if (error.error == QJsonParseError::IllegalValue) {
      error = QJsonParseError{};
      json_doc = QJsonDocument::fromJson("[" + read_data + "]", &error);

      if (error.error == QJsonParseError::NoError)
        return json_doc.array().first();
    }

    parse_error =
        std::make_pair(IReply::Error::ContentJsonParseError, error_str);

  } else {
    if (json_doc.isObject()) return QJsonValue(json_doc.object());
    if (json_doc.isArray()) return QJsonValue(json_doc.array());
    if (json_doc.isNull()) return QJsonValue(QJsonValue::Null);
  }

  return std::nullopt;
}

Data ReplyPrivate::parseCborData(ParseError& parse_error) {
  QCborStreamReader reader{m_network_reply};
  if (const auto error = reader.lastError(); error.c != QCborError::NoError)
    parse_error =
        std::make_pair(IReply::Error::ContentCborParseError, error.toString());

  return QCborValue::fromCbor(reader);
}

void ReplyPrivate::processReply(const Data& data,
                                const ParseError& parse_error) {
  Q_Q(Reply);
  const auto status =
      m_network_reply->attribute(QNetworkRequest::HttpStatusCodeAttribute)
          .toInt();

  if (!parse_error && status >= 300 &&
      !std::holds_alternative<std::nullopt_t>(data))
    Q_EMIT q->failed(status, data);
  else if (m_network_reply->error() != QNetworkReply::NoError)
    Q_EMIT q->error(m_network_reply->errorString(),
                    convertError(m_network_reply->error()));
  else if (parse_error)
    Q_EMIT q->error(parse_error->second, parse_error->first);
  else if (status >= 300 && std::holds_alternative<std::nullopt_t>(data))
    Q_EMIT q->failed(status, data);
  else {
    Q_EMIT q->succeeded(status, data);
  }
}

/* ---------------------------- WrappedReplyPrivate ------------------------- */

WrappedReplyPrivate::WrappedReplyPrivate(IReply* reply)
    : m_reply(reply), m_auto_delete(false) {}

WrappedReplyPrivate::~WrappedReplyPrivate() = default;

void WrappedReplyPrivate::abort() { m_reply->abort(); }

void WrappedReplyPrivate::retry() { m_reply->retry(); }

IApi* WrappedReplyPrivate::getApi() const { return m_reply->getApi(); }

IClient* WrappedReplyPrivate::getClient() const { return m_reply->getClient(); }

DataSerializer* WrappedReplyPrivate::getDataSerializer() const {
  return m_reply->getDataSerializer();
}

void WrappedReplyPrivate::setAutoDelete(bool enable) {
  Q_Q(WrappedReply);
  if (m_auto_delete == enable) return;

  m_auto_delete = enable;

  if (m_auto_delete) {
    QObject::connect(q, &IReply::succeeded, q, &QObject::deleteLater);
    QObject::connect(q, &IReply::error, q, &QObject::deleteLater);
    QObject::connect(q, &IReply::failed, q, &QObject::deleteLater);
  } else {
    QObject::disconnect(q, &IReply::succeeded, q, &QObject::deleteLater);
    QObject::disconnect(q, &IReply::error, q, &QObject::deleteLater);
    QObject::disconnect(q, &IReply::failed, q, &QObject::deleteLater);
  }

  Q_EMIT q->autoDeleteChanged(m_auto_delete);
}

bool WrappedReplyPrivate::isAutoDelete() const { return m_auto_delete; }

void WrappedReplyPrivate::connectReply() {
  Q_Q(WrappedReply);

  QObject::connect(m_reply, &IReply::succeeded, q, &IReply::succeeded);
  QObject::connect(m_reply, &IReply::failed, q, &IReply::failed);
  QObject::connect(m_reply, &IReply::error, q, &IReply::error);

  QObject::connect(m_reply, &IReply::downloadProgress, q,
                   &IReply::downloadProgress);
  QObject::connect(m_reply, &IReply::uploadProgress, q,
                   &IReply::uploadProgress);
}

}  // namespace detail

}  // namespace egnite::rest

#include "egnite/rest/moc_reply.cpp"
