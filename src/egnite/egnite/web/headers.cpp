/* ----------------------------------- Local -------------------------------- */
#include "egnite/egnite/web/headers.h"
/* -------------------------------------------------------------------------- */

namespace egnite::web {

Headers::Headers() = default;

Headers::~Headers() = default;

QNetworkRequest Headers::createRequest(const QUrl &url) const {
  auto request = m_request;
  request.setUrl(url);
  return request;
}

void Headers::setHeader(QNetworkRequest::KnownHeaders header, const QVariant &value) {
  m_request.setHeader(header, value);
}

void Headers::setRawHeader(const QByteArray &headerName, const QByteArray &headerValue) {
  m_request.setRawHeader(headerName, headerValue);
}

QVariant Headers::header(QNetworkRequest::KnownHeaders header) const {
  return m_request.header(header);
}

QByteArray Headers::rawHeader(const QByteArray &headerName) const {
  return m_request.rawHeader(headerName);
}

QList<QByteArray> Headers::rawHeaderList() const { return m_request.rawHeaderList(); }

bool Headers::hasRawHeader(const QByteArray &headerName) const {
  return m_request.hasRawHeader(headerName);
}

bool Headers::operator==(const Headers &other) { return m_request == other.m_request; }

bool Headers::operator!=(const Headers &other) { return m_request != other.m_request; }

} // namespace egnite::web
