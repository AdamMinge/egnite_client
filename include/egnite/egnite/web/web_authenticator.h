#ifndef EGNITE_WEB_AUTHENTICATOR_H
#define EGNITE_WEB_AUTHENTICATOR_H

/* ------------------------------------ Qt ---------------------------------- */
#include <QObject>
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
  Q_PROPERTY(unsigned accessTokenLifetime READ getAccessTokenLifetime WRITE setAccessTokenLifetime
                 NOTIFY onAccessTokenLifetimeChanged)
  Q_PROPERTY(unsigned refreshTokenLifetime READ getRefreshTokenLifetime WRITE
                 setRefreshTokenLifetime NOTIFY onRefreshTokenLifetimeChanged)
  Q_PROPERTY(SimpleJWTAuthenticatorRouting *routing READ getRouting WRITE setRouting NOTIFY
                 onRoutingChanged)

  Q_PROPERTY(bool isLoggedIn READ isLoggedIn NOTIFY onLoggedIn)
  Q_PROPERTY(bool isLoggedOut READ isLoggedOut NOTIFY onLoggedOut)

public:
  explicit SimpleJWTAuthenticator(QObject *parent = nullptr);
  ~SimpleJWTAuthenticator() override;

  [[nodiscard]] const QByteArray &getApiKey() const;
  void setApiKey(const QByteArray &api_key);

  [[nodiscard]] unsigned getAccessTokenLifetime() const;
  void setAccessTokenLifetime(unsigned accessTokenLifetime);

  [[nodiscard]] unsigned getRefreshTokenLifetime() const;
  void setRefreshTokenLifetime(unsigned refreshTokenLifetime);

  [[nodiscard]] SimpleJWTAuthenticatorRouting *getRouting() const;
  void setRouting(SimpleJWTAuthenticatorRouting *routing);

  Q_INVOKABLE void login(const QString &username, const QString &password);
  Q_INVOKABLE void logout();

  [[nodiscard]] bool isLoggedIn() const;
  [[nodiscard]] bool isLoggedOut() const;

Q_SIGNALS:
  void onApiKeyChanged(const QByteArray &api_key);
  void onAccessTokenLifetimeChanged(unsigned accessTokenLifetime);
  void onRefreshTokenLifetimeChanged(unsigned refreshTokenLifetime);
  void onRoutingChanged(SimpleJWTAuthenticatorRouting *routing);

  void onLoggedIn();
  void onLoggedOut();

private:
  QByteArray m_api_key;
  unsigned m_accessTokenLifetime;
  unsigned m_refreshTokenLifetime;
  SimpleJWTAuthenticatorRouting *m_routing;
};

} // namespace egnite::web

#endif // EGNITE_WEB_AUTHENTICATOR_H
