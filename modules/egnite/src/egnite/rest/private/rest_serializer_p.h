#ifndef EGNITE_REST_SERIALIZER_P_H
#define EGNITE_REST_SERIALIZER_P_H

/* ------------------------------------ Qt ---------------------------------- */
#include <QtCore/private/qobject_p.h>
/* -------------------------------------------------------------------------- */

namespace egnite::rest {

class RestSerializerPrivate : public QObjectPrivate {
 public:
  Q_DECLARE_PUBLIC(RestSerializer)

 public:
  RestSerializerPrivate();
};

}  // namespace egnite::rest

#endif  // EGNITE_REST_SERIALIZER_P_H