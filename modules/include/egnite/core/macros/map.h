#ifndef EGNITE_CORE_MACROS_MAP_H
#define EGNITE_CORE_MACROS_MAP_H

/* ----------------------------------- Local -------------------------------- */
#include "egnite/core/export.h"
/* -------------------------------------------------------------------------- */

#define EGNITE_EVAL0(...) __VA_ARGS__
#define EGNITE_EVAL1(...) EGNITE_EVAL0(EGNITE_EVAL0(EGNITE_EVAL0(__VA_ARGS__)))
#define EGNITE_EVAL2(...) EGNITE_EVAL1(EGNITE_EVAL1(EGNITE_EVAL1(__VA_ARGS__)))
#define EGNITE_EVAL3(...) EGNITE_EVAL2(EGNITE_EVAL2(EGNITE_EVAL2(__VA_ARGS__)))
#define EGNITE_EVAL4(...) EGNITE_EVAL3(EGNITE_EVAL3(EGNITE_EVAL3(__VA_ARGS__)))
#define EGNITE_EVAL(...) EGNITE_EVAL4(EGNITE_EVAL4(EGNITE_EVAL4(__VA_ARGS__)))

#define EGNITE_MAP_END(...)
#define EGNITE_MAP_OUT
#define EGNITE_MAP_COMMA ,

#define EGNITE_MAP_GET_END2() 0, EGNITE_MAP_END
#define EGNITE_MAP_GET_END1(...) EGNITE_MAP_GET_END2
#define EGNITE_MAP_GET_END(...) EGNITE_MAP_GET_END1
#define EGNITE_MAP_NEXT0(test, next, ...) next EGNITE_MAP_OUT
#define EGNITE_MAP_NEXT1(test, next) EGNITE_MAP_NEXT0(test, next, 0)
#define EGNITE_MAP_NEXT(test, next) \
  EGNITE_MAP_NEXT1(EGNITE_MAP_GET_END test, next)

#define EGNITE_MAP0(f, x, peek, ...) \
  f(x) EGNITE_MAP_NEXT(peek, EGNITE_MAP1)(f, peek, __VA_ARGS__)
#define EGNITE_MAP1(f, x, peek, ...) \
  f(x) EGNITE_MAP_NEXT(peek, EGNITE_MAP0)(f, peek, __VA_ARGS__)

#define EGNITE_MAP_LIST_NEXT1(test, next) \
  EGNITE_MAP_NEXT0(test, EGNITE_MAP_COMMA next, 0)
#define EGNITE_MAP_LIST_NEXT(test, next) \
  EGNITE_MAP_LIST_NEXT1(EGNITE_MAP_GET_END test, next)

#define EGNITE_MAP_LIST0(f, x, peek, ...) \
  f(x) EGNITE_MAP_LIST_NEXT(peek, EGNITE_MAP_LIST1)(f, peek, __VA_ARGS__)
#define EGNITE_MAP_LIST1(f, x, peek, ...) \
  f(x) EGNITE_MAP_LIST_NEXT(peek, EGNITE_MAP_LIST0)(f, peek, __VA_ARGS__)

#define EGNITE_MAP(f, ...) \
  EGNITE_EVAL(EGNITE_MAP1(f, __VA_ARGS__, ()()(), ()()(), ()()(), 0))
#define EGNITE_MAP_LIST(f, ...) \
  EGNITE_EVAL(EGNITE_MAP_LIST1(f, __VA_ARGS__, ()()(), ()()(), ()()(), 0))

#endif  // EGNITE_CORE_MACROS_MAP_H