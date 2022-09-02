#ifndef EGNITE_REST_CLIENT_P_H
#define EGNITE_REST_CLIENT_P_H

/* ------------------------------------ Qt ---------------------------------- */
#include <QtCore/private/qobject_p.h>

#include <QNetworkAccessManager>
/* ------------------------------------ Local ------------------------------- */
#include "egnite/rest/rest_client.h"
/* -------------------------------------------------------------------------- */

namespace egnite::rest::detail {

class RestClientPrivate : public QObjectPrivate {
 public:
  Q_DECLARE_PUBLIC(RestClient)

 public:
  explicit RestClientPrivate(
      const QUrl& url, const QVersionNumber& version,
      const RestHeaders& headers, const QUrlQuery& parameters,
      RestDataSerializer* data_serializer,
      RestReplyDecoratorManager* reply_decorator_manager);

  void setBaseUrl(const QUrl& url);
  [[nodiscard]] QUrl getBaseUrl() const;

  void setVersion(const QVersionNumber& version);
  [[nodiscard]] QVersionNumber getVersion() const;

  void setGlobalHeaders(const RestHeaders& headers);
  [[nodiscard]] RestHeaders getGlobalHeaders() const;

  void setGlobalParameters(const QUrlQuery& parameters);
  [[nodiscard]] QUrlQuery getGlobalParameters() const;

  [[nodiscard]] RestRequestBuilder getRequestBuilder() const;
  [[nodiscard]] QNetworkAccessManager* getNetworkAccessManager() const;
  [[nodiscard]] RestDataSerializer* getDataSerializer() const;
  [[nodiscard]] RestReplyDecoratorManager* getReplyDecoratorManager() const;

 private:
  QUrl m_base_url;
  QVersionNumber m_version;
  RestHeaders m_headers;
  QUrlQuery m_parameters;
  QNetworkAccessManager* m_manager;
  RestDataSerializer* m_data_serializer;
  RestReplyDecoratorManager* m_reply_decorator_manager;
};

}  // namespace egnite::rest::detail

#endif  // EGNITE_REST_CLIENT_P_H