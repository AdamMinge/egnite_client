#ifndef EGNITE_EXPORT_H
#define EGNITE_EXPORT_H

/* ----------------------------------- Local -------------------------------- */
#include "egnite/config.h"
/* -------------------------------------------------------------------------- */

#if defined(EGNITE_EXPORTS)
#define EGNITE_API EGNITE_API_EXPORT
#else
#define EGNITE_API EGNITE_API_IMPORT
#endif

#endif  // EGNITE_EXPORT_H
