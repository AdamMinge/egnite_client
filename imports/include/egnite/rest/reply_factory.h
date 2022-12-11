#ifndef EGNITE_QML_REST_REPLY_FACTORY_H
#define EGNITE_QML_REST_REPLY_FACTORY_H

/* ------------------------------------- Qt --------------------------------- */
#include <QtQml>
/* ----------------------------------- Egnite ------------------------------- */
#include <egnite/rest/reply_factory.h>
/* -------------------------------------------------------------------------- */

namespace egnite::rest {
class LoggerReplyFactory;
}

/* ---------------------------- QmlLoggerReplyFactory ----------------------- */

class QmlLoggerReplyFactory : public egnite::rest::IReplyFactory {
  Q_OBJECT
  QML_ELEMENT

 public:
  explicit QmlLoggerReplyFactory(QObject* parent = nullptr);
  ~QmlLoggerReplyFactory() override;

  [[nodiscard]] egnite::rest::IReply* create(
      egnite::rest::IReply* reply) override;

 private:
  egnite::rest::LoggerReplyFactory* m_factory;
};

#endif  // EGNITE_QML_REST_REPLY_DECORATOR_H