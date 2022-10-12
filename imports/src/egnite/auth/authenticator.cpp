/* ----------------------------------- Local -------------------------------- */
#include "authenticator.h"
/* -------------------------------------------------------------------------- */

QmlJwtAuthenticator::QmlJwtAuthenticator(QObject* parent)
    : egnite::auth::IJwtAuthenticator(parent), m_authenticator(nullptr) {}

QmlJwtAuthenticator::~QmlJwtAuthenticator() = default;

void QmlJwtAuthenticator::setPath(const QString& path) {
  if (m_revaluate_data.path == path) return;

  m_revaluate_data.path = path;
  revaluateAuthenticator();

  Q_EMIT pathChanged(m_revaluate_data.path);
}

QString QmlJwtAuthenticator::getPath() const { return m_revaluate_data.path; }

void QmlJwtAuthenticator::setClient(egnite::rest::Client* client) {
  if (m_revaluate_data.client == client) return;

  m_revaluate_data.client = client;
  revaluateAuthenticator();

  Q_EMIT clientChanged(m_revaluate_data.client);
}

egnite::rest::Client* QmlJwtAuthenticator::getClient() const {
  return m_revaluate_data.client;
}

void QmlJwtAuthenticator::setRouting(
    const egnite::auth::IJwtAuthenticator::Routing& routing) {
  if (m_revaluate_data.routing == routing) return;

  m_revaluate_data.routing = routing;
  revaluateAuthenticator();

  Q_EMIT routingChanged(m_revaluate_data.routing);
}

egnite::auth::IJwtAuthenticator::Routing QmlJwtAuthenticator::getRouting()
    const {
  return m_revaluate_data.routing;
}

void QmlJwtAuthenticator::classBegin() {}

void QmlJwtAuthenticator::componentComplete() {
  m_revaluate_data.init = true;
  revaluateAuthenticator();
}

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

QByteArray QmlJwtAuthenticator::getAccessToken() const {
  return m_authenticator ? m_authenticator->getAccessToken() : QByteArray{};
}

QByteArray QmlJwtAuthenticator::getRefreshToken() const {
  return m_authenticator ? m_authenticator->getRefreshToken() : QByteArray{};
}

void QmlJwtAuthenticator::revaluateAuthenticator() {
  if (!m_revaluate_data.init) return;
  if (m_authenticator) m_authenticator->deleteLater();

  Q_ASSERT(m_revaluate_data.client);
  m_authenticator = new egnite::auth::JwtAuthenticator(
      m_revaluate_data.client, m_revaluate_data.path, this);
  m_authenticator->setRouting(m_revaluate_data.routing);
}
