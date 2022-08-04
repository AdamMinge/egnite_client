#ifndef EGNITE_REST_HEADERS_H
#define EGNITE_REST_HEADERS_H

/* ------------------------------------ Qt ---------------------------------- */
#include <QNetworkRequest>
#include <QQmlPropertyMap>
/* ----------------------------------- Local -------------------------------- */
#include "egnite/export.h"
/* -------------------------------------------------------------------------- */

#define EGNITE_KNOWN_HEADER(propertyName, knowHeader)            \
 public:                                                         \
  Q_PROPERTY(QVariant propertyName READ get##propertyName WRITE  \
                 set##propertyName NOTIFY propertyName##Changed) \
  [[nodiscard]] QVariant get##propertyName() const {             \
    return getHeader(knowHeader);                                \
  }                                                              \
  void set##propertyName(const QVariant& value) {                \
    if (getHeader(knowHeader) == value) return;                  \
                                                                 \
    setHeader(knowHeader, value);                                \
    Q_EMIT propertyName##Changed(value);                         \
  }                                                              \
  Q_SIGNAL void propertyName##Changed(const QVariant& value);

namespace egnite::rest {

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

  EGNITE_KNOWN_HEADER(contentType, KnownHeaders::ContentType)
  EGNITE_KNOWN_HEADER(contentLength, KnownHeaders::ContentLength)
  EGNITE_KNOWN_HEADER(location, KnownHeaders::Location)
  EGNITE_KNOWN_HEADER(lastModified, KnownHeaders::LastModified)
  EGNITE_KNOWN_HEADER(cookie, KnownHeaders::Cookie)
  EGNITE_KNOWN_HEADER(setCookie, KnownHeaders::SetCookie)
  EGNITE_KNOWN_HEADER(contentDisposition, KnownHeaders::ContentDisposition)
  EGNITE_KNOWN_HEADER(userAgent, KnownHeaders::UserAgent)
  EGNITE_KNOWN_HEADER(server, KnownHeaders::Server)
  EGNITE_KNOWN_HEADER(ifModifiedSince, KnownHeaders::IfModifiedSince)
  EGNITE_KNOWN_HEADER(eTag, KnownHeaders::ETag)
  EGNITE_KNOWN_HEADER(ifMatch, KnownHeaders::IfMatch)
  EGNITE_KNOWN_HEADER(ifNoneMatch, KnownHeaders::IfNoneMatch)
  EGNITE_KNOWN_HEADER(apiKey, KnownHeaders::ApiKey)
  EGNITE_KNOWN_HEADER(authorization, KnownHeaders::Authorization)
  EGNITE_KNOWN_HEADER(accept, KnownHeaders::Accept)

 public:
  explicit Headers();
  ~Headers() override;

  QNetworkRequest createRequest(const QUrl& url) const;

  void setHeader(KnownHeaders header, const QVariant& value);
  [[nodiscard]] QVariant getHeader(KnownHeaders header) const;

  [[nodiscard]] QSet<KnownHeaders> getHeaderSet() const;
  [[nodiscard]] bool hasHeader(KnownHeaders header) const;

 protected:
  static QByteArray headerToName(KnownHeaders header);
  static KnownHeaders nameToHeader(const QByteArray& header_name);
  static QByteArray headerValue(KnownHeaders header, const QVariant& value);
};

}  // namespace egnite::rest

#endif  // EGNITE_REST_HEADERS_H
