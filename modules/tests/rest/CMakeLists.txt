# ----------------------------------------------------------------------- #
# -------------------------- Set CMake Version -------------------------- #
# ----------------------------------------------------------------------- #
cmake_minimum_required(VERSION 3.11)
# ----------------------------------------------------------------------- #
# --------------- Generate headers and set files variable --------------- #
# ----------------------------------------------------------------------- #
set(source_root ${EGNITE_SOURCE_DIR}/modules/tests/rest)
set(include_root ${EGNITE_SOURCE_DIR}/modules/tests/rest)

set(sources ${source_root}/main.cpp
            ${source_root}/rest_data_serializer_test.cpp)

set(includes ${include_root}/rest_data_serializer_test.h)
# ----------------------------------------------------------------------- #
# ---------------------- Qt libraries configuration --------------------- #
# ----------------------------------------------------------------------- #
set(CMAKE_AUTOMOC ON)
# ----------------------------------------------------------------------- #
# ----------------------- Find external libraries ----------------------- #
# ----------------------------------------------------------------------- #
find_package(GTest REQUIRED)
# ----------------------------------------------------------------------- #
# -------------------------- Create egnite test ------------------------- #
# ----------------------------------------------------------------------- #
egnite_add_executable(
  egnite-rest-test
  SOURCES
  ${sources}
  ${includes}
  DEPENDENCIES
  GTest::gtest
  GTest::gmock
  egnite::egnite-rest)

target_include_directories(
  egnite-rest-test
  PUBLIC $<BUILD_INTERFACE:${include_root}>
  PRIVATE ${source_root})
# ----------------------------------------------------------------------- #
