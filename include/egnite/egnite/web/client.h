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
#include "egnite/egnite/web/serializer.h"
/* -------------------------------------------------------------------------- */

namespace egnite::web {

class EGNITE_API Client : public QObject {
  Q_OBJECT

public:
  Q_PROPERTY(QUrl baseUrl READ getBaseUrl WRITE setBaseUrl NOTIFY baseUrlChanged)
  Q_PROPERTY(Headers headers READ getHeaders WRITE setHeaders NOTIFY headersChanged)
  Q_PROPERTY(Serializer *serializer READ getSerializer WRITE setSerializer NOTIFY serializerChanged)

public:
  explicit Client(QObject *parent = nullptr);
  ~Client() override;

  [[nodiscard]] const QUrl &getBaseUrl() const;
  [[nodiscard]] QUrl getUrl(const QUrl &url) const;

  void setBaseUrl(const QUrl &base_url);

  [[nodiscard]] const Headers &getHeaders() const;
  void setHeaders(const Headers &headers);

  [[nodiscard]] Serializer *getSerializer() const;
  void setSerializer(Serializer *serializer);

  Q_INVOKABLE QNetworkReply *get(const QUrl &url);
  Q_INVOKABLE QNetworkReply *post(const QUrl &url);
  Q_INVOKABLE QNetworkReply *put(const QUrl &url);
  Q_INVOKABLE QNetworkReply *patch(const QUrl &url);
  Q_INVOKABLE QNetworkReply *deleteResource(const QUrl &url);
  Q_INVOKABLE QNetworkReply *head(const QUrl &url);
  Q_INVOKABLE QNetworkReply *options(const QUrl &url);

  template <typename TYPE> QNetworkReply *post(const QUrl &url, const TYPE &data);
  template <typename TYPE> QNetworkReply *put(const QUrl &url, const TYPE &data);
  template <typename TYPE> QNetworkReply *patch(const QUrl &url, const TYPE &data);

Q_SIGNALS:
  void baseUrlChanged(const QUrl &base_url);
  void headersChanged(const egnite::web::Headers &headers);
  void serializerChanged(egnite::web::Serializer *serializer);

  void errorOccured(QNetworkReply::NetworkError code);
  void sslErrorOccured(const QList<QSslError> &errors);

protected:
  Q_INVOKABLE QNetworkReply *post(const QUrl &url, const QByteArray &data);
  Q_INVOKABLE QNetworkReply *put(const QUrl &url, const QByteArray &data);
  Q_INVOKABLE QNetworkReply *patch(const QUrl &url, const QByteArray &data);

private:
  void connectErrorListeners(QNetworkReply *reply);

private:
  QUrl m_base_url;
  Headers m_headers;
  Serializer *m_serializer;
  QNetworkAccessManager m_network_access_manager;
};

template <typename TYPE> QNetworkReply *Client::post(const QUrl &url, const TYPE &data) {
  return post(url, m_serializer->serialize(data));
}

template <typename TYPE> QNetworkReply *Client::put(const QUrl &url, const TYPE &data) {
  return put(url, m_serializer->serialize(data));
}

template <typename TYPE> QNetworkReply *Client::patch(const QUrl &url, const TYPE &data) {
  return patch(url, m_serializer->serialize(data));
}

} // namespace egnite::web

#endif // EGNITE_WEB_CLIENT_H
