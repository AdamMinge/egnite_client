/* ----------------------------------- Local -------------------------------- */
#include "egnite/rest/rest_client.h"

#include "egnite/rest/rest_api.h"
/* -------------------------------------------------------------------------- */

namespace egnite::rest {

RestClient::RestClient(QObject* parent) : QObject(parent) {}

RestClient::~RestClient() = default;

RestApi* RestClient::createApi(const QString& path) {
  return new RestApi(this, path);
}

void RestClient::setBaseUrl(const QUrl& url) {
  if (m_base_url == url) return;

  m_base_url = url;
  Q_EMIT baseUrlChanged(m_base_url);
}

QUrl RestClient::getBaseUrl() const { return m_base_url; }

void RestClient::setVersion(const QVersionNumber& version) {
  if (m_version == version) return;

  m_version = version;
  Q_EMIT versionChanged(m_version);
}

QVersionNumber RestClient::getVersion() const { return m_version; }

}  // namespace egnite::rest
