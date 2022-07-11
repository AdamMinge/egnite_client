/* ----------------------------------- Local -------------------------------- */
#include "egnite/egnite/web/web_authenticator.h"
/* -------------------------------------------------------------------------- */

namespace egnite::web {

/* ----------------------------- WebAuthenticator ------------------------- */

WebAuthenticator::WebAuthenticator(QObject *parent) : QObject(parent) {}

WebAuthenticator::~WebAuthenticator() = default;

/* -------------------------- SimpleJWTAuthenticator ---------------------- */

SimpleJWTAuthenticator::SimpleJWTAuthenticator(QObject *parent)
    : WebAuthenticator(parent), m_accessTokenLifetime(), m_refreshTokenLifetime() {}

SimpleJWTAuthenticator::~SimpleJWTAuthenticator() = default;

unsigned SimpleJWTAuthenticator::getAccessTokenLifetime() const { return m_accessTokenLifetime; }

void SimpleJWTAuthenticator::setAccessTokenLifetime(unsigned accessTokenLifetime) {
  if (m_accessTokenLifetime == accessTokenLifetime)
    return;

  m_accessTokenLifetime = accessTokenLifetime;
  Q_EMIT accessTokenLifetimeChanged(m_accessTokenLifetime);
}

unsigned SimpleJWTAuthenticator::getRefreshTokenLifetime() const { return m_accessTokenLifetime; }

void SimpleJWTAuthenticator::setRefreshTokenLifetime(unsigned refreshTokenLifetime) {
  if (m_refreshTokenLifetime == refreshTokenLifetime)
    return;

  m_refreshTokenLifetime = refreshTokenLifetime;
  Q_EMIT refreshTokenLifetimeChanged(m_refreshTokenLifetime);
}

} // namespace egnite::web
