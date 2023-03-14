# ----------------------------------------------------------------------- #
# -------------------------- Set CMake Version -------------------------- #
# ----------------------------------------------------------------------- #
cmake_minimum_required(VERSION 3.11)
# ----------------------------------------------------------------------- #
# -------------------------- Generate headers --------------------------- #
# ----------------------------------------------------------------------- #
set(flow_root ${EGNITE_SOURCE_DIR}/apps/flow)
set(source_root ${flow_root}/src/egnite/flow)
set(include_root ${flow_root}/include/egnite/flow)
set(resources_root ${flow_root}/resources)
# ----------------------------------------------------------------------- #
# -------------------------- Set files variable ------------------------- #
# ----------------------------------------------------------------------- #
set(sources ${source_root}/flow.cpp)

set(includes ${include_root}/flow.h)
# ----------------------------------------------------------------------- #
# ---------------------- Qt libraries configuration --------------------- #
# ----------------------------------------------------------------------- #
set(CMAKE_AUTOMOC ON)
# ----------------------------------------------------------------------- #
# ----------------------- Find external libraries ----------------------- #
# ----------------------------------------------------------------------- #
find_package(Qt6 REQUIRED COMPONENTS Qml QuickControls2)
find_package(QuickQanava REQUIRED)
# ----------------------------------------------------------------------- #
# ----------------------- Create egnite shared lib ---------------------- #
# ----------------------------------------------------------------------- #
egnite_add_qml_module(
  egnite-flow-qml
  URI
  egnite.flow
  VERSION
  ${EGNITE_VERSION_MAJOR}.${EGNITE_VERSION_MINOR}
  SOURCES
  ${sources}
  ${includes}
  DEPENDENCIES
  Qt::Qml
  Qt::QuickControls2
  QuickQanava)

target_include_directories(
  egnite-flow-qml
  PUBLIC $<BUILD_INTERFACE:${EGNITE_SOURCE_DIR}/apps/flow/include/egnite/flow>
  PRIVATE ${EGNITE_SOURCE_DIR}/apps/flow/src/egnite/flow)
# ----------------------------------------------------------------------- #
