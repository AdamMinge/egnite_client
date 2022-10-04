#ifndef EGNITE_QML_REST_CLIENT_H
#define EGNITE_QML_REST_CLIENT_H

/* ------------------------------------- Qt --------------------------------- */
#include <QtQml>
/* ----------------------------------- Egnite ------------------------------- */
#include <egnite/rest/client.h>
/* -------------------------------------------------------------------------- */

class QmlClient : public egnite::rest::Client {
  Q_OBJECT
  QML_ELEMENT

  Q_PROPERTY(
      QString version READ getVersion WRITE setVersion NOTIFY versionChanged)

 public:
  explicit QmlClient(QObject* parent = nullptr);
  ~QmlClient() override;

  void setVersion(const QString& version);
  QString getVersion() const;

 Q_SIGNALS:
  void versionChanged(const QString& version);
};

#endif  // EGNITE_QML_REST_CLIENT_H