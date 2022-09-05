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

/* ---------------------------------- Reply --------------------------------- */

Reply::Reply(QObject* parent) : QObject(parent) {}

Reply::Reply(QObjectPrivate& impl, QObject* parent) : QObject(impl, parent) {}

Reply::~Reply() = default;

/* --------------------------------- RawReply ------------------------------- */

RawReply::RawReply(Api* api, QNetworkReply* network_reply, QObject* parent)
    : Reply(*new detail::RawReplyPrivate(api, network_reply), parent) {
  connect(this, &RawReply::succeeded, this, &RawReply::completed);
  connect(this, &RawReply::failed, this, &RawReply::completed);
}

RawReply::~RawReply() = default;

void RawReply::abort() {
  Q_D(detail::RawReply);
  d->abort();
}

void RawReply::retry() {
  Q_D(detail::RawReply);
  d->retry();
}

Api* RawReply::getApi() const {
  Q_D(const detail::RawReply);
  return d->getApi();
}

Client* RawReply::getClient() const {
  Q_D(const detail::RawReply);
  return d->getClient();
}

DataSerializer* RawReply::getDataSerializer() const {
  Q_D(const detail::RawReply);
  return d->getDataSerializer();
}

/* --------------------------- RawReplyPrivate -------------------------- */

namespace detail {

static Reply::Error convertError(QNetworkReply::NetworkError error) {
  switch (error) {
    // clang-format off
    using enum QNetworkReply::NetworkError;

    case NoError:                             return Reply::Error::NoError;

    case ConnectionRefusedError:              return Reply::Error::ConnectionRefusedError;
    case RemoteHostClosedError:               return Reply::Error::RemoteHostClosedError;
    case HostNotFoundError:                   return Reply::Error::HostNotFoundError;
    case TimeoutError:                        return Reply::Error::TimeoutError;
    case OperationCanceledError:              return Reply::Error::OperationCanceledError;
    case SslHandshakeFailedError:             return Reply::Error::SslHandshakeFailedError;
    case TemporaryNetworkFailureError:        return Reply::Error::TemporaryNetworkFailureError;
    case NetworkSessionFailedError:           return Reply::Error::NetworkSessionFailedError;
    case BackgroundRequestNotAllowedError:    return Reply::Error::BackgroundRequestNotAllowedError;
    case TooManyRedirectsError:               return Reply::Error::TooManyRedirectsError;
    case InsecureRedirectError:               return Reply::Error::InsecureRedirectError;
    case UnknownNetworkError:                 return Reply::Error::UnknownNetworkError;

    case ProxyConnectionRefusedError:         return Reply::Error::ProxyConnectionRefusedError;
    case ProxyConnectionClosedError:          return Reply::Error::ProxyConnectionClosedError;
    case ProxyNotFoundError:                  return Reply::Error::ProxyNotFoundError;
    case ProxyTimeoutError:                   return Reply::Error::ProxyTimeoutError;
    case ProxyAuthenticationRequiredError:    return Reply::Error::ProxyAuthenticationRequiredError;
    case UnknownProxyError:                   return Reply::Error::UnknownProxyError;

    case ContentAccessDenied:                 return Reply::Error::ContentAccessDenied;
    case ContentOperationNotPermittedError:   return Reply::Error::ContentOperationNotPermittedError;
    case ContentNotFoundError:                return Reply::Error::ContentNotFoundError;
    case AuthenticationRequiredError:         return Reply::Error::AuthenticationRequiredError;
    case ContentReSendError:                  return Reply::Error::ContentReSendError;
    case ContentConflictError:                return Reply::Error::ContentConflictError;
    case ContentGoneError:                    return Reply::Error::ContentGoneError;
    case UnknownContentError:                 return Reply::Error::UnknownContentError;

    case ProtocolUnknownError:                return Reply::Error::ProtocolUnknownError;
    case ProtocolInvalidOperationError:       return Reply::Error::ProtocolInvalidOperationError;
    case ProtocolFailure:                     return Reply::Error::ProtocolFailure;

    case InternalServerError:                 return Reply::Error::InternalServerError;
    case OperationNotImplementedError:        return Reply::Error::OperationNotImplementedError;
    case ServiceUnavailableError:             return Reply::Error::ServiceUnavailableError;
    case UnknownServerError:                  return Reply::Error::UnknownServerError;
      // clang-format on

    default:
      Q_UNREACHABLE();
      break;
  }
}

static QNetworkReply::NetworkError convertError(Reply::Error error) {
  switch (error) {
    // clang-format off
    using enum Reply::Error;

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

const QByteArray RawReplyPrivate::PropertyBody =
    QByteArray{"__Egnite_Rest_ReplyPrivate_PropertyBody"};

const QByteArray RawReplyPrivate::ContentType = QByteArray{"ContentType"};
const QByteArray RawReplyPrivate::Accept = QByteArray{"Accept"};

const QByteArray RawReplyPrivate::ContentTypeJson =
    QByteArray{"application/json"};
const QByteArray RawReplyPrivate::ContentTypeCbor =
    QByteArray{"application/cbor"};

RawReplyPrivate::RawReplyPrivate(Api* api, QNetworkReply* network_reply)
    : m_api(api), m_network_reply(network_reply) {
  connectReply();
}

RawReplyPrivate::~RawReplyPrivate() {
  if (m_network_reply) m_network_reply->deleteLater();
}

void RawReplyPrivate::abort() { m_network_reply->abort(); }

void RawReplyPrivate::retry() {
  auto manager = m_network_reply->manager();
  auto request = m_network_reply->request();
  auto verb = m_network_reply->attribute(QNetworkRequest::CustomVerbAttribute)
                  .toByteArray();
  auto body = m_network_reply->property(PropertyBody).toByteArray();

  m_network_reply->deleteLater();
  m_network_reply = send(manager, request, verb, body);
  connectReply();
}

Api* RawReplyPrivate::getApi() const { return m_api; }

Client* RawReplyPrivate::getClient() const { return m_api->getClient(); }

DataSerializer* RawReplyPrivate::getDataSerializer() const {
  return m_api->getClient()->getDataSerializer();
}

QNetworkReply* RawReplyPrivate::send(QNetworkAccessManager* manager,
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

void RawReplyPrivate::replyFinished() {
  if (!m_network_reply) return;

  auto parse_error = ParseError{};
  auto content_type = parseContentType(parse_error);
  auto data =
      !parse_error ? parseData(content_type, parse_error) : Data{std::nullopt};

  processReply(data, parse_error);
}

void RawReplyPrivate::connectReply() {
  connect(m_network_reply, &QNetworkReply::finished, this,
          &RawReplyPrivate::replyFinished);

  Q_Q(Reply);
  QObject::connect(m_network_reply, &QNetworkReply::downloadProgress, q,
                   &Reply::downloadProgress);
  QObject::connect(m_network_reply, &QNetworkReply::uploadProgress, q,
                   &Reply::uploadProgress);
}

QByteArray RawReplyPrivate::parseContentType(ParseError& parse_error) {
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
          parse_error = std::make_pair(Reply::Error::ContentUnsupportedCharset,
                                       QStringLiteral("Unsupported charset: %1")
                                           .arg(QString::fromUtf8(args[1])));

      } else
        parse_error = std::make_pair(
            Reply::Error::ContentUnknownTypeDirective,
            QStringLiteral("Unknown content type directive: %1").arg(args[0]));
    }
  }

  return content_type;
}

Data RawReplyPrivate::parseData(const QByteArray& content_type,
                                ParseError& parse_error) {
  if (content_type == ContentTypeJson) return parseJsonData(parse_error);
  if (content_type == ContentTypeCbor) return parseCborData(parse_error);

  parse_error = std::make_pair(Reply::Error::ContentUnsupportedType,
                               QStringLiteral("Unsupported content type: %1")
                                   .arg(QString::fromUtf8(content_type)));
  return std::nullopt;
}

Data RawReplyPrivate::parseJsonData(ParseError& parse_error) {
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
        std::make_pair(Reply::Error::ContentJsonParseError, error_str);

  } else {
    if (json_doc.isObject()) return QJsonValue(json_doc.object());
    if (json_doc.isArray()) return QJsonValue(json_doc.array());
    if (json_doc.isNull()) return QJsonValue(QJsonValue::Null);
  }

  return std::nullopt;
}

Data RawReplyPrivate::parseCborData(ParseError& parse_error) {
  QCborStreamReader reader{m_network_reply};
  if (const auto error = reader.lastError(); error.c != QCborError::NoError)
    parse_error =
        std::make_pair(Reply::Error::ContentCborParseError, error.toString());

  return QCborValue::fromCbor(reader);
}

void RawReplyPrivate::processReply(const Data& data,
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

}  // namespace detail

}  // namespace egnite::rest

#include "egnite/rest/moc_reply.cpp"