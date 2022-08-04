# ----------------------------------------------------------------------- #
# -------------------------- Set CMake Version -------------------------- #
# ----------------------------------------------------------------------- #
cmake_minimum_required(VERSION 3.11)
# ----------------------------------------------------------------------- #
# -------------------------- Generate headers --------------------------- #
# ----------------------------------------------------------------------- #
set(source_root ${EGNITE_SOURCE_DIR}/modules/egnite/src)
set(include_root ${EGNITE_SOURCE_DIR}/modules/egnite/include/egnite)
set(resources_root ${EGNITE_SOURCE_DIR}/modules/egnite/resources)

configure_file(${include_root}/config.h.in ${include_root}/config.h)
# ----------------------------------------------------------------------- #
# -------------------------- Set files variable ------------------------- #
# ----------------------------------------------------------------------- #
set(sources
    ${source_root}/quick_egnite.cpp
    ${source_root}/quick_plugin.cpp
    ${source_root}/cmd/parser.cpp
    ${source_root}/archive/json.cpp
    ${source_root}/rest/messages.cpp
    ${source_root}/rest/headers.cpp
    ${source_root}/rest/serializer.cpp
    ${source_root}/rest/reply.cpp
    ${source_root}/rest/client.cpp
    ${source_root}/rest/authenticator.cpp)

set(includes
    ${include_root}/export.h
    ${include_root}/quick_egnite.h
    ${include_root}/quick_plugin.h
    ${include_root}/cmd/parser.h
    ${include_root}/macros/map.h
    ${include_root}/archive/json.h
    ${include_root}/archive/serializer/qt.h
    ${include_root}/archive/serializer/stl.h
    ${include_root}/archive/serializer/wrappers.h
    ${include_root}/rest/enums.h
    ${include_root}/rest/messages.h
    ${include_root}/rest/routers.h
    ${include_root}/rest/headers.h
    ${include_root}/rest/serializer.h
    ${include_root}/rest/reply.h
    ${include_root}/rest/client.h
    ${include_root}/rest/authenticator.h)

set(resources ${resources_root}/egnite.qrc)
# ----------------------------------------------------------------------- #
# ---------------------- Qt libraries configuration --------------------- #
# ----------------------------------------------------------------------- #
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)
# ----------------------------------------------------------------------- #
# ----------------------- Find external libraries ----------------------- #
# ----------------------------------------------------------------------- #
find_package(Qt6 REQUIRED COMPONENTS Widgets Qml)
find_package(Boost REQUIRED COMPONENTS serialization)
find_package(nlohmann_json REQUIRED)
# ----------------------------------------------------------------------- #
# ----------------------- Create egnite shared lib ---------------------- #
# ----------------------------------------------------------------------- #
egnite_add_module(
  egnite
  SOURCES
  ${sources}
  ${includes}
  ${resources}
  SRC_DIRECTORY
  ${source_root}
  INCLUDE_DIRECTORY
  ${include_root}/..
  DEPENDS
  Qt6::Widgets
  Qt6::Qml
  Boost::serialization
  nlohmann_json::nlohmann_json)
# ----------------------------------------------------------------------- #
