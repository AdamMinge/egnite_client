/* ----------------------------------- Local -------------------------------- */
#include "egnite/rest/rest_data_serializer.h"
/* -------------------------------------------------------------------------- */

namespace egnite::rest {

/* -------------------------- RestDataSerializer ---------------------------- */

RestDataSerializer::RestDataSerializer(QObject* parent) : QObject(parent) {}

RestDataSerializer::~RestDataSerializer() = default;

}  // namespace egnite::rest