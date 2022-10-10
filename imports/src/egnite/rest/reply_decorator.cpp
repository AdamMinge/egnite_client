/* ----------------------------------- Local -------------------------------- */
#include "reply_decorator.h"

#include "reply_factory.h"
/* ----------------------------------- Egnite ------------------------------- */
#include <egnite/rest/reply_decorator.h>
#include <egnite/rest/reply_decorator_factory.h>
/* -------------------------------------------------------------------------- */

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