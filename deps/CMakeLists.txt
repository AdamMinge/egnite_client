# ----------------------------------------------------------------------- #
# -------------------- Set CMake version and project -------------------- #
# ----------------------------------------------------------------------- #
cmake_minimum_required(VERSION 3.11)
# ----------------------------------------------------------------------- #
# -------------------------- Setup Dependency --------------------------- #
# ----------------------------------------------------------------------- #
macro(egnite_setup_dependency dependency)
  if(EGNITE_BUILD_DEPS)
    configure_file(external_${dependency}.txt.in ${dependency}/CMakeLists.txt
                   @ONLY)

    execute_process(
      COMMAND ${CMAKE_COMMAND} -G "${CMAKE_GENERATOR}" .
      RESULT_VARIABLE result
      WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/${dependency})
    if(result)
      message(FATAL_ERROR "CMake step for ${dependency} failed: ${result}")
    endif()

    execute_process(
      COMMAND ${CMAKE_COMMAND} --build .
      RESULT_VARIABLE result
      WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/${dependency})
    if(result)
      message(FATAL_ERROR "Build step for ${dependency} failed: ${result}")
    endif()
  endif()
  set(CMAKE_PREFIX_PATH
      ${CMAKE_CURRENT_BINARY_DIR}/${dependency}/src/${dependency}-install
      ${CMAKE_PREFIX_PATH})
  set(CMAKE_PREFIX_PATH
      ${CMAKE_PREFIX_PATH}
      PARENT_SCOPE)
endmacro()

egnite_setup_dependency(gtest)
egnite_setup_dependency(nlohmann_json)
egnite_setup_dependency(boost)
egnite_setup_dependency(qt)
# ----------------------------------------------------------------------- #
