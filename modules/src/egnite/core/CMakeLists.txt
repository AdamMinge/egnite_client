# ----------------------------------------------------------------------- #
# -------------------------- Set CMake Version -------------------------- #
# ----------------------------------------------------------------------- #
cmake_minimum_required(VERSION 3.11)
# ----------------------------------------------------------------------- #
# -------------------------- Generate headers --------------------------- #
# ----------------------------------------------------------------------- #
set(source_root ${EGNITE_SOURCE_DIR}/modules/src/egnite/core)
set(include_root ${EGNITE_SOURCE_DIR}/modules/include/egnite/core)
# ----------------------------------------------------------------------- #
# -------------------------- Set files variable ------------------------- #
# ----------------------------------------------------------------------- #
set(sources
    ${source_root}/plugins/plugin_manager.cpp
    ${source_root}/plugins/plugin_interface.cpp
    ${source_root}/plugins/detail/plugin_manager_p.h
    ${source_root}/plugins/detail/plugin_interface_p.h)

set(includes
    ${include_root}/export.h
    ${include_root}/utils/binder.h
    ${include_root}/utils/capture.h
    ${include_root}/macros/map.h
    ${include_root}/macros/binder.h
    ${include_root}/plugins/plugin_manager.h
    ${include_root}/plugins/plugin_interface.h
    ${include_root}/plugins/plugin_listener.h)
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
  egnite-core
  SOURCES
  ${sources}
  ${includes}
  DEPENDENCIES
  Qt6::Core
  Qt6::CorePrivate)

target_include_directories(
  egnite-core
  PUBLIC $<BUILD_INTERFACE:${EGNITE_SOURCE_DIR}/modules/include/>
  INTERFACE $<INSTALL_INTERFACE:include>
  PRIVATE ${EGNITE_SOURCE_DIR}/modules/src/)
# ----------------------------------------------------------------------- #
