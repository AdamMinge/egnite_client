# ----------------------------------------------------------------------- #
# -------------------------- Set CMake Version -------------------------- #
# ----------------------------------------------------------------------- #
cmake_minimum_required(VERSION 3.11)
# ----------------------------------------------------------------------- #
# -------------------------- Generate headers --------------------------- #
# ----------------------------------------------------------------------- #
set(source_root ${EGNITE_SOURCE_DIR}/modules/src/egnite/cmd)
set(include_root ${EGNITE_SOURCE_DIR}/modules/include/egnite/cmd)
# ----------------------------------------------------------------------- #
# -------------------------- Set files variable ------------------------- #
# ----------------------------------------------------------------------- #
set(sources ${source_root}/parser.cpp ${source_root}/detail/parser_p.h)

set(includes ${include_root}/export.h ${include_root}/parser.h)
# ----------------------------------------------------------------------- #
# ---------------------- Qt libraries configuration --------------------- #
# ----------------------------------------------------------------------- #
set(CMAKE_AUTOMOC ON)
# ----------------------------------------------------------------------- #
# ----------------------- Find external libraries ----------------------- #
# ----------------------------------------------------------------------- #
find_package(Qt6 REQUIRED COMPONENTS Core)
# ----------------------------------------------------------------------- #
# ----------------------- Create egnite shared lib ---------------------- #
# ----------------------------------------------------------------------- #
egnite_add_module(
  egnite-cmd
  SOURCES
  ${sources}
  ${includes}
  DEPENDENCIES
  Qt6::Core
  Qt6::CorePrivate)

target_include_directories(
  egnite-cmd
  PUBLIC $<BUILD_INTERFACE:${EGNITE_SOURCE_DIR}/modules/include/>
  INTERFACE $<INSTALL_INTERFACE:include>
  PRIVATE ${EGNITE_SOURCE_DIR}/modules/src/)
# ----------------------------------------------------------------------- #
