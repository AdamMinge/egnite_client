#ifndef EGNITE_QML_REST_CLIENT_H
#define EGNITE_QML_REST_CLIENT_H

/* ------------------------------------- Qt --------------------------------- */
#include <QtQml>
/* ----------------------------------- Egnite ------------------------------- */
#include <egnite/rest/client.h>
/* ----------------------------------- Local -------------------------------- */
#include "reply_decorator.h"
/* -------------------------------------------------------------------------- */

/* --------------------------------- QmlClient ------------------------------ */

class QmlClient : public egnite::rest::Client {
  Q_OBJECT
  QML_ELEMENT

  Q_PROPERTY(QString version READ getStrVersion WRITE setStrVersion NOTIFY
                 versionChanged)
  Q_PROPERTY(QmlReplyDecorator *replyDecorator READ getReplyDecorator)

 public:
  explicit QmlClient(QObject *parent = nullptr);
  ~QmlClient() override;

  void setStrVersion(const QString &version);
  [[nodiscard]] QString getStrVersion() const;

  [[nodiscard]] QmlReplyDecorator *getReplyDecorator() const override;

 Q_SIGNALS:
  void versionChanged(const QString &version);

 private:
  QmlReplyDecorator *m_reply_decorator;
};

#endif  // EGNITE_QML_REST_CLIENT_H