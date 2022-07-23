#ifndef EGNITE_BOOST_ARCHIVE_EXPORT_H
#define EGNITE_BOOST_ARCHIVE_EXPORT_H

/* ----------------------------------- Local -------------------------------- */
#include "egnite/config.h"
/* -------------------------------------------------------------------------- */

#if defined(BOOST_ARCHIVE_EXPORTS)
#define BOOST_ARCHIVE_API EGNITE_API_EXPORT
#else
#define BOOST_ARCHIVE_API EGNITE_API_IMPORT
#endif

#endif // EGNITE_BOOST_ARCHIVE_EXPORT_H
