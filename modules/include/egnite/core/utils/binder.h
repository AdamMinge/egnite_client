#ifndef EGNITE_CORE_UTILS_BINDER_H
#define EGNITE_CORE_UTILS_BINDER_H

/* ----------------------------------- Local -------------------------------- */
#include "egnite/core/export.h"
#include "egnite/core/utils/capture.h"
/* -------------------------------------------------------------------------- */

namespace egnite::core::utils {

namespace detail {

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
      return [xFn = capture(std::forward<TFn>(fn))](TDstArgs... args,
                                                    TDstMissArgs... missArgs) {
        std::invoke(xFn, args...);
      };
    }
  };

  template <typename TSrcClass, typename TSrcRet, typename... TDstMissArgs>
  struct SrcBindInfo<TSrcRet (TSrcClass::*)(TDstArgs..., TDstMissArgs...)> {
    template <typename TFn>
    static inline auto bind(TFn&& fn) {
      return [xFn = capture(std::forward<TFn>(fn))](TDstArgs... args,
                                                    TDstMissArgs... missArgs) {
        std::invoke(xFn, args...);
      };
    }
  };

  template <typename TSrcRet, typename... TDstMissArgs>
  struct SrcBindInfo<TSrcRet (*)(TDstArgs..., TDstMissArgs...)> {
    template <typename TFn>
    static inline auto bind(TFn&& fn) {
      return [xFn = capture(std::forward<TFn>(fn))](TDstArgs... args,
                                                    TDstMissArgs... missArgs) {
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
      return [xFn = capture(std::forward<TFn>(fn))](TDstArgs... args,
                                                    TDstMissArgs... missArgs) {
        std::invoke(xFn, args...);
      };
    }
  };

  template <typename TSrcClass, typename TSrcRet, typename... TDstMissArgs>
  struct SrcBindInfo<TSrcRet (TSrcClass::*)(TDstArgs..., TDstMissArgs...)> {
    template <typename TFn>
    static inline auto bind(TFn&& fn) {
      return [xFn = capture(std::forward<TFn>(fn))](TDstArgs... args,
                                                    TDstMissArgs... missArgs) {
        std::invoke(xFn, args...);
      };
    }
  };

  template <typename TSrcRet, typename... TDstMissArgs>
  struct SrcBindInfo<TSrcRet (*)(TDstArgs..., TDstMissArgs...)> {
    template <typename TFn>
    static inline auto bind(TFn&& fn) {
      return [xFn = capture(std::forward<TFn>(fn))](TDstArgs... args,
                                                    TDstMissArgs... missArgs) {
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
      return [xFn = capture(std::forward<TFn>(fn))](TDstArgs... args,
                                                    TDstMissArgs... missArgs) {
        std::invoke(xFn, args...);
      };
    }
  };

  template <typename TSrcClass, typename TSrcRet, typename... TDstMissArgs>
  struct SrcBindInfo<TSrcRet (TSrcClass::*)(TDstArgs..., TDstMissArgs...)> {
    template <typename TFn>
    static inline auto bind(TFn&& fn) {
      return [xFn = capture(std::forward<TFn>(fn))](TDstArgs... args,
                                                    TDstMissArgs... missArgs) {
        std::invoke(xFn, args...);
      };
    }
  };

  template <typename TSrcRet, typename... TDstMissArgs>
  struct SrcBindInfo<TSrcRet (*)(TDstArgs..., TDstMissArgs...)> {
    template <typename TFn>
    static inline auto bind(TFn&& fn) {
      return [xFn = capture(std::forward<TFn>(fn))](TDstArgs... args,
                                                    TDstMissArgs... missArgs) {
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

#endif  // EGNITE_CORE_UTILS_BINDER_H