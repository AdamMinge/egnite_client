# ----------------------------------------------------------------------- #
# -------------------------- Set CMake Version -------------------------- #
# ----------------------------------------------------------------------- #
cmake_minimum_required(VERSION 3.11)
# ----------------------------------------------------------------------- #
# --------------- Generate headers and set files variable --------------- #
# ----------------------------------------------------------------------- #
set(source_root ${EGNITE_SOURCE_DIR}/modules/tests/core)
set(include_root ${EGNITE_SOURCE_DIR}/modules/tests/core)

set(sources ${source_root}/main.cpp ${source_root}/binder_test.cpp)

set(includes ${include_root}/binder_test.h)
# ----------------------------------------------------------------------- #
# ----------------------- Find external libraries ----------------------- #
# ----------------------------------------------------------------------- #
find_package(GTest REQUIRED)
# ----------------------------------------------------------------------- #
# -------------------------- Create egnite test ------------------------- #
# ----------------------------------------------------------------------- #
egnite_add_executable(
  egnite-core-test
  SOURCES
  ${sources}
  ${includes}
  DEPENDENCIES
  GTest::gtest
  GTest::gmock
  egnite::egnite-core)

target_include_directories(
  egnite-core-test
  PUBLIC $<BUILD_INTERFACE:${include_root}>
  PRIVATE ${source_root})
# ----------------------------------------------------------------------- #
