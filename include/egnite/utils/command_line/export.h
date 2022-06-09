#ifndef EGNITE_COMMAND_LINE_EXPORT_H
#define EGNITE_COMMAND_LINE_EXPORT_H

/* ----------------------------------- Local -------------------------------- */
#include "egnite/config.h"
/* -------------------------------------------------------------------------- */

#if defined(COMMAND_LINE_EXPORTS)
#define COMMAND_LINE_API EGNITE_API_EXPORT
#else
#define COMMAND_LINE_API EGNITE_API_IMPORT
#endif

#endif// EGNITE_COMMAND_LINE_EXPORT_H
