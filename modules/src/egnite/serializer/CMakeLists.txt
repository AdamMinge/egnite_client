# ----------------------------------------------------------------------- #
# -------------------------- Set CMake Version -------------------------- #
# ----------------------------------------------------------------------- #
cmake_minimum_required(VERSION 3.11)
# ----------------------------------------------------------------------- #
# -------------------------- Generate headers --------------------------- #
# ----------------------------------------------------------------------- #
set(source_root ${EGNITE_SOURCE_DIR}/modules/src/egnite/serializer)
set(include_root ${EGNITE_SOURCE_DIR}/modules/include/egnite/serializer)
# ----------------------------------------------------------------------- #
# -------------------------- Set files variable ------------------------- #
# ----------------------------------------------------------------------- #
set(sources
    ${source_root}/json_archive.cpp ${source_root}/cbor_archive.cpp
    ${source_root}/detail/json_archive_p.h
    ${source_root}/detail/cbor_archive_p.h)

set(includes ${include_root}/export.h ${include_root}/json_archive.h
             ${include_root}/cbor_archive.h)
# ----------------------------------------------------------------------- #
# ---------------------- Qt libraries configuration --------------------- #
# ----------------------------------------------------------------------- #
set(CMAKE_AUTOMOC ON)
# ----------------------------------------------------------------------- #
# ----------------------- Find external libraries ----------------------- #
# ----------------------------------------------------------------------- #
find_package(Qt6 REQUIRED COMPONENTS Core)
find_package(Boost REQUIRED COMPONENTS serialization)
# ----------------------------------------------------------------------- #
# ----------------------- Create egnite shared lib ---------------------- #
# ----------------------------------------------------------------------- #
egnite_add_module(
  egnite-serializer
  SOURCES
  ${sources}
  ${includes}
  DEPENDENCIES
  Qt6::Core
  Qt6::CorePrivate
  Boost::serialization
  egnite::egnite-core)

target_include_directories(
  egnite-serializer
  PUBLIC $<BUILD_INTERFACE:${EGNITE_SOURCE_DIR}/modules/include/>
  INTERFACE $<INSTALL_INTERFACE:include>
  PRIVATE ${EGNITE_SOURCE_DIR}/modules/src/)
# ----------------------------------------------------------------------- #
