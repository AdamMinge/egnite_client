#ifndef EGNITE_REPLY_DECORATOR_P_H
#define EGNITE_REPLY_DECORATOR_P_H

/* ------------------------------------ Qt ---------------------------------- */
#include <QtCore/private/qobject_p.h>

#include <QHash>
/* ------------------------------------ Local ------------------------------- */
#include "egnite/rest/rest_reply_decorator_manager.h"
/* -------------------------------------------------------------------------- */

namespace egnite::rest {

class RestClient;

namespace detail {

class RestReplyDecoratorManagerPrivate : public QObjectPrivate {
 public:
  Q_DECLARE_PUBLIC(RestReplyDecoratorManager)

 public:
  explicit RestReplyDecoratorManagerPrivate();

  RestReply* decorate(RestReply* reply) const;

  void addDecorator(const QString& name,
                    RestReplyDecoratorManager::Decorator decorator);
  void removeDecorator(const QString& name);
  void removeAllDecorators();

 private:
  QHash<QString, RestReplyDecoratorManager::Decorator> m_decorators;
};

class RestReplyLogDecoratorPrivate {
 public:
  explicit RestReplyLogDecoratorPrivate(RestReplyLogDecorator::Options options);

  [[nodiscard]] RestReply* decorate(RestReply* reply) const;

  void setOptions(RestReplyLogDecorator::Options options);
  [[nodiscard]] RestReplyLogDecorator::Options getOptions() const;

 private:
  static void onSucceeded(RestReply* reply, int http_code,
                          const RestData& data);
  static void onFailed(RestReply* reply, int http_code, const RestData& data);
  static void onError(RestReply* reply, const QString& err_str,
                      RestReply::Error err_type);
  static void onDownloadProgress(RestReply* reply, qint64 bytes_received,
                                 qint64 bytes_total);
  static void onUploadProgress(RestReply* reply, qint64 bytes_received,
                               qint64 bytes_total);

 private:
  RestReplyLogDecorator::Options m_options;
};

}  // namespace detail

}  // namespace egnite::rest

#endif  // EGNITE_REPLY_DECORATOR_P_H