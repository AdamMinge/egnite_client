#ifndef EGNITE_QML_REST_CLIENT_H
#define EGNITE_QML_REST_CLIENT_H

/* ------------------------------------- Qt --------------------------------- */
#include <QList>
#include <QQmlListProperty>
#include <QQmlPropertyMap>
#include <QtQml>
/* ---------------------------------- Standard ------------------------------ */
#include <memory>
/* ----------------------------------- Egnite ------------------------------- */
#include <egnite/rest/client.h>
/* ----------------------------------- Local -------------------------------- */
#include "api.h"
/* -------------------------------------------------------------------------- */

class QmlClient : public egnite::rest::Client {
  Q_OBJECT
  QML_ELEMENT

  Q_PROPERTY(QQmlListProperty<QmlApi> api READ getApiList)
  Q_PROPERTY(
      QString version READ getVersion WRITE setVersion NOTIFY versionChanged)

 public:
  explicit QmlClient(QObject* parent = nullptr);
  ~QmlClient() override;

  [[nodiscard]] QQmlListProperty<QmlApi> getApiList();

  void setVersion(const QString& version);
  QString getVersion() const;

 Q_SIGNALS:
  void versionChanged(const QString& version);

 private:
  QList<QmlApi*> m_api_list;
};

#endif  // EGNITE_QML_REST_CLIENT_H