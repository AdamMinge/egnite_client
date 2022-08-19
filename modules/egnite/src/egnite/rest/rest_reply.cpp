/* ----------------------------------- Local -------------------------------- */
#include "egnite/rest/rest_reply.h"

#include "egnite/rest/detail/rest_reply_p.h"
/* ------------------------------------ Qt ---------------------------------- */
#include <QCborStreamReader>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
/* -------------------------------------------------------------------------- */

namespace egnite::rest {

/* -------------------------------- RestReply ------------------------------- */

RestReply::RestReply(QNetworkReply* network_reply, QObject* parent)
    : RestReply(*new detail::RestReplyPrivate(network_reply), parent) {}

RestReply::RestReply(detail::RestReplyPrivate& impl, QObject* parent)
    : QObject(impl, parent) {}

RestReply::~RestReply() = default;

void RestReply::abort() {
  Q_D(detail::RestReply);
  d->abort();
}

void RestReply::retry() {
  Q_D(detail::RestReply);
  d->retry();
}

/* ---------------------------- RestReplyPrivate ---------------------------- */

namespace detail {

static RestReply::Error convertError(QNetworkReply::NetworkError error) {
  switch (error) {
    // clang-format off
    using enum QNetworkReply::NetworkError;

    case NoError:                             return RestReply::Error::NoError;

    case ConnectionRefusedError:              return RestReply::Error::ConnectionRefusedError;
    case RemoteHostClosedError:               return RestReply::Error::RemoteHostClosedError;
    case HostNotFoundError:                   return RestReply::Error::HostNotFoundError;
    case TimeoutError:                        return RestReply::Error::TimeoutError;
    case OperationCanceledError:              return RestReply::Error::OperationCanceledError;
    case SslHandshakeFailedError:             return RestReply::Error::SslHandshakeFailedError;
    case TemporaryNetworkFailureError:        return RestReply::Error::TemporaryNetworkFailureError;
    case NetworkSessionFailedError:           return RestReply::Error::NetworkSessionFailedError;
    case BackgroundRequestNotAllowedError:    return RestReply::Error::BackgroundRequestNotAllowedError;
    case TooManyRedirectsError:               return RestReply::Error::TooManyRedirectsError;
    case InsecureRedirectError:               return RestReply::Error::InsecureRedirectError;
    case UnknownNetworkError:                 return RestReply::Error::UnknownNetworkError;

    case ProxyConnectionRefusedError:         return RestReply::Error::ProxyConnectionRefusedError;
    case ProxyConnectionClosedError:          return RestReply::Error::ProxyConnectionClosedError;
    case ProxyNotFoundError:                  return RestReply::Error::ProxyNotFoundError;
    case ProxyTimeoutError:                   return RestReply::Error::ProxyTimeoutError;
    case ProxyAuthenticationRequiredError:    return RestReply::Error::ProxyAuthenticationRequiredError;
    case UnknownProxyError:                   return RestReply::Error::UnknownProxyError;

    case ContentAccessDenied:                 return RestReply::Error::ContentAccessDenied;
    case ContentOperationNotPermittedError:   return RestReply::Error::ContentOperationNotPermittedError;
    case ContentNotFoundError:                return RestReply::Error::ContentNotFoundError;
    case AuthenticationRequiredError:         return RestReply::Error::AuthenticationRequiredError;
    case ContentReSendError:                  return RestReply::Error::ContentReSendError;
    case ContentConflictError:                return RestReply::Error::ContentConflictError;
    case ContentGoneError:                    return RestReply::Error::ContentGoneError;
    case UnknownContentError:                 return RestReply::Error::UnknownContentError;

    case ProtocolUnknownError:                return RestReply::Error::ProtocolUnknownError;
    case ProtocolInvalidOperationError:       return RestReply::Error::ProtocolInvalidOperationError;
    case ProtocolFailure:                     return RestReply::Error::ProtocolFailure;

    case InternalServerError:                 return RestReply::Error::InternalServerError;
    case OperationNotImplementedError:        return RestReply::Error::OperationNotImplementedError;
    case ServiceUnavailableError:             return RestReply::Error::ServiceUnavailableError;
    case UnknownServerError:                  return RestReply::Error::UnknownServerError;
      // clang-format on

    default:
      Q_UNREACHABLE();
      break;
  }
}

static QNetworkReply::NetworkError convertError(RestReply::Error error) {
  switch (error) {
    // clang-format off
    using enum RestReply::Error;

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

const QByteArray RestReplyPrivate::PropertyBody =
    QByteArray{"__Egnite_Rest_RestReplyPrivate_PropertyBody"};

const QByteArray RestReplyPrivate::ContentType = QByteArray{"ContentType"};
const QByteArray RestReplyPrivate::Accept = QByteArray{"Accept"};

const QByteArray RestReplyPrivate::ContentTypeJson =
    QByteArray{"application/json"};
const QByteArray RestReplyPrivate::ContentTypeCbor =
    QByteArray{"application/cbor"};

RestReplyPrivate::RestReplyPrivate(QNetworkReply* network_reply)
    : m_network_reply(network_reply) {
  connectReply();
}

RestReplyPrivate::~RestReplyPrivate() {
  if (m_network_reply) m_network_reply->deleteLater();
}

void RestReplyPrivate::abort() { m_network_reply->abort(); }

void RestReplyPrivate::retry() {
  auto manager = m_network_reply->manager();
  auto request = m_network_reply->request();
  auto verb = m_network_reply->attribute(QNetworkRequest::CustomVerbAttribute)
                  .toByteArray();
  auto body = m_network_reply->property(PropertyBody).toByteArray();

  m_network_reply->deleteLater();
  m_network_reply = send(manager, request, verb, body);
  connectReply();
}

QNetworkReply* RestReplyPrivate::send(QNetworkAccessManager* manager,
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

void RestReplyPrivate::replyFinished() {
  if (!m_network_reply) return;

  auto parse_error = ParseError{};
  auto content_type = parseContentType(parse_error);
  auto data = !parse_error ? parseData(content_type, parse_error)
                           : RestData{std::nullopt};

  processReply(data, parse_error);
}

void RestReplyPrivate::connectReply() {
  connect(m_network_reply, &QNetworkReply::finished, this,
          &RestReplyPrivate::replyFinished);

  Q_Q(RestReply);
  QObject::connect(m_network_reply, &QNetworkReply::downloadProgress, q,
                   &RestReply::downloadProgress);
  QObject::connect(m_network_reply, &QNetworkReply::uploadProgress, q,
                   &RestReply::uploadProgress);
}

QByteArray RestReplyPrivate::parseContentType(ParseError& parse_error) {
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
          parse_error =
              std::make_pair(RestReply::Error::ContentUnsupportedCharset,
                             QStringLiteral("Unsupported charset: %1")
                                 .arg(QString::fromUtf8(args[1])));

      } else
        parse_error = std::make_pair(
            RestReply::Error::ContentUnknownTypeDirective,
            QStringLiteral("Unknown content type directive: %1").arg(args[0]));
    }
  }

  return content_type;
}

RestData RestReplyPrivate::parseData(const QByteArray& content_type,
                                     ParseError& parse_error) {
  if (content_type == ContentTypeJson) return parseJsonData(parse_error);
  if (content_type == ContentTypeCbor) return parseCborData(parse_error);

  parse_error = std::make_pair(RestReply::Error::ContentUnsupportedType,
                               QStringLiteral("Unsupported content type: %1")
                                   .arg(QString::fromUtf8(content_type)));
  return std::nullopt;
}

RestData RestReplyPrivate::parseJsonData(ParseError& parse_error) {
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
        std::make_pair(RestReply::Error::ContentJsonParseError, error_str);

  } else {
    if (json_doc.isObject()) return QJsonValue(json_doc.object());
    if (json_doc.isArray()) return QJsonValue(json_doc.array());
    if (json_doc.isNull()) return QJsonValue(QJsonValue::Null);
  }

  return std::nullopt;
}

RestData RestReplyPrivate::parseCborData(ParseError& parse_error) {
  QCborStreamReader reader{m_network_reply};
  if (const auto error = reader.lastError(); error.c != QCborError::NoError)
    parse_error = std::make_pair(RestReply::Error::ContentCborParseError,
                                 error.toString());

  return QCborValue::fromCbor(reader);
}

void RestReplyPrivate::processReply(const RestData& data,
                                    const ParseError& parse_error) {
  Q_Q(RestReply);
  const auto status =
      m_network_reply->attribute(QNetworkRequest::HttpStatusCodeAttribute)
          .toInt();

  if (!parse_error && status >= 300 &&
      !std::holds_alternative<std::nullopt_t>(data))
    Q_EMIT q->failed(status, data, {});
  else if (m_network_reply->error() != QNetworkReply::NoError)
    Q_EMIT q->error(m_network_reply->errorString(),
                    convertError(m_network_reply->error()), {});
  else if (parse_error)
    Q_EMIT q->error(parse_error->second, parse_error->first, {});
  else if (status >= 300 && std::holds_alternative<std::nullopt_t>(data))
    Q_EMIT q->failed(status, data, {});
  else {
    Q_EMIT q->succeeded(status, data, {});
  }
}

}  // namespace detail

}  // namespace egnite::rest

#include "egnite/rest/moc_rest_reply.cpp"