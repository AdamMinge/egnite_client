# ----------------------------------------------------------------------- #
# -------------------------- Set CMake Version -------------------------- #
# ----------------------------------------------------------------------- #
cmake_minimum_required(VERSION 3.11)
# ----------------------------------------------------------------------- #
# -------------------------- Generate headers --------------------------- #
# ----------------------------------------------------------------------- #
set(source_root ${EGNITE_SOURCE_DIR}/modules/src/egnite/rest)
set(include_root ${EGNITE_SOURCE_DIR}/modules/include/egnite/rest)
# ----------------------------------------------------------------------- #
# -------------------------- Set files variable ------------------------- #
# ----------------------------------------------------------------------- #
set(sources
    ${source_root}/api.cpp
    ${source_root}/client.cpp
    ${source_root}/reply.cpp
    ${source_root}/raw_reply.cpp
    ${source_root}/wrapped_reply.cpp
    ${source_root}/reply_decorator.cpp
    ${source_root}/reply_decorator_factory.cpp
    ${source_root}/request_builder.cpp
    ${source_root}/data_serializer.cpp
    ${source_root}/detail/api_p.h
    ${source_root}/detail/client_p.h
    ${source_root}/detail/raw_reply_p.h
    ${source_root}/detail/wrapped_reply_p.h
    ${source_root}/detail/reply_decorator_p.h
    ${source_root}/detail/request_builder_p.h)

set(includes
    ${include_root}/export.h
    ${include_root}/global.h
    ${include_root}/api.h
    ${include_root}/client.h
    ${include_root}/reply.h
    ${include_root}/raw_reply.h
    ${include_root}/wrapped_reply.h
    ${include_root}/generic_reply.h
    ${include_root}/logger_reply.h
    ${include_root}/reply_decorator.h
    ${include_root}/reply_decorator_factory.h
    ${include_root}/request_builder.h
    ${include_root}/data_serializer.h)
# ----------------------------------------------------------------------- #
# ---------------------- Qt libraries configuration --------------------- #
# ----------------------------------------------------------------------- #
set(CMAKE_AUTOMOC ON)
# ----------------------------------------------------------------------- #
# ----------------------- Find external libraries ----------------------- #
# ----------------------------------------------------------------------- #
find_package(Qt6 REQUIRED COMPONENTS Core Network)
# ----------------------------------------------------------------------- #
# ----------------------- Create egnite shared lib ---------------------- #
# ----------------------------------------------------------------------- #
egnite_add_module(
  egnite-rest
  SOURCES
  ${sources}
  ${includes}
  DEPENDENCIES
  Qt6::Core
  Qt6::Network
  Qt6::CorePrivate
  egnite::egnite-core
  egnite::egnite-serializer)

target_include_directories(
  egnite-rest
  PUBLIC $<BUILD_INTERFACE:${EGNITE_SOURCE_DIR}/modules/include/>
  INTERFACE $<INSTALL_INTERFACE:include>
  PRIVATE ${EGNITE_SOURCE_DIR}/modules/src/)
# ----------------------------------------------------------------------- #
