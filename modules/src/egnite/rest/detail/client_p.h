#ifndef EGNITE_REST_CLIENT_P_H
#define EGNITE_REST_CLIENT_P_H

/* ------------------------------------ Qt ---------------------------------- */
#include <QtCore/private/qobject_p.h>

#include <QNetworkAccessManager>
#include <QScopedPointer>
/* ------------------------------------ Local ------------------------------- */
#include "egnite/rest/client.h"
/* -------------------------------------------------------------------------- */

namespace egnite::rest::detail {

class ClientPrivate : public QObjectPrivate {
 public:
  Q_DECLARE_PUBLIC(Client)

 public:
  explicit ClientPrivate(const QUrl& url, const QVersionNumber& version,
                         const Headers& headers, const QUrlQuery& parameters);

  void setBaseUrl(const QUrl& url);
  [[nodiscard]] QUrl getBaseUrl() const;

  void setVersion(const QVersionNumber& version);
  [[nodiscard]] QVersionNumber getVersion() const;

  void setGlobalHeaders(const Headers& headers);
  [[nodiscard]] Headers getGlobalHeaders() const;

  void setGlobalParameters(const QUrlQuery& parameters);
  [[nodiscard]] QUrlQuery getGlobalParameters() const;

  [[nodiscard]] RequestBuilder getRequestBuilder() const;
  [[nodiscard]] QNetworkAccessManager* getNetworkAccessManager() const;
  [[nodiscard]] DataSerializer* getDataSerializer() const;
  [[nodiscard]] ReplyDecorator* getReplyDecorator() const;

 private:
  QUrl m_base_url;
  QVersionNumber m_version;
  Headers m_headers;
  QUrlQuery m_parameters;
  QScopedPointer<QNetworkAccessManager> m_manager;
  QScopedPointer<DataSerializer> m_data_serializer;
  QScopedPointer<ReplyDecorator> m_reply_decorator;
};

}  // namespace egnite::rest::detail

#endif  // EGNITE_REST_CLIENT_P_H