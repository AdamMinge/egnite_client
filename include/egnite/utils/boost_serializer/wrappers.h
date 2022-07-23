#ifndef EGNITE_BOOST_SERIALIZER_WRAPPERS_H
#define EGNITE_BOOST_SERIALIZER_WRAPPERS_H

/* --------------------------------- Standard ------------------------------- */
#include <array>
#include <list>
#include <vector>
/* ----------------------------------- Boost -------------------------------- */
#include <boost/serialization/wrapper.hpp>
/* ----------------------------------- Local -------------------------------- */
#include "egnite/utils/boost_serializer/export.h"
/* -------------------------------------------------------------------------- */

namespace boost::serialization {

namespace detail {

template <typename TYPE>
concept IsResizable = requires(TYPE& value) {
  {value.resize(size_t{})};
};

} // namespace detail

/* ---------------------------- dynamic_array_wrapper --------------------- */

template <typename ARRAY>
class dynamic_array_wrapper
    : public serialization::wrapper_traits<const dynamic_array_wrapper<ARRAY> > {
public:
  dynamic_array_wrapper(ARRAY& array) : m_array(array){};

  size_t size() const { return m_array.size(); }
  auto& value(size_t index) const {
    resize(m_array, index + 1);
    return *std::next(m_array.begin(), index);
  }

  template <typename TYPE> void resize(TYPE& array, size_t size) const {}
  template <detail::IsResizable TYPE> void resize(TYPE& array, size_t size) const {
    array.resize(size);
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
