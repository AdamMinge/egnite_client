#ifndef EGNITE_WEB_HEADERS_H
#define EGNITE_WEB_HEADERS_H

/* ------------------------------------ Qt ---------------------------------- */
#include <QNetworkRequest>
#include <QQmlPropertyMap>
/* ----------------------------------- Local -------------------------------- */
#include "egnite/egnite/export.h"
/* -------------------------------------------------------------------------- */

#define EGNITE_KNOW_HEADER(propertyName, knowHeader)                                               \
public:                                                                                            \
  Q_PROPERTY(QVariant propertyName READ get##propertyName WRITE set##propertyName NOTIFY           \
                 propertyName##Changed)                                                            \
  [[nodiscard]] QVariant get##propertyName() const { return getHeader(knowHeader); }               \
  void set##propertyName(const QVariant& value) {                                                  \
    if (getHeader(knowHeader) == value)                                                            \
      return;                                                                                      \
                                                                                                   \
    setHeader(knowHeader, value);                                                                  \
    Q_EMIT propertyName##Changed(value);                                                           \
  }                                                                                                \
  Q_SIGNAL void propertyName##Changed(const QVariant& value);

namespace egnite::web {

class EGNITE_API Headers : public QQmlPropertyMap {
  Q_OBJECT

public:
  enum class KnownHeaders {
    ContentType,
    ContentLength,
    Location,
    LastModified,
    Cookie,
    SetCookie,
    ContentDisposition,
    UserAgent,
    Server,
    IfModifiedSince,
    ETag,
    IfMatch,
    IfNoneMatch,
    ApiKey,
    Authorization,
    Accept,
    Unknown,
  };

  EGNITE_KNOW_HEADER(contentType, KnownHeaders::ContentType)
  EGNITE_KNOW_HEADER(contentLength, KnownHeaders::ContentLength)
  EGNITE_KNOW_HEADER(location, KnownHeaders::Location)
  EGNITE_KNOW_HEADER(lastModified, KnownHeaders::LastModified)
  EGNITE_KNOW_HEADER(cookie, KnownHeaders::Cookie)
  EGNITE_KNOW_HEADER(setCookie, KnownHeaders::SetCookie)
  EGNITE_KNOW_HEADER(contentDisposition, KnownHeaders::ContentDisposition)
  EGNITE_KNOW_HEADER(userAgent, KnownHeaders::UserAgent)
  EGNITE_KNOW_HEADER(server, KnownHeaders::Server)
  EGNITE_KNOW_HEADER(ifModifiedSince, KnownHeaders::IfModifiedSince)
  EGNITE_KNOW_HEADER(eTag, KnownHeaders::ETag)
  EGNITE_KNOW_HEADER(ifMatch, KnownHeaders::IfMatch)
  EGNITE_KNOW_HEADER(ifNoneMatch, KnownHeaders::IfNoneMatch)
  EGNITE_KNOW_HEADER(apiKey, KnownHeaders::ApiKey)
  EGNITE_KNOW_HEADER(authorization, KnownHeaders::Authorization)
  EGNITE_KNOW_HEADER(accept, KnownHeaders::Accept)

public:
  explicit Headers();
  ~Headers() override;

  QNetworkRequest createRequest(const QUrl& url) const;

  void setHeader(KnownHeaders header, const QVariant& value);
  [[nodiscard]] QVariant getHeader(KnownHeaders header) const;

  [[nodiscard]] QList<KnownHeaders> getHeaderList() const;
  [[nodiscard]] bool hasHeader(KnownHeaders header) const;

protected:
  static QByteArray headerToName(KnownHeaders header);
  static KnownHeaders nameToHeader(const QByteArray& headerName);
  static QByteArray headerValue(KnownHeaders header, const QVariant& value);
};

} // namespace egnite::web

#endif // EGNITE_WEB_HEADERS_H
