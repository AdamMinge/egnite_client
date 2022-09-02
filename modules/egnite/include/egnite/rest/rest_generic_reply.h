#ifndef EGNITE_REST_GENERIC_REPLY_H
#define EGNITE_REST_GENERIC_REPLY_H

/* ----------------------------------- Local -------------------------------- */
#include "egnite/export.h"
#include "egnite/rest/rest_data_serializer.h"
#include "egnite/rest/rest_reply.h"
/* -------------------------------------------------------------------------- */

namespace egnite::rest {

/* --------------------------- GenericRestReplyBase ------------------------- */

template <typename DataType, typename ErrorType>
class GenericRestReplyBase : public RestReply {
 public:
  void abort() override;
  void retry() override;

  [[nodiscard]] RestApi* getApi() const override;
  [[nodiscard]] RestClient* getClient() const override;
  [[nodiscard]] RestDataSerializer* getDataSerializer() const override;

  template <typename Handler>
  GenericRestReplyBase<DataType, ErrorType>* onCompleted(
      Handler&& handler, QObject* scope = nullptr);

  template <typename Handler>
  GenericRestReplyBase<DataType, ErrorType>* onError(Handler&& handler,
                                                     QObject* scope = nullptr);

 protected:
  explicit GenericRestReplyBase(RestReply* reply, QObject* parent = nullptr);
  ~GenericRestReplyBase() override;

 private:
  void onCompletedImpl(std::function<void(int)> handler,
                       QObject* scope = nullptr);
  void onErrorImpl(std::function<void(const QString&, Error)> handler,
                   QObject* scope = nullptr);

 protected:
  RestReply* m_reply;
};

template <typename DataType, typename ErrorType>
GenericRestReplyBase<DataType, ErrorType>::GenericRestReplyBase(
    RestReply* reply, QObject* parent)
    : RestReply(parent), m_reply(reply) {
  connect(m_reply, &RestReply::completed, this, &RestReply::completed);
  connect(m_reply, &RestReply::succeeded, this, &RestReply::succeeded);
  connect(m_reply, &RestReply::failed, this, &RestReply::failed);
  connect(m_reply, &RestReply::error, this, &RestReply::error);

  connect(m_reply, &RestReply::downloadProgress, this,
          &RestReply::downloadProgress);
  connect(m_reply, &RestReply::uploadProgress, this,
          &RestReply::uploadProgress);
}

template <typename DataType, typename ErrorType>
GenericRestReplyBase<DataType, ErrorType>::~GenericRestReplyBase() {
  m_reply->deleteLater();
}

template <typename DataType, typename ErrorType>
void GenericRestReplyBase<DataType, ErrorType>::abort() {
  m_reply->abort();
}

template <typename DataType, typename ErrorType>
void GenericRestReplyBase<DataType, ErrorType>::retry() {
  m_reply->retry();
}

template <typename DataType, typename ErrorType>
RestApi* GenericRestReplyBase<DataType, ErrorType>::getApi() const {
  return m_reply->getApi();
}

template <typename DataType, typename ErrorType>
RestClient* GenericRestReplyBase<DataType, ErrorType>::getClient() const {
  return m_reply->getClient();
}

template <typename DataType, typename ErrorType>
RestDataSerializer*
GenericRestReplyBase<DataType, ErrorType>::getDataSerializer() const {
  return m_reply->getDataSerializer();
}

template <typename DataType, typename ErrorType>
template <typename Handler>
GenericRestReplyBase<DataType, ErrorType>*
GenericRestReplyBase<DataType, ErrorType>::onCompleted(Handler&& handler,
                                                       QObject* scope) {
  onCompletedImpl(
      utils::bindCallback<decltype(&GenericRestReplyBase::onCompletedImpl)>(
          std::forward<Handler>(handler)),
      scope);
  return this;
}

template <typename DataType, typename ErrorType>
template <typename Handler>
GenericRestReplyBase<DataType, ErrorType>*
GenericRestReplyBase<DataType, ErrorType>::onError(Handler&& handler,
                                                   QObject* scope) {
  onErrorImpl(utils::bindCallback<decltype(&GenericRestReplyBase::onErrorImpl)>(
                  std::forward<Handler>(handler)),
              scope);
  return this;
}

template <typename DataType, typename ErrorType>
void GenericRestReplyBase<DataType, ErrorType>::onCompletedImpl(
    std::function<void(int)> handler, QObject* scope) {
  m_reply->onCompleted(std::move(handler), scope);
}

template <typename DataType, typename ErrorType>
void GenericRestReplyBase<DataType, ErrorType>::onErrorImpl(
    std::function<void(const QString&, Error)> handler, QObject* scope) {
  m_reply->onError(std::move(handler), scope);
}

/* ------------------- GenericRestReply<DataType, ErrorType> ---------------- */

template <typename DataType, typename ErrorType>
class GenericRestReply : public GenericRestReplyBase<DataType, ErrorType> {
  friend RestApi;

 public:
  template <typename Handler>
  GenericRestReply<DataType, ErrorType>* onSucceeded(Handler&& handler,
                                                     QObject* scope = nullptr);

  template <typename Handler>
  GenericRestReply<DataType, ErrorType>* onFailed(Handler&& handler,
                                                  QObject* scope = nullptr);

 protected:
  using GenericRestReplyBase<DataType, ErrorType>::GenericRestReplyBase;

 private:
  void onSucceededImpl(std::function<void(int, const DataType&)> handler,
                       QObject* scope = nullptr);
  void onFailedImpl(std::function<void(int, const ErrorType&)> handler,
                    QObject* scope = nullptr);
};

template <typename DataType, typename ErrorType>
template <typename Handler>
GenericRestReply<DataType, ErrorType>*
GenericRestReply<DataType, ErrorType>::onSucceeded(Handler&& handler,
                                                   QObject* scope) {
  onSucceededImpl(
      utils::bindCallback<decltype(&GenericRestReply::onSucceededImpl)>(
          std::forward<Handler>(handler)),
      scope);
  return this;
}

template <typename DataType, typename ErrorType>
template <typename Handler>
GenericRestReply<DataType, ErrorType>*
GenericRestReply<DataType, ErrorType>::onFailed(Handler&& handler,
                                                QObject* scope) {
  onFailedImpl(utils::bindCallback<decltype(&GenericRestReply::onFailedImpl)>(
                   std::forward<Handler>(handler)),
               scope);
  return this;
}

template <typename DataType, typename ErrorType>
void GenericRestReply<DataType, ErrorType>::onSucceededImpl(
    std::function<void(int, const DataType&)> handler, QObject* scope) {
  this->m_reply->onSucceeded(
      [this, xFn = std::move(handler)](int http_code, const RestData& data) {
        RestDataSerializer* serializer = this->getDataSerializer();
        xFn(http_code, serializer->deserialize<DataType>(data));
      },
      scope);
}

template <typename DataType, typename ErrorType>
void GenericRestReply<DataType, ErrorType>::onFailedImpl(
    std::function<void(int, const ErrorType&)> handler, QObject* scope) {
  this->m_reply->onFailed(
      [this, xFn = std::move(handler)](int http_code, const RestData& data) {
        RestDataSerializer* serializer = this->getDataSerializer();
        xFn(http_code, serializer->deserialize<ErrorType>(data));
      },
      scope);
}

}  // namespace egnite::rest

#endif  // EGNITE_REST_GENERIC_REPLY_H