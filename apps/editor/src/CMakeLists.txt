# ----------------------------------------------------------------------- #
# -------------------------- Set CMake Version -------------------------- #
# ----------------------------------------------------------------------- #
cmake_minimum_required(VERSION 3.11)
# ----------------------------------------------------------------------- #
# -------------------------- Generate headers --------------------------- #
# ----------------------------------------------------------------------- #
set(editor_root ${EGNITE_SOURCE_DIR}/apps/editor)
set(source_root ${editor_root}/src)
set(include_root ${editor_root}/include)
set(qmls_root ${editor_root}/qml)
set(scripts_root ${editor_root}/scripts)
set(resources_root ${editor_root}/resources)
# ----------------------------------------------------------------------- #
# -------------------------- Set files variable ------------------------- #
# ----------------------------------------------------------------------- #
set(sources ${source_root}/main.cpp)

set(includes)

set(qmls
    ${qmls_root}/MainWindow.qml
    ${qmls_root}/MainPage.qml
    ${qmls_root}/AuthPage.qml
    ${qmls_root}/Pages/auth/LoginPage.qml
    ${qmls_root}/Pages/auth/SignUpPage.qml
    ${qmls_root}/Pages/home/HomePage.qml
    ${qmls_root}/Pages/notifications/NotificationsPage.qml
    ${qmls_root}/Pages/profile/ProfilePage.qml
    ${qmls_root}/Pages/plugins/PluginsPage.qml
    ${qmls_root}/Pages/settings/SettingsPage.qml)

set(scripts)

set(resources ${resources_root}/qtquickcontrols2.conf)
# ----------------------------------------------------------------------- #
# ---------------------- Qt libraries configuration --------------------- #
# ----------------------------------------------------------------------- #
set(CMAKE_AUTOMOC ON)
# ----------------------------------------------------------------------- #
# ----------------------- Find external libraries ----------------------- #
# ----------------------------------------------------------------------- #
find_package(Qt6 REQUIRED COMPONENTS Qml QuickControls2 Gui)
# ----------------------------------------------------------------------- #
# -------------------------- Create editor app -------------------------- #
# ----------------------------------------------------------------------- #
egnite_add_qml_executable(
  editor
  URI
  editor
  VERSION
  ${EGNITE_VERSION_MAJOR}.${EGNITE_VERSION_MINOR}
  SOURCES
  ${sources}
  ${includes}
  QML_DIR
  ${qmls_root}
  SCRIPTS_DIR
  ${scripts_root}
  RESOURCES_DIR
  ${resources_root}
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
  egnite::egnite-cmd)

target_include_directories(
  editor
  PUBLIC $<BUILD_INTERFACE:${include_root}>
  PRIVATE ${source_root})
# ----------------------------------------------------------------------- #
