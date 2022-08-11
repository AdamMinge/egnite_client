#ifndef EGNITE_REST_REPLY_H
#define EGNITE_REST_REPLY_H

/* ------------------------------------ Qt ---------------------------------- */
#include <QCborValue>
#include <QJsonValue>
#include <QNetworkReply>
#include <QObject>
#include <QPointer>
/* ---------------------------------- Standard ------------------------------ */
#include <variant>
/* ----------------------------------- Local -------------------------------- */
#include "egnite/export.h"
#include "egnite/utils/binder.h"
/* -------------------------------------------------------------------------- */

namespace egnite::rest {

class RestReplyPrivate;

class EGNITE_API RestReply : public QObject {
  Q_OBJECT

 public:
  enum class Error { Network, Parser, Failure };
  Q_ENUM(Error)

  using DataType = std::variant<std::nullopt_t, QJsonValue, QCborValue>;

 public:
  ~RestReply() override;

  void abort();
  void retry();
  void retry(std::chrono::milliseconds msecs);

  template <typename HANDLER>
  RestReply* onCompleted(HANDLER&& handler, QObject* scope = nullptr);
  template <typename HANDLER>
  RestReply* onSucceeded(HANDLER&& handler, QObject* scope = nullptr);
  template <typename HANDLER>
  RestReply* onFailed(HANDLER&& handler, QObject* scope = nullptr);
  template <typename HANDLER>
  RestReply* onError(HANDLER&& handler, QObject* scope = nullptr);

 Q_SIGNALS:
  void completed(int http_code, const DataType& data, QPrivateSignal);
  void succeeded(int http_code, const DataType& data, QPrivateSignal);
  void failed(int http_code, const DataType& data, QPrivateSignal);
  void error(const QString& error_str, Error error_type, QPrivateSignal);

  void downloadProgress(qint64 bytes_received, qint64 bytes_total);
  void uploadProgress(qint64 bytes_sent, qint64 bytes_total);

 protected:
  RestReply(QNetworkReply* network_reply, QObject* parent = nullptr);
  RestReply(RestReplyPrivate& impl, QObject* parent = nullptr);

 private:
  Q_DECLARE_PRIVATE(RestReply)
  Q_PRIVATE_SLOT(d_func(), void replyFinished())
  Q_PRIVATE_SLOT(d_func(), void retryReply())
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