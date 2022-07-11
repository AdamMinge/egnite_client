/* ----------------------------------- Local -------------------------------- */
#include "egnite/egnite/web/web_client.h"
#include "egnite/egnite/web/web_authenticator.h"
#include "egnite/egnite/web/web_authorizator.h"
/* -------------------------------------------------------------------------- */

namespace egnite::web {

WebClient::WebClient(QObject *parent)
    : QObject(parent), m_authenticator(nullptr), m_authorizator(nullptr) {}

WebClient::~WebClient() = default;

void WebClient::login(const QString &username, const QString &password) {
  QNetworkRequest login_request;

  m_authorizator->setRequestHeaders(login_request);
  // m_authenticator->setRequestHeaders(login_request);

  m_network_access_manager.post(login_request, QByteArray{});
}

void WebClient::logout() {}

QUrl WebClient::getBaseUrl() const { return m_base_url; }

void WebClient::setBaseUrl(const QUrl &base_url) {
  if (m_base_url == base_url)
    return;

  m_base_url = base_url;
  Q_EMIT baseUrlChanged(m_base_url);
}

WebAuthenticator *WebClient::getAuthenticator() const {
  return m_authenticator;
}

void WebClient::setAuthenticator(WebAuthenticator *authenticator) {
  if (m_authenticator == authenticator)
    return;

  m_authenticator = authenticator;
  Q_EMIT authenticatorChanged(m_authenticator);
}

WebAuthorizator *WebClient::getAuthorizator() const { return m_authorizator; }

void WebClient::setAuthorizator(WebAuthorizator *authorizator) {
  if (m_authorizator == authorizator)
    return;

  m_authorizator = authorizator;
  Q_EMIT authorizatorChanged(m_authorizator);
}

} // namespace egnite::web
