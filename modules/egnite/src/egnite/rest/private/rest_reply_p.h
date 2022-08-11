#ifndef EGNITE_REST_REPLY_P_H
#define EGNITE_REST_REPLY_P_H

/* ------------------------------------ Qt ---------------------------------- */
#include <QtCore/private/qobject_p.h>

#include <QByteArray>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
/* ---------------------------------- Standard ------------------------------ */
#include <chrono>
/* -------------------------------------------------------------------------- */

namespace egnite::rest {

class RestReplyPrivate : public QObjectPrivate {
 public:
  Q_DECLARE_PUBLIC(RestReply)

 public:
  static const QByteArray PropertyBody;
  static const QByteArray ContentTypeJson;
  static const QByteArray ContentTypeCbor;

 public:
  RestReplyPrivate(QNetworkReply* network_reply,
                   const std::chrono::milliseconds& retry_delay);
  ~RestReplyPrivate() override;

  void abort();
  void retry(std::chrono::milliseconds msecs);

  void replyFinished();

 public:
  static QNetworkReply* send(QNetworkAccessManager* manager,
                             const QNetworkRequest& request,
                             const QByteArray& verb, const QByteArray& body);

 private:
  void connectReply();
  void retryReply();

 private:
  QPointer<QNetworkReply> m_network_reply;
  std::chrono::milliseconds m_retry_delay;
};

}  // namespace egnite::rest

#endif  // EGNITE_REST_REPLY_P_H