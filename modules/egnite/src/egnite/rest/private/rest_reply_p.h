#ifndef EGNITE_REST_REPLY_P_H
#define EGNITE_REST_REPLY_P_H

/* ------------------------------------ Qt ---------------------------------- */
#include <QtCore/private/qobject_p.h>

#include <QNetworkReply>
/* ---------------------------------- Standard ------------------------------ */
#include <chrono>
/* -------------------------------------------------------------------------- */

namespace egnite::rest {

class RestReplyPrivate : public QObjectPrivate {
 public:
  Q_DECLARE_PUBLIC(RestReply)

 public:
  RestReplyPrivate(QNetworkReply* network_reply,
                   const std::chrono::milliseconds& retry_delay);

  QPointer<QNetworkReply> network_reply;
  std::chrono::milliseconds retry_delay;
};

}  // namespace egnite::rest

#endif  // EGNITE_REST_REPLY_P_H