# ----------------------------------------------------------------------- #
# -------------------------- Set CMake Version -------------------------- #
# ----------------------------------------------------------------------- #
cmake_minimum_required(VERSION 3.11)
# ----------------------------------------------------------------------- #
# -------------------------- Generate headers --------------------------- #
# ----------------------------------------------------------------------- #
set(source_root ${EGNITE_SOURCE_DIR}/imports/src/egnite/rest)
set(include_root ${EGNITE_SOURCE_DIR}/imports/include/egnite/rest)
# ----------------------------------------------------------------------- #
# -------------------------- Set files variable ------------------------- #
# ----------------------------------------------------------------------- #
set(sources
    ${source_root}/client.cpp ${source_root}/api.cpp ${source_root}/reply.cpp
    ${source_root}/reply_decorator.cpp ${source_root}/reply_factory.cpp)

set(includes
    ${include_root}/client.h ${include_root}/api.h ${include_root}/reply.h
    ${include_root}/reply_decorator.h ${include_root}/reply_factory.h)

set(resources)

set(qmls)
# ----------------------------------------------------------------------- #
# ---------------------- Qt libraries configuration --------------------- #
# ----------------------------------------------------------------------- #
set(CMAKE_AUTOMOC ON)
# ----------------------------------------------------------------------- #
# ----------------------- Find external libraries ----------------------- #
# ----------------------------------------------------------------------- #
find_package(Qt6 REQUIRED COMPONENTS Quick)
# ----------------------------------------------------------------------- #
# ----------------------- Create egnite shared lib ---------------------- #
# ----------------------------------------------------------------------- #
egnite_add_qml_module(
  egnite-rest-qml
  URI
  egnite.rest
  VERSION
  ${EGNITE_VERSION_MAJOR}.${EGNITE_VERSION_MINOR}
  SOURCES
  ${sources}
  ${includes}
  QML_FILES
  ${qmls}
  RESOURCES
  ${resources}
  DEPENDENCIES
  egnite::egnite-rest)

target_include_directories(
  egnite-rest-qml
  PUBLIC $<BUILD_INTERFACE:${EGNITE_SOURCE_DIR}/imports/include/egnite/rest>
  PRIVATE ${EGNITE_SOURCE_DIR}/imports/src/egnite/rest)
# ----------------------------------------------------------------------- #
