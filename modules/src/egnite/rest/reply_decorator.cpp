/* ----------------------------------- Local -------------------------------- */
#include "egnite/rest/reply_decorator.h"

#include "egnite/rest/detail/reply_decorator_p.h"
#include "egnite/rest/reply.h"
#include "egnite/rest/reply_decorator_factory.h"
/* -------------------------------------------------------------------------- */

namespace egnite::rest {

/* ------------------------------ ReplyDecorator ---------------------------- */

ReplyDecorator::ReplyDecorator(QObject* parent)
    : ReplyDecorator(*new detail::ReplyDecoratorPrivate(), parent) {}

ReplyDecorator::ReplyDecorator(detail::ReplyDecoratorPrivate& impl,
                               QObject* parent)
    : QObject(impl, parent) {}

ReplyDecorator::~ReplyDecorator() = default;

Reply* ReplyDecorator::decorate(Reply* reply) const {
  Q_D(const detail::ReplyDecorator);
  return d->decorate(reply);
}

ReplyDecoratorFactory* ReplyDecorator::at(qsizetype i) const {
  Q_D(const detail::ReplyDecorator);
  return d->at(i);
}

qsizetype ReplyDecorator::count() const {
  Q_D(const detail::ReplyDecorator);
  return d->count();
}

void ReplyDecorator::clear() {
  Q_D(detail::ReplyDecorator);
  d->clear();
}

void ReplyDecorator::append(ReplyDecoratorFactory* factory) {
  Q_D(detail::ReplyDecorator);
  d->append(factory);
}

void ReplyDecorator::prepend(ReplyDecoratorFactory* factory) {
  Q_D(detail::ReplyDecorator);
  d->prepend(factory);
}

void ReplyDecorator::remove(ReplyDecoratorFactory* factory) {
  Q_D(detail::ReplyDecorator);
  d->remove(factory);
}

/* -------------------------- ReplyDecoratorPrivate ------------------------- */

namespace detail {

ReplyDecoratorPrivate::ReplyDecoratorPrivate() {}

Reply* ReplyDecoratorPrivate::decorate(Reply* reply) const {
  for (auto& factory : m_factories) reply = factory->create(reply);
  return reply;
}

ReplyDecoratorFactory* ReplyDecoratorPrivate::at(qsizetype i) const {
  return m_factories.at(i);
}

qsizetype ReplyDecoratorPrivate::count() const { return m_factories.count(); }

void ReplyDecoratorPrivate::clear() { m_factories.clear(); }

void ReplyDecoratorPrivate::append(ReplyDecoratorFactory* factory) {
  m_factories.append(factory);
}

void ReplyDecoratorPrivate::prepend(ReplyDecoratorFactory* factory) {
  m_factories.prepend(factory);
}

void ReplyDecoratorPrivate::remove(ReplyDecoratorFactory* factory) {
  m_factories.removeAll(factory);
}

}  // namespace detail

}  // namespace egnite::rest

#include "egnite/rest/moc_reply_decorator.cpp"