/* ------------------------------------- Qt --------------------------------- */
#include <QJsonObject>
/* ----------------------------------- Local -------------------------------- */
#include "egnite/egnite/web/web_authenticator.h"
#include "egnite/egnite/web/web_messages.h"
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
    : WebAuthenticator(parent), m_api_key(), m_access_token_lifetime(60 * 60),
      m_refresh_token_lifetime(60 * 60 * 12), m_routing(new SimpleJWTAuthenticatorRouting(this)) {

  connect(this, &SimpleJWTAuthenticator::onWebClientChanged, this, [this]() {
    if (auto web_client = getWebClient(); web_client) {

      connect(&m_renew_access_token, &QTimer::timeout, this,
              &SimpleJWTAuthenticator::renewAccessToken);
      connect(&m_renew_refresh_token, &QTimer::timeout, this,
              &SimpleJWTAuthenticator::renewRefreshToken);

      connect(this, &SimpleJWTAuthenticator::onWebClientChanged, this,
              &SimpleJWTAuthenticator::updateHeaders);
      connect(this, &SimpleJWTAuthenticator::onApiKeyChanged, this,
              &SimpleJWTAuthenticator::updateHeaders);
      connect(this, &SimpleJWTAuthenticator::onAccessTokenChanged, this,
              &SimpleJWTAuthenticator::updateHeaders);

      updateHeaders();

    } else {
      disconnect(&m_renew_access_token, &QTimer::timeout, this,
                 &SimpleJWTAuthenticator::renewAccessToken);
      disconnect(&m_renew_refresh_token, &QTimer::timeout, this,
                 &SimpleJWTAuthenticator::renewRefreshToken);

      disconnect(this, &SimpleJWTAuthenticator::onApiKeyChanged, this,
                 &SimpleJWTAuthenticator::updateHeaders);
      disconnect(this, &SimpleJWTAuthenticator::onAccessTokenChanged, this,
                 &SimpleJWTAuthenticator::updateHeaders);
    }
  });
}

SimpleJWTAuthenticator::~SimpleJWTAuthenticator() = default;

const QByteArray &SimpleJWTAuthenticator::getApiKey() const { return m_api_key; }

void SimpleJWTAuthenticator::setApiKey(const QByteArray &api_key) {
  if (m_api_key == api_key)
    return;

  m_api_key = api_key;
  Q_EMIT onApiKeyChanged(m_api_key);
}

const QByteArray &SimpleJWTAuthenticator::getAccessToken() const { return m_access_token; }

void SimpleJWTAuthenticator::setAccessToken(const QByteArray &access_token) {
  if (m_access_token == access_token)
    return;

  m_access_token = access_token;
  Q_EMIT onAccessTokenChanged(m_access_token);
}

const QByteArray &SimpleJWTAuthenticator::getRefreshToken() const { return m_refresh_token; }

void SimpleJWTAuthenticator::setRefreshToken(const QByteArray &refresh_token) {
  if (m_refresh_token == refresh_token)
    return;

  m_refresh_token = refresh_token;
  Q_EMIT onRefreshTokenChanged(m_refresh_token);
}

unsigned SimpleJWTAuthenticator::getAccessTokenLifetime() const { return m_access_token_lifetime; }

void SimpleJWTAuthenticator::setAccessTokenLifetime(unsigned access_token_lifetime) {
  if (m_access_token_lifetime == access_token_lifetime)
    return;

  m_access_token_lifetime = access_token_lifetime;
  Q_EMIT onAccessTokenLifetimeChanged(m_access_token_lifetime);
}

unsigned SimpleJWTAuthenticator::getRefreshTokenLifetime() const {
  return m_refresh_token_lifetime;
}

void SimpleJWTAuthenticator::setRefreshTokenLifetime(unsigned refresh_token_lifetime) {
  if (m_refresh_token_lifetime == refresh_token_lifetime)
    return;

  m_refresh_token_lifetime = refresh_token_lifetime;
  Q_EMIT onRefreshTokenLifetimeChanged(m_refresh_token_lifetime);
}

SimpleJWTAuthenticatorRouting *SimpleJWTAuthenticator::getRouting() const { return m_routing; }

void SimpleJWTAuthenticator::setRouting(SimpleJWTAuthenticatorRouting *routing) {
  if (m_routing == routing)
    return;

  m_routing = routing;
  Q_EMIT onRoutingChanged(m_routing);
}

void SimpleJWTAuthenticator::login(const QString &username, const QString &password) {
  auto web_client = getWebClient();
  Q_ASSERT(web_client);

  auto reply = web_client->post(web_client->getUrl(m_routing->gettokenCreate()),
                                LoginRequest{.username = username, .password = password});

  // TODO add data and action on login reply
}

void SimpleJWTAuthenticator::logout() {
  auto web_client = getWebClient();
  Q_ASSERT(web_client);

  // TODO implement renew refresh token
}

void SimpleJWTAuthenticator::renewAccessToken() {
  auto web_client = getWebClient();
  Q_ASSERT(web_client);

  auto reply = web_client->post(web_client->getUrl(m_routing->gettokenRefresh()));

  // TODO action on reply renew access token
}

void SimpleJWTAuthenticator::renewRefreshToken() {
  auto web_client = getWebClient();
  Q_ASSERT(web_client);

  // TODO implement renew refresh token
}

void SimpleJWTAuthenticator::updateHeaders() {

  auto web_client = getWebClient();
  Q_ASSERT(web_client);

  auto headers = web_client->getHeaders();

  if (!m_api_key.isEmpty())
    headers.setRawHeader("Api-Key", m_api_key);
  if (!m_access_token.isEmpty())
    headers.setRawHeader("Authorization", "Bearer " + m_access_token);

  web_client->setHeaders(headers);
}

} // namespace egnite::web
