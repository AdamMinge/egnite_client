#ifndef EGNITE_REST_CLIENT_H
#define EGNITE_REST_CLIENT_H

/* ------------------------------------ Qt ---------------------------------- */
#include <QJsonObject>
#include <QNetworkAccessManager>
#include <QObject>
#include <QUrl>
/* ----------------------------------- Local -------------------------------- */
#include "egnite/export.h"
#include "egnite/rest/headers.h"
#include "egnite/rest/reply.h"
#include "egnite/rest/serializer.h"
/* -------------------------------------------------------------------------- */

namespace egnite::rest {

class EGNITE_API Client : public QObject {
  Q_OBJECT

 public:
  Q_PROPERTY(
      QUrl baseUrl READ getBaseUrl WRITE setBaseUrl NOTIFY baseUrlChanged)
  Q_PROPERTY(
      Headers *headers READ getHeaders WRITE setHeaders NOTIFY headersChanged)
  Q_PROPERTY(Serializer *serializer READ getSerializer WRITE setSerializer
                 NOTIFY serializerChanged)

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

  template <typename TYPE>
  Reply post(const QUrl &url, const TYPE &data);
  template <typename TYPE>
  Reply put(const QUrl &url, const TYPE &data);
  template <typename TYPE>
  Reply patch(const QUrl &url, const TYPE &data);

 Q_SIGNALS:
  void baseUrlChanged(const QUrl &base_url);
  void headersChanged(egnite::rest::Headers *headers);
  void serializerChanged(egnite::rest::Serializer *serializer);

  void errorOccurred(QNetworkReply::NetworkError error);
  void sslErrorOccurred(const QList<QSslError> &errors);

 protected:
  Reply post(const QUrl &url, const QByteArray &data);
  Reply put(const QUrl &url, const QByteArray &data);
  Reply patch(const QUrl &url, const QByteArray &data);

 private:
  QUrl m_base_url;
  Headers *m_headers;
  Serializer *m_serializer;
  QNetworkAccessManager m_network_access_manager;
};

template <typename TYPE>
Reply Client::post(const QUrl &url, const TYPE &data) {
  Q_ASSERT(m_serializer);
  return post(url, m_serializer->serialize(data));
}

template <typename TYPE>
Reply Client::put(const QUrl &url, const TYPE &data) {
  Q_ASSERT(m_serializer);
  return put(url, m_serializer->serialize(data));
}

template <typename TYPE>
Reply Client::patch(const QUrl &url, const TYPE &data) {
  Q_ASSERT(m_serializer);
  return patch(url, m_serializer->serialize(data));
}

}  // namespace egnite::rest

#endif  // EGNITE_REST_CLIENT_H
