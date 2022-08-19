#ifndef EGNITE_REST_REQUEST_BUILDER_P_H
#define EGNITE_REST_REQUEST_BUILDER_P_H

/* ------------------------------------ Local ------------------------------- */
#include "egnite/rest/rest_request_builder.h"
/* -------------------------------------------------------------------------- */

namespace egnite::rest::detail {

class RestRequestBuilderPrivate {
 public:
  RestRequestBuilderPrivate();

  void setBaseUrl(const QUrl& base_url);
  void setVersion(const QVersionNumber& version);

  void addPath(const QString& path);
  void addParameters(const QUrlQuery& parameters);
  void addHeaders(const RestHeaders& headers);

  QUrl buildUrl() const;
  QNetworkRequest build() const;

 private:
  QUrl m_base_url;
  QVersionNumber m_version;

  QStringList m_paths;
  QUrlQuery m_parameters;
  RestHeaders m_headers;
};

}  // namespace egnite::rest::detail

#endif  // EGNITE_REST_REQUEST_BUILDER_P_H