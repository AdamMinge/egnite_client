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
#include "egnite/rest/rest_global.h"
#include "egnite/utils/binder.h"
/* -------------------------------------------------------------------------- */

namespace egnite::rest {

namespace detail {
class RawRestReplyPrivate;
}

class RestApi;
class RestClient;
class RestDataSerializer;

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

 public:
  ~RestReply() override;

  virtual void abort() = 0;
  virtual void retry() = 0;

  [[nodiscard]] virtual RestApi* getApi() const = 0;
  [[nodiscard]] virtual RestClient* getClient() const = 0;
  [[nodiscard]] virtual RestDataSerializer* getDataSerializer() const = 0;

  template <typename Handler>
  RestReply* onCompleted(Handler&& handler, QObject* scope = nullptr);
  template <typename Handler>
  RestReply* onSucceeded(Handler&& handler, QObject* scope = nullptr);
  template <typename Handler>
  RestReply* onFailed(Handler&& handler, QObject* scope = nullptr);
  template <typename Handler>
  RestReply* onError(Handler&& handler, QObject* scope = nullptr);

 Q_SIGNALS:
  void completed(int http_code, const RestData& data, QPrivateSignal);
  void succeeded(int http_code, const RestData& data, QPrivateSignal);
  void failed(int http_code, const RestData& data, QPrivateSignal);
  void error(const QString& error_str, Error error_type, QPrivateSignal);

  void downloadProgress(qint64 bytes_received, qint64 bytes_total);
  void uploadProgress(qint64 bytes_sent, qint64 bytes_total);

 protected:
  RestReply(QObject* parent = nullptr);
  RestReply(QObjectPrivate& impl, QObject* parent = nullptr);
};

class EGNITE_API RawRestReply : public RestReply {
  Q_OBJECT

  friend RestApi;

 public:
  ~RawRestReply() override;

  void abort() override;
  void retry() override;

  [[nodiscard]] RestApi* getApi() const override;
  [[nodiscard]] RestClient* getClient() const override;
  [[nodiscard]] RestDataSerializer* getDataSerializer() const override;

 protected:
  RawRestReply(RestApi* api, QNetworkReply* network_reply,
               QObject* parent = nullptr);

 private:
  Q_DECLARE_PRIVATE(detail::RawRestReply)
};

template <typename Handler>
RestReply* RestReply::onCompleted(Handler&& handler, QObject* scope) {
  connect(this, &RestReply::completed, scope ? scope : this,
          utils::bindCallback<decltype(&RestReply::completed)>(
              std::forward<Handler>(handler)));
  return this;
}

template <typename Handler>
RestReply* RestReply::onSucceeded(Handler&& handler, QObject* scope) {
  connect(this, &RestReply::succeeded, scope ? scope : this,
          utils::bindCallback<decltype(&RestReply::succeeded)>(
              std::forward<Handler>(handler)));
  return this;
}

template <typename Handler>
RestReply* RestReply::onFailed(Handler&& handler, QObject* scope) {
  connect(this, &RestReply::failed, scope ? scope : this,
          utils::bindCallback<decltype(&RestReply::failed)>(
              std::forward<Handler>(handler)));
  return this;
}

template <typename Handler>
RestReply* RestReply::onError(Handler&& handler, QObject* scope) {
  connect(this, &RestReply::error, scope ? scope : this,
          utils::bindCallback<decltype(&RestReply::error)>(
              std::forward<Handler>(handler)));
  return this;
}

}  // namespace egnite::rest

#endif  // EGNITE_REST_REPLY_H