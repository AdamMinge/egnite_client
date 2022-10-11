/* ----------------------------------- Local -------------------------------- */
#include "jwt_authenticator_reply.h"
/* ----------------------------------- Egnite ------------------------------- */
#include <egnite/auth/jwt_authenticator_reply.h>
/* -------------------------------------------------------------------------- */

QmlJwtAuthenticatorReplyFactory::QmlJwtAuthenticatorReplyFactory(
    QObject* parent)
    : QmlReplyFactory(parent),
      m_init(false),
      m_authenticator(nullptr),
      m_factory(nullptr) {}

QmlJwtAuthenticatorReplyFactory::~QmlJwtAuthenticatorReplyFactory() = default;

void QmlJwtAuthenticatorReplyFactory::classBegin() {}

void QmlJwtAuthenticatorReplyFactory::componentComplete() {
  m_init = true;
  revaluateFactory();
}

void QmlJwtAuthenticatorReplyFactory::setAuthenticator(
    QmlJwtAuthenticator* authenticator) {
  if (m_authenticator == authenticator) return;

  m_authenticator = authenticator;
  revaluateFactory();

  Q_EMIT authenticatorChanged(m_authenticator);
}

QmlJwtAuthenticator* QmlJwtAuthenticatorReplyFactory::getAuthenticator() const {
  return m_authenticator;
}

egnite::rest::Reply* QmlJwtAuthenticatorReplyFactory::create(
    egnite::rest::Reply* reply) {
  if (!m_factory) return reply;
  return m_factory->create(reply);
}

void QmlJwtAuthenticatorReplyFactory::revaluateFactory() {
  if (!m_init) return;
  if (m_factory) m_factory->deleteLater();

  // Q_ASSERT(m_authenticator);
  qDebug() << "m_authenticator = " << m_authenticator;
  m_factory =
      new egnite::auth::JwtAuthenticatorReplyFactory(m_authenticator, this);
}
