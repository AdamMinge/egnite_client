# ----------------------------------------------------------------------- #
# -------------------------- Set CMake Version -------------------------- #
# ----------------------------------------------------------------------- #
cmake_minimum_required(VERSION 3.11)
# ----------------------------------------------------------------------- #
# -------------------- Create VEnv nad locate python -------------------- #
# ----------------------------------------------------------------------- #
if(NOT EXISTS "${CMAKE_CURRENT_BINARY_DIR}/venv")
  find_package(Python3)
  execute_process(COMMAND ${Python3_EXECUTABLE} "-m" "venv" "venv"
                  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})
endif()

set(ENV{VIRTUAL_ENV} "${CMAKE_CURRENT_BINARY_DIR}/venv")
set(Python3_FIND_VIRTUALENV FIRST)
unset(Python3_EXECUTABLE)

find_package(Python3)
# ----------------------------------------------------------------------- #
