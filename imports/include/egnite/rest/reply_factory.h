#ifndef EGNITE_QML_REST_REPLY_FACTORY_H
#define EGNITE_QML_REST_REPLY_FACTORY_H

/* ------------------------------------- Qt --------------------------------- */
#include <QtQml>
/* ----------------------------------- Egnite ------------------------------- */
#include <egnite/rest/reply_factory.h>
/* -------------------------------------------------------------------------- */

/* ---------------------------- QmlLoggerReplyFactory ----------------------- */

class QmlLoggerReplyFactory : public egnite::rest::LoggerReplyFactory {
  Q_OBJECT
  QML_ELEMENT

  Q_ENUM(egnite::rest::ILoggerReply::LogDetail)

 public:
  explicit QmlLoggerReplyFactory(
      egnite::rest::ILoggerReply::LogDetail log_detail =
          egnite::rest::ILoggerReply::LogDetail::LogAll,
      QObject* parent = nullptr);
  ~QmlLoggerReplyFactory() override;
};

#endif  // EGNITE_QML_REST_REPLY_DECORATOR_H