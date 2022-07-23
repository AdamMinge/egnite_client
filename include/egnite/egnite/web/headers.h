#ifndef EGNITE_WEB_HEADERS_H
#define EGNITE_WEB_HEADERS_H

/* ------------------------------------ Qt ---------------------------------- */
#include <QNetworkRequest>
/* ----------------------------------- Local -------------------------------- */
#include "egnite/egnite/export.h"
/* -------------------------------------------------------------------------- */

namespace egnite::web {

class EGNITE_API Headers {
public:
  explicit Headers();
  ~Headers();

  [[nodiscard]] QNetworkRequest createRequest(const QUrl &url) const;

  Q_INVOKABLE void setHeader(QNetworkRequest::KnownHeaders header, const QVariant &value);
  Q_INVOKABLE void setRawHeader(const QByteArray &headerName, const QByteArray &headerValue);

  Q_INVOKABLE [[nodiscard]] QVariant header(QNetworkRequest::KnownHeaders header) const;
  Q_INVOKABLE [[nodiscard]] QByteArray rawHeader(const QByteArray &headerName) const;
  Q_INVOKABLE [[nodiscard]] QList<QByteArray> rawHeaderList() const;

  Q_INVOKABLE [[nodiscard]] bool hasRawHeader(const QByteArray &headerName) const;

  [[nodiscard]] bool operator==(const Headers &other);
  [[nodiscard]] bool operator!=(const Headers &other);

private:
  QNetworkRequest m_request;
};

} // namespace egnite::web

#endif // EGNITE_WEB_HEADERS_H
