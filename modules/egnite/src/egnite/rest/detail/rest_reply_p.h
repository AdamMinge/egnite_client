#ifndef EGNITE_REST_REPLY_P_H
#define EGNITE_REST_REPLY_P_H

/* ------------------------------------ Qt ---------------------------------- */
#include <QtCore/private/qobject_p.h>

#include <QByteArray>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QPointer>
/* ------------------------------------ Local ------------------------------- */
#include "egnite/rest/rest_reply.h"
/* -------------------------------------------------------------------------- */

namespace egnite::rest::detail {

class RestReplyPrivate : public QObjectPrivate {
 public:
  Q_DECLARE_PUBLIC(RestReply)

 public:
  static const QByteArray PropertyBody;
  static const QByteArray Accept;
  static const QByteArray ContentType;
  static const QByteArray ContentTypeJson;
  static const QByteArray ContentTypeCbor;

  using ParseError = std::optional<std::pair<RestReply::Error, QString>>;

 public:
  RestReplyPrivate(QNetworkReply* network_reply);
  ~RestReplyPrivate() override;

  void abort();
  void retry();

 public:
  static QNetworkReply* send(QNetworkAccessManager* manager,
                             const QNetworkRequest& request,
                             const QByteArray& verb, const QByteArray& body);

 private:
  void replyFinished();
  void connectReply();

  [[nodiscard]] QByteArray parseContentType(ParseError& parse_error);
  [[nodiscard]] RestData parseData(const QByteArray& content_type,
                                   ParseError& parse_error);
  [[nodiscard]] RestData parseJsonData(ParseError& parse_error);
  [[nodiscard]] RestData parseCborData(ParseError& parse_error);
  void processReply(const RestData& data, const ParseError& parse_error);

 private:
  QPointer<QNetworkReply> m_network_reply;
};

}  // namespace egnite::rest::detail

#endif  // EGNITE_REST_REPLY_P_H