/* ----------------------------------- Local -------------------------------- */
#include "egnite/egnite/web/web_client.h"
/* -------------------------------------------------------------------------- */

namespace egnite::web
{

  WebClient::WebClient() = default;

  WebClient::~WebClient() = default;

  QUrl WebClient::getBaseUrl() const { return m_base_url; }

  void WebClient::setBaseUrl(const QUrl &base_url)
  {
    if (m_base_url == base_url) return;

    m_base_url = base_url;
    Q_EMIT baseUrlChanged(m_base_url);
  }

}// namespace egnite::web
