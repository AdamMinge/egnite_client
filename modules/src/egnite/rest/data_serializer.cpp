/* ----------------------------------- Local -------------------------------- */
#include "egnite/rest/data_serializer.h"
/* -------------------------------------------------------------------------- */

namespace egnite::rest {

/* ---------------------------- DataSerializer ------------------------------ */

DataSerializer::DataSerializer(QObject* parent) : QObject(parent) {}

DataSerializer::~DataSerializer() = default;

}  // namespace egnite::rest