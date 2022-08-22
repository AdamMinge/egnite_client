/* ----------------------------------- Local -------------------------------- */
#include "egnite/rest/rest_request_builder.h"

#include "egnite/rest/detail/rest_request_builder_p.h"
/* -------------------------------------------------------------------------- */

namespace egnite::rest {

/* --------------------------- RestRequestBuilder --------------------------- */

RestRequestBuilder::RestRequestBuilder()
    : m_impl(std::make_unique<detail::RestRequestBuilderPrivate>()) {}

RestRequestBuilder::~RestRequestBuilder() = default;

RestRequestBuilder::RestRequestBuilder(const RestRequestBuilder& other) {
  *m_impl = *other.m_impl;
}

RestRequestBuilder& RestRequestBuilder::operator=(
    const RestRequestBuilder& other) {
  *m_impl = *other.m_impl;
  return *this;
}

RestRequestBuilder::RestRequestBuilder(RestRequestBuilder&& other) {
  m_impl.reset(other.m_impl.release());
}

RestRequestBuilder& RestRequestBuilder::operator=(RestRequestBuilder&& other) {
  m_impl.reset(other.m_impl.release());
  return *this;
}

RestRequestBuilder& RestRequestBuilder::setBaseUrl(const QUrl& base_url) {
  m_impl->setBaseUrl(base_url);
  return *this;
}

RestRequestBuilder& RestRequestBuilder::setVersion(
    const QVersionNumber& version) {
  m_impl->setVersion(version);
  return *this;
}

RestRequestBuilder& RestRequestBuilder::addPath(const QString& path) {
  m_impl->addPath(path);
  return *this;
}

RestRequestBuilder& RestRequestBuilder::addParameters(
    const QUrlQuery& parameters) {
  m_impl->addParameters(parameters);
  return *this;
}

RestRequestBuilder& RestRequestBuilder::addParameter(
    const QString& parameter_name, const QString& parameter_value) {
  auto url_query = QUrlQuery{std::make_pair(parameter_name, parameter_value)};
  addParameters(url_query);
  return *this;
}

RestRequestBuilder& RestRequestBuilder::addHeaders(const RestHeaders& headers) {
  m_impl->addHeaders(headers);
  return *this;
}

RestRequestBuilder& RestRequestBuilder::addHeader(
    const QByteArray& header_name, const QByteArray& header_value) {
  auto headers = RestHeaders{std::make_pair(header_name, header_value)};
  addHeaders(headers);
  return *this;
}

QUrl RestRequestBuilder::buildUrl() const { return m_impl->buildUrl(); }

QNetworkRequest RestRequestBuilder::build() const { return m_impl->build(); }

/* ----------------------- RestRequestBuilderPrivate ------------------------ */

namespace detail {

RestRequestBuilderPrivate::RestRequestBuilderPrivate() = default;

void RestRequestBuilderPrivate::setBaseUrl(const QUrl& base_url) {
  m_base_url = base_url;
}

void RestRequestBuilderPrivate::setVersion(const QVersionNumber& version) {
  m_version = version;
}

void RestRequestBuilderPrivate::addPath(const QString& path) {
  m_paths.append(path);
}

void RestRequestBuilderPrivate::addParameters(const QUrlQuery& parameters) {
  m_parameters = parameters;
}

void RestRequestBuilderPrivate::addHeaders(const RestHeaders& headers) {
  m_headers = headers;
}

QUrl RestRequestBuilderPrivate::buildUrl() const {
  auto url = m_base_url;
  auto paths = url.path().split(QLatin1Char('/'), Qt::SkipEmptyParts);

  if (!m_version.isNull())
    paths.append(QLatin1Char('v') + m_version.normalized().toString());
  paths.append(m_paths);
  url.setPath(QLatin1Char('/') + paths.join(QLatin1Char('/')));

  if (!m_parameters.isEmpty()) url.setQuery(m_parameters);

  return url;
}

QNetworkRequest RestRequestBuilderPrivate::build() const {
  auto request = QNetworkRequest(buildUrl());

  for (auto iter = m_headers.begin(); iter != m_headers.end(); ++iter)
    request.setRawHeader(iter.key(), iter.value());

  return request;
}

}  // namespace detail

}  // namespace egnite::rest