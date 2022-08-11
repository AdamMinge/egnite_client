#ifndef EGNITE_REST_API_P_H
#define EGNITE_REST_API_P_H

/* ------------------------------------ Qt ---------------------------------- */
#include <QtCore/private/qobject_p.h>

#include <QString>
/* -------------------------------------------------------------------------- */

namespace egnite::rest {

class RestClient;

class RestApiPrivate : public QObjectPrivate {
 public:
  Q_DECLARE_PUBLIC(RestApi)

 public:
  RestApiPrivate(RestClient* client, const QString& subpath);

  RestClient* client;
  QString subpath;
};

}  // namespace egnite::rest

#endif  // EGNITE_REST_API_P_H