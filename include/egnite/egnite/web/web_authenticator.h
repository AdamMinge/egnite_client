#ifndef EGNITE_WEB_AUTHENTICATOR_H
#define EGNITE_WEB_AUTHENTICATOR_H

/* ------------------------------------ Qt ---------------------------------- */
#include <QObject>
#include <QTimer>
/* ----------------------------------- Local -------------------------------- */
#include "egnite/egnite/export.h"
#include "egnite/egnite/web/web_client.h"
#include "egnite/egnite/web/web_routing.h"
/* -------------------------------------------------------------------------- */

namespace egnite::web {

class WebClient;

class EGNITE_API WebAuthenticator : public QObject {
  Q_OBJECT

public:
  Q_PROPERTY(WebClient *webClient READ getWebClient WRITE setWebClient NOTIFY onWebClientChanged)

public:
  explicit WebAuthenticator(QObject *parent = nullptr);
  ~WebAuthenticator() override;

  [[nodiscard]] WebClient *getWebClient() const;
  void setWebClient(WebClient *webClient);

Q_SIGNALS:
  void onWebClientChanged(egnite::web::WebClient *webClient);

private:
  WebClient *m_web_client;
};

EGNITE_ROUTING(SimpleJWTAuthenticatorRouting, tokenCreate, tokenRefresh, tokenVerify)

class EGNITE_API SimpleJWTAuthenticator : public WebAuthenticator {
  Q_OBJECT

public:
  Q_PROPERTY(QByteArray apiKey READ getApiKey WRITE setApiKey NOTIFY onApiKeyChanged)
  Q_PROPERTY(QByteArray accessToken READ getAccessToken NOTIFY onAccessTokenChanged)
  Q_PROPERTY(QByteArray refreshToken READ getRefreshToken NOTIFY onRefreshTokenChanged)

  Q_PROPERTY(unsigned accessTokenLifetime READ getAccessTokenLifetime WRITE setAccessTokenLifetime
                 NOTIFY onAccessTokenLifetimeChanged)
  Q_PROPERTY(unsigned refreshTokenLifetime READ getRefreshTokenLifetime WRITE
                 setRefreshTokenLifetime NOTIFY onRefreshTokenLifetimeChanged)

  Q_PROPERTY(SimpleJWTAuthenticatorRouting *routing READ getRouting WRITE setRouting NOTIFY
                 onRoutingChanged)

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

  [[nodiscard]] SimpleJWTAuthenticatorRouting *getRouting() const;
  void setRouting(SimpleJWTAuthenticatorRouting *routing);

  Q_INVOKABLE void login(const QString &username, const QString &password);
  Q_INVOKABLE void logout();

Q_SIGNALS:
  void onApiKeyChanged(const QByteArray &api_key);
  void onAccessTokenChanged(const QByteArray &access_token);
  void onRefreshTokenChanged(const QByteArray &refresh_token);

  void onAccessTokenLifetimeChanged(unsigned access_token_lifetime);
  void onRefreshTokenLifetimeChanged(unsigned refresh_token_lifetime);

  void onRoutingChanged(egnite::web::SimpleJWTAuthenticatorRouting *routing);

private:
  void setAccessToken(const QByteArray &access_token);
  void setRefreshToken(const QByteArray &refresh_token);

  void renewAccessToken();
  void renewRefreshToken();

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

  SimpleJWTAuthenticatorRouting *m_routing;
};

} // namespace egnite::web

#endif // EGNITE_WEB_AUTHENTICATOR_H
