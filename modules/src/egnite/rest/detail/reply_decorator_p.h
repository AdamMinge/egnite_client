#ifndef EGNITE_REPLY_DECORATOR_P_H
#define EGNITE_REPLY_DECORATOR_P_H

/* ------------------------------------ Qt ---------------------------------- */
#include <QtCore/private/qobject_p.h>
/* ---------------------------------- Standard ------------------------------ */
#include <vector>
/* ------------------------------------ Local ------------------------------- */
#include "egnite/rest/reply_decorator.h"
/* -------------------------------------------------------------------------- */

namespace egnite::rest::detail {

struct ReplyFactory {
  int id;
  uint32_t priority;
  std::function<Reply*(Reply*)> factory;

  Reply* operator()(Reply* reply) const { return factory(reply); }
};

class ReplyDecoratorPrivate : public QObjectPrivate {
 public:
  Q_DECLARE_PUBLIC(ReplyDecorator)

 public:
  explicit ReplyDecoratorPrivate();

  Reply* decorate(Reply* reply) const;

  void registerFactory(int id, uint32_t priority,
                       std::function<Reply*(Reply*)> factory);
  void unregisterFactory(int id);
  void unregisterAllFactories();

 private:
  std::vector<ReplyFactory> m_factories;
};

}  // namespace egnite::rest::detail

#endif  // EGNITE_REPLY_DECORATOR_P_H