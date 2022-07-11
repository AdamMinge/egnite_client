/* ----------------------------------- Local -------------------------------- */
#include "egnite/egnite/web/web_authorizator.h"
/* -------------------------------------------------------------------------- */

namespace egnite::web {

/* ----------------------------- WebAuthorizator -------------------------- */

WebAuthorizator::WebAuthorizator(QObject *parent) : QObject(parent) {}

WebAuthorizator::~WebAuthorizator() = default;

/* --------------------------- ApiKeyAuthorizator ------------------------- */

ApiKeyAuthorizator::ApiKeyAuthorizator(QObject *parent) : WebAuthorizator(parent) {}

ApiKeyAuthorizator::~ApiKeyAuthorizator() = default;

void ApiKeyAuthorizator::setRequestHeaders(QNetworkRequest &request) {
  Q_ASSERT(!request.hasRawHeader("API-Key"));
  request.setRawHeader("API-Key", m_api_key.toLocal8Bit());
}

QString ApiKeyAuthorizator::getApiKey() const { return m_api_key; }

void ApiKeyAuthorizator::setApiKey(const QString &api_key) {
  if (m_api_key == api_key)
    return;

  m_api_key = api_key;
  Q_EMIT apiKeyChanged(m_api_key);
}

} // namespace egnite::web
