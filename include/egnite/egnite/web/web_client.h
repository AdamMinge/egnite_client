#ifndef EGNITE_WEB_CLIENT_H
#define EGNITE_WEB_CLIENT_H

/* ------------------------------------ Qt ---------------------------------- */
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QObject>
#include <QUrl>
/* ----------------------------------- Local -------------------------------- */
#include "egnite/egnite/export.h"
/* -------------------------------------------------------------------------- */

namespace egnite::web {

class EGNITE_API WebHeaders {
public:
  explicit WebHeaders();
  ~WebHeaders();

  [[nodiscard]] QNetworkRequest createRequest(const QUrl &url) const;

  Q_INVOKABLE void setHeader(QNetworkRequest::KnownHeaders header, const QVariant &value);
  Q_INVOKABLE void setRawHeader(const QByteArray &headerName, const QByteArray &headerValue);

  Q_INVOKABLE [[nodiscard]] QVariant header(QNetworkRequest::KnownHeaders header) const;
  Q_INVOKABLE [[nodiscard]] QByteArray rawHeader(const QByteArray &headerName) const;
  Q_INVOKABLE [[nodiscard]] QList<QByteArray> rawHeaderList() const;

  Q_INVOKABLE [[nodiscard]] bool hasRawHeader(const QByteArray &headerName) const;

  [[nodiscard]] bool operator==(const WebHeaders &other);
  [[nodiscard]] bool operator!=(const WebHeaders &other);

private:
  QNetworkRequest m_request;
};

class EGNITE_API WebClient : public QObject {
  Q_OBJECT

public:
  Q_PROPERTY(QUrl baseUrl READ getBaseUrl WRITE setBaseUrl NOTIFY onBaseUrlChanged)
  Q_PROPERTY(WebHeaders headers READ getHeaders WRITE setHeaders NOTIFY onHeadersChanged)

public:
  explicit WebClient(QObject *parent = nullptr);
  ~WebClient() override;

  [[nodiscard]] const QUrl &getBaseUrl() const;
  void setBaseUrl(const QUrl &base_url);

  [[nodiscard]] const WebHeaders &getHeaders() const;
  void setHeaders(const WebHeaders &headers);

  Q_INVOKABLE QNetworkReply *get(const QUrl &url);
  Q_INVOKABLE QNetworkReply *post(const QUrl &url);
  Q_INVOKABLE QNetworkReply *post(const QUrl &url, QIODevice *data);
  Q_INVOKABLE QNetworkReply *post(const QUrl &url, const QByteArray &data);
  Q_INVOKABLE QNetworkReply *post(const QUrl &url, QHttpMultiPart *multiPart);
  Q_INVOKABLE QNetworkReply *put(const QUrl &url);
  Q_INVOKABLE QNetworkReply *put(const QUrl &url, QIODevice *data);
  Q_INVOKABLE QNetworkReply *put(const QUrl &url, const QByteArray &data);
  Q_INVOKABLE QNetworkReply *put(const QUrl &url, QHttpMultiPart *multiPart);
  Q_INVOKABLE QNetworkReply *patch(const QUrl &url);
  Q_INVOKABLE QNetworkReply *patch(const QUrl &url, QIODevice *data);
  Q_INVOKABLE QNetworkReply *patch(const QUrl &url, const QByteArray &data);
  Q_INVOKABLE QNetworkReply *patch(const QUrl &url, QHttpMultiPart *multiPart);
  Q_INVOKABLE QNetworkReply *deleteResource(const QUrl &url);
  Q_INVOKABLE QNetworkReply *head(const QUrl &url);
  Q_INVOKABLE QNetworkReply *options(const QUrl &url);

Q_SIGNALS:
  void onBaseUrlChanged(const QUrl &base_url);
  void onHeadersChanged(const egnite::web::WebHeaders &headers);

private:
  QUrl m_base_url;
  WebHeaders m_headers;
  QNetworkAccessManager m_network_access_manager;
};

} // namespace egnite::web

#endif // EGNITE_WEB_CLIENT_H
