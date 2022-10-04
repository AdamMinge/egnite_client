#ifndef EGNITE_QML_AUTH_JWT_AUTHENTICATOR_H
#define EGNITE_QML_AUTH_JWT_AUTHENTICATOR_H

/* ------------------------------------ Qt ---------------------------------- */
#include <QObject>
#include <QQmlParserStatus>
#include <QUrl>
#include <QtQml>
/* ----------------------------------- Egnite ------------------------------- */
#include <egnite/auth/jwt_authenticator.h>
#include <egnite/rest/client.h>
/* -------------------------------------------------------------------------- */

class QmlJwtAuthenticator : public QObject, public QQmlParserStatus {
  Q_OBJECT
  QML_ELEMENT
  Q_INTERFACES(QQmlParserStatus)

  Q_PROPERTY(QString path READ getPath WRITE setPath NOTIFY pathChanged)
  Q_PROPERTY(egnite::rest::Client* client READ getClient WRITE setClient NOTIFY
                 clientChanged)
  Q_PROPERTY(egnite::auth::JwtAuthenticator::Routing routing READ getRouting
                 WRITE setRouting NOTIFY routingChanged)

 public:
  explicit QmlJwtAuthenticator(QObject* parent = nullptr);
  ~QmlJwtAuthenticator() override;

  Q_INVOKABLE void login(const QString& username, const QString& password);
  Q_INVOKABLE void refresh();
  Q_INVOKABLE void logout();

  void classBegin() override;
  void componentComplete() override;

  void setPath(const QString& path);
  [[nodiscard]] QString getPath() const;

  void setClient(egnite::rest::Client* client);
  [[nodiscard]] egnite::rest::Client* getClient() const;

  void setRouting(const egnite::auth::JwtAuthenticator::Routing& routing);
  [[nodiscard]] egnite::auth::JwtAuthenticator::Routing getRouting() const;

 Q_SIGNALS:
  void pathChanged(const QString& path);
  void clientChanged(egnite::rest::Client* client);
  void routingChanged(const egnite::auth::JwtAuthenticator::Routing& routing);

 private:
  void revaluateAuthenticator();

 private:
  bool m_init;
  QString m_path;
  egnite::rest::Client* m_client;
  egnite::auth::JwtAuthenticator::Routing m_routing;
  egnite::auth::JwtAuthenticator* m_authenticator;
};

#endif  // EGNITE_QML_AUTH_JWT_AUTHENTICATOR_H