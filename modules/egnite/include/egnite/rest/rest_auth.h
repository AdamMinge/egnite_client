#ifndef EGNITE_REST_AUTH_H
#define EGNITE_REST_AUTH_H

/* ------------------------------------ Qt ---------------------------------- */
#include <QObject>
/* ----------------------------------- Local -------------------------------- */
#include "egnite/export.h"
/* -------------------------------------------------------------------------- */

namespace egnite::rest {

namespace detail {
class RestAuthPrivate;
}

class EGNITE_API RestAuth : public QObject {
  Q_OBJECT

 protected:
  RestAuth(detail::RestAuthPrivate& impl, QObject* parent = nullptr);

 private:
  Q_DECLARE_PRIVATE(detail::RestAuth);
};

}  // namespace egnite::rest

#endif  // EGNITE_REST_AUTH_H