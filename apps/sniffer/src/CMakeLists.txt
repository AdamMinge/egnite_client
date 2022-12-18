# ----------------------------------------------------------------------- #
# -------------------------- Set CMake Version -------------------------- #
# ----------------------------------------------------------------------- #
cmake_minimum_required(VERSION 3.11)
# ----------------------------------------------------------------------- #
# -------------------------- Generate headers --------------------------- #
# ----------------------------------------------------------------------- #
set(sniffer_root ${EGNITE_SOURCE_DIR}/apps/sniffer)
set(source_root ${sniffer_root}/src)
set(include_root ${sniffer_root}/include)
set(qmls_root ${sniffer_root}/qml)
set(resources_root ${sniffer_root}/resources)
# ----------------------------------------------------------------------- #
# -------------------------- Set files variable ------------------------- #
# ----------------------------------------------------------------------- #
set(sources ${source_root}/main.cpp)

set(includes)

set(qmls
    ${qmls_root}/MainWindow.qml ${qmls_root}/MainMenu.qml
    ${qmls_root}/MainStack.qml ${qmls_root}/Auth/Authentication.qml
    ${qmls_root}/Auth/LoginPage.qml ${qmls_root}/Auth/SignUpPage.qml)

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
# -------------------------- Create sniffer app ------------------------- #
# ----------------------------------------------------------------------- #
egnite_add_qml_executable(
  sniffer
  URI
  sniffer
  VERSION
  ${EGNITE_VERSION_MAJOR}.${EGNITE_VERSION_MINOR}
  SOURCES
  ${sources}
  ${includes}
  QML_DIR
  ${qmls_root}
  QML_FILES
  ${qmls}
  RESOURCES
  ${resources}
  DEPENDENCIES
  Qt::Gui
  Qt::Qml
  Qt::QuickControls2
  egnite::egnite-cmd
  egnite::egnite-rest)

target_include_directories(
  sniffer
  PUBLIC $<BUILD_INTERFACE:${include_root}>
  PRIVATE ${source_root})
# ----------------------------------------------------------------------- #
