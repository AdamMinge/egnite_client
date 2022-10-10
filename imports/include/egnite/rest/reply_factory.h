#ifndef EGNITE_QML_REST_REPLY_FACTORY_H
#define EGNITE_QML_REST_REPLY_FACTORY_H

/* ------------------------------------- Qt --------------------------------- */
#include <QtQml>
/* -------------------------------------------------------------------------- */

namespace egnite::rest {
class ReplyDecoratorFactory;
}

class QmlReplyFactory : public QObject {
  Q_OBJECT
  QML_ELEMENT

 public:
  explicit QmlReplyFactory(QObject* parent = nullptr);
  ~QmlReplyFactory() override;

  virtual egnite::rest::ReplyDecoratorFactory* getFactory() const = 0;
};

class QmlDebugReplyFactory : public QmlReplyFactory {
  Q_OBJECT
  QML_ELEMENT

 public:
  explicit QmlDebugReplyFactory(QObject* parent = nullptr);
  ~QmlDebugReplyFactory() override;

  egnite::rest::ReplyDecoratorFactory* getFactory() const override;

 private:
  egnite::rest::ReplyDecoratorFactory* m_factory;
};

#endif  // EGNITE_QML_REST_REPLY_DECORATOR_H