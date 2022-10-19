# ----------------------------------------------------------------------- #
# -------------------------- Set CMake Version -------------------------- #
# ----------------------------------------------------------------------- #
cmake_minimum_required(VERSION 3.11)
# ----------------------------------------------------------------------- #
# -------------------- Create VEnv nad locate python -------------------- #
# ----------------------------------------------------------------------- #
egnite_create_python_env(DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
egnite_init_python_env(PYTHON_ENV ${CMAKE_CURRENT_BINARY_DIR}/venv REQUIREMENTS
                       ${CMAKE_CURRENT_SOURCE_DIR}/requirements.txt)
# ----------------------------------------------------------------------- #
