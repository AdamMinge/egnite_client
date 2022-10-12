/* ----------------------------------- Local -------------------------------- */
#include "reply_factory.h"
/* ----------------------------------- Egnite ------------------------------- */
#include <egnite/auth/reply_factory.h>
/* -------------------------------------------------------------------------- */

QmlJwtAuthenticatorReplyFactory::QmlJwtAuthenticatorReplyFactory(
    QObject* parent)
    : egnite::rest::IReplyFactory(parent), m_factory(nullptr) {}

QmlJwtAuthenticatorReplyFactory::~QmlJwtAuthenticatorReplyFactory() = default;

void QmlJwtAuthenticatorReplyFactory::setAuthenticator(
    QmlJwtAuthenticator* authenticator) {
  if (m_revaluate_data.authenticator == authenticator) return;

  m_revaluate_data.authenticator = authenticator;
  revaluateFactory();

  Q_EMIT authenticatorChanged(m_revaluate_data.authenticator);
}

QmlJwtAuthenticator* QmlJwtAuthenticatorReplyFactory::getAuthenticator() const {
  return m_revaluate_data.authenticator;
}

void QmlJwtAuthenticatorReplyFactory::classBegin() {}

void QmlJwtAuthenticatorReplyFactory::componentComplete() {
  m_revaluate_data.init = true;
  revaluateFactory();
}

egnite::rest::IReply* QmlJwtAuthenticatorReplyFactory::create(
    egnite::rest::IReply* reply) {
  if (!m_factory) return reply;
  return m_factory->create(reply);
}

void QmlJwtAuthenticatorReplyFactory::revaluateFactory() {
  if (!m_revaluate_data.init) return;
  if (m_factory) m_factory->deleteLater();

  Q_ASSERT(m_revaluate_data.authenticator);
  m_factory = new egnite::auth::JwtAuthenticatorReplyFactory(
      m_revaluate_data.authenticator, this);
}
