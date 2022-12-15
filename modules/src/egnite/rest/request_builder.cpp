/* ----------------------------------- Local -------------------------------- */
#include "egnite/rest/request_builder.h"

#include "egnite/rest/detail/request_builder_p.h"
/* -------------------------------------------------------------------------- */

namespace egnite::rest {

/* ----------------------------- RequestBuilder ----------------------------- */

RequestBuilder::RequestBuilder()
    : m_impl(std::make_unique<detail::RequestBuilderPrivate>()) {}

RequestBuilder::~RequestBuilder() = default;

RequestBuilder::RequestBuilder(const RequestBuilder& other)
    : m_impl(std::make_unique<detail::RequestBuilderPrivate>()) {
  *m_impl = *other.m_impl;
}

RequestBuilder& RequestBuilder::operator=(const RequestBuilder& other) {
  *m_impl = *other.m_impl;
  return *this;
}

RequestBuilder::RequestBuilder(RequestBuilder&& other) {
  m_impl.reset(other.m_impl.release());
}

RequestBuilder& RequestBuilder::operator=(RequestBuilder&& other) {
  m_impl.reset(other.m_impl.release());
  return *this;
}

RequestBuilder& RequestBuilder::setBaseUrl(const QUrl& base_url) {
  m_impl->setBaseUrl(base_url);
  return *this;
}

RequestBuilder& RequestBuilder::setVersion(const QVersionNumber& version) {
  m_impl->setVersion(version);
  return *this;
}

RequestBuilder& RequestBuilder::addPath(const QString& path) {
  m_impl->addPath(path);
  return *this;
}

RequestBuilder& RequestBuilder::addParameters(const QUrlQuery& parameters) {
  m_impl->addParameters(parameters);
  return *this;
}

RequestBuilder& RequestBuilder::addParameter(const QString& parameter_name,
                                             const QString& parameter_value) {
  auto url_query = QUrlQuery{std::make_pair(parameter_name, parameter_value)};
  addParameters(url_query);
  return *this;
}

RequestBuilder& RequestBuilder::addHeaders(const Headers& headers) {
  m_impl->addHeaders(headers);
  return *this;
}

RequestBuilder& RequestBuilder::addHeader(const QByteArray& header_name,
                                          const QByteArray& header_value) {
  auto headers = Headers{std::make_pair(header_name, header_value)};
  addHeaders(headers);
  return *this;
}

QUrl RequestBuilder::buildUrl() const { return m_impl->buildUrl(); }

QNetworkRequest RequestBuilder::build() const { return m_impl->build(); }

/* ----------------------- RequestBuilderPrivate ------------------------ */

namespace detail {

RequestBuilderPrivate::RequestBuilderPrivate() = default;

void RequestBuilderPrivate::setBaseUrl(const QUrl& base_url) {
  m_base_url = base_url;
}

void RequestBuilderPrivate::setVersion(const QVersionNumber& version) {
  m_version = version;
}

void RequestBuilderPrivate::addPath(const QString& path) {
  if (!path.isEmpty()) m_paths.append(path);
}

void RequestBuilderPrivate::addParameters(const QUrlQuery& parameters) {
  m_parameters.setQueryItems(parameters.queryItems());
}

void RequestBuilderPrivate::addHeaders(const Headers& headers) {
  m_headers.insert(headers);
}

QUrl RequestBuilderPrivate::buildUrl() const {
  auto url = m_base_url;
  auto paths = url.path().split(QLatin1Char('/'), Qt::SkipEmptyParts);

  if (!m_version.isNull())
    paths.append(QLatin1Char('v') + m_version.normalized().toString());
  paths.append(m_paths);
  url.setPath(QLatin1Char('/') + paths.join(QLatin1Char('/')));

  if (!m_parameters.isEmpty()) url.setQuery(m_parameters);

  return url;
}

QNetworkRequest RequestBuilderPrivate::build() const {
  auto request = QNetworkRequest(buildUrl());

  for (auto iter = m_headers.begin(); iter != m_headers.end(); ++iter)
    request.setRawHeader(iter.key(), iter.value());

  return request;
}

}  // namespace detail

}  // namespace egnite::rest