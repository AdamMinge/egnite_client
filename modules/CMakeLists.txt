# ----------------------------------------------------------------------- #
# -------------------------- Set CMake Version -------------------------- #
# ----------------------------------------------------------------------- #
cmake_minimum_required(VERSION 3.11)
# ----------------------------------------------------------------------- #
# -------------------------- Add subdirectories ------------------------- #
# ----------------------------------------------------------------------- #
add_subdirectory(src/egnite)

if(EGNITE_BUILD_TEST)
  add_subdirectory(tests)
endif()
# ----------------------------------------------------------------------- #
