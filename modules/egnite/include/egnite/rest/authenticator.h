#ifndef EGNITE_REST_AUTHENTICATOR_H
#define EGNITE_REST_AUTHENTICATOR_H

/* ------------------------------------ Qt ---------------------------------- */
#include <QObject>
#include <QTimer>
/* ----------------------------------- Local -------------------------------- */
#include "egnite/export.h"
#include "egnite/rest/client.h"
#include "egnite/rest/routers.h"
/* -------------------------------------------------------------------------- */

namespace egnite::rest {

class Client;

class EGNITE_API Authenticator : public QObject {
  Q_OBJECT

 public:
  Q_PROPERTY(Client *client READ getClient WRITE setClient NOTIFY clientChanged)

 public:
  explicit Authenticator(QObject *parent = nullptr);
  ~Authenticator() override;

  [[nodiscard]] Client *getClient() const;
  void setClient(Client *webClient);

 Q_SIGNALS:
  void clientChanged(egnite::rest::Client *client);

 private:
  Client *m_client;
};

class EGNITE_API SimpleJWTAuthenticator : public Authenticator {
  Q_OBJECT

 public:
  Q_PROPERTY(
      QByteArray accessToken READ getAccessToken NOTIFY accessTokenChanged)
  Q_PROPERTY(
      QByteArray refreshToken READ getRefreshToken NOTIFY refreshTokenChanged)

  Q_PROPERTY(unsigned accessTokenLifetime READ getAccessTokenLifetime WRITE
                 setAccessTokenLifetime NOTIFY accessTokenLifetimeChanged)
  Q_PROPERTY(unsigned refreshTokenLifetime READ getRefreshTokenLifetime WRITE
                 setRefreshTokenLifetime NOTIFY refreshTokenLifetimeChanged)

  EGNITE_ROUTING(egnite::rest::SimpleJWTAuthenticatorRouting);

 public:
  explicit SimpleJWTAuthenticator(QObject *parent = nullptr);
  ~SimpleJWTAuthenticator() override;

  [[nodiscard]] const QByteArray &getAccessToken() const;
  [[nodiscard]] const QByteArray &getRefreshToken() const;

  [[nodiscard]] unsigned getAccessTokenLifetime() const;
  void setAccessTokenLifetime(unsigned accessTokenLifetime);

  [[nodiscard]] unsigned getRefreshTokenLifetime() const;
  void setRefreshTokenLifetime(unsigned refreshTokenLifetime);

  Q_INVOKABLE void login(const QString &username, const QString &password);
  Q_INVOKABLE void logout();

 Q_SIGNALS:
  void accessTokenChanged(const QByteArray &access_token);
  void refreshTokenChanged(const QByteArray &refresh_token);

  void accessTokenLifetimeChanged(unsigned access_token_lifetime);
  void refreshTokenLifetimeChanged(unsigned refresh_token_lifetime);

  void loginSuccess();
  void logoutSuccess();

  void loginError(QNetworkReply::NetworkError error);
  void logoutError(QNetworkReply::NetworkError error);

 private:
  void setAccessToken(const QByteArray &access_token);
  void setRefreshToken(const QByteArray &refresh_token);

  void renewAccessToken();
  void renewRefreshToken();

 private Q_SLOTS:
  void updateHeaders();

 private:
  QByteArray m_access_token;
  QByteArray m_refresh_token;

  unsigned m_access_token_lifetime;
  unsigned m_refresh_token_lifetime;

  QTimer m_renew_access_token;
  QTimer m_renew_refresh_token;
};

}  // namespace egnite::rest

#endif  // EGNITE_REST_AUTHENTICATOR_H
