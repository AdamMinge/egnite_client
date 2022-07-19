#ifndef EGNITE_QT_BOOST_SERIALIZER_EXPORT_H
#define EGNITE_QT_BOOST_SERIALIZER_EXPORT_H

/* ----------------------------------- Local -------------------------------- */
#include "egnite/config.h"
/* -------------------------------------------------------------------------- */

#if defined(QT_BOOST_SERIALIZER_EXPORTS)
#define QT_BOOST_SERIALIZER_API EGNITE_API_EXPORT
#else
#define QT_BOOST_SERIALIZER_API EGNITE_API_IMPORT
#endif

#endif // EGNITE_QT_BOOST_SERIALIZER_EXPORT_H
