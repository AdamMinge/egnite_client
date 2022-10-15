# ----------------------------------------------------------------------- #
# ----------- Define a function that helps add egnite qml module -------- #
# ----------------------------------------------------------------------- #
function(egnite_add_qml_module target)

  cmake_parse_arguments(
    THIS "" "URI;VERSION"
    "SOURCES;QML_FILES;RESOURCES;QML_DEPENDENCIES;DEPENDENCIES" ${ARGN})

  if(NOT "${THIS_UNPARSED_ARGUMENTS}" STREQUAL "")
    message(
      FATAL_ERROR
        "Extra unparsed arguments when calling egnite_add_qml_module: ${THIS_UNPARSED_ARGUMENTS}"
    )
  endif()

  _egnite_add_qt_resource_alias(RESOURCES ${THIS_QML_FILES})
  _egnite_add_qt_resource_alias(RESOURCES ${THIS_RESOURCES})

  qt_add_qml_module(
    ${target}
    URI
    ${THIS_URI}
    VERSION
    ${THIS_VERSION}
    QML_FILES
    ${THIS_QML_FILES}
    SOURCES
    ${THIS_SOURCES}
    RESOURCES
    ${THIS_RESOURCES}
    DEPENDENCIES
    ${THIS_QML_DEPENDENCIES})
  add_library(egnite::${target} ALIAS ${target})

  _egnite_config_module(${target} egniteQmlConfigExport)

  if(THIS_DEPENDENCIES)
    target_link_libraries(${target} PUBLIC ${THIS_DEPENDENCIES})
  endif()

endfunction()
# ----------------------------------------------------------------------- #
# ------------- Define a function that helps add egnite module ---------- #
# ----------------------------------------------------------------------- #
function(egnite_add_module target)

  cmake_parse_arguments(THIS "" "" "SOURCES;DEPENDENCIES" ${ARGN})

  if(NOT "${THIS_UNPARSED_ARGUMENTS}" STREQUAL "")
    message(
      FATAL_ERROR
        "Extra unparsed arguments when calling egnite_add_module: ${THIS_UNPARSED_ARGUMENTS}"
    )
  endif()

  add_library(${target} ${THIS_SOURCES})
  add_library(egnite::${target} ALIAS ${target})

  _egnite_config_module(${target} egniteConfigExport)

  if(THIS_DEPENDENCIES)
    target_link_libraries(${target} PUBLIC ${THIS_DEPENDENCIES})
  endif()

endfunction()
# ----------------------------------------------------------------------- #
# -------------- Define a function that helps export egnite ------------- #
# ----------------------------------------------------------------------- #
function(egnite_export_modules)

  if(BUILD_SHARED_LIBS)
    set(config_name "shared")
  else()
    set(config_name "static")
  endif()

  set(current_dir "${EGNITE_SOURCE_DIR}/cmake")
  set(targets_config_filename "egnite-${config_name}-targets.cmake")
  set(config_package_location ${CMAKE_INSTALL_LIBDIR}/cmake/egnite)

  include(CMakePackageConfigHelpers)
  write_basic_package_version_file(
    "${CMAKE_CURRENT_BINARY_DIR}/egnite-config-version.cmake"
    VERSION ${EGNITE_VERSION_MAJOR}.${EGNITE_VERSION_MINOR}
    COMPATIBILITY SameMajorVersion)

  export(EXPORT egniteConfigExport
         FILE "${CMAKE_CURRENT_BINARY_DIR}/${targets_config_filename}")

  configure_package_config_file(
    "${current_dir}/egnite-config.cmake.in"
    "${CMAKE_CURRENT_BINARY_DIR}/egnite-config.cmake"
    INSTALL_DESTINATION "${config_package_location}")
  configure_package_config_file(
    "${current_dir}/egnite-config-dependencies.cmake.in"
    "${CMAKE_CURRENT_BINARY_DIR}/egnite-config-dependencies.cmake"
    INSTALL_DESTINATION "${config_package_location}")

  install(
    EXPORT egniteConfigExport
    FILE ${targets_config_filename}
    DESTINATION ${config_package_location})

  install(
    FILES ${CMAKE_CURRENT_BINARY_DIR}/egnite-config.cmake
          ${CMAKE_CURRENT_BINARY_DIR}/egnite-config-dependencies.cmake
          ${CMAKE_CURRENT_BINARY_DIR}/egnite-config-version.cmake
    DESTINATION ${config_package_location})

  install(
    DIRECTORY ${EGNITE_SOURCE_DIR}/modules/include
    DESTINATION .
    COMPONENT devel
    FILES_MATCHING
    PATTERN "*.hpp"
    PATTERN "*.inl"
    PATTERN "*.h")

endfunction()
# ----------------------------------------------------------------------- #
# ----------- Define a function that helps add qml application ---------- #
# ----------------------------------------------------------------------- #
function(egnite_add_qml_executable target)

  cmake_parse_arguments(
    THIS "" "URI;VERSION"
    "SOURCES;QML_FILES;RESOURCES;QML_DEPENDENCIES;DEPENDENCIES" ${ARGN})

  if(NOT "${THIS_UNPARSED_ARGUMENTS}" STREQUAL "")
    message(
      FATAL_ERROR
        "Extra unparsed arguments when calling egnite_add_qml_executable: ${THIS_UNPARSED_ARGUMENTS}"
    )
  endif()

  _egnite_add_qt_resource_alias(RESOURCES ${THIS_QML_FILES})
  _egnite_add_qt_resource_alias(RESOURCES ${THIS_RESOURCES})

  qt_add_executable(${target} ${THIS_SOURCES})
  qt_add_qml_module(
    ${target}
    URI
    ${THIS_URI}
    VERSION
    ${THIS_VERSION}
    QML_FILES
    ${THIS_QML_FILES}
    RESOURCES
    ${THIS_RESOURCES}
    DEPENDENCIES
    ${THIS_QML_DEPENDENCIES})

  _egnite_config_executable(${target})

  if(THIS_DEPENDENCIES)
    target_link_libraries(${target} PUBLIC ${THIS_DEPENDENCIES})
  endif()

endfunction()
# ----------------------------------------------------------------------- #
# ------------- Define a function that helps add application ------------ #
# ----------------------------------------------------------------------- #
function(egnite_add_executable target)

  cmake_parse_arguments(THIS "" "" "SOURCES;DEPENDENCIES" ${ARGN})

  if(NOT "${THIS_UNPARSED_ARGUMENTS}" STREQUAL "")
    message(
      FATAL_ERROR
        "Extra unparsed arguments when calling _egnite_add_executable: ${THIS_UNPARSED_ARGUMENTS}"
    )
  endif()

  add_executable(${target} ${THIS_SOURCES})

  _egnite_config_executable(${target})

  if(THIS_DEPENDENCIES)
    target_link_libraries(${target} PUBLIC ${THIS_DEPENDENCIES})
  endif()

endfunction()
# ----------------------------------------------------------------------- #
# ----------- Define a function that helps add static analysis ---------- #
# ----------------------------------------------------------------------- #
function(egnite_static_analyzers)
  if(EGNITE_ENABLE_CLANG_TIDY)
    find_program(CLANGTIDY clang-tidy)
    if(CLANGTIDY)
      set(CMAKE_CXX_CLANG_TIDY ${CLANGTIDY}
                               -extra-arg=-Wno-unknown-warning-option)
    else()
      message(FATAL_ERROR "Clang-Tidy requested but executable not found")
    endif()
  endif()

  if(EGNITE_ENABLE_CPPCHECK)
    find_program(CPPCHECK cppcheck)
    if(CPPCHECK)
      set(CMAKE_CXX_CPPCHECK ${CPPCHECK} --suppress=missingInclude --enable=all
                             --inline-suppr --inconclusive --force)
    else()
      message(FATAL_ERROR "Cppcheck requested but executable not found")
    endif()
  endif()
endfunction()
# ----------------------------------------------------------------------- #
# ------------ Define a function that helps configure module ------------ #
# ----------------------------------------------------------------------- #
function(_egnite_config_module target export)

  string(REPLACE "-" "_" NAME_UPPER "${target}")
  string(TOUPPER "${NAME_UPPER}" NAME_UPPER)
  set_target_properties(${target} PROPERTIES DEFINE_SYMBOL
                                             ${NAME_UPPER}_EXPORTS)

  if(BUILD_SHARED_LIBS)
    set_target_properties(${target} PROPERTIES DEBUG_POSTFIX -d)
  else()
    set_target_properties(${target} PROPERTIES DEBUG_POSTFIX -s-d)
    set_target_properties(${target} PROPERTIES RELEASE_POSTFIX -s)
  endif()

  set_target_properties(${target} PROPERTIES COMPILE_FEATURES cxx_std_20)
  set_target_properties(${target} PROPERTIES LINKER_LANGUAGE CXX)

  install(
    TARGETS ${target}
    EXPORT ${export}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR} COMPONENT bin
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR} COMPONENT lib
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR} COMPONENT lib)

  if(NOT BUILD_SHARED_LIBS)
    target_compile_definitions(${target} PUBLIC "EGNITE_STATIC")
  endif()

  set_target_properties(${target} PROPERTIES CXX_STANDARD 20)
  target_compile_definitions(${target} PUBLIC QT_NO_KEYWORDS)

endfunction()
# ----------------------------------------------------------------------- #
# ---------- Define a function that helps configure application --------- #
# ----------------------------------------------------------------------- #
function(_egnite_config_executable target)

  set_target_properties(${target} PROPERTIES DEBUG_POSTFIX -d)

  if(THIS_DEPENDENCIES)
    target_link_libraries(${target} PUBLIC ${THIS_DEPENDENCIES})
  endif()

  install(TARGETS ${target} RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
                                    COMPONENT bin)

  target_compile_definitions(${target} PUBLIC QT_NO_KEYWORDS)
  set_target_properties(${target} PROPERTIES CXX_STANDARD 20)

endfunction()
# ----------------------------------------------------------------------- #
# ---------- Define a function that helps add resources aliases --------- #
# ----------------------------------------------------------------------- #
function(_egnite_add_qt_resource_alias)
  cmake_parse_arguments(THIS "" "" "RESOURCES" ${ARGN})

  if(NOT "${THIS_UNPARSED_ARGUMENTS}" STREQUAL "")
    message(
      FATAL_ERROR
        "Extra unparsed arguments when calling _egnite_add_qt_resource_alias: ${THIS_UNPARSED_ARGUMENTS}"
    )
  endif()

  foreach(resource ${THIS_RESOURCES})
    get_filename_component(file ${resource} NAME)
    set_source_files_properties(${resource} PROPERTIES QT_RESOURCE_ALIAS
                                                       ${file})
  endforeach()
endfunction()
# ----------------------------------------------------------------------- #
