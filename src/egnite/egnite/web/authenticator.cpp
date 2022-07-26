/* ------------------------------------- Qt --------------------------------- */
#include <QJsonObject>
/* ----------------------------------- Local -------------------------------- */
#include "egnite/egnite/web/authenticator.h"
#include "egnite/egnite/web/messages.h"
/* -------------------------------------------------------------------------- */

namespace egnite::web {

/* ------------------------------- Authenticator -------------------------- */

Authenticator::Authenticator(QObject *parent) : QObject(parent), m_client(nullptr) {}

Authenticator::~Authenticator() = default;

Client *Authenticator::getClient() const { return m_client; }

void Authenticator::setClient(Client *client) {
  if (m_client == client)
    return;

  m_client = client;
  Q_EMIT clientChanged(m_client);
}

/* -------------------------- SimpleJWTAuthenticator ---------------------- */

SimpleJWTAuthenticator::SimpleJWTAuthenticator(QObject *parent)
    : Authenticator(parent), m_access_token_lifetime(60 * 60),
      m_refresh_token_lifetime(60 * 60 * 12), m_routing(new SimpleJWTAuthenticatorRouting(this)) {

  connect(this, &SimpleJWTAuthenticator::clientChanged, this, [this]() {
    if (auto client = getClient(); client) {

      connect(&m_renew_access_token, &QTimer::timeout, this,
              &SimpleJWTAuthenticator::renewAccessToken);
      connect(&m_renew_refresh_token, &QTimer::timeout, this,
              &SimpleJWTAuthenticator::renewRefreshToken);

      connect(this, &SimpleJWTAuthenticator::accessTokenChanged, this,
              &SimpleJWTAuthenticator::updateHeaders);

      updateHeaders();

    } else {
      disconnect(&m_renew_access_token, &QTimer::timeout, this,
                 &SimpleJWTAuthenticator::renewAccessToken);
      disconnect(&m_renew_refresh_token, &QTimer::timeout, this,
                 &SimpleJWTAuthenticator::renewRefreshToken);
 
      disconnect(this, &SimpleJWTAuthenticator::accessTokenChanged, this,
                 &SimpleJWTAuthenticator::updateHeaders);
    }
  });
}

SimpleJWTAuthenticator::~SimpleJWTAuthenticator() = default;

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
  auto client = getClient();
  Q_ASSERT(client);

  auto reply = client->post(client->getUrl(m_routing->gettokenCreate()),
                            LoginRequest{.username = username, .password = password});

  // TODO - Implement scenario for success and fail
  reply.onSuccess<LoginResponse>([](auto &response) { qDebug() << "login succeed"; })
      .onError([](auto error) { qDebug() << "login failed = " << error; });
}

void SimpleJWTAuthenticator::logout() {
  auto client = getClient();
  Q_ASSERT(client);

  auto reply = client->post(client->getUrl(m_routing->gettokenBlacklist()),
                            LogoutRequest{.refresh_token = m_refresh_token});

  // TODO - Implement scenario for success and fail
  reply.onSuccess([]() { qDebug() << "logout succeed"; }).onError([](auto error) {
    qDebug() << "logout failed = " << error;
  });
}

void SimpleJWTAuthenticator::renewAccessToken() {
  auto client = getClient();
  Q_ASSERT(client);

  auto reply = client->post(client->getUrl(m_routing->gettokenRefresh()),
                            RenewAccessTokenRequest{.refresh_token = m_refresh_token});

  // TODO - Implement scenario for success and fail
  reply
      .onSuccess<RenewAccessTokenResponse>(
          [](auto &response) { qDebug() << "renew token succeed"; })
      .onError([](auto error) { qDebug() << "renew token failed = " << error; });
}

void SimpleJWTAuthenticator::renewRefreshToken() {
  auto client = getClient();
  Q_ASSERT(client);

  // TODO - Research about refreshing refresh token ( maybe stay login and sens login request? )
}

void SimpleJWTAuthenticator::updateHeaders() {

  auto client = getClient();
  Q_ASSERT(client);

  auto headers = client->getHeaders();
  if (!m_access_token.isEmpty())
    headers->setHeader(Headers::KnownHeaders::Authorization, "Bearer " + m_access_token);
}

} // namespace egnite::web
