/* ----------------------------------- Local -------------------------------- */
#include "egnite/rest/rest_reply_decorator_manager.h"

#include "egnite/rest/detail/rest_reply_decorator_manager_p.h"
/* -------------------------------------------------------------------------- */

namespace egnite::rest {

/* ------------------------ RestReplyDecoratorManager------------------------ */

RestReplyDecoratorManager::RestReplyDecoratorManager(QObject* parent)
    : RestReplyDecoratorManager(*new detail::RestReplyDecoratorManagerPrivate(),
                                parent) {}

RestReplyDecoratorManager::~RestReplyDecoratorManager() = default;

RestReplyDecoratorManager::RestReplyDecoratorManager(
    detail::RestReplyDecoratorManagerPrivate& impl, QObject* parent)
    : QObject(impl, parent) {}

RestReply* RestReplyDecoratorManager::decorate(RestReply* reply) const {
  Q_D(const detail::RestReplyDecoratorManager);
  return d->decorate(reply);
}

void RestReplyDecoratorManager::addDecorator(const QString& name,
                                             Decorator decorator) {
  Q_D(detail::RestReplyDecoratorManager);
  d->addDecorator(name, decorator);
}

void RestReplyDecoratorManager::removeDecorator(const QString& name) {
  Q_D(detail::RestReplyDecoratorManager);
  d->removeDecorator(name);
}

void RestReplyDecoratorManager::removeAllDecorators() {
  Q_D(detail::RestReplyDecoratorManager);
  d->removeAllDecorators();
}

/* -------------------- RestReplyDecoratorManagerPrivate -------------------- */

namespace detail {

RestReplyDecoratorManagerPrivate::RestReplyDecoratorManagerPrivate() {}

RestReply* RestReplyDecoratorManagerPrivate::decorate(RestReply* reply) const {
  for (auto& decorator : m_decorators) reply = decorator(reply);
  return reply;
}

void RestReplyDecoratorManagerPrivate::addDecorator(
    const QString& name, RestReplyDecoratorManager::Decorator decorator) {
  m_decorators.insert(name, decorator);
}

void RestReplyDecoratorManagerPrivate::removeDecorator(const QString& name) {
  m_decorators.remove(name);
}

void RestReplyDecoratorManagerPrivate::removeAllDecorators() {
  m_decorators.clear();
}

}  // namespace detail

}  // namespace egnite::rest

#include "egnite/rest/moc_rest_reply_decorator_manager.cpp"