/* ----------------------------------- Local -------------------------------- */
#include "reply_decorator.h"
/* -------------------------------------------------------------------------- */

/* ------------------------------ QmlReplyFactory --------------------------- */

QmlReplyFactory::QmlReplyFactory(QObject* parent) : QObject(parent) {}

QmlReplyFactory::~QmlReplyFactory() = default;

/* ---------------------------- QmlDebugReplyFactory ------------------------ */

class DebugReplyDecoratorFactory : public egnite::rest::ReplyDecoratorFactory {
 public:
  explicit DebugReplyDecoratorFactory(QObject* parent = nullptr)
      : egnite::rest::ReplyDecoratorFactory(parent) {}

  [[nodiscard]] egnite::rest::Reply* create(
      egnite::rest::Reply* reply) override {
    auto decorated_reply = new egnite::rest::LoggerReply<QDebug>(
        qDebug().nospace(), reply, reply->parent());
    reply->setParent(decorated_reply);
    return decorated_reply;
  }
};

QmlDebugReplyFactory::QmlDebugReplyFactory(QObject* parent)
    : QmlReplyFactory(parent),
      m_factory(new DebugReplyDecoratorFactory(this)) {}

QmlDebugReplyFactory::~QmlDebugReplyFactory() = default;

egnite::rest::ReplyDecoratorFactory* QmlDebugReplyFactory::getFactory() const {
  return m_factory;
}

/* ----------------------------- QmlReplyDecorator -------------------------- */

QmlReplyDecorator::QmlReplyDecorator(egnite::rest::ReplyDecorator* decorator,
                                     QObject* parent)
    : QObject(parent), m_decorator(decorator) {}

QmlReplyDecorator::~QmlReplyDecorator() = default;

QQmlListProperty<QmlReplyFactory> QmlReplyDecorator::getFactories() {
  return QQmlListProperty<QmlReplyFactory>(
      this, this, &QmlReplyDecorator::appendFactory,
      &QmlReplyDecorator::factoriesCount, &QmlReplyDecorator::factoryAt,
      &QmlReplyDecorator::clearFactories);
}

void QmlReplyDecorator::appendFactory(QmlReplyFactory* factory) {
  m_decorator->append(factory->getFactory());
}

qsizetype QmlReplyDecorator::factoriesCount() const {
  return m_decorator->count();
}

QmlReplyFactory* QmlReplyDecorator::factoryAt(qsizetype i) const {
  return qobject_cast<QmlReplyFactory*>(m_decorator->at(i));
}

void QmlReplyDecorator::clearFactories() { m_decorator->clear(); }

void QmlReplyDecorator::appendFactory(QQmlListProperty<QmlReplyFactory>* list,
                                      QmlReplyFactory* factory) {
  reinterpret_cast<QmlReplyDecorator*>(list->data)->appendFactory(factory);
}

qsizetype QmlReplyDecorator::factoriesCount(
    QQmlListProperty<QmlReplyFactory>* list) {
  return reinterpret_cast<QmlReplyDecorator*>(list->data)->factoriesCount();
}

QmlReplyFactory* QmlReplyDecorator::factoryAt(
    QQmlListProperty<QmlReplyFactory>* list, qsizetype i) {
  return reinterpret_cast<QmlReplyDecorator*>(list->data)->factoryAt(i);
}

void QmlReplyDecorator::clearFactories(
    QQmlListProperty<QmlReplyFactory>* list) {
  reinterpret_cast<QmlReplyDecorator*>(list->data)->clearFactories();
}