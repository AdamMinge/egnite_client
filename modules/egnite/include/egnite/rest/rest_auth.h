#ifndef EGNITE_REST_AUTH_H
#define EGNITE_REST_AUTH_H

/* ------------------------------------ Qt ---------------------------------- */
#include <QObject>
/* ----------------------------------- Local -------------------------------- */
#include "egnite/export.h"
/* -------------------------------------------------------------------------- */

namespace egnite::rest {

class RestAuthPrivate;

class EGNITE_API RestAuth : public QObject {
  Q_OBJECT

 protected:
  RestAuth(RestAuthPrivate& impl, QObject* parent = nullptr);

 private:
  Q_DECLARE_PRIVATE(RestAuth);
};

}  // namespace egnite::rest

#endif  // EGNITE_REST_AUTH_H