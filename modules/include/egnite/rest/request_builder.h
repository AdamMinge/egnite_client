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
#include "egnite/rest/export.h"
#include "egnite/rest/global.h"
/* -------------------------------------------------------------------------- */

namespace egnite::rest {

namespace detail {
class RequestBuilderPrivate;
}

class EGNITE_REST_API RequestBuilder {
 public:
  explicit RequestBuilder();
  ~RequestBuilder();

  RequestBuilder(const RequestBuilder& other);
  RequestBuilder& operator=(const RequestBuilder& other);

  RequestBuilder(RequestBuilder&& other);
  RequestBuilder& operator=(RequestBuilder&& other);

  RequestBuilder& setBaseUrl(const QUrl& base_url);
  RequestBuilder& setVersion(const QVersionNumber& version);

  RequestBuilder& addPath(const QString& path);

  RequestBuilder& addParameters(const QUrlQuery& parameters);
  RequestBuilder& addParameter(const QString& parameter_name,
                               const QString& parameter_value);

  RequestBuilder& addHeaders(const Headers& headers);
  RequestBuilder& addHeader(const QByteArray& header_name,
                            const QByteArray& header_value);

  QUrl buildUrl() const;
  QNetworkRequest build() const;

 private:
  std::unique_ptr<detail::RequestBuilderPrivate> m_impl;
};

}  // namespace egnite::rest

#endif  // EGNITE_REST_REQUEST_BUILDER_H