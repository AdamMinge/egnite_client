#ifndef EGNITE_WEB_AUTHENTICATOR_H
#define EGNITE_WEB_AUTHENTICATOR_H

/* ------------------------------------ Qt ---------------------------------- */
#include <QObject>
#include <QTimer>
/* ----------------------------------- Local -------------------------------- */
#include "egnite/egnite/export.h"
#include "egnite/egnite/web/client.h"
#include "egnite/egnite/web/routers.h"
/* -------------------------------------------------------------------------- */

namespace egnite::web {

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
  void clientChanged(egnite::web::Client *Client);

private:
  Client *m_client;
};

class EGNITE_API SimpleJWTAuthenticator : public Authenticator {
  Q_OBJECT

  using replySucceedListener = std::function<void(QNetworkReply *)>;
  using replyFailedListener = std::function<void(QNetworkReply *)>;

public:
  Q_PROPERTY(QByteArray apiKey READ getApiKey WRITE setApiKey NOTIFY apiKeyChanged)
  Q_PROPERTY(QByteArray accessToken READ getAccessToken NOTIFY accessTokenChanged)
  Q_PROPERTY(QByteArray refreshToken READ getRefreshToken NOTIFY refreshTokenChanged)

  Q_PROPERTY(unsigned accessTokenLifetime READ getAccessTokenLifetime WRITE setAccessTokenLifetime
                 NOTIFY accessTokenLifetimeChanged)
  Q_PROPERTY(unsigned refreshTokenLifetime READ getRefreshTokenLifetime WRITE
                 setRefreshTokenLifetime NOTIFY refreshTokenLifetimeChanged)

  EGNITE_ROUTING(egnite::web::SimpleJWTAuthenticatorRouting);

public:
  explicit SimpleJWTAuthenticator(QObject *parent = nullptr);
  ~SimpleJWTAuthenticator() override;

  [[nodiscard]] const QByteArray &getApiKey() const;
  void setApiKey(const QByteArray &api_key);

  [[nodiscard]] const QByteArray &getAccessToken() const;
  [[nodiscard]] const QByteArray &getRefreshToken() const;

  [[nodiscard]] unsigned getAccessTokenLifetime() const;
  void setAccessTokenLifetime(unsigned accessTokenLifetime);

  [[nodiscard]] unsigned getRefreshTokenLifetime() const;
  void setRefreshTokenLifetime(unsigned refreshTokenLifetime);

  Q_INVOKABLE void login(const QString &username, const QString &password);
  Q_INVOKABLE void logout();

Q_SIGNALS:
  void apiKeyChanged(const QByteArray &api_key);
  void accessTokenChanged(const QByteArray &access_token);
  void refreshTokenChanged(const QByteArray &refresh_token);

  void accessTokenLifetimeChanged(unsigned access_token_lifetime);
  void refreshTokenLifetimeChanged(unsigned refresh_token_lifetime);

private:
  void setAccessToken(const QByteArray &access_token);
  void setRefreshToken(const QByteArray &refresh_token);

  void renewAccessToken();
  void renewRefreshToken();

  void connectReply(QNetworkReply *reply, replySucceedListener succeed,
                    replyFailedListener failed = nullptr);

private Q_SLOTS:
  void updateHeaders();

private:
  QByteArray m_api_key;
  QByteArray m_access_token;
  QByteArray m_refresh_token;

  unsigned m_access_token_lifetime;
  unsigned m_refresh_token_lifetime;

  QTimer m_renew_access_token;
  QTimer m_renew_refresh_token;
};

} // namespace egnite::web

#endif // EGNITE_WEB_AUTHENTICATOR_H
