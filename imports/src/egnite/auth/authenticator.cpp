/* ----------------------------------- Local -------------------------------- */
#include "authenticator.h"
/* -------------------------------------------------------------------------- */

/* ---------------------------- QmlJwtAuthenticator ------------------------- */

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

void QmlJwtAuthenticator::login(const QString& email, const QString& password) {
  if (!m_authenticator) {
    qmlWarning(this) << "components wasn't evaluate correctly or not at all";
    return;
  }

  m_authenticator->login(email, password);
}

void QmlJwtAuthenticator::refresh() {
  if (!m_authenticator) {
    qmlWarning(this) << "components wasn't evaluate correctly or not at all";
    return;
  }

  m_authenticator->refresh();
}

void QmlJwtAuthenticator::logout() {
  if (!m_authenticator) {
    qmlWarning(this) << "components wasn't evaluate correctly or not at all";
    return;
  }

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

  if (!m_revaluate_data.client) {
    qmlWarning(this) << "client property must be set";
  } else {
    m_authenticator = new egnite::auth::JwtAuthenticator(
        m_revaluate_data.client, m_revaluate_data.path, this);
    m_authenticator->setRouting(m_revaluate_data.routing);

    connect(m_authenticator, &IJwtAuthenticator::accessTokenChanged, this,
            &QmlJwtAuthenticator::accessTokenChanged);
    connect(m_authenticator, &IJwtAuthenticator::refreshTokenChanged, this,
            &QmlJwtAuthenticator::refreshTokenChanged);

    connect(m_authenticator, &IJwtAuthenticator::routingChanged, this,
            &QmlJwtAuthenticator::routingChanged);

    connect(m_authenticator, &IJwtAuthenticator::loginSucceeded, this,
            &QmlJwtAuthenticator::loginSucceeded);
    connect(m_authenticator, &IJwtAuthenticator::logoutSucceeded, this,
            &QmlJwtAuthenticator::logoutSucceeded);
    connect(m_authenticator, &IJwtAuthenticator::refreshSucceeded, this,
            &QmlJwtAuthenticator::refreshSucceeded);

    connect(m_authenticator, &IJwtAuthenticator::loginFailed, this,
            &QmlJwtAuthenticator::loginFailed);
    connect(m_authenticator, &IJwtAuthenticator::logoutFailed, this,
            &QmlJwtAuthenticator::logoutFailed);
    connect(m_authenticator, &IJwtAuthenticator::refreshFailed, this,
            &QmlJwtAuthenticator::refreshFailed);
  }
}
