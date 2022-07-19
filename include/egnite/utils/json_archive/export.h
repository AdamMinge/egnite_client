#ifndef EGNITE_JSON_ARCHIVE_EXPORT_H
#define EGNITE_JSON_ARCHIVE_EXPORT_H

/* ----------------------------------- Local -------------------------------- */
#include "egnite/config.h"
/* -------------------------------------------------------------------------- */

#if defined(JSON_ARCHIVE_EXPORTS)
#define JSON_ARCHIVE_API EGNITE_API_EXPORT
#else
#define JSON_ARCHIVE_API EGNITE_API_IMPORT
#endif

#endif // EGNITE_JSON_ARCHIVE_EXPORT_H
