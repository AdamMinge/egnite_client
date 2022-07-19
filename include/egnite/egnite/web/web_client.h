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
#include "egnite/egnite/web/web_headers.h"
#include "egnite/egnite/web/web_serializer.h"
/* -------------------------------------------------------------------------- */

namespace egnite::web {

class EGNITE_API WebClient : public QObject {
  Q_OBJECT

public:
  Q_PROPERTY(QUrl baseUrl READ getBaseUrl WRITE setBaseUrl NOTIFY onBaseUrlChanged)
  Q_PROPERTY(WebHeaders headers READ getHeaders WRITE setHeaders NOTIFY onHeadersChanged)
  Q_PROPERTY(
      WebSerializer *serializer READ getSerializer WRITE setSerializer NOTIFY onSerializerChanged)

public:
  explicit WebClient(QObject *parent = nullptr);
  ~WebClient() override;

  [[nodiscard]] const QUrl &getBaseUrl() const;
  [[nodiscard]] QUrl getUrl(const QUrl &url) const;

  void setBaseUrl(const QUrl &base_url);

  [[nodiscard]] const WebHeaders &getHeaders() const;
  void setHeaders(const WebHeaders &headers);

  [[nodiscard]] WebSerializer *getSerializer() const;
  void setSerializer(WebSerializer *serializer);

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
  void onBaseUrlChanged(const QUrl &base_url);
  void onHeadersChanged(const egnite::web::WebHeaders &headers);
  void onSerializerChanged(WebSerializer *serializer);

protected:
  Q_INVOKABLE QNetworkReply *post(const QUrl &url, const QByteArray &data);
  Q_INVOKABLE QNetworkReply *put(const QUrl &url, const QByteArray &data);
  Q_INVOKABLE QNetworkReply *patch(const QUrl &url, const QByteArray &data);

private:
  QUrl m_base_url;
  WebHeaders m_headers;
  WebSerializer *m_serializer;
  QNetworkAccessManager m_network_access_manager;
};

template <typename TYPE> QNetworkReply *WebClient::post(const QUrl &url, const TYPE &data) {
  return post(url, m_serializer->serialize(data));
}

template <typename TYPE> QNetworkReply *WebClient::put(const QUrl &url, const TYPE &data) {
  return put(url, m_serializer->serialize(data));
}

template <typename TYPE> QNetworkReply *WebClient::patch(const QUrl &url, const TYPE &data) {
  return patch(url, m_serializer->serialize(data));
}

} // namespace egnite::web

#endif // EGNITE_WEB_CLIENT_H
