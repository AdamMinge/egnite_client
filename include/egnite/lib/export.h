#ifndef EGNITE_LIB_EGNITE_EXPORT_H
#define EGNITE_LIB_EGNITE_EXPORT_H

/* ----------------------------------- Local -------------------------------- */
#include "egnite/config.h"
/* -------------------------------------------------------------------------- */

#if defined(LIB_EGNITE_EXPORTS)
#define LIB_EGNITE_API EGNITE_API_EXPORT
#else
#define LIB_EGNITE_API EGNITE_API_IMPORT
#endif


#endif//EGNITE_LIB_EGNITE_EXPORT_H
