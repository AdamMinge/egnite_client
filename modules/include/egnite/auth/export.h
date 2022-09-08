#ifndef EGNITE_AUTH_EXPORT_H
#define EGNITE_AUTH_EXPORT_H

/* ----------------------------------- Local -------------------------------- */
#include "egnite/config.h"
/* -------------------------------------------------------------------------- */

#if defined(EGNITE_AUTH_EXPORTS)
#define EGNITE_AUTH_API EGNITE_API_EXPORT
#else
#define EGNITE_AUTH_API EGNITE_API_IMPORT
#endif

#endif  // EGNITE_AUTH_EXPORT_H
