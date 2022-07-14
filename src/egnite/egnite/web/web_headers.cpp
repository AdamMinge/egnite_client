/* ----------------------------------- Local -------------------------------- */
#include "egnite/egnite/web/web_headers.h"
/* -------------------------------------------------------------------------- */

namespace egnite::web {

WebHeaders::WebHeaders() = default;

WebHeaders::~WebHeaders() = default;

QNetworkRequest WebHeaders::createRequest(const QUrl &url) const {
  auto request = m_request;
  request.setUrl(url);
  return request;
}

void WebHeaders::setHeader(QNetworkRequest::KnownHeaders header, const QVariant &value) {
  m_request.setHeader(header, value);
}

void WebHeaders::setRawHeader(const QByteArray &headerName, const QByteArray &headerValue) {
  m_request.setRawHeader(headerName, headerValue);
}

QVariant WebHeaders::header(QNetworkRequest::KnownHeaders header) const {
  return m_request.header(header);
}

QByteArray WebHeaders::rawHeader(const QByteArray &headerName) const {
  return m_request.rawHeader(headerName);
}

QList<QByteArray> WebHeaders::rawHeaderList() const { return m_request.rawHeaderList(); }

bool WebHeaders::hasRawHeader(const QByteArray &headerName) const {
  return m_request.hasRawHeader(headerName);
}

bool WebHeaders::operator==(const WebHeaders &other) { return m_request == other.m_request; }

bool WebHeaders::operator!=(const WebHeaders &other) { return m_request != other.m_request; }

} // namespace egnite::web
