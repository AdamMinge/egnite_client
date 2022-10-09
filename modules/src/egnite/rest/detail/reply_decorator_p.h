#ifndef EGNITE_REPLY_DECORATOR_P_H
#define EGNITE_REPLY_DECORATOR_P_H

/* ------------------------------------ Qt ---------------------------------- */
#include <QtCore/private/qobject_p.h>

#include <QList>
/* ------------------------------------ Local ------------------------------- */
#include "egnite/rest/reply_decorator.h"
/* -------------------------------------------------------------------------- */

namespace egnite::rest::detail {

class ReplyDecoratorPrivate : public QObjectPrivate {
 public:
  Q_DECLARE_PUBLIC(ReplyDecorator)

 public:
  explicit ReplyDecoratorPrivate();

  Reply* decorate(Reply* reply) const;

  ReplyDecoratorFactory* at(qsizetype i) const;
  qsizetype count() const;
  void clear();

  void append(ReplyDecoratorFactory* factory);
  void prepend(ReplyDecoratorFactory* factory);
  void remove(ReplyDecoratorFactory* factory);

 private:
  QList<ReplyDecoratorFactory*> m_factories;
};

}  // namespace egnite::rest::detail

#endif  // EGNITE_REPLY_DECORATOR_P_H