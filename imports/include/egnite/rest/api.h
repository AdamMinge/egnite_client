#ifndef EGNITE_QML_REST_API_H
#define EGNITE_QML_REST_API_H

/* ------------------------------------ Qt ---------------------------------- */
#include <QObject>
#include <QQmlParserStatus>
#include <QtQml>
/* ----------------------------------- Egnite ------------------------------- */
#include <egnite/rest/client.h>
/* -------------------------------------------------------------------------- */

namespace egnite::rest {
class Api;
}  // namespace egnite::rest

class QmlApi : public QObject, public QQmlParserStatus {
  Q_OBJECT
  QML_ELEMENT
  Q_INTERFACES(QQmlParserStatus)

  Q_PROPERTY(QString path READ getPath WRITE setPath NOTIFY pathChanged)
  Q_PROPERTY(egnite::rest::IClient* client READ getClient WRITE setClient NOTIFY
                 clientChanged)

 public:
  explicit QmlApi(QObject* parent = nullptr);
  ~QmlApi() override;

  void classBegin() override;
  void componentComplete() override;

  void setPath(const QString& path);
  [[nodiscard]] QString getPath() const;

  void setClient(egnite::rest::IClient* client);
  [[nodiscard]] egnite::rest::IClient* getClient() const;

 Q_SIGNALS:
  void pathChanged(const QString& path);
  void clientChanged(egnite::rest::IClient* client);

 private:
  void revaluateApi();

 private:
  bool m_init;
  QString m_path;
  egnite::rest::IClient* m_client;
  egnite::rest::Api* m_api;
};

#endif  // EGNITE_QML_REST_API_H