/* -------------------------------------------------------------------------------------------------- */
#ifndef EGNITE_CONFIG_H
#define EGNITE_CONFIG_H
/* -------------------------------------------------------------------------------------------------- */
/* ------------------------------------- Define EGNITE version -------------------------------------- */
/* -------------------------------------------------------------------------------------------------- */
// clang-format off
#define EGNITE_VERSION_MAJOR 0
#define EGNITE_VERSION_MINOR 1
#define EGNITE_VERSION_PATCH 0
#define EGNITE_VERSION 0.1.0
#define EGNITE_VERSION_STR "0.1.0"
// clang-format on
/* -------------------------------------------------------------------------------------------------- */
/* --------------------------------- Identify the operating system
 * ---------------------------------- */
/* -------------------------------------------------------------------------------------------------- */
#if defined(_WIN32)
#define EGNITE_OS_WINDOWS
#elif defined(__linux__)
#define EGNITE_OS_LINUX
#else
#error This operating system is not supported by EGNITE library
#endif
/* -------------------------------------------------------------------------------------------------- */
/* --------------------------------- Define a portable debug macro ---------------------------------- */
/* -------------------------------------------------------------------------------------------------- */
#if !defined(NDEBUG)
#define EGNITE_DEBUG
#endif
/* -------------------------------------------------------------------------------------------------- */
/* ------------------- Define helpers to create portable import / export macros --------------------- */
/* -------------------------------------------------------------------------------------------------- */
#if !defined(EGNITE_STATIC)
#if defined(EGNITE_OS_WINDOWS)
#define EGNITE_API_EXPORT __declspec(dllexport)
#define EGNITE_API_IMPORT __declspec(dllimport)
#else
#if __GNUC__ >= 4
#define EGNITE_API_EXPORT __attribute__((__visibility__("default")))
#define EGNITE_API_IMPORT __attribute__((__visibility__("default")))
#else
#define EGNITE_API_EXPORT
#define EGNITE_API_IMPORT
#endif
#endif
#else
#define EGNITE_API_EXPORT
#define EGNITE_API_IMPORT
#endif
/* -------------------------------------------------------------------------------------------------- */
/* --------------------------- Define helpers to set items as depracted ----------------------------- */
/* -------------------------------------------------------------------------------------------------- */
#if !defined(EGNITE_DEPRECATED_WARNINGS)
#define EGNITE_DEPRECATED [[deprecated]]
#else
#define EGNITE_DEPRECATED
#endif
/* -------------------------------------------------------------------------------------------------- */
#endif // EGNITE_CONFIG_H
/* -------------------------------------------------------------------------------------------------- */
