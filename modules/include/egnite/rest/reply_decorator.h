#ifndef EGNITE_REPLY_DECORATOR_MANAGER_H
#define EGNITE_REPLY_DECORATOR_MANAGER_H

/* ------------------------------------ Qt ---------------------------------- */
#include <QObject>
/* ---------------------------------- Standard ------------------------------ */
#include <memory>
/* ----------------------------------- Local -------------------------------- */
#include "egnite/rest/export.h"
#include "egnite/rest/reply.h"
/* -------------------------------------------------------------------------- */

namespace egnite::rest {

namespace detail {
class ReplyDecoratorPrivate;
}  // namespace detail

class EGNITE_REST_API ReplyDecoratorFactory : public QObject {
  Q_OBJECT

 public:
  ~ReplyDecoratorFactory() override;

  [[nodiscard]] virtual Reply* create(Reply* reply) = 0;

 protected:
  explicit ReplyDecoratorFactory(QObject* parent = nullptr);
  explicit ReplyDecoratorFactory(QObjectPrivate& impl,
                                 QObject* parent = nullptr);
};

template <typename Type, typename... Args>
requires std::is_base_of_v<Reply, Type>
class EGNITE_REST_API ReplyDecoratorFactoryWithArgs
    : public ReplyDecoratorFactory {
 public:
  ReplyDecoratorFactoryWithArgs(Args... args, QObject* parent = nullptr)
      : ReplyDecoratorFactory(parent),
        m_values(std::make_tuple(std::move(args)...)) {}
  ~ReplyDecoratorFactoryWithArgs() override = default;

  [[nodiscard]] Reply* create(Reply* reply) override {
    auto decorated_reply = std::apply(
        [reply](const Args&... args) {
          return new Type(args..., reply, reply->parent());
        },
        m_values);

    reply->setParent(decorated_reply);
    return decorated_reply;
  }

 private:
  std::tuple<Args...> m_values;
};

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

#endif  // EGNITE_REPLY_DECORATOR_MANAGER_H