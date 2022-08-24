#ifndef EGNITE_SERIALIZER_ARRAY_WRAPPER_H
#define EGNITE_SERIALIZER_ARRAY_WRAPPER_H

/* ----------------------------------- Boost -------------------------------- */
#include <boost/serialization/array_wrapper.hpp>
#include <boost/serialization/wrapper.hpp>
/* ----------------------------------- Local -------------------------------- */
#include "egnite/export.h"
/* -------------------------------------------------------------------------- */

namespace boost::serialization {

template <typename Array>
class dynamic_array_wrapper
    : public wrapper_traits<const dynamic_array_wrapper<Array>> {};

}  // namespace boost::serialization

#endif  // EGNITE_SERIALIZER_ARRAY_WRAPPER_H