#ifndef EGNITE_CORE_EXPORT_H
#define EGNITE_CORE_EXPORT_H

/* ----------------------------------- Local -------------------------------- */
#include "egnite/config.h"
/* -------------------------------------------------------------------------- */

#if defined(EGNITE_CORE_EXPORTS)
#define EGNITE_CORE_API EGNITE_API_EXPORT
#else
#define EGNITE_CORE_API EGNITE_API_IMPORT
#endif

#endif  // EGNITE_CORE_EXPORT_H
