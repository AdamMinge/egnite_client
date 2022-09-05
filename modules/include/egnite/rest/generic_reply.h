#ifndef EGNITE_REST_GENERIC_REPLY_H
#define EGNITE_REST_GENERIC_REPLY_H

/* ----------------------------------- Local -------------------------------- */
#include "egnite/rest/data_serializer.h"
#include "egnite/rest/export.h"
#include "egnite/rest/reply.h"
/* -------------------------------------------------------------------------- */

namespace egnite::rest {

/* ----------------------------- GenericReplyBase --------------------------- */

template <typename DataType, typename ErrorType>
class GenericReplyBase : public Reply {
 public:
  void abort() override;
  void retry() override;

  [[nodiscard]] Api* getApi() const override;
  [[nodiscard]] Client* getClient() const override;
  [[nodiscard]] DataSerializer* getDataSerializer() const override;

  template <typename Handler>
  GenericReplyBase<DataType, ErrorType>* onCompleted(Handler&& handler,
                                                     QObject* scope = nullptr);

  template <typename Handler>
  GenericReplyBase<DataType, ErrorType>* onError(Handler&& handler,
                                                 QObject* scope = nullptr);

 protected:
  explicit GenericReplyBase(Reply* reply, QObject* parent = nullptr);
  ~GenericReplyBase() override;

 private:
  void onCompletedImpl(std::function<void(int)> handler,
                       QObject* scope = nullptr);
  void onErrorImpl(std::function<void(const QString&, Error)> handler,
                   QObject* scope = nullptr);

 protected:
  Reply* m_reply;
};

template <typename DataType, typename ErrorType>
GenericReplyBase<DataType, ErrorType>::GenericReplyBase(Reply* reply,
                                                        QObject* parent)
    : Reply(parent), m_reply(reply) {
  connect(m_reply, &Reply::completed, this, &Reply::completed);
  connect(m_reply, &Reply::succeeded, this, &Reply::succeeded);
  connect(m_reply, &Reply::failed, this, &Reply::failed);
  connect(m_reply, &Reply::error, this, &Reply::error);

  connect(m_reply, &Reply::downloadProgress, this, &Reply::downloadProgress);
  connect(m_reply, &Reply::uploadProgress, this, &Reply::uploadProgress);
}

template <typename DataType, typename ErrorType>
GenericReplyBase<DataType, ErrorType>::~GenericReplyBase() {
  m_reply->deleteLater();
}

template <typename DataType, typename ErrorType>
void GenericReplyBase<DataType, ErrorType>::abort() {
  m_reply->abort();
}

template <typename DataType, typename ErrorType>
void GenericReplyBase<DataType, ErrorType>::retry() {
  m_reply->retry();
}

template <typename DataType, typename ErrorType>
Api* GenericReplyBase<DataType, ErrorType>::getApi() const {
  return m_reply->getApi();
}

template <typename DataType, typename ErrorType>
Client* GenericReplyBase<DataType, ErrorType>::getClient() const {
  return m_reply->getClient();
}

template <typename DataType, typename ErrorType>
DataSerializer* GenericReplyBase<DataType, ErrorType>::getDataSerializer()
    const {
  return m_reply->getDataSerializer();
}

template <typename DataType, typename ErrorType>
template <typename Handler>
GenericReplyBase<DataType, ErrorType>*
GenericReplyBase<DataType, ErrorType>::onCompleted(Handler&& handler,
                                                   QObject* scope) {
  onCompletedImpl(
      core::utils::bindCallback<decltype(&GenericReplyBase::onCompletedImpl)>(
          std::forward<Handler>(handler)),
      scope);
  return this;
}

template <typename DataType, typename ErrorType>
template <typename Handler>
GenericReplyBase<DataType, ErrorType>*
GenericReplyBase<DataType, ErrorType>::onError(Handler&& handler,
                                               QObject* scope) {
  onErrorImpl(
      core::utils::bindCallback<decltype(&GenericReplyBase::onErrorImpl)>(
          std::forward<Handler>(handler)),
      scope);
  return this;
}

template <typename DataType, typename ErrorType>
void GenericReplyBase<DataType, ErrorType>::onCompletedImpl(
    std::function<void(int)> handler, QObject* scope) {
  m_reply->onCompleted(std::move(handler), scope);
}

template <typename DataType, typename ErrorType>
void GenericReplyBase<DataType, ErrorType>::onErrorImpl(
    std::function<void(const QString&, Error)> handler, QObject* scope) {
  m_reply->onError(std::move(handler), scope);
}

/* --------------------- GenericReply<DataType, ErrorType> ------------------ */

template <typename DataType, typename ErrorType>
class GenericReply : public GenericReplyBase<DataType, ErrorType> {
  friend Api;

 public:
  template <typename Handler>
  GenericReply<DataType, ErrorType>* onSucceeded(Handler&& handler,
                                                 QObject* scope = nullptr);

  template <typename Handler>
  GenericReply<DataType, ErrorType>* onFailed(Handler&& handler,
                                              QObject* scope = nullptr);

 protected:
  using GenericReplyBase<DataType, ErrorType>::GenericReplyBase;

 private:
  void onSucceededImpl(std::function<void(int, const DataType&)> handler,
                       QObject* scope = nullptr);
  void onFailedImpl(std::function<void(int, const ErrorType&)> handler,
                    QObject* scope = nullptr);
};

template <typename DataType, typename ErrorType>
template <typename Handler>
GenericReply<DataType, ErrorType>*
GenericReply<DataType, ErrorType>::onSucceeded(Handler&& handler,
                                               QObject* scope) {
  onSucceededImpl(
      core::utils::bindCallback<decltype(&GenericReply::onSucceededImpl)>(
          std::forward<Handler>(handler)),
      scope);
  return this;
}

template <typename DataType, typename ErrorType>
template <typename Handler>
GenericReply<DataType, ErrorType>* GenericReply<DataType, ErrorType>::onFailed(
    Handler&& handler, QObject* scope) {
  onFailedImpl(core::utils::bindCallback<decltype(&GenericReply::onFailedImpl)>(
                   std::forward<Handler>(handler)),
               scope);
  return this;
}

template <typename DataType, typename ErrorType>
void GenericReply<DataType, ErrorType>::onSucceededImpl(
    std::function<void(int, const DataType&)> handler, QObject* scope) {
  this->m_reply->onSucceeded(
      [this, xFn = std::move(handler)](int http_code, const Data& data) {
        DataSerializer* serializer = this->getDataSerializer();
        xFn(http_code, serializer->deserialize<DataType>(data));
      },
      scope);
}

template <typename DataType, typename ErrorType>
void GenericReply<DataType, ErrorType>::onFailedImpl(
    std::function<void(int, const ErrorType&)> handler, QObject* scope) {
  this->m_reply->onFailed(
      [this, xFn = std::move(handler)](int http_code, const Data& data) {
        DataSerializer* serializer = this->getDataSerializer();
        xFn(http_code, serializer->deserialize<ErrorType>(data));
      },
      scope);
}

}  // namespace egnite::rest

#endif  // EGNITE_REST_GENERIC_REPLY_H