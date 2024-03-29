#ifndef EGNITE_CORE_MACROS_BINDER_H
#define EGNITE_CORE_MACROS_BINDER_H

/* ----------------------------------- Local -------------------------------- */
#include "egnite/core/export.h"
#include "egnite/core/macros/map.h"
#include "egnite/core/utils/binder.h"
/* -------------------------------------------------------------------------- */

#define EGNITE_DEFINE_BINDER(class, binder_name, signal_name)          \
  template <typename Handler>                                          \
  auto binder_name(Handler&& handler, QObject* scope = nullptr) {      \
    connect(this, &class ::signal_name, scope ? scope : this,          \
            core::utils::bindCallback<decltype(&class ::signal_name)>( \
                std::forward<Handler>(handler)));                      \
    return this;                                                       \
  }

#define EGNITE_DEFINE_BINDER_EXTENDED(class, return_class, binder_name, \
                                      signal_name)                      \
  template <typename Handler>                                           \
  auto binder_name(Handler&& handler, QObject* scope = nullptr) {       \
    connect(this, &class ::signal_name, scope ? scope : this,           \
            core::utils::bindCallback<decltype(&class ::signal_name)>(  \
                std::forward<Handler>(handler)));                       \
    return static_cast<return_class*>(this);                            \
  }

#endif  // EGNITE_CORE_MACRO_BINDER_H