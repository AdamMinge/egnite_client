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
set(scripts_root ${viewer_root}/scripts)
set(resources_root ${sniffer_root}/resources)
# ----------------------------------------------------------------------- #
# -------------------------- Set files variable ------------------------- #
# ----------------------------------------------------------------------- #
set(sources ${source_root}/main.cpp)

set(includes)

set(qmls
    ${qmls_root}/MainWindow.qml
    ${qmls_root}/MainPage.qml
    ${qmls_root}/AuthPage.qml
    ${qmls_root}/MainMenu/MainMenu.qml
    ${qmls_root}/Pages/Auth/LoginPage.qml
    ${qmls_root}/Pages/Auth/SignUpPage.qml
    ${qmls_root}/Framework/AnimatedPagesView.qml
    ${qmls_root}/Framework/AnimatedHideableItem.qml)

set(scripts)

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
  SCRIPTS_DIR
  ${scripts_root}
  QML_FILES
  ${qmls}
  SCRIPTS
  ${scripts}
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
