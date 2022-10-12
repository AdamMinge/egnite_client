/* ----------------------------------- Local -------------------------------- */
#include "reply_decorator.h"
/* ----------------------------------- Egnite ------------------------------- */
#include <egnite/rest/reply_decorator.h>
#include <egnite/rest/reply_factory.h>
/* -------------------------------------------------------------------------- */

QmlReplyDecorator::QmlReplyDecorator(egnite::rest::IReplyDecorator* decorator,
                                     QObject* parent)
    : egnite::rest::ReplyDecorator(parent) {}

QmlReplyDecorator::~QmlReplyDecorator() = default;

QQmlListProperty<QObject> QmlReplyDecorator::getFactories() {
  return QQmlListProperty<QObject>(
      this, this, &QmlReplyDecorator::appendFactory,
      &QmlReplyDecorator::factoriesCount, &QmlReplyDecorator::factoryAt,
      &QmlReplyDecorator::clearFactories);
}

void QmlReplyDecorator::appendFactory(QQmlListProperty<QObject>* list,
                                      QObject* object) {
  auto decorator = reinterpret_cast<QmlReplyDecorator*>(list->data);
  auto factory = qobject_cast<egnite::rest::IReplyFactory*>(object);

  Q_ASSERT(factory);
  decorator->append(qobject_cast<egnite::rest::IReplyFactory*>(factory));
}

void QmlReplyDecorator::clearFactories(QQmlListProperty<QObject>* list) {
  auto decorator = reinterpret_cast<QmlReplyDecorator*>(list->data);
  decorator->clear();
}

qsizetype QmlReplyDecorator::factoriesCount(QQmlListProperty<QObject>* list) {
  auto decorator = reinterpret_cast<QmlReplyDecorator*>(list->data);
  return decorator->count();
}

QObject* QmlReplyDecorator::factoryAt(QQmlListProperty<QObject>* list,
                                      qsizetype i) {
  auto decorator = reinterpret_cast<QmlReplyDecorator*>(list->data);
  return decorator->at(i);
}