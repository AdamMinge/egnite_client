#ifndef EGNITE_REST_AUTH_P_H
#define EGNITE_REST_AUTH_P_H

/* ------------------------------------ Qt ---------------------------------- */
#include <QtCore/private/qobject_p.h>
/* ------------------------------------ Local ------------------------------- */
#include "egnite/rest/rest_auth.h"
/* -------------------------------------------------------------------------- */

namespace egnite::rest::detail {

class RestAuthPrivate : public QObjectPrivate {
 public:
  Q_DECLARE_PUBLIC(RestAuth)

 public:
  RestAuthPrivate();
};

}  // namespace egnite::rest::detail

#endif  // EGNITE_REST_AUTH_P_H