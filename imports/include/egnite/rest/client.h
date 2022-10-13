#ifndef EGNITE_QML_REST_CLIENT_H
#define EGNITE_QML_REST_CLIENT_H

/* ------------------------------------- Qt --------------------------------- */
#include <QJSValue>
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

  Q_PROPERTY(QJSValue globalHeaders READ getGlobalQmlHeaders WRITE
                 setGlobalQmlHeaders NOTIFY globalQmlHeadersChanged)
  Q_PROPERTY(QJSValue globalParameters READ getGlobalQmlParameters WRITE
                 setGlobalQmlParameters NOTIFY globalQmlParametersChanged)

 public:
  explicit QmlClient(QObject *parent = nullptr);
  ~QmlClient() override;

  [[nodiscard]] QmlReplyDecorator *getReplyDecorator() const override;

  void setStrVersion(const QString &version);
  [[nodiscard]] QString getStrVersion() const;

  [[nodiscard]] QJSValue getGlobalQmlHeaders() const;
  [[nodiscard]] QJSValue getGlobalQmlParameters() const;

  void setGlobalQmlHeaders(QJSValue object);
  void setGlobalQmlParameters(QJSValue object);

 Q_SIGNALS:
  void versionChanged(const QString &version);
  void globalQmlHeadersChanged(const QJSValue &object);
  void globalQmlParametersChanged(const QJSValue &object);

 private:
  QmlReplyDecorator *m_reply_decorator;
};

#endif  // EGNITE_QML_REST_CLIENT_H