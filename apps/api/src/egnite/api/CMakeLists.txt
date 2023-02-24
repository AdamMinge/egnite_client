# ----------------------------------------------------------------------- #
# -------------------------- Set CMake Version -------------------------- #
# ----------------------------------------------------------------------- #
cmake_minimum_required(VERSION 3.11)
# ----------------------------------------------------------------------- #
# -------------------------- Generate headers --------------------------- #
# ----------------------------------------------------------------------- #
set(schema_root ${EGNITE_SOURCE_DIR}/apps/api)
set(resources_root ${schema_root}/resources)
# ----------------------------------------------------------------------- #
# -------------------------- Set files variable ------------------------- #
# ----------------------------------------------------------------------- #
set(schemas
    ${resources_root}/clients/egnite_client.xml
    ${resources_root}/apis/registration_api.xml
    ${resources_root}/models/activate_email_request.xml
    ${resources_root}/models/exception_response.xml
    ${resources_root}/models/register_request.xml
    ${resources_root}/models/register_response.xml
    ${resources_root}/models/resend_activation_email_request.xml)
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
egnite_add_qml_module(
  egnite-api-qml
  URI
  egnite.api
  VERSION
  ${EGNITE_VERSION_MAJOR}.${EGNITE_VERSION_MINOR}
  DEPENDENCIES
  Qt::Qml
  Qt::QuickControls2
  egnite-rest-qml)

egnite_add_schema_converter(egnite-api-qml INTERFACE qml SOURCES ${schemas})
# ----------------------------------------------------------------------- #
