/* ----------------------------------- Local -------------------------------- */
#include "reply_decorator.h"

#include "reply_factory.h"
/* ----------------------------------- Egnite ------------------------------- */
#include <egnite/rest/reply_decorator.h>
#include <egnite/rest/reply_factory.h>
/* -------------------------------------------------------------------------- */

QmlReplyDecorator::QmlReplyDecorator(egnite::rest::IReplyDecorator* decorator,
                                     QObject* parent)
    : egnite::rest::ReplyDecorator(parent) {}

QmlReplyDecorator::~QmlReplyDecorator() = default;

QQmlListProperty<QmlReplyFactory> QmlReplyDecorator::getFactories() {
  return QQmlListProperty<QmlReplyFactory>(
      this, this, &QmlReplyDecorator::appendFactory,
      &QmlReplyDecorator::factoriesCount, &QmlReplyDecorator::factoryAt,
      &QmlReplyDecorator::clearFactories);
}

void QmlReplyDecorator::appendFactory(QQmlListProperty<QmlReplyFactory>* list,
                                      QmlReplyFactory* factory) {
  auto decorator = reinterpret_cast<QmlReplyDecorator*>(list->data);
  decorator->append(factory);
}

void QmlReplyDecorator::clearFactories(
    QQmlListProperty<QmlReplyFactory>* list) {
  auto decorator = reinterpret_cast<QmlReplyDecorator*>(list->data);
  decorator->clear();
}

qsizetype QmlReplyDecorator::factoriesCount(
    QQmlListProperty<QmlReplyFactory>* list) {
  auto decorator = reinterpret_cast<QmlReplyDecorator*>(list->data);
  return decorator->count();
}

QmlReplyFactory* QmlReplyDecorator::factoryAt(
    QQmlListProperty<QmlReplyFactory>* list, qsizetype i) {
  auto decorator = reinterpret_cast<QmlReplyDecorator*>(list->data);
  return qobject_cast<QmlReplyFactory*>(decorator->at(i));
}
