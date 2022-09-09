/* ----------------------------------- Local -------------------------------- */
#include "egnite/auth/jwt_authenticator.h"

#include "egnite/auth/detail/jwt_authenticator_p.h"
/* ----------------------------------- Egnite ------------------------------- */
#include <egnite/rest/api.h>
#include <egnite/rest/client.h>
#include <egnite/rest/reply_decorator_manager.h>
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

void JwtAuthenticator::logout() {
  Q_D(detail::JwtAuthenticator);
  d->logout();
}

rest::Client* JwtAuthenticator::getClient() const {
  Q_D(const detail::JwtAuthenticator);
  return d->getClient();
}

void JwtAuthenticator::setHeaders(const rest::Headers& headers) {
  Q_D(detail::JwtAuthenticator);
  d->setHeaders(headers);
}

rest::Headers JwtAuthenticator::getHeaders() const {
  Q_D(const detail::JwtAuthenticator);
  return d->getHeaders();
}

void JwtAuthenticator::setParameters(const QUrlQuery& parameters) {
  Q_D(detail::JwtAuthenticator);
  d->setParameters(parameters);
}

QUrlQuery JwtAuthenticator::getParameters() const {
  Q_D(const detail::JwtAuthenticator);
  return d->getParameters();
}

QByteArray JwtAuthenticator::getAccessToken() const {
  Q_D(const detail::JwtAuthenticator);
  return d->getAccessToken();
}

QByteArray JwtAuthenticator::getRefreshToken() const {
  Q_D(const detail::JwtAuthenticator);
  return d->getRefreshToken();
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
    : Reply(impl, parent) {}

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

const QString JwtAuthenticatorPrivate::ReplyDecorator =
    QByteArray{"__Egnite_Auth_JwtAuthenticatorPrivate_ReplyDecorator"};

JwtAuthenticatorPrivate::JwtAuthenticatorPrivate(rest::Client* client,
                                                 const QString& path)
    : m_api(client->createApi(path)) {
  Q_Q(JwtAuthenticator);
  auto decorator_manager = m_api->getClient()->getReplyDecoratorManager();
  decorator_manager->addDecorator(
      ReplyDecorator, [q](rest::Reply* reply) -> rest::Reply* {
        auto reply_wrapper =
            new JwtAuthenticatorReply(q, reply, reply->parent());
        reply->setParent(reply_wrapper);

        return reply_wrapper;
      });
}

JwtAuthenticatorPrivate::~JwtAuthenticatorPrivate() {
  auto decorator_manager = m_api->getClient()->getReplyDecoratorManager();
  decorator_manager->removeDecorator(ReplyDecorator);
}

void JwtAuthenticatorPrivate::login(const QString& username,
                                    const QString& password) {}

void JwtAuthenticatorPrivate::logout() {}

rest::Client* JwtAuthenticatorPrivate::getClient() const {
  return m_api->getClient();
}

void JwtAuthenticatorPrivate::setHeaders(const rest::Headers& headers) {
  Q_Q(JwtAuthenticator);
  if (m_headers == headers) return;

  m_headers = headers;
  Q_EMIT q->headersChanged(m_headers);
}

rest::Headers JwtAuthenticatorPrivate::getHeaders() const { return m_headers; }

void JwtAuthenticatorPrivate::setParameters(const QUrlQuery& parameters) {
  Q_Q(JwtAuthenticator);
  if (m_parameters == parameters) return;

  m_parameters = parameters;
  Q_EMIT q->parametersChanged(m_parameters);
}

QUrlQuery JwtAuthenticatorPrivate::getParameters() const {
  return m_parameters;
}

QByteArray JwtAuthenticatorPrivate::getAccessToken() const {
  return m_access_token;
}

QByteArray JwtAuthenticatorPrivate::getRefreshToken() const {
  return m_refresh_token;
}

/* ------------------------- JwtAuthenticatorReplyPrivate ------------------- */

JwtAuthenticatorReplyPrivate::JwtAuthenticatorReplyPrivate(
    JwtAuthenticator* authenticator, rest::Reply* reply)
    : m_authenticator(authenticator), m_reply(reply) {}

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