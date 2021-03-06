# -------------------------------------------------------------------------------------------------- #
# -------------------------------------- Set CMake Version ----------------------------------------- #
# -------------------------------------------------------------------------------------------------- #
cmake_minimum_required(VERSION 3.11)
# -------------------------------------------------------------------------------------------------- #
# -------------------------------------- Set files variable ---------------------------------------- #
# -------------------------------------------------------------------------------------------------- #
set(source_root ${EGNITE_SOURCE_DIR}/src/egnite/utils/cmd)
set(include_root ${EGNITE_SOURCE_DIR}/include/egnite/utils/cmd)

set(sources
        ${source_root}/parser.cpp)

set(includes
        ${include_root}/export.h
        ${include_root}/parser.h)
# -------------------------------------------------------------------------------------------------- #
# ---------------------------------- Qt libraries configuration ------------------------------------ #
# -------------------------------------------------------------------------------------------------- #
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)
# -------------------------------------------------------------------------------------------------- #
# ----------------------------------- Find external libraries -------------------------------------- #
# -------------------------------------------------------------------------------------------------- #
find_package(Qt6 REQUIRED COMPONENTS Widgets)
# -------------------------------------------------------------------------------------------------- #
# ------------------------------------ Create utils shared lib ------------------------------------- #
# -------------------------------------------------------------------------------------------------- #
egnite_add_utils(cmd
        SOURCES ${sources} ${includes}
        DEPENDS Qt6::Widgets)
# -------------------------------------------------------------------------------------------------- #
