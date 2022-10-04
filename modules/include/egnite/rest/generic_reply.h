#ifndef EGNITE_REST_GENERIC_REPLY_H
#define EGNITE_REST_GENERIC_REPLY_H

/* ----------------------------------- Local -------------------------------- */
#include "egnite/rest/data_serializer.h"
#include "egnite/rest/export.h"
#include "egnite/rest/wrapped_reply.h"
/* -------------------------------------------------------------------------- */

namespace egnite::rest {

/* ----------------------------- GenericReplyBase --------------------------- */

template <typename DataType, typename ErrorType,
          template <typename, typename> typename ChildGenericReply>
class GenericReplyBase : public WrappedReply {
 public:
  template <typename Handler>
  ChildGenericReply<DataType, ErrorType>* onCompleted(Handler&& handler,
                                                      QObject* scope = nullptr);

  template <typename Handler>
  ChildGenericReply<DataType, ErrorType>* onError(Handler&& handler,
                                                  QObject* scope = nullptr);

  EGNITE_DEFINE_BINDER(Reply, onDownloadProgress, downloadProgress);
  EGNITE_DEFINE_BINDER(Reply, onUploadProgress, uploadProgress);

 protected:
  explicit GenericReplyBase(Reply* reply, QObject* parent = nullptr);
  ~GenericReplyBase() override;
};

template <typename DataType, typename ErrorType,
          template <typename, typename> typename ChildGenericReply>
GenericReplyBase<DataType, ErrorType, ChildGenericReply>::GenericReplyBase(
    Reply* reply, QObject* parent)
    : WrappedReply(reply, parent) {}

template <typename DataType, typename ErrorType,
          template <typename, typename> typename ChildGenericReply>
GenericReplyBase<DataType, ErrorType, ChildGenericReply>::~GenericReplyBase() =
    default;

template <typename DataType, typename ErrorType,
          template <typename, typename> typename ChildGenericReply>
template <typename Handler>
ChildGenericReply<DataType, ErrorType>*
GenericReplyBase<DataType, ErrorType, ChildGenericReply>::onCompleted(
    Handler&& handler, QObject* scope) {
  WrappedReply::onCompleted(
      core::utils::bindCallback<void(int)>(std::forward<Handler>(handler)),
      scope);
  return static_cast<ChildGenericReply<DataType, ErrorType>*>(this);
}

template <typename DataType, typename ErrorType,
          template <typename, typename> typename ChildGenericReply>
template <typename Handler>
ChildGenericReply<DataType, ErrorType>*
GenericReplyBase<DataType, ErrorType, ChildGenericReply>::onError(
    Handler&& handler, QObject* scope) {
  WrappedReply::onError(core::utils::bindCallback<void(const QString&, Error)>(
                            std::forward<Handler>(handler)),
                        scope);
  return static_cast<ChildGenericReply<DataType, ErrorType>*>(this);
}

/* --------------------- GenericReply<DataType, ErrorType> ------------------ */

template <typename DataType, typename ErrorType>
class GenericReply
    : public GenericReplyBase<DataType, ErrorType, GenericReply> {
  friend Api;

 public:
  template <typename Handler>
  GenericReply<DataType, ErrorType>* onSucceeded(Handler&& handler,
                                                 QObject* scope = nullptr);

  template <typename Handler>
  GenericReply<DataType, ErrorType>* onFailed(Handler&& handler,
                                              QObject* scope = nullptr);

 protected:
  using GenericReplyBase<DataType, ErrorType, GenericReply>::GenericReplyBase;
};

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
  friend Api;

 public:
  template <typename Handler>
  GenericReply<DataType, void>* onSucceeded(Handler&& handler,
                                            QObject* scope = nullptr);

  template <typename Handler>
  GenericReply<DataType, void>* onFailed(Handler&& handler,
                                         QObject* scope = nullptr);

 protected:
  using GenericReplyBase<DataType, void, GenericReply>::GenericReplyBase;
};

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
  friend Api;

 public:
  template <typename Handler>
  GenericReply<void, ErrorType>* onSucceeded(Handler&& handler,
                                             QObject* scope = nullptr);

  template <typename Handler>
  GenericReply<void, ErrorType>* onFailed(Handler&& handler,
                                          QObject* scope = nullptr);

 protected:
  using GenericReplyBase<void, ErrorType, GenericReply>::GenericReplyBase;
};

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

}  // namespace egnite::rest

#endif  // EGNITE_REST_GENERIC_REPLY_H