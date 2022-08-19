# ----------------------------------------------------------------------- #
# -------------------------- Set CMake Version -------------------------- #
# ----------------------------------------------------------------------- #
cmake_minimum_required(VERSION 3.11)
# ----------------------------------------------------------------------- #
# -------------------------- Generate headers --------------------------- #
# ----------------------------------------------------------------------- #
set(source_root ${EGNITE_SOURCE_DIR}/modules/egnite/src/egnite)
set(include_root ${EGNITE_SOURCE_DIR}/modules/egnite/include/egnite)
set(resources_root ${EGNITE_SOURCE_DIR}/modules/egnite/resources)

configure_file(${include_root}/config.h.in ${include_root}/config.h)
# ----------------------------------------------------------------------- #
# -------------------------- Set files variable ------------------------- #
# ----------------------------------------------------------------------- #
set(sources
    ${source_root}/cmd/parser.cpp
    ${source_root}/rest/rest_api.cpp
    ${source_root}/rest/rest_auth.cpp
    ${source_root}/rest/rest_client.cpp
    ${source_root}/rest/rest_reply.cpp
    ${source_root}/rest/rest_request_builder.cpp
    ${source_root}/rest/detail/rest_api_p.h
    ${source_root}/rest/detail/rest_auth_p.h
    ${source_root}/rest/detail/rest_client_p.h
    ${source_root}/rest/detail/rest_reply_p.h
    ${source_root}/rest/detail/rest_request_builder_p.h)

set(includes
    ${include_root}/config.h
    ${include_root}/export.h
    ${include_root}/cmd/parser.h
    ${include_root}/utils/binder.h
    ${include_root}/rest/rest_api.h
    ${include_root}/rest/rest_auth.h
    ${include_root}/rest/rest_client.h
    ${include_root}/rest/rest_reply.h
    ${include_root}/rest/rest_request_builder.h)

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
find_package(Qt6 REQUIRED COMPONENTS Widgets Network)
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
  ${source_root}/..
  INCLUDE_DIRECTORY
  ${include_root}/..
  DEPENDS
  Qt6::Widgets
  Qt6::Network
  Qt6::CorePrivate)
# ----------------------------------------------------------------------- #
