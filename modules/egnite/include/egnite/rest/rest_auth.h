#ifndef EGNITE_REST_AUTH_H
#define EGNITE_REST_AUTH_H

/* ------------------------------------ Qt ---------------------------------- */
#include <QObject>
/* ----------------------------------- Local -------------------------------- */
#include "egnite/export.h"
/* -------------------------------------------------------------------------- */

namespace egnite::rest {

class EGNITE_API RestAuth : public QObject {
  Q_OBJECT

 public:
};

}  // namespace egnite::rest

#endif  // EGNITE_REST_AUTH_H