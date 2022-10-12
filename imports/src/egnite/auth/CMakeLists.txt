# ----------------------------------------------------------------------- #
# -------------------------- Set CMake Version -------------------------- #
# ----------------------------------------------------------------------- #
cmake_minimum_required(VERSION 3.11)
# ----------------------------------------------------------------------- #
# -------------------------- Generate headers --------------------------- #
# ----------------------------------------------------------------------- #
set(source_root ${EGNITE_SOURCE_DIR}/imports/src/egnite/auth)
set(include_root ${EGNITE_SOURCE_DIR}/imports/include/egnite/auth)
# ----------------------------------------------------------------------- #
# -------------------------- Set files variable ------------------------- #
# ----------------------------------------------------------------------- #
set(sources ${source_root}/authenticator.cpp ${source_root}/reply_factory.cpp)

set(includes ${include_root}/authenticator.h ${include_root}/reply_factory.h)

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
  egnite-auth-qml
  URI
  egnite.auth
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
  egnite::egnite-auth)

target_include_directories(
  egnite-auth-qml
  PUBLIC $<BUILD_INTERFACE:${EGNITE_SOURCE_DIR}/imports/include/egnite/auth>
  PRIVATE ${EGNITE_SOURCE_DIR}/imports/src/egnite/auth)
# ----------------------------------------------------------------------- #
