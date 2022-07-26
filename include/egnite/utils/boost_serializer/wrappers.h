#ifndef EGNITE_BOOST_SERIALIZER_WRAPPERS_H
#define EGNITE_BOOST_SERIALIZER_WRAPPERS_H

/* --------------------------------- Standard ------------------------------- */
#include <array>
#include <list>
#include <vector>
/* ----------------------------------- Boost -------------------------------- */
#include <boost/serialization/array_wrapper.hpp>
#include <boost/serialization/wrapper.hpp>
/* ----------------------------------- Local -------------------------------- */
#include "egnite/utils/boost_serializer/export.h"
/* -------------------------------------------------------------------------- */

namespace boost::serialization {

template <typename ARRAY>
class dynamic_array_wrapper
    : public serialization::wrapper_traits<const dynamic_array_wrapper<ARRAY> > {
public:
  dynamic_array_wrapper(ARRAY& array) : m_array(array){};

  size_t size() const { return m_array.size(); }
  auto& value(size_t index) const {
    if constexpr (!std::is_const_v<ARRAY>) {
      if (m_array.size() <= index)
        m_array.resize(index + 1);
    }

    return *std::next(m_array.begin(), index);
  }

private:
  ARRAY& m_array;
};

template <typename TYPE>
const serialization::dynamic_array_wrapper<TYPE> make_dynamic_array(TYPE& array) {
  return serialization::dynamic_array_wrapper<TYPE>(array);
}

} // namespace boost::serialization

#endif // EGNITE_BOOST_SERIALIZER_WRAPPERS_H
