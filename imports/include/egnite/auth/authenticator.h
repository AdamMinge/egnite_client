#ifndef EGNITE_QML_AUTH_AUTHENTICATOR_H
#define EGNITE_QML_AUTH_AUTHENTICATOR_H

/* ------------------------------------ Qt ---------------------------------- */
#include <QQmlParserStatus>
#include <QtQml>
/* ----------------------------------- Egnite ------------------------------- */
#include <egnite/auth/authenticator.h>
#include <egnite/rest/client.h>
/* -------------------------------------------------------------------------- */

class QmlJwtAuthenticator : public egnite::auth::IJwtAuthenticator,
                            public QQmlParserStatus {
  Q_OBJECT
  QML_ELEMENT
  Q_INTERFACES(QQmlParserStatus)

  Q_PROPERTY(QString path READ getPath WRITE setPath NOTIFY pathChanged)
  Q_PROPERTY(egnite::rest::Client* client READ getClient WRITE setClient NOTIFY
                 clientChanged)
  Q_PROPERTY(egnite::auth::JwtAuthenticator::Routing routing READ getRouting
                 WRITE setRouting NOTIFY routingChanged)

  Q_PROPERTY(QByteArray accessToken READ getAccessToken)
  Q_PROPERTY(QByteArray refreshToken READ getRefreshToken)

 public:
  explicit QmlJwtAuthenticator(QObject* parent = nullptr);
  ~QmlJwtAuthenticator() override;

  void setPath(const QString& path);
  [[nodiscard]] QString getPath() const;

  void setClient(egnite::rest::Client* client);
  [[nodiscard]] egnite::rest::Client* getClient() const;

  void setRouting(const egnite::auth::IJwtAuthenticator::Routing& routing);
  [[nodiscard]] egnite::auth::IJwtAuthenticator::Routing getRouting() const;

  void classBegin() override;
  void componentComplete() override;

  Q_INVOKABLE void login(const QString& username,
                         const QString& password) override;
  Q_INVOKABLE void refresh() override;
  Q_INVOKABLE void logout() override;

  [[nodiscard]] QByteArray getAccessToken() const override;
  [[nodiscard]] QByteArray getRefreshToken() const override;

 Q_SIGNALS:
  void pathChanged(const QString& path);
  void clientChanged(egnite::rest::Client* client);
  void routingChanged(const egnite::auth::JwtAuthenticator::Routing& routing);

 private:
  void revaluateAuthenticator();

 private:
  struct RevaluateData {
    bool init = false;
    QString path = "";
    egnite::rest::Client* client;
    egnite::auth::JwtAuthenticator::Routing routing;
  };

 private:
  RevaluateData m_revaluate_data;
  egnite::auth::IJwtAuthenticator* m_authenticator;
};

#endif  // EGNITE_QML_AUTH_AUTHENTICATOR_H