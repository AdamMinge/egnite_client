#ifndef EGNITE_REST_REQUEST_BUILDER_P_H
#define EGNITE_REST_REQUEST_BUILDER_P_H

/* ------------------------------------ Local ------------------------------- */
#include "egnite/rest/request_builder.h"
/* -------------------------------------------------------------------------- */

namespace egnite::rest::detail {

/* ---------------------------- RequestBuilderPrivate ----------------------- */

class RequestBuilderPrivate {
 public:
  explicit RequestBuilderPrivate();

  void setBaseUrl(const QUrl& base_url);
  void setVersion(const QVersionNumber& version);

  void addPath(const QString& path);
  void updateFromRelativeUrl(const QUrl& url, bool merge_query);

  void addParameters(const QUrlQuery& parameters);
  void addHeaders(const Headers& headers);

  QUrl buildUrl() const;
  QNetworkRequest build() const;

 private:
  QUrl m_base_url;
  QVersionNumber m_version;

  QStringList m_paths;
  QUrlQuery m_parameters;
  Headers m_headers;
};

}  // namespace egnite::rest::detail

#endif  // EGNITE_REST_REQUEST_BUILDER_P_H