/* ----------------------------------- Local -------------------------------- */
#include "egnite/egnite/web/serializer.h"
/* -------------------------------------------------------------------------- */

namespace egnite::web {

/* --------------------------------- Serializer --------------------------- */

Serializer::Serializer(Format format) : m_format(format) {}

Serializer::~Serializer() = default;

Serializer::Format Serializer::getFormat() const { return m_format; }

/* ------------------------------ JsonSerializer -------------------------- */

JsonSerializer::JsonSerializer() : Serializer(Format::Json) {}

JsonSerializer::~JsonSerializer() = default;

} // namespace egnite::web
