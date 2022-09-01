#ifndef EGNITE_REPLY_DECORATOR_MANAGER_H
#define EGNITE_REPLY_DECORATOR_MANAGER_H

/* ------------------------------------ Qt ---------------------------------- */
#include <QObject>
/* ----------------------------------- Local -------------------------------- */
#include "egnite/export.h"
/* -------------------------------------------------------------------------- */

namespace egnite::rest {

namespace detail {
class RestReplyDecoratorManagerPrivate;
}

class RestReply;

class EGNITE_API RestReplyDecoratorManager : public QObject {
  Q_OBJECT

 public:
  using Decorator = std::function<RestReply*(RestReply*)>;

 public:
  RestReplyDecoratorManager(QObject* parent = nullptr);
  ~RestReplyDecoratorManager() override;

  RestReply* decorate(RestReply* reply) const;

  void addDecorator(const QString& name, Decorator decorator);
  void removeDecorator(const QString& name);
  void removeAllDecorators();

 protected:
  RestReplyDecoratorManager(detail::RestReplyDecoratorManagerPrivate& impl,
                            QObject* parent = nullptr);

 private:
  Q_DECLARE_PRIVATE(detail::RestReplyDecoratorManager);
};

}  // namespace egnite::rest

#endif  // EGNITE_REPLY_DECORATOR_MANAGER_H