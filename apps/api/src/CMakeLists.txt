# ----------------------------------------------------------------------- #
# -------------------------- Set CMake Version -------------------------- #
# ----------------------------------------------------------------------- #
cmake_minimum_required(VERSION 3.11)
# ----------------------------------------------------------------------- #
# -------------------------- Generate headers --------------------------- #
# ----------------------------------------------------------------------- #
set(api_root ${EGNITE_SOURCE_DIR}/apps/api)
set(source_root ${api_root}/src)
set(include_root ${api_root}/include)
set(schema_root ${api_root}/schema)
set(qmls_root ${api_root}/qml)
set(resources_root ${api_root}/resources)
# ----------------------------------------------------------------------- #
# -------------------------- Set files variable ------------------------- #
# ----------------------------------------------------------------------- #
set(sources)

set(includes)

set(schemas ${schema_root}/client.xml ${schema_root}/api.xml
            ${schema_root}/model.xml)

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
  api
  URI
  egnite.api
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
  api
  PUBLIC $<BUILD_INTERFACE:${include_root}>
  PRIVATE ${source_root})

egnite_add_schema_converter(api INTERFACE qt SOURCES ${schemas})
# ----------------------------------------------------------------------- #
