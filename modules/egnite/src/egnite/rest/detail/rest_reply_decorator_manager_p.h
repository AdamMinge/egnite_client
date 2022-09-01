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
  RestReplyDecoratorManagerPrivate();

  RestReply* decorate(RestReply* reply) const;

  void addDecorator(const QString& name,
                    RestReplyDecoratorManager::Decorator decorator);
  void removeDecorator(const QString& name);
  void removeAllDecorators();

 private:
  QHash<QString, RestReplyDecoratorManager::Decorator> m_decorators;
};

}  // namespace detail

}  // namespace egnite::rest

#endif  // EGNITE_REPLY_DECORATOR_P_H