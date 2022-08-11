/* ----------------------------------- Local -------------------------------- */
#include "egnite/rest/rest_serializer.h"

#include "egnite/rest/private/rest_serializer_p.h"
/* -------------------------------------------------------------------------- */

namespace egnite::rest {

/* ----------------------------- RestSerializer ----------------------------- */

RestSerializer::RestSerializer(RestSerializerPrivate& impl, QObject* parent)
    : QObject(impl, parent) {}

/* -------------------------- RestSerializerPrivate ------------------------- */

RestSerializerPrivate::RestSerializerPrivate() {}
}  // namespace egnite::rest
