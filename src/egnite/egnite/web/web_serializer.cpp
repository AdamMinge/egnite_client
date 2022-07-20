/* ----------------------------------- Local -------------------------------- */
#include "egnite/egnite/web/web_serializer.h"
/* -------------------------------------------------------------------------- */

namespace egnite::web {

/* ------------------------------- WebSerializer -------------------------- */

WebSerializer::WebSerializer(Format format) : m_format(format) {}

WebSerializer::~WebSerializer() = default;

WebSerializer::Format WebSerializer::getFormat() const { return m_format; }

/* ------------------------------ JsonSerializer -------------------------- */

JsonSerializer::JsonSerializer() : WebSerializer(Format::Json) {}

JsonSerializer::~JsonSerializer() = default;

} // namespace egnite::web
