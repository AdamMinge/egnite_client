#ifndef EGNITE_WEB_CLIENT_H
#define EGNITE_WEB_CLIENT_H

/* ------------------------------------ Qt ---------------------------------- */
#include <QJsonObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QObject>
#include <QUrl>
/* ----------------------------------- Local -------------------------------- */
#include "egnite/egnite/export.h"
#include "egnite/egnite/web/headers.h"
#include "egnite/egnite/web/reply.h"
#include "egnite/egnite/web/serializer.h"
/* -------------------------------------------------------------------------- */

namespace egnite::web {

class EGNITE_API Client : public QObject {
  Q_OBJECT

public:
  Q_PROPERTY(QUrl baseUrl READ getBaseUrl WRITE setBaseUrl NOTIFY baseUrlChanged)
  Q_PROPERTY(Headers *headers READ getHeaders WRITE setHeaders NOTIFY headersChanged)
  Q_PROPERTY(Serializer *serializer READ getSerializer WRITE setSerializer NOTIFY serializerChanged)

public:
  explicit Client(QObject *parent = nullptr);
  ~Client() override;

  [[nodiscard]] const QUrl &getBaseUrl() const;
  [[nodiscard]] QUrl getUrl(const QUrl &url) const;

  void setBaseUrl(const QUrl &base_url);

  [[nodiscard]] Headers *getHeaders() const;
  void setHeaders(Headers *headers);

  [[nodiscard]] Serializer *getSerializer() const;
  void setSerializer(Serializer *serializer);

  Reply get(const QUrl &url);
  Reply post(const QUrl &url);
  Reply put(const QUrl &url);
  Reply patch(const QUrl &url);
  Reply deleteResource(const QUrl &url);
  Reply head(const QUrl &url);
  Reply options(const QUrl &url);

  template <typename TYPE> Reply post(const QUrl &url, const TYPE &data);
  template <typename TYPE> Reply put(const QUrl &url, const TYPE &data);
  template <typename TYPE> Reply patch(const QUrl &url, const TYPE &data);

Q_SIGNALS:
  void baseUrlChanged(const QUrl &base_url);
  void headersChanged(egnite::web::Headers *headers);
  void serializerChanged(egnite::web::Serializer *serializer);

  void errorOccured(QNetworkReply::NetworkError code);
  void sslErrorOccured(const QList<QSslError> &errors);

protected:
  Reply post(const QUrl &url, const QByteArray &data);
  Reply put(const QUrl &url, const QByteArray &data);
  Reply patch(const QUrl &url, const QByteArray &data);

private:
  Reply createReply(QNetworkReply *reply);

private:
  QUrl m_base_url;
  Headers *m_headers;
  Serializer *m_serializer;
  QNetworkAccessManager m_network_access_manager;
};

template <typename TYPE> Reply Client::post(const QUrl &url, const TYPE &data) {
  return post(url, m_serializer->serialize(data));
}

template <typename TYPE> Reply Client::put(const QUrl &url, const TYPE &data) {
  return put(url, m_serializer->serialize(data));
}

template <typename TYPE> Reply Client::patch(const QUrl &url, const TYPE &data) {
  return patch(url, m_serializer->serialize(data));
}

} // namespace egnite::web

#endif // EGNITE_WEB_CLIENT_H
