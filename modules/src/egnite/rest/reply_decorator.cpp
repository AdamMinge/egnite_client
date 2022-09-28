/* ----------------------------------- Local -------------------------------- */
#include "egnite/rest/reply_decorator.h"

#include "egnite/rest/detail/api_p.h"
#include "egnite/rest/detail/reply_decorator_p.h"
#include "egnite/rest/reply.h"
/* -------------------------------------------------------------------------- */

namespace egnite::rest {

/* ------------------------------ ReplyDecorator ---------------------------- */

ReplyDecorator::ReplyDecorator(QObject* parent)
    : ReplyDecorator(*new detail::ReplyDecoratorPrivate(), parent) {}

ReplyDecorator::~ReplyDecorator() = default;

ReplyDecorator::ReplyDecorator(detail::ReplyDecoratorPrivate& impl,
                               QObject* parent)
    : QObject(impl, parent) {}

Reply* ReplyDecorator::decorate(Reply* reply) const {
  Q_D(const detail::ReplyDecorator);
  return d->decorate(reply);
}

void ReplyDecorator::registerFactory(int id, uint32_t priority,
                                     std::function<Reply*(Reply*)> factory) {
  Q_D(detail::ReplyDecorator);
  d->registerFactory(id, priority, std::move(factory));
}

void ReplyDecorator::unregisterFactory(int id) {
  Q_D(detail::ReplyDecorator);
  d->unregisterFactory(id);
}

void ReplyDecorator::unregisterAllFactories() {
  Q_D(detail::ReplyDecorator);
  d->unregisterAllFactories();
}

/* -------------------------- ReplyDecoratorPrivate ------------------------- */

namespace detail {

ReplyDecoratorPrivate::ReplyDecoratorPrivate() {}

Reply* ReplyDecoratorPrivate::decorate(Reply* reply) const {
  for (auto& factory : m_factories) reply = factory(reply);
  return reply;
}

void ReplyDecoratorPrivate::registerFactory(
    int id, uint32_t priority, std::function<Reply*(Reply*)> factory) {
  auto reply_factory = ReplyFactory{
      .id = id, .priority = priority, .factory = std::move(factory)};
  m_factories.insert(
      std::upper_bound(m_factories.begin(), m_factories.end(), reply_factory,
                       [](const auto& inserted, const auto& current) {
                         return inserted.priority < current.priority;
                       }),
      std::move(reply_factory));
}

void ReplyDecoratorPrivate::unregisterFactory(int id) {
  std::erase_if(m_factories, [id](const auto& item) { return item.id == id; });
}

void ReplyDecoratorPrivate::unregisterAllFactories() { m_factories.clear(); }

}  // namespace detail

}  // namespace egnite::rest

#include "egnite/rest/moc_reply_decorator.cpp"