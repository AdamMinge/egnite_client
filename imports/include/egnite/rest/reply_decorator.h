#ifndef EGNITE_QML_REST_REPLY_DECORATOR_H
#define EGNITE_QML_REST_REPLY_DECORATOR_H

/* ------------------------------------- Qt --------------------------------- */
#include <QtQml>
/* ----------------------------------- Egnite ------------------------------- */
#include <egnite/rest/reply_decorator.h>
/* -------------------------------------------------------------------------- */

/* ----------------------------- QmlReplyDecorator -------------------------- */

class QmlReplyDecorator : public egnite::rest::WrappedReplyDecorator {
  Q_OBJECT
  QML_ELEMENT

  Q_PROPERTY(QQmlListProperty<QObject> factories READ getFactories)

 public:
  explicit QmlReplyDecorator(egnite::rest::IReplyDecorator* decorator,
                             QObject* parent = nullptr);
  ~QmlReplyDecorator() override;

  [[nodiscard]] QQmlListProperty<QObject> getFactories();

 private:
  static void appendFactory(QQmlListProperty<QObject>* list, QObject* object);
  static void clearFactories(QQmlListProperty<QObject>* list);
  [[nodiscard]] static qsizetype factoriesCount(
      QQmlListProperty<QObject>* list);
  [[nodiscard]] static QObject* factoryAt(QQmlListProperty<QObject>* list,
                                          qsizetype i);
};

#endif  // EGNITE_QML_REST_REPLY_DECORATOR_H