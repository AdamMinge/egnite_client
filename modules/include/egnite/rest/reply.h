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
#include "egnite/rest/data_serializer.h"
#include "egnite/rest/export.h"
#include "egnite/rest/global.h"
/* -------------------------------------------------------------------------- */

namespace egnite::rest {

namespace detail {
class ReplyPrivate;
class WrappedReplyPrivate;
}  // namespace detail

class IApi;
class IClient;
class DataSerializer;

/* ----------------------------------- IReply ------------------------------- */

class EGNITE_REST_API IReply : public QObject {
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
  ~IReply() override;

  virtual void abort() = 0;
  virtual void retry() = 0;

  [[nodiscard]] virtual IApi* getApi() const = 0;
  [[nodiscard]] virtual IClient* getClient() const = 0;
  [[nodiscard]] virtual DataSerializer* getDataSerializer() const = 0;

  virtual void setAutoDelete(bool enable) = 0;
  [[nodiscard]] virtual bool isAutoDelete() const = 0;

  EGNITE_DEFINE_BINDER(IReply, onCompleted, completed);
  EGNITE_DEFINE_BINDER(IReply, onSucceeded, succeeded);
  EGNITE_DEFINE_BINDER(IReply, onFailed, failed);
  EGNITE_DEFINE_BINDER(IReply, onError, error);
  EGNITE_DEFINE_BINDER(IReply, OnUnsucceeded, unsucceeded);

  EGNITE_DEFINE_BINDER(IReply, onDownloadProgress, downloadProgress);
  EGNITE_DEFINE_BINDER(IReply, onUploadProgress, uploadProgress);

 Q_SIGNALS:
  void completed(int http_code, const Data& data);
  void succeeded(int http_code, const Data& data);
  void failed(int http_code, const Data& data);
  void error(const QString& error_str, Error error_type);
  void unsucceeded();

  void downloadProgress(qint64 bytes_received, qint64 bytes_total);
  void uploadProgress(qint64 bytes_sent, qint64 bytes_total);

  void autoDeleteChanged(bool enable);

 protected:
  explicit IReply(QObject* parent = nullptr);
  explicit IReply(QObjectPrivate& impl, QObject* parent = nullptr);
};

/* ------------------------------------ Reply ------------------------------- */

class EGNITE_REST_API Reply : public IReply {
  Q_OBJECT

 public:
  Reply(IApi* api, QNetworkReply* network_reply, QObject* parent = nullptr);
  ~Reply() override;

  void abort() override;
  void retry() override;

  [[nodiscard]] IApi* getApi() const override;
  [[nodiscard]] IClient* getClient() const override;
  [[nodiscard]] DataSerializer* getDataSerializer() const override;

  void setAutoDelete(bool enable) override;
  [[nodiscard]] bool isAutoDelete() const override;

 private:
  Q_DECLARE_PRIVATE(detail::Reply)
};

/* -------------------------------- WrappedReply ---------------------------- */

class EGNITE_REST_API WrappedReply : public IReply {
  Q_OBJECT

 public:
  ~WrappedReply() override;

  void abort() override;
  void retry() override;

  [[nodiscard]] IApi* getApi() const override;
  [[nodiscard]] IClient* getClient() const override;
  [[nodiscard]] DataSerializer* getDataSerializer() const override;

  void setAutoDelete(bool enable) override;
  [[nodiscard]] bool isAutoDelete() const override;

 protected:
  WrappedReply(IReply* reply, QObject* parent = nullptr);

 private:
  Q_DECLARE_PRIVATE(detail::WrappedReply)
};

/* ----------------------------- GenericReplyBase --------------------------- */

template <typename DataType, typename ErrorType,
          template <typename, typename> typename ChildGenericReply>
class GenericReplyBase : public WrappedReply {
 public:
  using ChildReply = ChildGenericReply<DataType, ErrorType>;

 public:
  template <typename Handler>
  ChildReply* onCompleted(Handler&& handler, QObject* scope = nullptr);

  EGNITE_DEFINE_BINDER_EXTENDED(IReply, ChildReply, onError, error);
  EGNITE_DEFINE_BINDER_EXTENDED(IReply, ChildReply, OnUnsucceeded, unsucceeded);

  EGNITE_DEFINE_BINDER_EXTENDED(IReply, ChildReply, onDownloadProgress,
                                downloadProgress);
  EGNITE_DEFINE_BINDER_EXTENDED(IReply, ChildReply, onUploadProgress,
                                uploadProgress);

 protected:
  explicit GenericReplyBase(IReply* reply, QObject* parent = nullptr);
  ~GenericReplyBase() override;
};

template <typename DataType, typename ErrorType,
          template <typename, typename> typename ChildGenericReply>
GenericReplyBase<DataType, ErrorType, ChildGenericReply>::GenericReplyBase(
    IReply* reply, QObject* parent)
    : WrappedReply(reply, parent) {}

template <typename DataType, typename ErrorType,
          template <typename, typename> typename ChildGenericReply>
GenericReplyBase<DataType, ErrorType, ChildGenericReply>::~GenericReplyBase() =
    default;

template <typename DataType, typename ErrorType,
          template <typename, typename> typename ChildGenericReply>
template <typename Handler>
GenericReplyBase<DataType, ErrorType, ChildGenericReply>::ChildReply*
GenericReplyBase<DataType, ErrorType, ChildGenericReply>::onCompleted(
    Handler&& handler, QObject* scope) {
  WrappedReply::onCompleted(
      core::utils::bindCallback<void(int)>(std::forward<Handler>(handler)),
      scope);
  return static_cast<ChildGenericReply<DataType, ErrorType>*>(this);
}

/* --------------------- GenericReply<DataType, ErrorType> ------------------ */

template <typename DataType, typename ErrorType>
class GenericReply
    : public GenericReplyBase<DataType, ErrorType, GenericReply> {
 public:
  explicit GenericReply(IReply* reply, QObject* parent = nullptr);

  template <typename Handler>
  GenericReply<DataType, ErrorType>* onSucceeded(Handler&& handler,
                                                 QObject* scope = nullptr);

  template <typename Handler>
  GenericReply<DataType, ErrorType>* onFailed(Handler&& handler,
                                              QObject* scope = nullptr);
};

template <typename DataType, typename ErrorType>
GenericReply<DataType, ErrorType>::GenericReply(IReply* reply, QObject* parent)
    : GenericReplyBase<DataType, ErrorType, GenericReply>(reply, parent) {}

template <typename DataType, typename ErrorType>
template <typename Handler>
GenericReply<DataType, ErrorType>*
GenericReply<DataType, ErrorType>::onSucceeded(Handler&& handler,
                                               QObject* scope) {
  WrappedReply::onSucceeded(
      [this, xFn = core::utils::bindCallback<void(int, const DataType&)>(
                 std::forward<Handler>(handler))](int http_code,
                                                  const Data& data) {
        DataSerializer* serializer = this->getDataSerializer();
        xFn(http_code, serializer->deserialize<DataType>(data));
      },
      scope);

  return this;
}

template <typename DataType, typename ErrorType>
template <typename Handler>
GenericReply<DataType, ErrorType>* GenericReply<DataType, ErrorType>::onFailed(
    Handler&& handler, QObject* scope) {
  WrappedReply::onFailed(
      [this, xFn = core::utils::bindCallback<void(int, const ErrorType&)>(
                 std::forward<Handler>(handler))](int http_code,
                                                  const Data& data) {
        DataSerializer* serializer = this->getDataSerializer();
        xFn(http_code, serializer->deserialize<ErrorType>(data));
      },
      scope);

  return this;
}

/* ------------------------ GenericReply<DataType, void> -------------------- */

template <typename DataType>
class GenericReply<DataType, void>
    : public GenericReplyBase<DataType, void, GenericReply> {
 public:
  explicit GenericReply(IReply* reply, QObject* parent = nullptr);

  template <typename Handler>
  GenericReply<DataType, void>* onSucceeded(Handler&& handler,
                                            QObject* scope = nullptr);

  template <typename Handler>
  GenericReply<DataType, void>* onFailed(Handler&& handler,
                                         QObject* scope = nullptr);
};

template <typename DataType>
GenericReply<DataType, void>::GenericReply(IReply* reply, QObject* parent)
    : GenericReplyBase<DataType, void, GenericReply>(reply, parent) {}

template <typename DataType>
template <typename Handler>
GenericReply<DataType, void>* GenericReply<DataType, void>::onSucceeded(
    Handler&& handler, QObject* scope) {
  WrappedReply::onSucceeded(
      [this, xFn = core::utils::bindCallback<void(int, const DataType&)>(
                 std::forward<Handler>(handler))](int http_code,
                                                  const Data& data) {
        DataSerializer* serializer = this->getDataSerializer();
        xFn(http_code, serializer->deserialize<DataType>(data));
      },
      scope);

  return this;
}

template <typename DataType>
template <typename Handler>
GenericReply<DataType, void>* GenericReply<DataType, void>::onFailed(
    Handler&& handler, QObject* scope) {
  WrappedReply::onFailed(
      core::utils::bindCallback<void(int)>(std::forward<Handler>(handler)),
      scope);

  return this;
}

/* ----------------------- GenericReply<void, ErrorType> -------------------- */

template <typename ErrorType>
class GenericReply<void, ErrorType>
    : public GenericReplyBase<void, ErrorType, GenericReply> {
 public:
  explicit GenericReply(IReply* reply, QObject* parent = nullptr);

  template <typename Handler>
  GenericReply<void, ErrorType>* onSucceeded(Handler&& handler,
                                             QObject* scope = nullptr);

  template <typename Handler>
  GenericReply<void, ErrorType>* onFailed(Handler&& handler,
                                          QObject* scope = nullptr);
};

template <typename ErrorType>
GenericReply<void, ErrorType>::GenericReply(IReply* reply, QObject* parent)
    : GenericReplyBase<void, ErrorType, GenericReply>(reply, parent) {}

template <typename ErrorType>
template <typename Handler>
GenericReply<void, ErrorType>* GenericReply<void, ErrorType>::onSucceeded(
    Handler&& handler, QObject* scope) {
  WrappedReply::onSucceeded(
      core::utils::bindCallback<void(int)>(std::forward<Handler>(handler)),
      scope);

  return this;
}

template <typename ErrorType>
template <typename Handler>
GenericReply<void, ErrorType>* GenericReply<void, ErrorType>::onFailed(
    Handler&& handler, QObject* scope) {
  WrappedReply::onFailed(
      [this, xFn = core::utils::bindCallback<void(int, const ErrorType&)>(
                 std::forward<Handler>(handler))](int http_code,
                                                  const Data& data) {
        DataSerializer* serializer = this->getDataSerializer();
        xFn(http_code, serializer->deserialize<ErrorType>(data));
      },
      scope);

  return this;
}

/* -------------------------------- ILoggerReply ---------------------------- */

class ILoggerReply : public WrappedReply {
  Q_OBJECT

 public:
  enum LogDetail {
    LogCompleted = 1 << 0,
    LogSucceeded = 1 << 1,
    LogFailed = 1 << 2,
    LogError = 1 << 3,
    LogDownloadProgress = 1 << 4,
    LogUploadProgress = 1 << 5,

    LogOnlyResult = LogSucceeded | LogFailed | LogError,
    LogAll = LogCompleted | LogSucceeded | LogFailed | LogError |
             LogDownloadProgress | LogUploadProgress
  };
  Q_ENUM(LogDetail)

 public:
  explicit ILoggerReply(IReply* reply, QObject* parent = nullptr);
  ~ILoggerReply() override;

  virtual void setLogDetail(LogDetail log_detail) = 0;
  [[nodiscard]] virtual LogDetail getLogDetail() const = 0;

 private:
  void logCompleted(int http_code, const Data& data);
  void logSucceeded(int http_code, const Data& data);
  void logFailed(int http_code, const Data& data);
  void logError(const QString& error_str, Error error_type);

  void logDownloadProgress(qint64 bytes_received, qint64 bytes_total);
  void logUploadProgress(qint64 bytes_sent, qint64 bytes_total);

  void logTheme(const QString& action);
  void logData(const Data& data);

  virtual void flush() = 0;
  virtual void write(QStringView text) = 0;
};

/* -------------------------------- LoggerReply ----------------------------- */

template <typename Logger>
class LoggerReply : public ILoggerReply {
 public:
  explicit LoggerReply(std::unique_ptr<Logger> logger, IReply* reply,
                       LogDetail log_detail = LogDetail::LogAll,
                       QObject* parent = nullptr);
  ~LoggerReply() override;

  void setLogDetail(LogDetail log_detail) override;
  [[nodiscard]] LogDetail getLogDetail() const override;

 private:
  void flush() override;
  void write(QStringView text) override;

 private:
  std::unique_ptr<Logger> m_logger;
  LogDetail m_log_detail;
};

template <typename Logger>
LoggerReply<Logger>::LoggerReply(std::unique_ptr<Logger> logger, IReply* reply,
                                 LogDetail log_detail, QObject* parent)
    : ILoggerReply(reply, parent),
      m_logger(std::move(logger)),
      m_log_detail(log_detail) {}

template <typename Logger>
LoggerReply<Logger>::~LoggerReply() = default;

template <typename Logger>
void LoggerReply<Logger>::setLogDetail(LogDetail log_detail) {
  m_log_detail = log_detail;
}

template <typename Logger>
ILoggerReply::LogDetail LoggerReply<Logger>::getLogDetail() const {
  return m_log_detail;
}

template <typename Logger>
void LoggerReply<Logger>::flush() {
  m_logger->flush();
}

template <typename Logger>
void LoggerReply<Logger>::write(QStringView text) {
  *m_logger << text;
}

}  // namespace egnite::rest

#endif  // EGNITE_REST_REPLY_H