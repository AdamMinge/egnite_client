# ----------------------------------------------------------------------- #
# -------------------- Set CMake version and project -------------------- #
# ----------------------------------------------------------------------- #
cmake_minimum_required(VERSION 3.11)
project(
  EGNITE
  VERSION 1.0.0
  LANGUAGES CXX)
# ----------------------------------------------------------------------- #
# ---------------------- Include config and macros ---------------------- #
# ----------------------------------------------------------------------- #
include(${EGNITE_SOURCE_DIR}/cmake/config.cmake)
include(${EGNITE_SOURCE_DIR}/cmake/macros.cmake)
# ----------------------------------------------------------------------- #
# -------------------------- Add subdirectories ------------------------- #
# ----------------------------------------------------------------------- #
add_subdirectory(deps)

add_subdirectory(modules)
add_subdirectory(imports)
add_subdirectory(apps)

add_subdirectory(docs)
# ----------------------------------------------------------------------- #
# --------------------------- Static Analyzers -------------------------- #
# ----------------------------------------------------------------------- #
if(EGNITE_ENABLE_CLANG_TIDY)
  find_program(CLANGTIDY clang-tidy)
  if(CLANGTIDY)
    set(CMAKE_CXX_CLANG_TIDY ${CLANGTIDY}
                             -extra-arg=-Wno-unknown-warning-option)
  else()
    message(FATAL_ERROR "Clang-Tidy requested but executable not found")
  endif()
endif()

if(EGNITE_ENABLE_CPPCHECK)
  find_program(CPPCHECK cppcheck)
  if(CPPCHECK)
    set(CMAKE_CXX_CPPCHECK ${CPPCHECK} --suppress=missingInclude --enable=all
                           --inline-suppr --inconclusive --force)
  else()
    message(FATAL_ERROR "Cppcheck requested but executable not found")
  endif()
endif()
# ----------------------------------------------------------------------- #
