/* ----------------------------------- Local -------------------------------- */
#include "jwt_authenticator.h"
/* -------------------------------------------------------------------------- */

QmlJwtAuthenticator::QmlJwtAuthenticator(QObject* parent)
    : QObject(parent),
      m_init(false),
      m_client(nullptr),
      m_authenticator(nullptr) {}

QmlJwtAuthenticator::~QmlJwtAuthenticator() = default;

void QmlJwtAuthenticator::login(const QString& username,
                                const QString& password) {
  Q_ASSERT(m_authenticator);
  m_authenticator->login(username, password);
}

void QmlJwtAuthenticator::refresh() {
  Q_ASSERT(m_authenticator);
  m_authenticator->refresh();
}

void QmlJwtAuthenticator::logout() {
  Q_ASSERT(m_authenticator);
  m_authenticator->logout();
}

void QmlJwtAuthenticator::classBegin() {}

void QmlJwtAuthenticator::componentComplete() {
  m_init = true;
  revaluateAuthenticator();
}

void QmlJwtAuthenticator::setPath(const QString& path) {
  if (m_path == path) return;

  m_path = path;
  revaluateAuthenticator();

  Q_EMIT pathChanged(m_path);
}

QString QmlJwtAuthenticator::getPath() const { return m_path; }

void QmlJwtAuthenticator::setClient(egnite::rest::Client* client) {
  if (m_client == client) return;

  m_client = client;
  revaluateAuthenticator();

  Q_EMIT clientChanged(m_client);
}

egnite::rest::Client* QmlJwtAuthenticator::getClient() const {
  return m_client;
}

void QmlJwtAuthenticator::setRouting(
    const egnite::auth::JwtAuthenticator::Routing& routing) {
  if (m_routing == routing) return;

  m_routing = routing;
  revaluateAuthenticator();

  Q_EMIT routingChanged(m_routing);
}

egnite::auth::JwtAuthenticator::Routing QmlJwtAuthenticator::getRouting()
    const {
  return m_routing;
}

void QmlJwtAuthenticator::revaluateAuthenticator() {
  if (!m_init || !m_client) return;
  if (m_authenticator) m_authenticator->deleteLater();

  m_authenticator = new egnite::auth::JwtAuthenticator(m_client, m_path, this);
  m_authenticator->setRouting(m_routing);
}
