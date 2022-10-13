/* ----------------------------------- Local -------------------------------- */
#include "egnite/auth/authenticator.h"

#include "egnite/auth/detail/authenticator_p.h"
/* ----------------------------------- Egnite ------------------------------- */
#include <egnite/rest/api.h>
#include <egnite/rest/client.h>
/* -------------------------------------------------------------------------- */

namespace egnite::auth {

/* ------------------------------- IAuthenticator --------------------------- */

IAuthenticator::IAuthenticator(QObject* parent) : QObject(parent) {}

IAuthenticator::IAuthenticator(QObjectPrivate& impl, QObject* parent)
    : QObject(impl, parent) {}

IAuthenticator::~IAuthenticator() = default;

/* ----------------------------- IJwtAuthenticator -------------------------- */

IJwtAuthenticator::IJwtAuthenticator(QObject* parent)
    : IAuthenticator(parent) {}

IJwtAuthenticator::IJwtAuthenticator(QObjectPrivate& impl, QObject* parent)
    : IAuthenticator(impl, parent) {}

IJwtAuthenticator::~IJwtAuthenticator() = default;

/* ----------------------------- JwtAuthenticator --------------------------- */

JwtAuthenticator::JwtAuthenticator(rest::IClient* client,
                                   const QString& subpath, QObject* parent)
    : JwtAuthenticator(*new detail::JwtAuthenticatorPrivate(client, subpath),
                       parent) {}

JwtAuthenticator::JwtAuthenticator(detail::JwtAuthenticatorPrivate& impl,
                                   QObject* parent)
    : IJwtAuthenticator(impl, parent) {}

JwtAuthenticator::~JwtAuthenticator() {}

void JwtAuthenticator::login(const QString& username, const QString& password) {
  Q_D(detail::JwtAuthenticator);
  d->login(username, password);
}

void JwtAuthenticator::refresh() {
  Q_D(detail::JwtAuthenticator);
  d->refresh();
}

void JwtAuthenticator::logout() {
  Q_D(detail::JwtAuthenticator);
  d->logout();
}

rest::IClient* JwtAuthenticator::getClient() const {
  Q_D(const detail::JwtAuthenticator);
  return d->getClient();
}

QByteArray JwtAuthenticator::getAccessToken() const {
  Q_D(const detail::JwtAuthenticator);
  return d->getAccessToken();
}

QByteArray JwtAuthenticator::getRefreshToken() const {
  Q_D(const detail::JwtAuthenticator);
  return d->getRefreshToken();
}

void JwtAuthenticator::setRouting(const JwtAuthenticator::Routing& routing) {
  Q_D(detail::JwtAuthenticator);
  d->setRouting(routing);
}

JwtAuthenticator::Routing JwtAuthenticator::getRouting() const {
  Q_D(const detail::JwtAuthenticator);
  return d->getRouting();
}

/* --------------------------- JwtAuthenticatorPrivate ---------------------- */

namespace detail {

const QByteArray JwtAuthenticatorPrivate::TokenHeader =
    QByteArray{"Authorization"};
const QByteArray JwtAuthenticatorPrivate::TokenPrefix = QByteArray{"Bearer"};

JwtAuthenticatorPrivate::JwtAuthenticatorPrivate(rest::IClient* client,
                                                 const QString& path)
    : m_api(client->createApi(path)) {}

JwtAuthenticatorPrivate::~JwtAuthenticatorPrivate() = default;

void JwtAuthenticatorPrivate::login(const QString& username,
                                    const QString& password) {
  Q_Q(JwtAuthenticator);
  const auto request = IJwtAuthenticator::ObtainTokenRequest{
      .username = username, .password = password};

  auto reply = m_api->post<IJwtAuthenticator::ObtainTokenResponse,
                           IAuthenticator::ErrorResponse>(m_routing.obtain,
                                                          request, {}, {}, q);

  reply->setAutoDelete(true);
  reply
      ->onSucceeded(
          [q, this](int code,
                    const IJwtAuthenticator::ObtainTokenResponse& data) {
            updateAccessToken(data.access);
            updateRefreshToken(data.refresh);
            Q_EMIT q->loginSucceeded();
          })
      ->onError([q](const QString& detail) { Q_EMIT q->loginFailed(detail); })
      ->onFailed([q](int code, const IAuthenticator::ErrorResponse& data) {
        Q_EMIT q->loginFailed(data.detail);
      });
}

void JwtAuthenticatorPrivate::refresh() {
  Q_Q(JwtAuthenticator);
  const auto request =
      IJwtAuthenticator::TokenRefreshRequest{.refresh = m_refresh_token};

  auto reply = m_api->post<IJwtAuthenticator::TokenRefreshResponse,
                           IAuthenticator::ErrorResponse>(m_routing.refresh,
                                                          request, {}, {}, q);

  reply->setAutoDelete(true);
  reply
      ->onSucceeded(
          [q, this](int code,
                    const IJwtAuthenticator::TokenRefreshResponse& data) {
            updateAccessToken(data.access);
            Q_EMIT q->refreshSucceeded();
          })
      ->onError([q](const QString& detail) { Q_EMIT q->refreshFailed(detail); })
      ->onFailed([q](int code, const IAuthenticator::ErrorResponse& data) {
        Q_EMIT q->refreshFailed(data.detail);
      });
}

void JwtAuthenticatorPrivate::logout() {
  Q_Q(JwtAuthenticator);
  const auto request =
      IJwtAuthenticator::TokenBlacklistRequest{.refresh = m_refresh_token};

  auto reply = m_api->post<void, IAuthenticator::ErrorResponse>(
      m_routing.blacklist, request, {}, {}, q);

  reply->setAutoDelete(true);
  reply
      ->onSucceeded([q, this](int code) {
        updateAccessToken(QByteArray{});
        updateRefreshToken(QByteArray{});
        Q_EMIT q->logoutSucceeded();
      })
      ->onError([q](const QString& detail) { Q_EMIT q->logoutFailed(detail); })
      ->onFailed([q](int code, const IAuthenticator::ErrorResponse& data) {
        Q_EMIT q->logoutFailed(data.detail);
      });
}

rest::IClient* JwtAuthenticatorPrivate::getClient() const {
  return m_api->getClient();
}

QByteArray JwtAuthenticatorPrivate::getAccessToken() const {
  return m_access_token;
}

QByteArray JwtAuthenticatorPrivate::getRefreshToken() const {
  return m_refresh_token;
}

void JwtAuthenticatorPrivate::setRouting(
    const JwtAuthenticator::Routing& routing) {
  Q_Q(JwtAuthenticator);
  if (m_routing == routing) return;

  m_routing = routing;
  Q_EMIT q->routingChanged(m_routing);
}

JwtAuthenticator::Routing JwtAuthenticatorPrivate::getRouting() const {
  return m_routing;
}

void JwtAuthenticatorPrivate::updateAccessToken(const QByteArray& token) {
  m_access_token = token;

  auto client = m_api->getClient();
  auto headers = client->getGlobalHeaders();
  headers[TokenHeader] = TokenPrefix + " " + m_access_token;

  client->setGlobalHeaders(headers);
}

void JwtAuthenticatorPrivate::updateRefreshToken(const QByteArray& token) {
  m_refresh_token = token;
}

}  // namespace detail

}  // namespace egnite::auth

#include "egnite/auth/moc_authenticator.cpp"
