#ifndef EGNITE_CORE_UTILS_CAPTURE_H
#define EGNITE_CORE_UTILS_CAPTURE_H

/* ----------------------------------- Local -------------------------------- */
#include "egnite/core/export.h"
/* -------------------------------------------------------------------------- */

namespace egnite::core::utils {

/* ------------------------ Lambda perfect capture  ------------------------- */

template <typename T>
auto capture(T&& t) {
  return std::conditional_t<std::is_lvalue_reference<T>::value,
                            std::reference_wrapper<std::remove_reference_t<T>>,
                            T>{std::forward<T>(t)};
}

}  // namespace egnite::core::utils

#endif  // EGNITE_CORE_UTILS_CAPTURE_H