# ----------------------------------------------------------------------- #
# -------------------------- Set CMake Version -------------------------- #
# ----------------------------------------------------------------------- #
cmake_minimum_required(VERSION 3.11)
# ----------------------------------------------------------------------- #
# -------------------------- Generate headers --------------------------- #
# ----------------------------------------------------------------------- #
set(viewer_root ${EGNITE_SOURCE_DIR}/apps/viewer)
set(source_root ${viewer_root}/src)
set(include_root ${viewer_root}/include)
set(qmls_root ${viewer_root}/qml)
set(scripts_root ${viewer_root}/scripts)
set(resources_root ${viewer_root}/resources)
# ----------------------------------------------------------------------- #
# -------------------------- Set files variable ------------------------- #
# ----------------------------------------------------------------------- #
set(sources ${source_root}/main.cpp)

set(includes)

set(qmls
    ${qmls_root}/MainWindow.qml
    ${qmls_root}/MainPage.qml
    ${qmls_root}/AuthPage.qml
    ${qmls_root}/Pages/home/HomePage.qml
    ${qmls_root}/Pages/messages/MessagesPage.qml
    ${qmls_root}/Pages/notifications/NotificationsPage.qml
    ${qmls_root}/Pages/profile/ProfilePage.qml
    ${qmls_root}/Pages/setting/SettingPage.qml)

set(scripts)

set(resources
    ${resources_root}/qtquickcontrols2.conf
    ${resources_root}/font/FontAwesome.ttf
    ${resources_root}/icon/home.svg
    ${resources_root}/icon/messages.svg
    ${resources_root}/icon/notifications.svg
    ${resources_root}/icon/profile.svg
    ${resources_root}/icon/setting.svg)
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
  egnite::egnite-cmd
  egnite::egnite-rest)

target_include_directories(
  viewer
  PUBLIC $<BUILD_INTERFACE:${include_root}>
  PRIVATE ${source_root})
# ----------------------------------------------------------------------- #
