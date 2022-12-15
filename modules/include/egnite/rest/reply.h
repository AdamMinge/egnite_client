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

/* -------------------------------- LoggerReply ----------------------------- */

template <typename Logger>
class LoggerReply : public WrappedReply {
 public:
  explicit LoggerReply(std::unique_ptr<Logger> logger, IReply* reply,
                       QObject* parent = nullptr);
  ~LoggerReply() override;

 protected:
  void logCompleted(int http_code, const Data& data);
  void logSucceeded(int http_code, const Data& data);
  void logFailed(int http_code, const Data& data);
  void logError(const QString& error_str, Error error_type);

  void logDownloadProgress(qint64 bytes_received, qint64 bytes_total);
  void logUploadProgress(qint64 bytes_sent, qint64 bytes_total);

 private:
  void logTheme(const QString& action);
  void logData(const Data& data);
  void flush();

 private:
  std::unique_ptr<Logger> m_logger;
};

template <typename Logger>
LoggerReply<Logger>::LoggerReply(std::unique_ptr<Logger> logger, IReply* reply,
                                 QObject* parent)
    : WrappedReply(reply, parent), m_logger(std::move(logger)) {
  onCompleted([this](int code, const Data& data) {
    logCompleted(code, data);
    flush();
  });
  onSucceeded([this](int code, const Data& data) {
    logSucceeded(code, data);
    flush();
  });
  onFailed([this](int code, const Data& data) {
    logFailed(code, data);
    flush();
  });
  onError([this](const QString& str, Error type) {
    logError(str, type);
    flush();
  });
  onDownloadProgress([this](qint64 r, qint64 t) {
    logDownloadProgress(r, t);
    flush();
  });
  onUploadProgress([this](qint64 s, qint64 t) {
    logUploadProgress(s, t);
    flush();
  });
}

template <typename Logger>
LoggerReply<Logger>::~LoggerReply() = default;

template <typename Logger>
void LoggerReply<Logger>::logCompleted(int http_code, const Data& data) {
  logTheme("Completed");
  *m_logger << "\t" << QString("http_code: %1").arg(http_code) << "\n";
  logData(data);
}

template <typename Logger>
void LoggerReply<Logger>::logSucceeded(int http_code, const Data& data) {
  logTheme("Succeeded");
  *m_logger << "\t" << QString("http_code: %1").arg(http_code) << "\n";
  logData(data);
}

template <typename Logger>
void LoggerReply<Logger>::logFailed(int http_code, const Data& data) {
  logTheme("Failed");
  *m_logger << "\t" << QString("http code: %1").arg(http_code) << "\n";
  logData(data);
}

template <typename Logger>
void LoggerReply<Logger>::logError(const QString& error_str, Error error_type) {
  logTheme("Error");
  *m_logger << "\t" << QString("detail: %1").arg(error_str) << "\n"
            << "\t" << QString("type: %1").arg(static_cast<size_t>(error_type))
            << "\n";
}

template <typename Logger>
void LoggerReply<Logger>::logDownloadProgress(qint64 bytes_received,
                                              qint64 bytes_total) {
  logTheme("Download Progress");
  *m_logger << "\t" << QString("bytes received: %1").arg(bytes_received) << "\n"
            << "\t" << QString("bytes total: %1").arg(bytes_total) << "\n";
}

template <typename Logger>
void LoggerReply<Logger>::logUploadProgress(qint64 bytes_sent,
                                            qint64 bytes_total) {
  logTheme("Upload Progress");
  *m_logger << "\t" << QString("bytes sent: %1").arg(bytes_sent) << "\n"
            << "\t" << QString("bytes total: %1").arg(bytes_total) << "\n";
}

template <typename Logger>
void LoggerReply<Logger>::logTheme(const QString& action) {
  *m_logger << QString("%1 Reply ").arg(action) << "[" << this << "]: \n";
}

template <typename Logger>
void LoggerReply<Logger>::logData(const Data& data) {
  *m_logger << "\t"
            << "data: ";
  std::visit(
      core::utils::overloaded{[this](std::nullopt_t) { *m_logger << "null"; },
                              [this](const auto& body) {
                                *m_logger << convertDataToByteArray(body);
                              }},
      data);
  *m_logger << "\n";
}

template <typename Logger>
void LoggerReply<Logger>::flush() {
  m_logger->flush();
}

}  // namespace egnite::rest

#endif  // EGNITE_REST_REPLY_H