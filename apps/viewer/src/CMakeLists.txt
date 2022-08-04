# ----------------------------------------------------------------------- #
# -------------------------- Set CMake Version -------------------------- #
# ----------------------------------------------------------------------- #
cmake_minimum_required(VERSION 3.11)
# ----------------------------------------------------------------------- #
# -------------------------- Set files variable ------------------------- #
# ----------------------------------------------------------------------- #
set(source_root ${EGNITE_SOURCE_DIR}/apps/viewer/src)
set(include_root ${EGNITE_SOURCE_DIR}/apps/viewer/include)
set(uis_root ${EGNITE_SOURCE_DIR}/apps/viewer/ui)
set(resources_root ${EGNITE_SOURCE_DIR}/apps/viewer/resources)

set(sources ${source_root}/main.cpp)

set(includes)

set(uis)

set(resources ${resources_root}/viewer.qrc)
# ----------------------------------------------------------------------- #
# ---------------------- Qt libraries configuration --------------------- #
# ----------------------------------------------------------------------- #
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_AUTOUIC_SEARCH_PATHS ${uis_root})
# ----------------------------------------------------------------------- #
# ----------------------- Find external libraries ----------------------- #
# ----------------------------------------------------------------------- #
find_package(Qt6 REQUIRED COMPONENTS Widgets Qml QuickControls2)
# ----------------------------------------------------------------------- #
# -------------------------- Create viewer app -------------------------- #
# ----------------------------------------------------------------------- #
egnite_add_application(
  viewer
  SOURCES
  ${sources}
  ${includes}
  ${uis}
  ${resources}
  SRC_DIRECTORY
  ${source_root}
  INCLUDE_DIRECTORY
  ${include_root}
  DEPENDS
  Qt::Widgets
  Qt::Qml
  Qt::QuickControls2
  egnite::egnite)
# ----------------------------------------------------------------------- #
