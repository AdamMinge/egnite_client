#ifndef EGNITE_CORE_BINDER_H
#define EGNITE_CORE_BINDER_H

/* ----------------------------------- Local -------------------------------- */
#include "egnite/core/export.h"
/* -------------------------------------------------------------------------- */

namespace egnite::core::utils {

namespace detail {

/* ------------------------ Lambda perfect capture  ------------------------- */

template <typename T>
auto capture(T&& t) {
  return std::conditional_t<std::is_lvalue_reference<T>::value,
                            std::reference_wrapper<std::remove_reference_t<T>>,
                            T>{std::forward<T>(t)};
}

/* ---------------------------- BindInfo default ---------------------------- */

template <typename TDst>
struct DstBindInfo : public DstBindInfo<decltype(&TDst::operator())> {};

/* ----------------------- BindInfo const class method ---------------------- */

template <typename TDstClass, typename TDstRet, typename... TDstArgs>
struct DstBindInfo<TDstRet (TDstClass::*)(TDstArgs...) const> {
  template <typename TSrc>
  struct SrcBindInfo : public SrcBindInfo<decltype(&TSrc::operator())> {};

  template <typename TSrcClass, typename TSrcRet, typename... TDstMissArgs>
  struct SrcBindInfo<TSrcRet (TSrcClass::*)(TDstArgs..., TDstMissArgs...)
                         const> {
    template <typename TFn>
    static inline auto bind(TFn&& fn) {
      return [xFn = capture(fn)](TDstArgs... args, TDstMissArgs... missArgs) {
        std::invoke(xFn, args...);
      };
    }
  };

  template <typename TSrcClass, typename TSrcRet, typename... TDstMissArgs>
  struct SrcBindInfo<TSrcRet (TSrcClass::*)(TDstArgs..., TDstMissArgs...)> {
    template <typename TFn>
    static inline auto bind(TFn&& fn) {
      return [xFn = capture(fn)](TDstArgs... args, TDstMissArgs... missArgs) {
        std::invoke(xFn, args...);
      };
    }
  };

  template <typename TSrcRet, typename... TDstMissArgs>
  struct SrcBindInfo<TSrcRet (*)(TDstArgs..., TDstMissArgs...)> {
    template <typename TFn>
    static inline auto bind(TFn&& fn) {
      return [xFn = capture(fn)](TDstArgs... args, TDstMissArgs... missArgs) {
        std::invoke(xFn, args...);
      };
    }
  };
};

/* ---------------------- BindInfo no const class method -------------------- */

template <typename TDstClass, typename TDstRet, typename... TDstArgs>
struct DstBindInfo<TDstRet (TDstClass::*)(TDstArgs...)> {
  template <typename TSrc>
  struct SrcBindInfo : public SrcBindInfo<decltype(&TSrc::operator())> {};

  template <typename TSrcClass, typename TSrcRet, typename... TDstMissArgs>
  struct SrcBindInfo<TSrcRet (TSrcClass::*)(TDstArgs..., TDstMissArgs...)
                         const> {
    template <typename TFn>
    static inline auto bind(TFn&& fn) {
      return [xFn = capture(fn)](TDstArgs... args, TDstMissArgs... missArgs) {
        std::invoke(xFn, args...);
      };
    }
  };

  template <typename TSrcClass, typename TSrcRet, typename... TDstMissArgs>
  struct SrcBindInfo<TSrcRet (TSrcClass::*)(TDstArgs..., TDstMissArgs...)> {
    template <typename TFn>
    static inline auto bind(TFn&& fn) {
      return [xFn = capture(fn)](TDstArgs... args, TDstMissArgs... missArgs) {
        std::invoke(xFn, args...);
      };
    }
  };

  template <typename TSrcRet, typename... TDstMissArgs>
  struct SrcBindInfo<TSrcRet (*)(TDstArgs..., TDstMissArgs...)> {
    template <typename TFn>
    static inline auto bind(TFn&& fn) {
      return [xFn = capture(fn)](TDstArgs... args, TDstMissArgs... missArgs) {
        std::invoke(xFn, args...);
      };
    }
  };
};

/* --------------------- BindInfo no const class method --------------------- */

template <typename TDstRet, typename... TDstArgs>
struct DstBindInfo<TDstRet (*)(TDstArgs...)> {
  template <typename TSrc>
  struct SrcBindInfo : public SrcBindInfo<decltype(&TSrc::operator())> {};

  template <typename TSrcClass, typename TSrcRet, typename... TDstMissArgs>
  struct SrcBindInfo<TSrcRet (TSrcClass::*)(TDstArgs..., TDstMissArgs...)
                         const> {
    template <typename TFn>
    static inline auto bind(TFn&& fn) {
      return [xFn = capture(fn)](TDstArgs... args, TDstMissArgs... missArgs) {
        std::invoke(xFn, args...);
      };
    }
  };

  template <typename TSrcClass, typename TSrcRet, typename... TDstMissArgs>
  struct SrcBindInfo<TSrcRet (TSrcClass::*)(TDstArgs..., TDstMissArgs...)> {
    template <typename TFn>
    static inline auto bind(TFn&& fn) {
      return [xFn = capture(fn)](TDstArgs... args, TDstMissArgs... missArgs) {
        std::invoke(xFn, args...);
      };
    }
  };

  template <typename TSrcRet, typename... TDstMissArgs>
  struct SrcBindInfo<TSrcRet (*)(TDstArgs..., TDstMissArgs...)> {
    template <typename TFn>
    static inline auto bind(TFn&& fn) {
      return [xFn = capture(fn)](TDstArgs... args, TDstMissArgs... missArgs) {
        std::invoke(xFn, args...);
      };
    }
  };
};

}  // namespace detail

/* ------------------------------- bindCallback ----------------------------- */

template <typename TSrcFn, typename TDstFn>
static inline auto bindCallback(TDstFn&& fn) {
  using DstBindInfo = detail::DstBindInfo<std::decay_t<TDstFn>>;
  using SrcBindInfo = typename DstBindInfo::SrcBindInfo<std::decay_t<TSrcFn>>;

  return SrcBindInfo::bind(std::forward<TDstFn>(fn));
}

/* -------------------------------- Overloaded ------------------------------ */

template <class... Ts>
struct overloaded : Ts... {
  using Ts::operator()...;
};

}  // namespace egnite::core::utils

#endif  // EGNITE_CORE_BINDER_H