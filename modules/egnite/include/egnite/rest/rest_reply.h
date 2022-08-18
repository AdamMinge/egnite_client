#ifndef EGNITE_REST_REPLY_H
#define EGNITE_REST_REPLY_H

/* ------------------------------------ Qt ---------------------------------- */
#include <QCborValue>
#include <QJsonValue>
#include <QNetworkReply>
#include <QObject>
/* ---------------------------------- Standard ------------------------------ */
#include <variant>
/* ----------------------------------- Local -------------------------------- */
#include "egnite/export.h"
#include "egnite/utils/binder.h"
/* -------------------------------------------------------------------------- */

namespace egnite::rest {

namespace detail {
class RestReplyPrivate;
}

class RestApi;

class EGNITE_API RestReply : public QObject {
  Q_OBJECT

  friend RestApi;

 public:
  enum class Error {
    NoError = 0,

    ConnectionRefusedError = 1,
    RemoteHostClosedError,
    HostNotFoundError,
    TimeoutError,
    OperationCanceledError,
    SslHandshakeFailedError,
    TemporaryNetworkFailureError,
    NetworkSessionFailedError,
    BackgroundRequestNotAllowedError,
    TooManyRedirectsError,
    InsecureRedirectError,
    UnknownNetworkError = 99,

    ProxyConnectionRefusedError = 101,
    ProxyConnectionClosedError,
    ProxyNotFoundError,
    ProxyTimeoutError,
    ProxyAuthenticationRequiredError,
    UnknownProxyError = 199,

    ContentAccessDenied = 201,
    ContentOperationNotPermittedError,
    ContentNotFoundError,
    AuthenticationRequiredError,
    ContentReSendError,
    ContentConflictError,
    ContentGoneError,
    ContentUnsupportedType,
    ContentUnknownTypeDirective,
    ContentUnsupportedCharset,
    ContentJsonParseError,
    ContentCborParseError,
    UnknownContentError = 299,

    ProtocolUnknownError = 301,
    ProtocolInvalidOperationError,
    ProtocolFailure = 399,

    InternalServerError = 401,
    OperationNotImplementedError,
    ServiceUnavailableError,
    UnknownServerError = 499
  };
  Q_ENUM(Error)

  using Data = std::variant<std::nullopt_t, QJsonValue, QCborValue>;

 public:
  ~RestReply() override;

  void abort();
  void retry();

  template <typename HANDLER>
  RestReply* onCompleted(HANDLER&& handler, QObject* scope = nullptr);
  template <typename HANDLER>
  RestReply* onSucceeded(HANDLER&& handler, QObject* scope = nullptr);
  template <typename HANDLER>
  RestReply* onFailed(HANDLER&& handler, QObject* scope = nullptr);
  template <typename HANDLER>
  RestReply* onError(HANDLER&& handler, QObject* scope = nullptr);

 Q_SIGNALS:
  void completed(int http_code, const Data& data, QPrivateSignal);
  void succeeded(int http_code, const Data& data, QPrivateSignal);
  void failed(int http_code, const Data& data, QPrivateSignal);
  void error(const QString& error_str, Error error_type, QPrivateSignal);

  void downloadProgress(qint64 bytes_received, qint64 bytes_total);
  void uploadProgress(qint64 bytes_sent, qint64 bytes_total);

 protected:
  RestReply(QNetworkReply* network_reply, QObject* parent = nullptr);
  RestReply(detail::RestReplyPrivate& impl, QObject* parent = nullptr);

 private:
  Q_DECLARE_PRIVATE(detail::RestReply)
};

template <typename HANDLER>
RestReply* RestReply::onCompleted(HANDLER&& handler, QObject* scope) {
  connect(this, &RestReply::completed, scope ? scope : this,
          utils::bindCallback<decltype(&RestReply::completed)>(
              std::forward<HANDLER>(handler)));
  return this;
}

template <typename HANDLER>
RestReply* RestReply::onSucceeded(HANDLER&& handler, QObject* scope) {
  connect(this, &RestReply::succeeded, scope ? scope : this,
          utils::bindCallback<decltype(&RestReply::succeeded)>(
              std::forward<HANDLER>(handler)));
  return this;
}

template <typename HANDLER>
RestReply* RestReply::onFailed(HANDLER&& handler, QObject* scope) {
  connect(this, &RestReply::failed, scope ? scope : this,
          utils::bindCallback<decltype(&RestReply::failed)>(
              std::forward<HANDLER>(handler)));
  return this;
}

template <typename HANDLER>
RestReply* RestReply::onError(HANDLER&& handler, QObject* scope) {
  connect(this, &RestReply::error, scope ? scope : this,
          utils::bindCallback<decltype(&RestReply::error)>(
              std::forward<HANDLER>(handler)));
  return this;
}

}  // namespace egnite::rest

#endif  // EGNITE_REST_REPLY_H