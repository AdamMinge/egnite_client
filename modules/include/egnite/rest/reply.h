#ifndef EGNITE_REST_REPLY_H
#define EGNITE_REST_REPLY_H

/* ------------------------------------ Qt ---------------------------------- */
#include <QCborValue>
#include <QJsonValue>
#include <QNetworkReply>
#include <QObject>
/* ---------------------------------- Standard ------------------------------ */
#include <variant>
/* ----------------------------------- Egnite ------------------------------- */
#include <egnite/core/utils/binder.h>
/* ----------------------------------- Local -------------------------------- */
#include "egnite/rest/export.h"
#include "egnite/rest/global.h"
/* -------------------------------------------------------------------------- */

namespace egnite::rest {

namespace detail {
class RawReplyPrivate;
}

class Api;
class Client;
class DataSerializer;

class EGNITE_REST_API Reply : public QObject {
  Q_OBJECT

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
  ~Reply() override;

  virtual void abort() = 0;
  virtual void retry() = 0;

  [[nodiscard]] virtual Api* getApi() const = 0;
  [[nodiscard]] virtual Client* getClient() const = 0;
  [[nodiscard]] virtual DataSerializer* getDataSerializer() const = 0;

  template <typename Handler>
  Reply* onCompleted(Handler&& handler, QObject* scope = nullptr);
  template <typename Handler>
  Reply* onSucceeded(Handler&& handler, QObject* scope = nullptr);
  template <typename Handler>
  Reply* onFailed(Handler&& handler, QObject* scope = nullptr);
  template <typename Handler>
  Reply* onError(Handler&& handler, QObject* scope = nullptr);

  template <typename Handler>
  Reply* onDownloadProgress(Handler&& handler, QObject* scope = nullptr);
  template <typename Handler>
  Reply* onUploadProgress(Handler&& handler, QObject* scope = nullptr);

 Q_SIGNALS:
  void completed(int http_code, const Data& data);
  void succeeded(int http_code, const Data& data);
  void failed(int http_code, const Data& data);
  void error(const QString& error_str, Error error_type);

  void downloadProgress(qint64 bytes_received, qint64 bytes_total);
  void uploadProgress(qint64 bytes_sent, qint64 bytes_total);

 protected:
  explicit Reply(QObject* parent = nullptr);
  explicit Reply(QObjectPrivate& impl, QObject* parent = nullptr);
};

class EGNITE_REST_API RawReply : public Reply {
  Q_OBJECT

  friend Api;

 public:
  ~RawReply() override;

  void abort() override;
  void retry() override;

  [[nodiscard]] Api* getApi() const override;
  [[nodiscard]] Client* getClient() const override;
  [[nodiscard]] DataSerializer* getDataSerializer() const override;

 protected:
  RawReply(Api* api, QNetworkReply* network_reply, QObject* parent = nullptr);

 private:
  Q_DECLARE_PRIVATE(detail::RawReply)
};

template <typename Handler>
Reply* Reply::onCompleted(Handler&& handler, QObject* scope) {
  connect(this, &Reply::completed, scope ? scope : this,
          core::utils::bindCallback<decltype(&Reply::completed)>(
              std::forward<Handler>(handler)));
  return this;
}

template <typename Handler>
Reply* Reply::onSucceeded(Handler&& handler, QObject* scope) {
  connect(this, &Reply::succeeded, scope ? scope : this,
          core::utils::bindCallback<decltype(&Reply::succeeded)>(
              std::forward<Handler>(handler)));
  return this;
}

template <typename Handler>
Reply* Reply::onFailed(Handler&& handler, QObject* scope) {
  connect(this, &Reply::failed, scope ? scope : this,
          core::utils::bindCallback<decltype(&Reply::failed)>(
              std::forward<Handler>(handler)));
  return this;
}

template <typename Handler>
Reply* Reply::onError(Handler&& handler, QObject* scope) {
  connect(this, &Reply::error, scope ? scope : this,
          core::utils::bindCallback<decltype(&Reply::error)>(
              std::forward<Handler>(handler)));
  return this;
}

template <typename Handler>
Reply* Reply::onDownloadProgress(Handler&& handler, QObject* scope) {
  connect(this, &Reply::downloadProgress, scope ? scope : this,
          core::utils::bindCallback<decltype(&Reply::downloadProgress)>(
              std::forward<Handler>(handler)));
  return this;
}

template <typename Handler>
Reply* Reply::onUploadProgress(Handler&& handler, QObject* scope) {
  connect(this, &Reply::uploadProgress, scope ? scope : this,
          core::utils::bindCallback<decltype(&Reply::uploadProgress)>(
              std::forward<Handler>(handler)));
  return this;
}

}  // namespace egnite::rest

#endif  // EGNITE_REST_REPLY_H