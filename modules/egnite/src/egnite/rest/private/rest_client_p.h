#ifndef EGNITE_REST_CLIENT_P_H
#define EGNITE_REST_CLIENT_P_H

/* ------------------------------------ Qt ---------------------------------- */
#include <QtCore/private/qobject_p.h>

#include <QUrl>
#include <QVersionNumber>
/* -------------------------------------------------------------------------- */

namespace egnite::rest {

class RestClientPrivate : public QObjectPrivate {
 public:
  Q_DECLARE_PUBLIC(RestClient)

 public:
  RestClientPrivate(const QUrl& url, const QVersionNumber& version);

  QUrl base_url;
  QVersionNumber version;
};

}  // namespace egnite::rest

#endif  // EGNITE_REST_CLIENT_P_H