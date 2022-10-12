#ifndef EGNITE_QML_REST_REPLY_FACTORY_H
#define EGNITE_QML_REST_REPLY_FACTORY_H

/* ------------------------------------- Qt --------------------------------- */
#include <QtQml>
/* ----------------------------------- Egnite ------------------------------- */
#include <egnite/rest/reply_factory.h>
/* -------------------------------------------------------------------------- */

namespace egnite::rest {
class DebugReplyFactory;
}

/* ---------------------------- QmlDebugReplyFactory ------------------------ */

class QmlDebugReplyFactory : public egnite::rest::IReplyFactory {
  Q_OBJECT
  QML_ELEMENT

 public:
  explicit QmlDebugReplyFactory(QObject* parent = nullptr);
  ~QmlDebugReplyFactory() override;

  [[nodiscard]] egnite::rest::IReply* create(
      egnite::rest::IReply* reply) override;

 private:
  egnite::rest::DebugReplyFactory* m_factory;
};

#endif  // EGNITE_QML_REST_REPLY_DECORATOR_H