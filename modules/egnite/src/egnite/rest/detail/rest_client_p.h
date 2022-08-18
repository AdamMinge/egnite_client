#ifndef EGNITE_REST_CLIENT_P_H
#define EGNITE_REST_CLIENT_P_H

/* ------------------------------------ Qt ---------------------------------- */
#include <QtCore/private/qobject_p.h>
/* ------------------------------------ Local ------------------------------- */
#include "egnite/rest/rest_client.h"
/* -------------------------------------------------------------------------- */

namespace egnite::rest::detail {

class RestClientPrivate : public QObjectPrivate {
 public:
  Q_DECLARE_PUBLIC(RestClient)

 public:
  RestClientPrivate(const QUrl& url, const QVersionNumber& version,
                    const RestClient::Headers& headers,
                    const QUrlQuery& parameters);

  void setBaseUrl(const QUrl& url);
  [[nodiscard]] QUrl getBaseUrl() const;

  void setVersion(const QVersionNumber& version);
  [[nodiscard]] QVersionNumber getVersion() const;

  void setGlobalHeaders(const RestClient::Headers& headers);
  [[nodiscard]] RestClient::Headers getGlobalHeaders() const;

  void setGlobalParameters(const QUrlQuery& parameters);
  [[nodiscard]] QUrlQuery getGlobalParameters() const;

 private:
  QUrl m_base_url;
  QVersionNumber m_version;
  RestClient::Headers m_headers;
  QUrlQuery m_parameters;
};

}  // namespace egnite::rest::detail

#endif  // EGNITE_REST_CLIENT_P_H