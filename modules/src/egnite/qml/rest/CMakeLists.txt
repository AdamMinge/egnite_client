# ----------------------------------------------------------------------- #
# -------------------------- Set CMake Version -------------------------- #
# ----------------------------------------------------------------------- #
cmake_minimum_required(VERSION 3.11)
# ----------------------------------------------------------------------- #
# -------------------------- Generate headers --------------------------- #
# ----------------------------------------------------------------------- #
set(source_root ${EGNITE_SOURCE_DIR}/modules/src/egnite_qml/rest)
set(include_root ${EGNITE_SOURCE_DIR}/modules/include/egnite_qml/rest)
set(resource_root ${viewer_root}/modules/resource/egnite_qml/rest)
set(qmls_root ${viewer_root}/modules/qml/egnite_qml/rest)
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
  egnite-qml-rest
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
  egnite-qml-rest
  PUBLIC $<BUILD_INTERFACE:${EGNITE_SOURCE_DIR}/egnite_qml/include/>
  INTERFACE $<INSTALL_INTERFACE:include>
  PRIVATE ${EGNITE_SOURCE_DIR}/egnite_qml/src/)
# ----------------------------------------------------------------------- #
