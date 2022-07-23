# -------------------------------------------------------------------------------------------------- #
# -------------------------------------- Set CMake Version ----------------------------------------- #
# -------------------------------------------------------------------------------------------------- #
cmake_minimum_required(VERSION 3.11)
# -------------------------------------------------------------------------------------------------- #
# -------------------------------------- Set files variable ---------------------------------------- #
# -------------------------------------------------------------------------------------------------- #
set(source_root ${EGNITE_SOURCE_DIR}/src/egnite/egnite)
set(include_root ${EGNITE_SOURCE_DIR}/include/egnite/egnite)
set(resources_root ${EGNITE_SOURCE_DIR}/resources/egnite)

set(sources
        ${source_root}/quick_egnite.cpp
        ${source_root}/quick_plugin.cpp

        ${source_root}/web/messages.cpp
        ${source_root}/web/headers.cpp
        ${source_root}/web/serializer.cpp
        ${source_root}/web/client.cpp
        ${source_root}/web/authenticator.cpp)

set(includes
        ${include_root}/export.h
        ${include_root}/quick_egnite.h
        ${include_root}/quick_plugin.h

        ${include_root}/macros/map.h

        ${include_root}/web/messages.h
        ${include_root}/web/routers.h
        ${include_root}/web/headers.h
        ${include_root}/web/serializer.h
        ${include_root}/web/client.h
        ${include_root}/web/authenticator.h)

set(resources
        ${resources_root}/egnite.qrc)
# -------------------------------------------------------------------------------------------------- #
# ---------------------------------- Qt libraries configuration ------------------------------------ #
# -------------------------------------------------------------------------------------------------- #
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)
# -------------------------------------------------------------------------------------------------- #
# ----------------------------------- Find external libraries -------------------------------------- #
# -------------------------------------------------------------------------------------------------- #
find_package(Qt6 REQUIRED COMPONENTS Widgets Qml)
find_package(Boost REQUIRED COMPONENTS serialization)
# -------------------------------------------------------------------------------------------------- #
# ------------------------------------ Create utils shared lib ------------------------------------- #
# -------------------------------------------------------------------------------------------------- #
egnite_add_module(egnite
        SOURCES ${sources} ${includes} ${resources}
        DEPENDS
        Qt6::Widgets
        Qt6::Qml
        Boost::serialization
        utils::boost_archive)
# -------------------------------------------------------------------------------------------------- #
