/* ----------------------------------- Local -------------------------------- */
#include "egnite/egnite/web/reply.h"
/* -------------------------------------------------------------------------- */

namespace egnite::web {

Reply& Reply::onSuccess(std::function<void(void)>&& method) {
  m_reply->connect(m_reply, &QNetworkReply::finished, m_reply,
                   [reply = m_reply, serializer = m_serializer, method = std::move(method)]() {
                     if (reply->error() == QNetworkReply::NoError) {
                       method();
                     }
                   });
  return *this;
}

Reply& Reply::onError(std::function<void(QNetworkReply::NetworkError)>&& method) {
  m_reply->connect(m_reply, &QNetworkReply::errorOccurred, m_reply, std::move(method));
  return *this;
}

Reply& Reply::onSslError(std::function<void(const QList<QSslError>&)>&& method) {
  m_reply->connect(m_reply, &QNetworkReply::sslErrors, m_reply, std::move(method));
  return *this;
}

} // namespace egnite::web
