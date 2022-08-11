#ifndef EGNITE_REST_AUTH_P_H
#define EGNITE_REST_AUTH_P_H

/* ------------------------------------ Qt ---------------------------------- */
#include <QtCore/private/qobject_p.h>
/* -------------------------------------------------------------------------- */

namespace egnite::rest {

class RestAuthPrivate : public QObjectPrivate {
 public:
  Q_DECLARE_PUBLIC(RestAuth)

 public:
  RestAuthPrivate();
};

}  // namespace egnite::rest

#endif  // EGNITE_REST_AUTH_P_H