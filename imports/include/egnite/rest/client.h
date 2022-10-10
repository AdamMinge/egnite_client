#ifndef EGNITE_QML_REST_CLIENT_H
#define EGNITE_QML_REST_CLIENT_H

/* ------------------------------------- Qt --------------------------------- */
#include <QtQml>
/* ----------------------------------- Egnite ------------------------------- */
#include <egnite/rest/client.h>
/* ----------------------------------- Local -------------------------------- */
#include "reply_decorator.h"
/* -------------------------------------------------------------------------- */

class QmlClient : public egnite::rest::Client {
  Q_OBJECT
  QML_ELEMENT

  Q_PROPERTY(
      QString version READ getVersion WRITE setVersion NOTIFY versionChanged)
  Q_PROPERTY(QmlReplyDecorator *replyDecorator READ getReplyDecorator)

 public:
  explicit QmlClient(QObject *parent = nullptr);
  ~QmlClient() override;

  void setVersion(const QString &version);
  QString getVersion() const;

  QmlReplyDecorator *getReplyDecorator() const;

 Q_SIGNALS:
  void versionChanged(const QString &version);

 private:
  QmlReplyDecorator *m_reply_decorator;
};

#endif  // EGNITE_QML_REST_CLIENT_H