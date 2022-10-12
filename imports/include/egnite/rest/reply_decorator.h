#ifndef EGNITE_QML_REST_REPLY_DECORATOR_H
#define EGNITE_QML_REST_REPLY_DECORATOR_H

/* ------------------------------------- Qt --------------------------------- */
#include <QtQml>
/* ----------------------------------- Egnite ------------------------------- */
#include <egnite/rest/reply_decorator.h>
/* -------------------------------------------------------------------------- */

class QmlReplyFactory;

/* ----------------------------- QmlReplyDecorator -------------------------- */

class QmlReplyDecorator : public egnite::rest::ReplyDecorator {
  Q_OBJECT
  QML_ELEMENT

  Q_PROPERTY(QQmlListProperty<QmlReplyFactory> factories READ getFactories)

 public:
  explicit QmlReplyDecorator(egnite::rest::IReplyDecorator* decorator,
                             QObject* parent = nullptr);
  ~QmlReplyDecorator() override;

  [[nodiscard]] QQmlListProperty<QmlReplyFactory> getFactories();

 private:
  static void appendFactory(QQmlListProperty<QmlReplyFactory>* list,
                            QmlReplyFactory* factory);
  static void clearFactories(QQmlListProperty<QmlReplyFactory>* list);
  [[nodiscard]] static qsizetype factoriesCount(
      QQmlListProperty<QmlReplyFactory>* list);
  [[nodiscard]] static QmlReplyFactory* factoryAt(
      QQmlListProperty<QmlReplyFactory>* list, qsizetype i);
};

#endif  // EGNITE_QML_REST_REPLY_DECORATOR_H