#ifndef EGNITE_REST_SERIALIZER_H
#define EGNITE_REST_SERIALIZER_H

/* ------------------------------------ Qt ---------------------------------- */
#include <QObject>
/* ----------------------------------- Local -------------------------------- */
#include "egnite/export.h"
/* -------------------------------------------------------------------------- */

namespace egnite::rest {

class RestSerializerPrivate;

class EGNITE_API RestSerializer : public QObject {
  Q_OBJECT

 protected:
  RestSerializer(RestSerializerPrivate& impl, QObject* parent = nullptr);

 private:
  Q_DECLARE_PRIVATE(RestSerializer);
};

}  // namespace egnite::rest

#endif  // EGNITE_REST_SERIALIZER_H