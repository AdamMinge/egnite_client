#ifndef EGNITE_REST_REQUEST_BUILDER_H
#define EGNITE_REST_REQUEST_BUILDER_H

/* ------------------------------------ Qt ---------------------------------- */
#include <QByteArray>
#include <QNetworkRequest>
#include <QString>
#include <QUrl>
#include <QUrlQuery>
#include <QVersionNumber>
/* ---------------------------------- Standard ------------------------------ */
#include <memory>
/* ----------------------------------- Local -------------------------------- */
#include "egnite/export.h"
#include "egnite/rest/rest_global.h"
/* -------------------------------------------------------------------------- */

namespace egnite::rest {

namespace detail {
class RestRequestBuilderPrivate;
}

class EGNITE_API RestRequestBuilder {
 public:
  explicit RestRequestBuilder();
  ~RestRequestBuilder();

  RestRequestBuilder(const RestRequestBuilder& other);
  RestRequestBuilder& operator=(const RestRequestBuilder& other);

  RestRequestBuilder(RestRequestBuilder&& other);
  RestRequestBuilder& operator=(RestRequestBuilder&& other);

  RestRequestBuilder& setBaseUrl(const QUrl& base_url);
  RestRequestBuilder& setVersion(const QVersionNumber& version);

  RestRequestBuilder& addPath(const QString& path);

  RestRequestBuilder& addParameters(const QUrlQuery& parameters);
  RestRequestBuilder& addParameter(const QString& parameter_name,
                                   const QString& parameter_value);

  RestRequestBuilder& addHeaders(const RestHeaders& headers);
  RestRequestBuilder& addHeader(const QByteArray& header_name,
                                const QByteArray& header_value);

  QUrl buildUrl() const;
  QNetworkRequest build() const;

 private:
  std::unique_ptr<detail::RestRequestBuilderPrivate> m_impl;
};

}  // namespace egnite::rest

#endif  // EGNITE_REST_REQUEST_BUILDER_H