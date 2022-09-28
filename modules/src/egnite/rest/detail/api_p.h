#ifndef EGNITE_REST_API_P_H
#define EGNITE_REST_API_P_H

/* ------------------------------------ Qt ---------------------------------- */
#include <QtCore/private/qobject_p.h>

#include <QNetworkReply>
#include <QNetworkRequest>
/* ------------------------------------ Local ------------------------------- */
#include "egnite/rest/api.h"
/* -------------------------------------------------------------------------- */

namespace egnite::rest {

class Client;

namespace detail {

class ApiPrivate : public QObjectPrivate {
 public:
  Q_DECLARE_PUBLIC(Api)

 public:
  static const QByteArray GetVerb;
  static const QByteArray PostVerb;
  static const QByteArray PutVerb;
  static const QByteArray PatchVerb;
  static const QByteArray DeleteVerb;
  static const QByteArray HeadVerb;

 public:
  explicit ApiPrivate(Client* client, QNetworkAccessManager* manager,
                      const QString& path);

  [[nodiscard]] Client* getClient() const;
  [[nodiscard]] DataSerializer* getDataSerializer() const;
  [[nodiscard]] ReplyDecorator* getReplyDecorator() const;
  [[nodiscard]] QString getPath() const;

  [[nodiscard]] DataSerializer::Format getRequestDataFormat(
      const Headers& headers) const;

  QNetworkReply* create(const QByteArray& verb, const QString& path,
                        const QUrlQuery& parameters,
                        const Headers& headers) const;
  QNetworkReply* create(const QByteArray& verb, const QString& path,
                        const QJsonValue& data, const QUrlQuery& parameters,
                        const Headers& headers) const;
  QNetworkReply* create(const QByteArray& verb, const QString& path,
                        const QCborValue& data, const QUrlQuery& parameters,
                        const Headers& headers) const;

 public:
  [[nodiscard]] static QByteArray convertData(const Data& body);
  [[nodiscard]] static QByteArray convertData(const QJsonValue& body);
  [[nodiscard]] static QByteArray convertData(const QCborValue& body);

 private:
  Client* m_client;
  QNetworkAccessManager* m_manager;
  QString m_path;
};

}  // namespace detail

}  // namespace egnite::rest

#endif  // EGNITE_REST_API_P_H