/* ----------------------------------- Local -------------------------------- */
#include "egnite/egnite/web/headers.h"
/* ------------------------------------ Qt ---------------------------------- */
#include <QDateTime>
#include <QLocale>
#include <QNetworkCookie>
/* -------------------------------------------------------------------------- */
#include <QDebug>
namespace egnite::web {

Headers::Headers() : QQmlPropertyMap(this, nullptr) {}

Headers::~Headers() = default;

QNetworkRequest Headers::createRequest(const QUrl& url) const {
  QNetworkRequest request(url);

  for (auto& headerName : keys()) {
    auto header = nameToHeader(headerName.toLocal8Bit());
    request.setRawHeader(headerName.toLocal8Bit(), headerValue(header, getHeader(header)));
  }

  return request;
}

void Headers::setHeader(KnownHeaders header, const QVariant& value) {
  insert(headerToName(header), value);
}

QVariant Headers::getHeader(KnownHeaders header) const { return value(headerToName(header)); }

QList<Headers::KnownHeaders> Headers::getHeaderList() const {
  QList<Headers::KnownHeaders> headerList;
  for (auto& headerName : keys())
    headerList.append(nameToHeader(headerName.toLocal8Bit()));
  return headerList;
}

bool Headers::hasHeader(KnownHeaders header) const { return contains(headerToName(header)); }

QByteArray Headers::headerToName(KnownHeaders header) {
  switch (header) {
  case KnownHeaders::ContentType:
    return "Content-Type";

  case KnownHeaders::ContentLength:
    return "Content-Length";

  case KnownHeaders::Location:
    return "Location";

  case KnownHeaders::LastModified:
    return "Last-Modified";

  case KnownHeaders::IfModifiedSince:
    return "If-Modified-Since";

  case KnownHeaders::ETag:
    return "ETag";

  case KnownHeaders::IfMatch:
    return "If-Match";

  case KnownHeaders::IfNoneMatch:
    return "If-None-Match";

  case KnownHeaders::Cookie:
    return "Cookie";

  case KnownHeaders::SetCookie:
    return "Set-Cookie";

  case KnownHeaders::ContentDisposition:
    return "Content-Disposition";

  case KnownHeaders::UserAgent:
    return "User-Agent";

  case KnownHeaders::Server:
    return "Server";

  case KnownHeaders::ApiKey:
    return "Api-Key";

  case KnownHeaders::Authorization:
    return "Authorization";

  case KnownHeaders::Accept:
    return "accept";

  case KnownHeaders::Unknown:
  default:
    return QByteArray{};
  }
}

Headers::KnownHeaders Headers::nameToHeader(const QByteArray& headerName) {
  if (headerName.isEmpty())
    return KnownHeaders::Unknown;

  switch (tolower(headerName.at(0))) {
  case 'a':
    if (headerName.compare("api-key", Qt::CaseInsensitive) == 0)
      return KnownHeaders::ApiKey;
    if (headerName.compare("authorization", Qt::CaseInsensitive) == 0)
      return KnownHeaders::Authorization;
    if (headerName.compare("accept", Qt::CaseInsensitive) == 0)
      return KnownHeaders::Accept;
    break;

  case 'c':
    if (headerName.compare("content-type", Qt::CaseInsensitive) == 0)
      return KnownHeaders::ContentType;
    else if (headerName.compare("content-length", Qt::CaseInsensitive) == 0)
      return KnownHeaders::ContentLength;
    else if (headerName.compare("cookie", Qt::CaseInsensitive) == 0)
      return KnownHeaders::Cookie;
    else if (qstricmp(headerName.constData(), "content-disposition") == 0)
      return KnownHeaders::ContentDisposition;
    break;

  case 'e':
    if (qstricmp(headerName.constData(), "etag") == 0)
      return KnownHeaders::ETag;
    break;

  case 'i':
    if (qstricmp(headerName.constData(), "if-modified-since") == 0)
      return KnownHeaders::IfModifiedSince;
    if (qstricmp(headerName.constData(), "if-match") == 0)
      return KnownHeaders::IfMatch;
    if (qstricmp(headerName.constData(), "if-none-match") == 0)
      return KnownHeaders::IfNoneMatch;
    break;

  case 'l':
    if (headerName.compare("location", Qt::CaseInsensitive) == 0)
      return KnownHeaders::Location;
    else if (headerName.compare("last-modified", Qt::CaseInsensitive) == 0)
      return KnownHeaders::LastModified;
    break;

  case 's':
    if (headerName.compare("set-cookie", Qt::CaseInsensitive) == 0)
      return KnownHeaders::SetCookie;
    else if (headerName.compare("server", Qt::CaseInsensitive) == 0)
      return KnownHeaders::Server;
    break;

  case 'u':
    if (headerName.compare("user-agent", Qt::CaseInsensitive) == 0)
      return KnownHeaders::UserAgent;
    break;
  }

  return KnownHeaders::Unknown;
}

QByteArray Headers::headerValue(KnownHeaders header, const QVariant& value) {
  switch (header) {
  case KnownHeaders::ContentType:
  case KnownHeaders::ContentLength:
  case KnownHeaders::ContentDisposition:
  case KnownHeaders::UserAgent:
  case KnownHeaders::Server:
  case KnownHeaders::ETag:
  case KnownHeaders::IfMatch:
  case KnownHeaders::IfNoneMatch:
  case KnownHeaders::ApiKey:
  case KnownHeaders::Authorization:
  case KnownHeaders::Accept:
    return value.toByteArray();

  case KnownHeaders::Location:
    switch (value.userType()) {
    case QMetaType::QUrl:
      return value.toUrl().toEncoded();

    default:
      return value.toByteArray();
    }

  case KnownHeaders::LastModified:
  case KnownHeaders::IfModifiedSince:
    switch (value.userType()) {
    case QMetaType::QDate:
    case QMetaType::QDateTime:
      return QLocale::c()
          .toString(value.toDateTime(), u"ddd, dd MMM yyyy hh:mm:ss 'GMT'")
          .toLatin1();

    default:
      return value.toByteArray();
    }

  case KnownHeaders::Cookie: {
    QList<QNetworkCookie> cookies = qvariant_cast<QList<QNetworkCookie> >(value);
    if (cookies.isEmpty() && value.userType() == qMetaTypeId<QNetworkCookie>())
      cookies << qvariant_cast<QNetworkCookie>(value);

    QByteArray result;
    bool first = true;
    for (const QNetworkCookie& cookie : qAsConst(cookies)) {
      if (!first)
        result += "; ";
      first = false;
      result += cookie.toRawForm(QNetworkCookie::NameAndValueOnly);
    }
    return result;
  }

  case KnownHeaders::SetCookie: {
    QList<QNetworkCookie> cookies = qvariant_cast<QList<QNetworkCookie> >(value);
    if (cookies.isEmpty() && value.userType() == qMetaTypeId<QNetworkCookie>())
      cookies << qvariant_cast<QNetworkCookie>(value);

    QByteArray result;
    bool first = true;
    for (const QNetworkCookie& cookie : qAsConst(cookies)) {
      if (!first)
        result += ", ";
      first = false;
      result += cookie.toRawForm(QNetworkCookie::Full);
    }
    return result;
  }

  default:
    return QByteArray{};
  }
}

} // namespace egnite::web
