#ifndef EGNITE_BOOST_SERIALIZER_STL_H
#define EGNITE_BOOST_SERIALIZER_STL_H

/* --------------------------------- Standard ------------------------------- */
#include <array>
#include <list>
#include <vector>
/* ----------------------------------- Boost -------------------------------- */
#include <boost/serialization/wrapper.hpp>
/* ----------------------------------- Local -------------------------------- */
#include "egnite/utils/boost_serializer/export.h"
#include "egnite/utils/boost_serializer/wrappers.h"
/* -------------------------------------------------------------------------- */

namespace boost::serialization {

/* ----------------------------------- vector ----------------------------- */

template <class Archive, class U, class Allocator>
inline void save(Archive& ar, const std::vector<U, Allocator>& t,
                 const unsigned int /* file_version */) {
  ar << make_dynamic_array(t);
}

template <class Archive, class U, class Allocator>
inline void load(Archive& ar, std::vector<U, Allocator>& t, const unsigned int /* file_version */) {
  ar >> make_dynamic_array(t);
}

template <class Archive, class U, class Allocator>
inline void serialize(Archive& ar, std::vector<U, Allocator>& t, const unsigned int file_version) {
  split_free(ar, t, file_version);
}

/* ----------------------------------- list ------------------------------- */

template <class Archive, class U, class Allocator>
inline void save(Archive& ar, const std::list<U, Allocator>& t,
                 const unsigned int /* file_version */) {
  ar << make_dynamic_array(t);
}

template <class Archive, class U, class Allocator>
inline void load(Archive& ar, std::list<U, Allocator>& t, const unsigned int /* file_version */) {
  ar >> make_dynamic_array(t);
}

template <class Archive, class U, class Allocator>
inline void serialize(Archive& ar, std::list<U, Allocator>& t, const unsigned int file_version) {
  split_free(ar, t, file_version);
}

/* ----------------------------------- array ------------------------------ */

template <class Archive, class U, size_t Size>
inline void save(Archive& ar, const std::array<U, Size>& t, const unsigned int /* file_version */) {
  ar << make_array(t.data(), t.size());
}

template <class Archive, class U, size_t Size>
inline void load(Archive& ar, std::array<U, Size>& t, const unsigned int /* file_version */) {
  ar >> make_array(t.data(), t.size());
}

template <class Archive, class U, size_t Size>
inline void serialize(Archive& ar, std::array<U, Size>& t, const unsigned int file_version) {
  split_free(ar, t, file_version);
}

} // namespace boost::serialization

#endif // EGNITE_BOOST_SERIALIZER_STL_H
