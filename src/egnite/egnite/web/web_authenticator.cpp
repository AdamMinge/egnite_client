/* ------------------------------------- Qt --------------------------------- */
#include <QUrlQuery>
/* ----------------------------------- Local -------------------------------- */
#include "egnite/egnite/web/web_authenticator.h"
/* -------------------------------------------------------------------------- */

namespace egnite::web {

/* ----------------------------- WebAuthenticator ------------------------- */

WebAuthenticator::WebAuthenticator(QObject *parent) : QObject(parent), m_web_client(nullptr) {}

WebAuthenticator::~WebAuthenticator() = default;

WebClient *WebAuthenticator::getWebClient() const { return m_web_client; }

void WebAuthenticator::setWebClient(WebClient *webClient) {
  if (m_web_client == webClient)
    return;

  m_web_client = webClient;
  Q_EMIT onWebClientChanged(m_web_client);
}

/* -------------------------- SimpleJWTAuthenticator ---------------------- */

SimpleJWTAuthenticator::SimpleJWTAuthenticator(QObject *parent)
    : WebAuthenticator(parent), m_api_key(), m_accessTokenLifetime(), m_refreshTokenLifetime(),
      m_routing(new SimpleJWTAuthenticatorRouting(this)) {}

SimpleJWTAuthenticator::~SimpleJWTAuthenticator() = default;

const QByteArray &SimpleJWTAuthenticator::getApiKey() const { return m_api_key; }

void SimpleJWTAuthenticator::setApiKey(const QByteArray &api_key) {
  if (m_api_key == api_key)
    return;

  m_api_key = api_key;
  Q_EMIT onApiKeyChanged(m_api_key);
}

unsigned SimpleJWTAuthenticator::getAccessTokenLifetime() const { return m_accessTokenLifetime; }

void SimpleJWTAuthenticator::setAccessTokenLifetime(unsigned accessTokenLifetime) {
  if (m_accessTokenLifetime == accessTokenLifetime)
    return;

  m_accessTokenLifetime = accessTokenLifetime;
  Q_EMIT onAccessTokenLifetimeChanged(m_accessTokenLifetime);
}

unsigned SimpleJWTAuthenticator::getRefreshTokenLifetime() const { return m_accessTokenLifetime; }

void SimpleJWTAuthenticator::setRefreshTokenLifetime(unsigned refreshTokenLifetime) {
  if (m_refreshTokenLifetime == refreshTokenLifetime)
    return;

  m_refreshTokenLifetime = refreshTokenLifetime;
  Q_EMIT onRefreshTokenLifetimeChanged(m_refreshTokenLifetime);
}

SimpleJWTAuthenticatorRouting *SimpleJWTAuthenticator::getRouting() const { return m_routing; }

void SimpleJWTAuthenticator::setRouting(SimpleJWTAuthenticatorRouting *routing) {
  if (m_routing == routing)
    return;

  m_routing = routing;
  Q_EMIT onRoutingChanged(m_routing);
}

void SimpleJWTAuthenticator::login(const QString &username, const QString &password) {
  QUrlQuery login_data;
  login_data.addQueryItem(QLatin1String("username"), username);
  login_data.addQueryItem(QLatin1String("password"), password);

  auto reply = getWebClient()->post(
      getWebClient()->getBaseUrl().resolved(QUrl("/login")),
      login_data.toString(QUrl::ComponentFormattingOption::FullyDecoded).toLocal8Bit());
}

void SimpleJWTAuthenticator::logout() {
  auto reply = getWebClient()->post(getWebClient()->getBaseUrl().resolved(QUrl("/logout")));
}

bool SimpleJWTAuthenticator::isLoggedIn() const { return false; }

bool SimpleJWTAuthenticator::isLoggedOut() const { return true; }

} // namespace egnite::web
