#ifndef EGNITE_REST_SERIALIZER_H
#define EGNITE_REST_SERIALIZER_H

/* ------------------------------------ Qt ---------------------------------- */
#include <QObject>
/* ----------------------------------- Local -------------------------------- */
#include "egnite/export.h"
/* -------------------------------------------------------------------------- */

namespace egnite::rest {

class EGNITE_API RestSerializer : public QObject {
  Q_OBJECT

 public:
};

}  // namespace egnite::rest

#endif  // EGNITE_REST_SERIALIZER_H