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
  GenericRestReplyBase<DataType, ErrorType>* onCompleted(
      HANDLER&& handler, QObject* scope = nullptr);
  GenericRestReplyBase<DataType, ErrorType>* onError(HANDLER&& handler,
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
GenericRestReplyBase<DataType, ErrorType>*
GenericRestReplyBase<DataType, ErrorType>::onCompleted(HANDLER&& handler,
                                                       QObject* scope) {
  onCompletedImpl(
      utils::bindCallback<decltype(&GenericRestReplyBase::onCompletedImpl)>(
          std::forward<HANDLER>(handler)),
      scope);
  return this;
}

template <typename DataType, typename ErrorType>
GenericRestReplyBase<DataType, ErrorType>*
GenericRestReplyBase<DataType, ErrorType>::onError(HANDLER&& handler,
                                                   QObject* scope) {
  onErrorImpl(utils::bindCallback<decltype(&GenericRestReplyBase::onErrorImpl)>(
                  std::forward<HANDLER>(handler)),
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
  GenericRestReply<DataType, ErrorType>* onSucceeded(HANDLER&& handler,
                                                     QObject* scope = nullptr);
  GenericRestReply<DataType, ErrorType>* onFailed(HANDLER&& handler,
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
GenericRestReply<DataType, ErrorType>*
GenericRestReplyBase<DataType, ErrorType>::onSucceeded(HANDLER&& handler,
                                                       QObject* scope) {
  onSucceededImpl(
      utils::bindCallback<decltype(&GenericRestReplyBase::onSucceededImpl)>(
          std::forward<HANDLER>(handler)),
      scope);
  return this;
}

template <typename DataType, typename ErrorType>
GenericRestReply<DataType, ErrorType>*
GenericRestReplyBase<DataType, ErrorType>::onFailed(HANDLER&& handler,
                                                    QObject* scope) {
  onFailedImpl(
      utils::bindCallback<decltype(&GenericRestReplyBase::onFailedImpl)>(
          std::forward<HANDLER>(handler)),
      scope);
  return this;
}

template <typename DataType, typename ErrorType>
void GenericRestReplyBase<DataType, ErrorType>::onSucceededImpl(
    std::function<void(int, const DataType&)> handler, QObject* scope) {
  RestReply::onSucceeded(
      [this, xFn = std::move(handler)](int http_code, const RestData& data) {
        std::visit(detail::overloaded{
                       [&](std::nullopt_t) { xFn(http_code, DataType{}); },
                       [&](auto data) {}},
                   data)
      },
      scope);
}

template <typename DataType, typename ErrorType>
void GenericRestReplyBase<DataType, ErrorType>::onFailedImpl(
    std::function<void(int, const ErrorType&)> handler, QObject* scope) {
  RestReply::onFailed(std::visit(
      detail::overloaded{[&](std::nullopt_t) { xFn(http_code, ErrorType{}); },
                         [&](auto data) {}},
      data) scope);
}

}  // namespace egnite::rest

#endif  // EGNITE_REST_GENERIC_REPLY_H