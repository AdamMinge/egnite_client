#ifndef EGNITE_REST_API_P_H
#define EGNITE_REST_API_P_H

/* ------------------------------------ Qt ---------------------------------- */
#include <QtCore/private/qobject_p.h>

#include <QNetworkReply>
#include <QNetworkRequest>
/* ------------------------------------ Local ------------------------------- */
#include "egnite/rest/api.h"
/* -------------------------------------------------------------------------- */

namespace egnite::rest::detail {

/* ---------------------------------- ApiPrivate ---------------------------- */

class ApiPrivate : public QObjectPrivate {
 public:
  Q_DECLARE_PUBLIC(Api)

 public:
  explicit ApiPrivate(IClient* client, QNetworkAccessManager* manager,
                      const QString& path);

  [[nodiscard]] IClient* getClient() const;
  [[nodiscard]] IReplyDecorator* getReplyDecorator() const;
  [[nodiscard]] DataSerializer* getDataSerializer() const;
  [[nodiscard]] QString getPath() const;

  void setGlobalHeaders(const Headers& headers);
  [[nodiscard]] Headers getGlobalHeaders() const;

  void setGlobalParameters(const QUrlQuery& parameters);
  [[nodiscard]] QUrlQuery getGlobalParameters() const;

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

  [[nodiscard]] RequestBuilder getRequestBuilder() const;

 public:
  [[nodiscard]] static QByteArray convertData(const Data& body);
  [[nodiscard]] static QByteArray convertData(const QJsonValue& body);
  [[nodiscard]] static QByteArray convertData(const QCborValue& body);

 private:
  IClient* m_client;
  QNetworkAccessManager* m_manager;
  Headers m_headers;
  QUrlQuery m_parameters;
  QString m_path;
};

}  // namespace egnite::rest::detail

#endif  // EGNITE_REST_API_P_H