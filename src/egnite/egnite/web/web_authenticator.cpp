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
  Q_EMIT webClientChanged(m_web_client);
}

/* -------------------------- SimpleJWTAuthenticator ---------------------- */

SimpleJWTAuthenticator::SimpleJWTAuthenticator(QObject *parent)
    : WebAuthenticator(parent), m_api_key(), m_access_token_lifetime(60 * 60),
      m_refresh_token_lifetime(60 * 60 * 12), m_routing(new SimpleJWTAuthenticatorRouting(this)) {

  connect(this, &SimpleJWTAuthenticator::webClientChanged, this, [this]() {
    if (auto web_client = getWebClient(); web_client) {

      connect(&m_renew_access_token, &QTimer::timeout, this,
              &SimpleJWTAuthenticator::renewAccessToken);
      connect(&m_renew_refresh_token, &QTimer::timeout, this,
              &SimpleJWTAuthenticator::renewRefreshToken);

      connect(this, &SimpleJWTAuthenticator::webClientChanged, this,
              &SimpleJWTAuthenticator::updateHeaders);
      connect(this, &SimpleJWTAuthenticator::apiKeyChanged, this,
              &SimpleJWTAuthenticator::updateHeaders);
      connect(this, &SimpleJWTAuthenticator::accessTokenChanged, this,
              &SimpleJWTAuthenticator::updateHeaders);

      updateHeaders();

    } else {
      disconnect(&m_renew_access_token, &QTimer::timeout, this,
                 &SimpleJWTAuthenticator::renewAccessToken);
      disconnect(&m_renew_refresh_token, &QTimer::timeout, this,
                 &SimpleJWTAuthenticator::renewRefreshToken);

      disconnect(this, &SimpleJWTAuthenticator::apiKeyChanged, this,
                 &SimpleJWTAuthenticator::updateHeaders);
      disconnect(this, &SimpleJWTAuthenticator::accessTokenChanged, this,
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
  Q_EMIT apiKeyChanged(m_api_key);
}

const QByteArray &SimpleJWTAuthenticator::getAccessToken() const { return m_access_token; }

void SimpleJWTAuthenticator::setAccessToken(const QByteArray &access_token) {
  if (m_access_token == access_token)
    return;

  m_access_token = access_token;
  Q_EMIT accessTokenChanged(m_access_token);
}

const QByteArray &SimpleJWTAuthenticator::getRefreshToken() const { return m_refresh_token; }

void SimpleJWTAuthenticator::setRefreshToken(const QByteArray &refresh_token) {
  if (m_refresh_token == refresh_token)
    return;

  m_refresh_token = refresh_token;
  Q_EMIT refreshTokenChanged(m_refresh_token);
}

unsigned SimpleJWTAuthenticator::getAccessTokenLifetime() const { return m_access_token_lifetime; }

void SimpleJWTAuthenticator::setAccessTokenLifetime(unsigned access_token_lifetime) {
  if (m_access_token_lifetime == access_token_lifetime)
    return;

  m_access_token_lifetime = access_token_lifetime;
  Q_EMIT accessTokenLifetimeChanged(m_access_token_lifetime);
}

unsigned SimpleJWTAuthenticator::getRefreshTokenLifetime() const {
  return m_refresh_token_lifetime;
}

void SimpleJWTAuthenticator::setRefreshTokenLifetime(unsigned refresh_token_lifetime) {
  if (m_refresh_token_lifetime == refresh_token_lifetime)
    return;

  m_refresh_token_lifetime = refresh_token_lifetime;
  Q_EMIT refreshTokenLifetimeChanged(m_refresh_token_lifetime);
}

void SimpleJWTAuthenticator::login(const QString &username, const QString &password) {
  auto web_client = getWebClient();
  Q_ASSERT(web_client);

  auto reply = web_client->post(web_client->getUrl(m_routing->gettokenCreate()),
                                LoginRequest{.username = username, .password = password});

  connectReply( // TODO - Implement scenario for success and fail
      reply, [](auto reply) { qDebug() << "login succeed"; },
      [](auto reply) { qDebug() << "login failed"; });
}

void SimpleJWTAuthenticator::logout() {
  auto web_client = getWebClient();
  Q_ASSERT(web_client);

  auto reply = web_client->post(web_client->getUrl(m_routing->gettokenBlacklist()),
                                LogoutRequest{.refresh_token = m_refresh_token});

  connectReply( // TODO - Implement scenario for success and fail
      reply, [](auto reply) { qDebug() << "logout succeed"; },
      [](auto reply) { qDebug() << "logout failed"; });
}

void SimpleJWTAuthenticator::renewAccessToken() {
  auto web_client = getWebClient();
  Q_ASSERT(web_client);

  auto reply = web_client->post(web_client->getUrl(m_routing->gettokenRefresh()),
                                RenewAccessTokenRequest{.refresh_token = m_refresh_token});

  connectReply( // TODO - Implement scenario for success and fail
      reply, [](auto reply) { qDebug() << "renewAccessToken succeed"; },
      [](auto reply) { qDebug() << "renewAccessToken failed"; });
}

void SimpleJWTAuthenticator::renewRefreshToken() {
  auto web_client = getWebClient();
  Q_ASSERT(web_client);

  // TODO - Research about refreshing refresh token ( maybe stay login and sens login request? )
}

void SimpleJWTAuthenticator::connectReply(QNetworkReply *reply, replySucceedListener success,
                                          replyFailedListener failed) {
  connect(reply, &QNetworkReply::finished, this,
          [this, success = std::move(success), failed = std::move(failed)]() {
            auto reply = qobject_cast<QNetworkReply *>(sender());
            Q_ASSERT(reply);

            if (reply->error() == QNetworkReply::NoError)
              success(reply);
            else
              failed(reply);
          });
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
