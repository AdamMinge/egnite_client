#ifndef EGNITE_CMD_EXPORT_H
#define EGNITE_CMD_EXPORT_H

/* ----------------------------------- Local -------------------------------- */
#include "egnite/config.h"
/* -------------------------------------------------------------------------- */

#if defined(EGNITE_CMD_EXPORTS)
#define EGNITE_CMD_API EGNITE_API_EXPORT
#else
#define EGNITE_CMD_API EGNITE_API_IMPORT
#endif

#endif  // EGNITE_CMD_EXPORT_H
