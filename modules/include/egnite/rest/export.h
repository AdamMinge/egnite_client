#ifndef EGNITE_REST_EXPORT_H
#define EGNITE_REST_EXPORT_H

/* ----------------------------------- Local -------------------------------- */
#include "egnite/config.h"
/* -------------------------------------------------------------------------- */

#if defined(EGNITE_REST_EXPORTS)
#define EGNITE_REST_API EGNITE_API_EXPORT
#else
#define EGNITE_REST_API EGNITE_API_IMPORT
#endif

#endif  // EGNITE_REST_EXPORT_H
