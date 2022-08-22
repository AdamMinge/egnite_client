#ifndef EGNITE_REST_GENERIC_REPLY_H
#define EGNITE_REST_GENERIC_REPLY_H

/* ----------------------------------- Local -------------------------------- */
#include "egnite/export.h"
#include "egnite/rest/rest_reply.h"
/* -------------------------------------------------------------------------- */

namespace egnite::rest {

/* --------------------------- GenericRestReplyBase ------------------------- */

template <typename DataType, typename ErrorType>
class GenericRestReplyBase : public RestReply {
 public:
  template <typename Handler>
  GenericRestReplyBase<DataType, ErrorType>* onCompleted(
      Handler&& handler, QObject* scope = nullptr);

  template <typename Handler>
  GenericRestReplyBase<DataType, ErrorType>* onError(Handler&& handler,
                                                     QObject* scope = nullptr);

 protected:
  GenericRestReplyBase(RestApi* api, QNetworkReply* network_reply);

 private:
  void onCompletedImpl(std::function<void(int)> handler,
                       QObject* scope = nullptr);
  void onErrorImpl(std::function<void(const QString&, Error)> handler,
                   QObject* scope = nullptr);
}

template <typename DataType, typename ErrorType>
GenericRestReplyBase<DataType, ErrorType>::GenericRestReplyBase(
    RestApi* api, QNetworkReply* network_reply)
    : RestReply(api, network_reply) {
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
  RestReply::onCompleted(std::move(handler), scope);
}

template <typename DataType, typename ErrorType>
void GenericRestReplyBase<DataType, ErrorType>::onErrorImpl(
    std::function<void(const QString&, Error)> handler, QObject* scope) {
  RestReply::onError(std::move(handler), scope);
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
}

template <typename DataType, typename ErrorType>
template <typename Handler>
GenericRestReply<DataType, ErrorType>*
GenericRestReplyBase<DataType, ErrorType>::onSucceeded(Handler&& handler,
                                                       QObject* scope) {
  onSucceededImpl(
      utils::bindCallback<decltype(&GenericRestReplyBase::onSucceededImpl)>(
          std::forward<Handler>(handler)),
      scope);
  return this;
}

template <typename DataType, typename ErrorType>
template <typename Handler>
GenericRestReply<DataType, ErrorType>*
GenericRestReplyBase<DataType, ErrorType>::onFailed(Handler&& handler,
                                                    QObject* scope) {
  onFailedImpl(
      utils::bindCallback<decltype(&GenericRestReplyBase::onFailedImpl)>(
          std::forward<Handler>(handler)),
      scope);
  return this;
}

template <typename DataType, typename ErrorType>
void GenericRestReplyBase<DataType, ErrorType>::onSucceededImpl(
    std::function<void(int, const DataType&)> handler, QObject* scope) {
  RestReply::onSucceeded(
      [this, xFn = std::move(handler)](int http_code, const RestData& data) {
        xFn(http_code,
            getClient()->getSerializer()->deserialize<DataType>(data));
      },
      scope);
}

template <typename DataType, typename ErrorType>
void GenericRestReplyBase<DataType, ErrorType>::onFailedImpl(
    std::function<void(int, const ErrorType&)> handler, QObject* scope) {
  RestReply::onFailed(std::visit(
      xFn(http_code,
          getClient()->getSerializer()->deserialize<ErrorType>(data));
      data) scope);
}

}  // namespace egnite::rest

#endif  // EGNITE_REST_GENERIC_REPLY_H