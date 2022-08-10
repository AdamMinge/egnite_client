# ----------------------------------------------------------------------- #
# -------------------------- Set CMake Version -------------------------- #
# ----------------------------------------------------------------------- #
cmake_minimum_required(VERSION 3.11)
# ----------------------------------------------------------------------- #
# --------------- Generate headers and set files variable --------------- #
# ----------------------------------------------------------------------- #
set(source_root ${EGNITE_SOURCE_DIR}/modules/egnite/tests)
set(include_root ${EGNITE_SOURCE_DIR}/modules/egnite/tests)

set(sources ${source_root}/main.cpp)

set(includes)
# ----------------------------------------------------------------------- #
# ----------------------- Find external libraries ----------------------- #
# ----------------------------------------------------------------------- #
find_package(GTest REQUIRED)
# ----------------------------------------------------------------------- #
# -------------------------- Create egnite test ------------------------- #
# ----------------------------------------------------------------------- #
egnite_add_test(
  egnite_test
  SOURCES
  ${sources}
  ${includes}
  SRC_DIRECTORY
  ${source_root}
  INCLUDE_DIRECTORY
  ${include_root}
  DEPENDS
  GTest::gtest
  GTest::gmock
  egnite::egnite)
# ----------------------------------------------------------------------- #
