#ifndef EGNITE_QML_REST_REPLY_DECORATOR_H
#define EGNITE_QML_REST_REPLY_DECORATOR_H

/* ------------------------------------- Qt --------------------------------- */
#include <QtQml>
/* -------------------------------------------------------------------------- */

namespace egnite::rest {
class ReplyDecorator;
}

class QmlReplyFactory;

class QmlReplyDecorator : public QObject {
  Q_OBJECT
  QML_ELEMENT

  Q_PROPERTY(QQmlListProperty<QmlReplyFactory> factories READ getFactories)

 public:
  explicit QmlReplyDecorator(egnite::rest::ReplyDecorator* decorator,
                             QObject* parent = nullptr);
  ~QmlReplyDecorator() override;

  QQmlListProperty<QmlReplyFactory> getFactories();
  void appendFactory(QmlReplyFactory* factory);
  qsizetype factoriesCount() const;
  QmlReplyFactory* factoryAt(qsizetype i) const;
  void clearFactories();

 private:
  static void appendFactory(QQmlListProperty<QmlReplyFactory>* list,
                            QmlReplyFactory* factory);
  static qsizetype factoriesCount(QQmlListProperty<QmlReplyFactory>* list);
  static QmlReplyFactory* factoryAt(QQmlListProperty<QmlReplyFactory>* list,
                                    qsizetype i);
  static void clearFactories(QQmlListProperty<QmlReplyFactory>* list);

 private:
  egnite::rest::ReplyDecorator* m_decorator;
};

#endif  // EGNITE_QML_REST_REPLY_DECORATOR_H