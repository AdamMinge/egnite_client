#ifndef EGNITE_REPLY_FACTORY_H
#define EGNITE_REPLY_FACTORY_H

/* ------------------------------------ Qt ---------------------------------- */
#include <QObject>
/* ----------------------------------- Local -------------------------------- */
#include "egnite/rest/export.h"
#include "egnite/rest/reply.h"
/* -------------------------------------------------------------------------- */

namespace egnite::rest {

/* -------------------------------- IReplyFactory --------------------------- */

class EGNITE_REST_API IReplyFactory : public QObject {
  Q_OBJECT

 public:
  ~IReplyFactory() override;

  [[nodiscard]] virtual IReply* create(IReply* reply) = 0;

 protected:
  explicit IReplyFactory(QObject* parent = nullptr);
  explicit IReplyFactory(QObjectPrivate& impl, QObject* parent = nullptr);
};

/* --------------------------- ReplyFactoryWithArgs ------------------------- */

template <typename Type, typename... Args>
requires std::is_base_of_v<IReply, Type>
class ReplyFactoryWithArgs : public IReplyFactory {
 public:
  ReplyFactoryWithArgs(Args... args, QObject* parent = nullptr)
      : IReplyFactory(parent), m_values(std::make_tuple(std::move(args)...)) {}
  ~ReplyFactoryWithArgs() override = default;

  [[nodiscard]] IReply* create(IReply* reply) override {
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

/* ----------------------------- DebugReplyFactory -------------------------- */

class EGNITE_REST_API DebugReplyFactory : public IReplyFactory {
  Q_OBJECT

 public:
  explicit DebugReplyFactory(QObject* parent);
  ~DebugReplyFactory() override;

  [[nodiscard]] IReply* create(IReply* reply) override;
};

}  // namespace egnite::rest

#endif  // EGNITE_REPLY_FACTORY_H