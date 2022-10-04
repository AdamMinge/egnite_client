#ifndef EGNITE_REST_RAW_REPLY_P_H
#define EGNITE_REST_RAW_REPLY_P_H

/* ------------------------------------ Qt ---------------------------------- */
#include <QtCore/private/qobject_p.h>

#include <QByteArray>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QPointer>
/* ------------------------------------ Local ------------------------------- */
#include "egnite/rest/raw_reply.h"
/* -------------------------------------------------------------------------- */

namespace egnite::rest::detail {

class RawReplyPrivate : public QObjectPrivate {
 public:
  Q_DECLARE_PUBLIC(RawReply)

 public:
  static const QByteArray PropertyBody;
  static const QByteArray Accept;
  static const QByteArray ContentType;
  static const QByteArray ContentTypeJson;
  static const QByteArray ContentTypeCbor;

  using ParseError = std::optional<std::pair<RawReply::Error, QString>>;

 public:
  explicit RawReplyPrivate(Api* api, QNetworkReply* network_reply);
  ~RawReplyPrivate() override;

  void abort();
  void retry();

  [[nodiscard]] Api* getApi() const;
  [[nodiscard]] Client* getClient() const;
  [[nodiscard]] DataSerializer* getDataSerializer() const;

  void setAutoDelete(bool enable);
  [[nodiscard]] bool isAutoDelete() const;

  void connectReply();

 public:
  static QNetworkReply* send(QNetworkAccessManager* manager,
                             const QNetworkRequest& request,
                             const QByteArray& verb, const QByteArray& body);

 private:
  void replyFinished();

  [[nodiscard]] QByteArray parseContentType(ParseError& parse_error);
  [[nodiscard]] Data parseData(const QByteArray& content_type,
                               ParseError& parse_error);
  [[nodiscard]] Data parseJsonData(ParseError& parse_error);
  [[nodiscard]] Data parseCborData(ParseError& parse_error);
  void processReply(const Data& data, const ParseError& parse_error);

 private:
  Api* m_api;
  QPointer<QNetworkReply> m_network_reply;
  bool m_auto_delete;
};

}  // namespace egnite::rest::detail

#endif  // EGNITE_REST_RAW_REPLY_P_H