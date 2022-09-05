#ifndef EGNITE_SERIALIZER_EXPORT_H
#define EGNITE_SERIALIZER_EXPORT_H

/* ----------------------------------- Local -------------------------------- */
#include "egnite/config.h"
/* -------------------------------------------------------------------------- */

#if defined(EGNITE_SERIALIZER_EXPORTS)
#define EGNITE_SERIALIZER_API EGNITE_API_EXPORT
#else
#define EGNITE_SERIALIZER_API EGNITE_API_IMPORT
#endif

#endif  // EGNITE_SERIALIZER_EXPORT_H
