# ----------------------------------------------------------------------- #
# -------------------------- Set CMake Version -------------------------- #
# ----------------------------------------------------------------------- #
cmake_minimum_required(VERSION 3.11)
# ----------------------------------------------------------------------- #
# -------------------------- Set files variable ------------------------- #
# ----------------------------------------------------------------------- #
set(viewer_root ${EGNITE_SOURCE_DIR}/apps/viewer)
set(source_root ${viewer_root}/src)
set(include_root ${viewer_root}/include)
set(qmls_root ${viewer_root}/qml)
set(resources_root ${viewer_root}/resources)

set(sources ${source_root}/main.cpp)

set(includes)

set(qmls ${qmls_root}/AuthWindow.qml ${qmls_root}/LoginPage.qml
         ${qmls_root}/Main.qml ${qmls_root}/SignUpPage.qml)

set(resources ${resources_root}/conf/qtquickcontrols2.conf
              ${resources_root}/font/FontAwesome.ttf)
# ----------------------------------------------------------------------- #
# ---------------------- Qt libraries configuration --------------------- #
# ----------------------------------------------------------------------- #
set(CMAKE_AUTOMOC ON)
# ----------------------------------------------------------------------- #
# ----------------------- Find external libraries ----------------------- #
# ----------------------------------------------------------------------- #
find_package(Qt6 REQUIRED COMPONENTS Qml QuickControls2 Gui)
# ----------------------------------------------------------------------- #
# -------------------------- Create viewer app -------------------------- #
# ----------------------------------------------------------------------- #
egnite_add_qml_executable(
  viewer
  URI
  viewer
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
  Qt::Gui
  Qt::Qml
  Qt::QuickControls2
  egnite::egnite-qml-rest
  egnite::egnite-cmd)

target_include_directories(
  sniffer
  PUBLIC $<BUILD_INTERFACE:${include_root}>
  PRIVATE ${source_root})
# ----------------------------------------------------------------------- #
