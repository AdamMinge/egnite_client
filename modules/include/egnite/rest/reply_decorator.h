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

template <typename Type>
concept ChildReply = std::is_base_of_v<Reply, Type>;

class EGNITE_REST_API ReplyDecorator : public QObject {
  Q_OBJECT

 public:
  explicit ReplyDecorator(QObject* parent = nullptr);
  ~ReplyDecorator() override;

  [[nodiscard]] Reply* decorate(Reply* reply) const;

  template <ChildReply Type, typename... Args>
  void registerFactory(uint32_t priority, Args&&... args);
  template <ChildReply Type>
  void unregisterFactory();

  void unregisterAllFactories();

 protected:
  explicit ReplyDecorator(detail::ReplyDecoratorPrivate& impl,
                          QObject* parent = nullptr);

 private:
  void registerFactory(int id, uint32_t priority,
                       std::function<Reply*(Reply*)> factory);
  void unregisterFactory(int id);

 private:
  Q_DECLARE_PRIVATE(detail::ReplyDecorator);
};

template <ChildReply Type, typename... Args>
void ReplyDecorator::registerFactory(uint32_t priority, Args&&... args) {
  registerFactory(QMetaType::fromType<Type>().id(), priority,
                  [... args = std::forward<Args>(args)](Reply* reply) {
                    auto decorated_reply = new Type(std::forward<Args>(args)...,
                                                    reply, reply->parent());
                    reply->setParent(decorated_reply);
                    return decorated_reply;
                  });
}

template <ChildReply Type>
void ReplyDecorator::unregisterFactory() {
  unregisterFactory(QMetaType::fromType<Type>().id());
}

}  // namespace egnite::rest

#endif  // EGNITE_REPLY_DECORATOR_MANAGER_H