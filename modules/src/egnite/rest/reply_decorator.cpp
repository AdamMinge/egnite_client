/* ----------------------------------- Local -------------------------------- */
#include "egnite/rest/reply_decorator.h"

#include "egnite/rest/detail/reply_decorator_p.h"
#include "egnite/rest/reply.h"
#include "egnite/rest/reply_factory.h"
/* -------------------------------------------------------------------------- */

namespace egnite::rest {

/* ------------------------------ IReplyDecorator --------------------------- */

IReplyDecorator::IReplyDecorator(QObject* parent) : QObject(parent) {}
IReplyDecorator::IReplyDecorator(QObjectPrivate& impl, QObject* parent)
    : QObject(impl, parent) {}

IReplyDecorator::~IReplyDecorator() = default;

/* ------------------------------ ReplyDecorator ---------------------------- */

ReplyDecorator::ReplyDecorator(QObject* parent)
    : IReplyDecorator(*new detail::ReplyDecoratorPrivate(), parent) {}

ReplyDecorator::~ReplyDecorator() = default;

IReply* ReplyDecorator::decorate(IReply* reply) const {
  Q_D(const detail::ReplyDecorator);
  return d->decorate(reply);
}

IReplyFactory* ReplyDecorator::at(qsizetype i) const {
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

void ReplyDecorator::append(IReplyFactory* factory) {
  Q_D(detail::ReplyDecorator);
  d->append(factory);
}

void ReplyDecorator::prepend(IReplyFactory* factory) {
  Q_D(detail::ReplyDecorator);
  d->prepend(factory);
}

void ReplyDecorator::remove(IReplyFactory* factory) {
  Q_D(detail::ReplyDecorator);
  d->remove(factory);
}

/* -------------------------- WrappedReplyDecorator ------------------------- */

WrappedReplyDecorator::WrappedReplyDecorator(IReplyDecorator* reply_decorator,
                                             QObject* parent)
    : IReplyDecorator(
          *new detail::WrappedReplyDecoratorPrivate(reply_decorator), parent) {}

WrappedReplyDecorator::~WrappedReplyDecorator() = default;

IReply* WrappedReplyDecorator::decorate(IReply* reply) const {
  Q_D(const detail::WrappedReplyDecorator);
  return d->decorate(reply);
}

IReplyFactory* WrappedReplyDecorator::at(qsizetype i) const {
  Q_D(const detail::WrappedReplyDecorator);
  return d->at(i);
}

qsizetype WrappedReplyDecorator::count() const {
  Q_D(const detail::WrappedReplyDecorator);
  return d->count();
}

void WrappedReplyDecorator::clear() {
  Q_D(detail::WrappedReplyDecorator);
  d->clear();
}

void WrappedReplyDecorator::append(IReplyFactory* factory) {
  Q_D(detail::WrappedReplyDecorator);
  d->append(factory);
}

void WrappedReplyDecorator::prepend(IReplyFactory* factory) {
  Q_D(detail::WrappedReplyDecorator);
  d->prepend(factory);
}

void WrappedReplyDecorator::remove(IReplyFactory* factory) {
  Q_D(detail::WrappedReplyDecorator);
  d->remove(factory);
}

/* -------------------------- ReplyDecoratorPrivate ------------------------- */

namespace detail {

ReplyDecoratorPrivate::ReplyDecoratorPrivate() {}

IReply* ReplyDecoratorPrivate::decorate(IReply* reply) const {
  for (auto& factory : m_factories) reply = factory->create(reply);
  return reply;
}

IReplyFactory* ReplyDecoratorPrivate::at(qsizetype i) const {
  return m_factories.at(i);
}

qsizetype ReplyDecoratorPrivate::count() const { return m_factories.count(); }

void ReplyDecoratorPrivate::clear() { m_factories.clear(); }

void ReplyDecoratorPrivate::append(IReplyFactory* factory) {
  m_factories.append(factory);
}

void ReplyDecoratorPrivate::prepend(IReplyFactory* factory) {
  m_factories.prepend(factory);
}

void ReplyDecoratorPrivate::remove(IReplyFactory* factory) {
  m_factories.removeAll(factory);
}

/* ----------------------- WrappedReplyDecoratorPrivate --------------------- */

WrappedReplyDecoratorPrivate::WrappedReplyDecoratorPrivate(
    IReplyDecorator* reply_decorator)
    : m_reply_decorator(reply_decorator) {}

IReply* WrappedReplyDecoratorPrivate::decorate(IReply* reply) const {
  return m_reply_decorator->decorate(reply);
}

IReplyFactory* WrappedReplyDecoratorPrivate::at(qsizetype i) const {
  return m_reply_decorator->at(i);
}

qsizetype WrappedReplyDecoratorPrivate::count() const {
  return m_reply_decorator->count();
}

void WrappedReplyDecoratorPrivate::clear() { m_reply_decorator->clear(); }

void WrappedReplyDecoratorPrivate::append(IReplyFactory* factory) {
  m_reply_decorator->append(factory);
}

void WrappedReplyDecoratorPrivate::prepend(IReplyFactory* factory) {
  m_reply_decorator->prepend(factory);
}

void WrappedReplyDecoratorPrivate::remove(IReplyFactory* factory) {
  m_reply_decorator->remove(factory);
}

}  // namespace detail

}  // namespace egnite::rest

#include "egnite/rest/moc_reply_decorator.cpp"