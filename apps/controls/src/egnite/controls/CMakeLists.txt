# ----------------------------------------------------------------------- #
# -------------------------- Set CMake Version -------------------------- #
# ----------------------------------------------------------------------- #
cmake_minimum_required(VERSION 3.11)
# ----------------------------------------------------------------------- #
# -------------------------- Generate headers --------------------------- #
# ----------------------------------------------------------------------- #
set(controls_root ${EGNITE_SOURCE_DIR}/apps/controls)
set(qmls_root ${controls_root}/qml)
set(scripts_root ${controls_root}/scripts)
set(resources_root ${controls_root}/resources)
# ----------------------------------------------------------------------- #
# -------------------------- Set files variable ------------------------- #
# ----------------------------------------------------------------------- #
set(qmls
    ${qmls_root}/AnimatedPagesView/AnimatedPagesView.qml
    ${qmls_root}/AnimatedPagesView/AnimatedHideableItem.qml
    ${qmls_root}/NavigationBar/NavigationBar.qml
    ${qmls_root}/NavigationBar/NavigationItem.qml)

set(qmls_singletons ${qmls_root}/Globals/Fonts.qml)

set(scripts)

set(resources ${resources_root}/font/icofont.ttf)
# ----------------------------------------------------------------------- #
# ---------------------- Qt libraries configuration --------------------- #
# ----------------------------------------------------------------------- #
set(CMAKE_AUTOMOC ON)
# ----------------------------------------------------------------------- #
# ----------------------- Find external libraries ----------------------- #
# ----------------------------------------------------------------------- #
find_package(Qt6 REQUIRED COMPONENTS Qml QuickControls2)
# ----------------------------------------------------------------------- #
# ----------------------- Create egnite shared lib ---------------------- #
# ----------------------------------------------------------------------- #
egnite_add_qml_module(
  egnite-controls-qml
  URI
  egnite.controls
  VERSION
  ${EGNITE_VERSION_MAJOR}.${EGNITE_VERSION_MINOR}
  QML_DIR
  ${qmls_root}
  SCRIPTS_DIR
  ${scripts_root}
  RESOURCES_DIR
  ${resources_root}
  QML_FILES
  ${qmls}
  QML_SINGLETON_FILES
  ${qmls_singletons}
  SCRIPTS
  ${scripts}
  RESOURCES
  ${resources}
  DEPENDENCIES
  Qt::Qml
  Qt::QuickControls2)
# ----------------------------------------------------------------------- #
