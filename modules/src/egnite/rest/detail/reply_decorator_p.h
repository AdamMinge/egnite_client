#ifndef EGNITE_REPLY_DECORATOR_P_H
#define EGNITE_REPLY_DECORATOR_P_H

/* ------------------------------------ Qt ---------------------------------- */
#include <QtCore/private/qobject_p.h>

#include <QList>
/* ------------------------------------ Local ------------------------------- */
#include "egnite/rest/reply_decorator.h"
/* -------------------------------------------------------------------------- */

namespace egnite::rest::detail {

/* ---------------------------- ReplyDecoratorPrivate ----------------------- */

class ReplyDecoratorPrivate : public QObjectPrivate {
 public:
  Q_DECLARE_PUBLIC(ReplyDecorator)

 public:
  explicit ReplyDecoratorPrivate();

  IReply* decorate(IReply* reply) const;

  IReplyFactory* at(qsizetype i) const;
  qsizetype count() const;
  void clear();

  void append(IReplyFactory* factory);
  void prepend(IReplyFactory* factory);
  void remove(IReplyFactory* factory);

 private:
  QList<IReplyFactory*> m_factories;
};

}  // namespace egnite::rest::detail

#endif  // EGNITE_REPLY_DECORATOR_P_H