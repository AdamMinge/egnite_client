#ifndef EGNITE_REPLY_DECORATOR_P_H
#define EGNITE_REPLY_DECORATOR_P_H

/* ------------------------------------ Qt ---------------------------------- */
#include <QtCore/private/qobject_p.h>
/* ---------------------------------- Standard ------------------------------ */
#include <map>
#include <vector>
/* ------------------------------------ Local ------------------------------- */
#include "egnite/rest/reply_decorator_manager.h"
/* -------------------------------------------------------------------------- */

namespace egnite::rest {

class Client;

namespace detail {

using ReplyDecoratorContainer = std::multimap<uint32_t, ReplyDecorator*>;

class ReplyDecoratorManagerPrivate : public QObjectPrivate {
 public:
  Q_DECLARE_PUBLIC(ReplyDecoratorManager)

 public:
  explicit ReplyDecoratorManagerPrivate();

  Reply* decorate(Reply* reply) const;

  void addDecorator(ReplyDecorator* decorator, uint32_t priority);
  void removeDecorator(ReplyDecorator* decorator);
  void removeAllDecorators();

 private:
  ReplyDecoratorContainer m_decorators;
};

class ReplyLogDecoratorPrivate {
 public:
  explicit ReplyLogDecoratorPrivate(ReplyLogDecorator::Options options);

  [[nodiscard]] Reply* decorate(Reply* reply) const;

  void setOptions(ReplyLogDecorator::Options options);
  [[nodiscard]] ReplyLogDecorator::Options getOptions() const;

 private:
  static void onSucceeded(Reply* reply, int http_code, const Data& data);
  static void onFailed(Reply* reply, int http_code, const Data& data);
  static void onError(Reply* reply, const QString& err_str,
                      Reply::Error err_type);
  static void onDownloadProgress(Reply* reply, qint64 bytes_received,
                                 qint64 bytes_total);
  static void onUploadProgress(Reply* reply, qint64 bytes_received,
                               qint64 bytes_total);

 private:
  ReplyLogDecorator::Options m_options;
};

}  // namespace detail

}  // namespace egnite::rest

#endif  // EGNITE_REPLY_DECORATOR_P_H