# ----------------------------------------------------------------------- #
# -------------------------- Set CMake Version -------------------------- #
# ----------------------------------------------------------------------- #
cmake_minimum_required(VERSION 3.11)
# ----------------------------------------------------------------------- #
# -------------------------- Generate headers --------------------------- #
# ----------------------------------------------------------------------- #
set(source_root ${EGNITE_SOURCE_DIR}/imports/src/egnite/rest)
set(include_root ${EGNITE_SOURCE_DIR}/imports/include/egnite/rest)
set(resource_root ${viewer_root}/imports/resource/egnite/rest)
set(qmls_root ${viewer_root}/imports/qml/egnite/rest)
# ----------------------------------------------------------------------- #
# -------------------------- Set files variable ------------------------- #
# ----------------------------------------------------------------------- #
set(sources)

set(includes)

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
  egnite-rest-import
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
  egnite-rest-import
  PUBLIC $<BUILD_INTERFACE:${EGNITE_SOURCE_DIR}/imports/include/>
  INTERFACE $<INSTALL_INTERFACE:include>
  PRIVATE ${EGNITE_SOURCE_DIR}/imports/src/)
# ----------------------------------------------------------------------- #
