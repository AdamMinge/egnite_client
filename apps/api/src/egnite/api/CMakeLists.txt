# ----------------------------------------------------------------------- #
# -------------------------- Set CMake Version -------------------------- #
# ----------------------------------------------------------------------- #
cmake_minimum_required(VERSION 3.11)
# ----------------------------------------------------------------------- #
# -------------------------- Generate headers --------------------------- #
# ----------------------------------------------------------------------- #
set(api_root ${EGNITE_SOURCE_DIR}/apps/api)
set(resources_root ${api_root}/resources)
# ----------------------------------------------------------------------- #
# -------------------------- Set files variable ------------------------- #
# ----------------------------------------------------------------------- #
set(interface qml)

if(interface MATCHES qml)
  set(schemas
      ${resources_root}/schemas/qml/clients/egnite_client.xml
      ${resources_root}/schemas/qml/apis/registration_api.xml
      ${resources_root}/schemas/qml/apis/plugins_api.xml
      ${resources_root}/schemas/qml/apis/ratings_api.xml)
else()
  set(schemas
      ${resources_root}/schemas/qt/clients/egnite_client.xml
      ${resources_root}/schemas/qt/apis/registration_api.xml
      ${resources_root}/schemas/qt/models/activate_email_request.xml
      ${resources_root}/schemas/qt/models/exception_response.xml
      ${resources_root}/schemas/qt/models/register_request.xml
      ${resources_root}/schemas/qt/models/register_response.xml
      ${resources_root}/schemas/qt/models/resend_activation_email_request.xml)
endif()
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
egnite_add_schema_converter(egnite-api-qml generated_sources INTERFACE
                            ${interface} SOURCES ${schemas})

egnite_add_qml_module(
  egnite-api-qml
  URI
  egnite.api
  VERSION
  ${EGNITE_VERSION_MAJOR}.${EGNITE_VERSION_MINOR}
  SOURCES
  ${generated_sources}
  DEPENDENCIES
  Qt::Qml
  Qt::QuickControls2
  egnite-rest-qml)

target_sources(egnite-api-qml PRIVATE ${generated_sources})

target_include_directories(
  egnite-api-qml PRIVATE $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/schema/>)
# ----------------------------------------------------------------------- #
