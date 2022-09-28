/* ----------------------------------- Local -------------------------------- */
#include "egnite/auth/jwt_authenticator.h"

#include "egnite/auth/detail/jwt_authenticator_p.h"
/* ----------------------------------- Egnite ------------------------------- */
#include <egnite/rest/api.h>
#include <egnite/rest/client.h>
#include <egnite/rest/reply_decorator.h>
/* -------------------------------------------------------------------------- */

namespace egnite::auth {

/* ------------------------------ JwtAuthenticator -------------------------- */

JwtAuthenticator::JwtAuthenticator(rest::Client* client, const QString& subpath,
                                   QObject* parent)
    : JwtAuthenticator(*new detail::JwtAuthenticatorPrivate(client, subpath),
                       parent) {}

JwtAuthenticator::JwtAuthenticator(detail::JwtAuthenticatorPrivate& impl,
                                   QObject* parent)
    : Authenticator(impl, parent) {}

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

rest::Client* JwtAuthenticator::getClient() const {
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

/* ---------------------------- JwtAuthenticatorReply ----------------------- */

JwtAuthenticatorReply::JwtAuthenticatorReply(JwtAuthenticator* authenticator,
                                             rest::Reply* reply,
                                             QObject* parent)
    : JwtAuthenticatorReply(
          *new detail::JwtAuthenticatorReplyPrivate(authenticator, reply),
          parent) {}

JwtAuthenticatorReply::JwtAuthenticatorReply(
    detail::JwtAuthenticatorReplyPrivate& impl, QObject* parent)
    : Reply(impl, parent) {
  connect(this, &rest::Reply::succeeded, this, &rest::Reply::completed);
  connect(this, &rest::Reply::failed, this, &rest::Reply::completed);
}

JwtAuthenticatorReply::~JwtAuthenticatorReply() = default;

void JwtAuthenticatorReply::abort() {
  Q_D(detail::JwtAuthenticatorReply);
  d->abort();
}

void JwtAuthenticatorReply::retry() {
  Q_D(detail::JwtAuthenticatorReply);
  d->retry();
}

rest::Api* JwtAuthenticatorReply::getApi() const {
  Q_D(const detail::JwtAuthenticatorReply);
  return d->getApi();
}

rest::Client* JwtAuthenticatorReply::getClient() const {
  Q_D(const detail::JwtAuthenticatorReply);
  return d->getClient();
}

rest::DataSerializer* JwtAuthenticatorReply::getDataSerializer() const {
  Q_D(const detail::JwtAuthenticatorReply);
  return d->getDataSerializer();
}

/* --------------------------- JwtAuthenticatorPrivate ---------------------- */

namespace detail {

const QByteArray JwtAuthenticatorPrivate::TokenHeader =
    QByteArray{"Authorization"};
const QByteArray JwtAuthenticatorPrivate::TokenPrefix = QByteArray{"Bearer"};
const JwtAuthenticator::Routing JwtAuthenticatorPrivate::DefaultRouting =
    JwtAuthenticator::Routing{
        .obtain = "", .refresh = "/refresh", .blacklist = "/blacklist"};

JwtAuthenticatorPrivate::JwtAuthenticatorPrivate(rest::Client* client,
                                                 const QString& path)
    : m_api(client->createApi(path)), m_routing(DefaultRouting) {
  Q_Q(JwtAuthenticator);
  auto reply_decorator = m_api->getClient()->getReplyDecorator();
  reply_decorator->registerFactory<JwtAuthenticatorReply>(0x00FFFFFF, q);
}

JwtAuthenticatorPrivate::~JwtAuthenticatorPrivate() {
  auto reply_decorator = m_api->getClient()->getReplyDecorator();
  reply_decorator->unregisterFactory<JwtAuthenticatorReply>();
}

void JwtAuthenticatorPrivate::login(const QString& username,
                                    const QString& password) {
  Q_Q(JwtAuthenticator);
  const auto request = JwtAuthenticator::ObtainTokenRequest{
      .username = username, .password = password};

  auto reply = m_api->post<JwtAuthenticator::ObtainTokenResponse,
                           Authenticator::ErrorResponse>(m_routing.obtain,
                                                         request, {}, {}, q);

  reply
      ->onSucceeded(
          [q, this](int code,
                    const JwtAuthenticator::ObtainTokenResponse& data) {
            updateAccessToken(data.access);
            updateRefreshToken(data.refresh);
            Q_EMIT q->loginSucceeded();
          })
      ->onError([q](const QString& detail) { Q_EMIT q->loginFailed(detail); })
      ->onFailed([q](int code, const Authenticator::ErrorResponse& data) {
        Q_EMIT q->loginFailed(data.detail);
      });
}

void JwtAuthenticatorPrivate::refresh() {
  Q_Q(JwtAuthenticator);
  const auto request =
      JwtAuthenticator::TokenRefreshRequest{.refresh = m_refresh_token};

  auto reply = m_api->post<JwtAuthenticator::TokenRefreshResponse,
                           Authenticator::ErrorResponse>(m_routing.refresh,
                                                         request, {}, {}, q);

  reply
      ->onSucceeded(
          [q, this](int code,
                    const JwtAuthenticator::TokenRefreshResponse& data) {
            updateAccessToken(data.access);
            Q_EMIT q->refreshSucceeded();
          })
      ->onError([q](const QString& detail) { Q_EMIT q->refreshFailed(detail); })
      ->onFailed([q](int code, const Authenticator::ErrorResponse& data) {
        Q_EMIT q->refreshFailed(data.detail);
      });
}

void JwtAuthenticatorPrivate::logout() {
  Q_Q(JwtAuthenticator);
  const auto request =
      JwtAuthenticator::TokenBlacklistRequest{.refresh = m_refresh_token};

  auto reply = m_api->post<void, Authenticator::ErrorResponse>(
      m_routing.blacklist, request, {}, {}, q);

  reply
      ->onSucceeded([q, this](int code) {
        updateAccessToken(QByteArray{});
        updateRefreshToken(QByteArray{});
        Q_EMIT q->logoutSucceeded();
      })
      ->onError([q](const QString& detail) { Q_EMIT q->logoutFailed(detail); })
      ->onFailed([q](int code, const Authenticator::ErrorResponse& data) {
        Q_EMIT q->logoutFailed(data.detail);
      });
}

rest::Client* JwtAuthenticatorPrivate::getClient() const {
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

/* ------------------------- JwtAuthenticatorReplyPrivate ------------------- */

JwtAuthenticatorReplyPrivate::JwtAuthenticatorReplyPrivate(
    JwtAuthenticator* authenticator, rest::Reply* reply)
    : m_authenticator(authenticator), m_reply(reply) {
  Q_Q(JwtAuthenticatorReply);

  QObject::connect(m_reply, &rest::Reply::succeeded, q,
                   &rest::Reply::succeeded);
  QObject::connect(m_reply, &rest::Reply::error, q, &rest::Reply::error);

  QObject::connect(m_reply, &rest::Reply::downloadProgress, q,
                   &rest::Reply::downloadProgress);
  QObject::connect(m_reply, &rest::Reply::uploadProgress, q,
                   &rest::Reply::uploadProgress);

  m_reply->onFailed([this, q](int code, const rest::Data& data) {
    if (code == 401) {
      m_authenticator
          ->onRefreshFailed([q, code, data]() { Q_EMIT q->failed(code, data); })
          ->onRefreshSucceeded([q]() { q->retry(); });
      m_authenticator->refresh();
    } else {
      Q_EMIT q->failed(code, data);
    }
  });
}

void JwtAuthenticatorReplyPrivate::abort() { m_reply->abort(); }

void JwtAuthenticatorReplyPrivate::retry() { m_reply->retry(); }

rest::Api* JwtAuthenticatorReplyPrivate::getApi() const {
  return m_reply->getApi();
}

rest::Client* JwtAuthenticatorReplyPrivate::getClient() const {
  return m_reply->getClient();
}

rest::DataSerializer* JwtAuthenticatorReplyPrivate::getDataSerializer() const {
  return m_reply->getDataSerializer();
}

}  // namespace detail

}  // namespace egnite::auth

#include "egnite/auth/moc_jwt_authenticator.cpp"