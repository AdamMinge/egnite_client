#ifndef EGNITE_REPLY_DECORATOR_FACTORY_H
#define EGNITE_REPLY_DECORATOR_FACTORY_H

/* ------------------------------------ Qt ---------------------------------- */
#include <QObject>
/* ----------------------------------- Local -------------------------------- */
#include "egnite/rest/export.h"
#include "egnite/rest/reply.h"
/* -------------------------------------------------------------------------- */

namespace egnite::rest {

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
class ReplyDecoratorFactoryWithArgs : public ReplyDecoratorFactory {
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

}  // namespace egnite::rest

#endif  // EGNITE_REPLY_DECORATOR_FACTORY_H