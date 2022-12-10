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
class WrappedReplyDecoratorPrivate;
}  // namespace detail

class IReply;
class IReplyFactory;

/* ------------------------------- IReplyDecorator -------------------------- */

class EGNITE_REST_API IReplyDecorator : public QObject {
  Q_OBJECT

 public:
  ~IReplyDecorator() override;

  [[nodiscard]] virtual IReply* decorate(IReply* reply) const = 0;

  virtual IReplyFactory* at(qsizetype i) const = 0;
  virtual qsizetype count() const = 0;
  virtual void clear() = 0;

  virtual void append(IReplyFactory* factory) = 0;
  virtual void prepend(IReplyFactory* factory) = 0;
  virtual void remove(IReplyFactory* factory) = 0;

 protected:
  explicit IReplyDecorator(QObject* parent = nullptr);
  explicit IReplyDecorator(QObjectPrivate& impl, QObject* parent = nullptr);
};

/* ------------------------------- ReplyDecorator --------------------------- */

class EGNITE_REST_API ReplyDecorator : public IReplyDecorator {
  Q_OBJECT

 public:
  explicit ReplyDecorator(QObject* parent = nullptr);
  ~ReplyDecorator() override;

  [[nodiscard]] IReply* decorate(IReply* reply) const override;

  IReplyFactory* at(qsizetype i) const override;
  qsizetype count() const override;
  void clear() override;

  void append(IReplyFactory* factory) override;
  void prepend(IReplyFactory* factory) override;
  void remove(IReplyFactory* factory) override;

 private:
  Q_DECLARE_PRIVATE(detail::ReplyDecorator);
};

/* -------------------------- WrappedReplyDecorator ------------------------- */

class EGNITE_REST_API WrappedReplyDecorator : public IReplyDecorator {
  Q_OBJECT

 public:
  ~WrappedReplyDecorator() override;

  [[nodiscard]] IReply* decorate(IReply* reply) const override;

  IReplyFactory* at(qsizetype i) const override;
  qsizetype count() const override;
  void clear() override;

  void append(IReplyFactory* factory) override;
  void prepend(IReplyFactory* factory) override;
  void remove(IReplyFactory* factory) override;

 protected:
  explicit WrappedReplyDecorator(IReplyDecorator* reply_decorator,
                                 QObject* parent = nullptr);

 private:
  Q_DECLARE_PRIVATE(detail::WrappedReplyDecorator);
};

}  // namespace egnite::rest

#endif  // EGNITE_REPLY_DECORATOR_H