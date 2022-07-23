#ifndef EGNITE_BOOST_SERIALIZER_EXPORT_H
#define EGNITE_BOOST_SERIALIZER_EXPORT_H

/* ----------------------------------- Local -------------------------------- */
#include "egnite/config.h"
/* -------------------------------------------------------------------------- */

#if defined(BOOST_SERIALIZER_EXPORTS)
#define BOOST_SERIALIZER_API EGNITE_API_EXPORT
#else
#define BOOST_SERIALIZER_API EGNITE_API_IMPORT
#endif

#endif // EGNITE_BOOST_SERIALIZER_EXPORT_H
