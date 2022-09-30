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
#include <egnite/core/macros/binder.h>
/* ----------------------------------- Local -------------------------------- */
#include "egnite/rest/export.h"
#include "egnite/rest/global.h"
/* -------------------------------------------------------------------------- */

namespace egnite::rest {

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

  virtual void setAutoDelete(bool enable) = 0;
  [[nodiscard]] virtual bool isAutoDelete() const = 0;

  EGNITE_DEFINE_BINDER(Reply, onCompleted, completed);
  EGNITE_DEFINE_BINDER(Reply, onSucceeded, succeeded);
  EGNITE_DEFINE_BINDER(Reply, onFailed, failed);
  EGNITE_DEFINE_BINDER(Reply, onError, error);

  EGNITE_DEFINE_BINDER(Reply, onDownloadProgress, downloadProgress);
  EGNITE_DEFINE_BINDER(Reply, onUploadProgress, uploadProgress);

 Q_SIGNALS:
  void completed(int http_code, const Data& data);
  void succeeded(int http_code, const Data& data);
  void failed(int http_code, const Data& data);
  void error(const QString& error_str, Error error_type);

  void downloadProgress(qint64 bytes_received, qint64 bytes_total);
  void uploadProgress(qint64 bytes_sent, qint64 bytes_total);

  void autoDeleteChanged(bool enable);

 protected:
  explicit Reply(QObject* parent = nullptr);
  explicit Reply(QObjectPrivate& impl, QObject* parent = nullptr);
};

}  // namespace egnite::rest

#endif  // EGNITE_REST_REPLY_H