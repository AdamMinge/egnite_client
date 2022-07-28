#ifndef EGNITE_WEB_REPLY_H
#define EGNITE_WEB_REPLY_H

/* ------------------------------------ Qt ---------------------------------- */
#include <QNetworkReply>
/* ----------------------------------- Local -------------------------------- */
#include "egnite/egnite/export.h"
#include "egnite/egnite/web/serializer.h"
/* -------------------------------------------------------------------------- */

namespace egnite::web {

class EGNITE_API Reply {

public:
  explicit Reply(QNetworkReply* reply, Serializer* serializer)
      : m_reply(reply), m_serializer(serializer) {}

  template <typename SUCCESS_TYPE>
  Reply& onSuccess(std::function<void(const SUCCESS_TYPE&)>&& method);
  Reply& onSuccess(std::function<void(void)>&& method);

  Reply& onError(std::function<void(QNetworkReply::NetworkError)>&& method);
  Reply& onSslError(std::function<void(const QList<QSslError>&)>&& method);

private:
  QNetworkReply* m_reply;
  Serializer* m_serializer;
};

template <typename SUCCESS_TYPE>
Reply& Reply::onSuccess(std::function<void(const SUCCESS_TYPE&)>&& method) {
  m_reply->connect(m_reply, &QNetworkReply::finished, m_reply,
                   [reply = m_reply, serializer = m_serializer, method = std::move(method)]() {
                     if (reply->error() == QNetworkReply::NoError) {
                       method(serializer->deserialize<SUCCESS_TYPE>(reply->readAll()));
                     }
                   });
  return *this;
}

} // namespace egnite::web

#endif // EGNITE_WEB_REPLY_H
