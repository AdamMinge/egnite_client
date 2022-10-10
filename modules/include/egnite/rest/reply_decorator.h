#ifndef EGNITE_REPLY_DECORATOR_H
#define EGNITE_REPLY_DECORATOR_H

/* ------------------------------------ Qt ---------------------------------- */
#include <QObject>
/* ----------------------------------- Local -------------------------------- */
#include "egnite/rest/export.h"
/* -------------------------------------------------------------------------- */

namespace egnite::rest {

namespace detail {
class ReplyDecoratorPrivate;
}  // namespace detail

class Reply;
class ReplyDecoratorFactory;

class EGNITE_REST_API ReplyDecorator : public QObject {
  Q_OBJECT

 public:
  explicit ReplyDecorator(QObject* parent = nullptr);
  ~ReplyDecorator() override;

  [[nodiscard]] Reply* decorate(Reply* reply) const;

  ReplyDecoratorFactory* at(qsizetype i) const;
  qsizetype count() const;
  void clear();

  void append(ReplyDecoratorFactory* factory);
  void prepend(ReplyDecoratorFactory* factory);
  void remove(ReplyDecoratorFactory* factory);

 protected:
  ReplyDecorator(detail::ReplyDecoratorPrivate& impl,
                 QObject* parent = nullptr);

 private:
  Q_DECLARE_PRIVATE(detail::ReplyDecorator);
};

}  // namespace egnite::rest

#endif  // EGNITE_REPLY_DECORATOR_H