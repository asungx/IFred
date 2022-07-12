cmake_minimum_required( VERSION 3.22 )

# Set compiler flags
set( CMAKE_CXX_FLAGS_DEBUG   "-Wall -ggdb -O0 -std=c++20" )
set( CMAKE_CXX_FLAGS_RELEASE "-Wall -Werror -O3 -std=c++20" )

# Export compile commands for debuggers
set( CMAKE_EXPORT_COMPILE_COMMANDS ON )

# Copy compile_commands.json to root directory so debugging tools can find it.
add_custom_target(
    do_always ALL
    ${CMAKE_COMMAND} -E copy_if_different
    ${CMAKE_BINARY_DIR}/compile_commands.json
    ${CMAKE_CURRENT_LIST_DIR}
)

set(PROJECT_NAME IFred)
set(PROJECT_LIB  IFred_lib)

project( ${PROJECT_NAME}
    LANGUAGES CXX
)

#------------------------------------------------------------------------------
# Manually set C++ files
set( HEADERS
    ${PROJECT_SOURCE_DIR}/source/interpreter/interpreter.hpp
)

set( SOURCE_FILES
    ${PROJECT_SOURCE_DIR}/source/interpreter/interpreter.cpp
)

set( ENTRY_POINT
    ${PROJECT_SOURCE_DIR}/source/main.cpp
)

#------------------------------------------------------------------------------
# Dependencies

# Get conda directory
# Note: This does NOT work if conda is also soft-linked in the user's PATH
execute_process(
    COMMAND bash -c "whereis conda"
    OUTPUT_VARIABLE CONDA_HOME
    OUTPUT_STRIP_TRAILING_WHITESPACE
)
string( REGEX MATCH "^conda: (.+)\/bin\/conda$"
    MATCHED
    ${CONDA_HOME}
)
if (MATCHED)
    set( CONDA_HOME ${CMAKE_MATCH_1} )
else()
    message( FATAL_ERROR "Cannot find conda directory" )
endif()

# Set libuuid variables
set( LibUUID_LIBRARY     ${CONDA_HOME}/lib/libuuid.a )
set( LibUUID_INCLUDE_DIR ${CONDA_HOME}/include/uuid )

# Include xeus library
set( XEUS_REQUIRED_VERSION 2.4.1 )
find_package( xeus ${XEUS_REQUIRED_VERSION} REQUIRED )

# Load FetchContent module
include( FetchContent )

# CLI11 - command line parser library
# Version: 2.2
FetchContent_Declare(
    CLI11
    GIT_REPOSITORY https://github.com/CLIUtils/CLI11
    GIT_TAG b9be5b9444772324459989177108a6a65b8b2769
)

# spdlog - logging library
# Version: 1.10.0
FetchContent_Declare(
    spdlog
    GIT_REPOSITORY https://github.com/gabime/spdlog
    GIT_TAG 76fb40d95455f249bd70824ecfcae7a8f0930fa3
)

# fmt - formatting library
# Version: 8.1.1
FetchContent_Declare(
    fmt
    GIT_REPOSITORY https://github.com/fmtlib/fmt
    GIT_TAG b6f4ceaed0a0a24ccf575fab6c56dd50ccf6f1a9
)

# Fetch nlohmann/json
# Version: 3.10.5
FetchContent_Declare (
    nlohmann_json
    GIT_REPOSITORY https://github.com/ArthurSonzogni/nlohmann_json_cmake_fetchcontent.git
    GIT_TAG 6aebf09233951e4ce30a63919186a70b2b195756
)

FetchContent_MakeAvailable(
    CLI11
    fmt
    nlohmann_json
    spdlog
)

#------------------------------------------------------------------------------
# Defining executables and libraries

add_library( ${PROJECT_LIB} STATIC
    ${HEADERS}
    ${SOURCE_FILES}
)

add_executable( ${PROJECT_NAME}
    ${ENTRY_POINT}
)

#------------------------------------------------------------------------------
# Headers

target_include_directories( ${PROJECT_NAME} PRIVATE
    source/interpreter
)

#------------------------------------------------------------------------------
# Linking

target_link_libraries( ${PROJECT_LIB} PRIVATE
    nlohmann_json::nlohmann_json
    xeus-static
)

target_link_libraries( ${PROJECT_NAME} PRIVATE
    ${PROJECT_LIB}
    CLI11::CLI11
    fmt::fmt
    spdlog::spdlog
    xeus-static
)

# Add paths to linker search and installed rpath
set_target_properties(${PROJECT_NAME} PROPERTIES
    INSTALL_RPATH_USE_LINK_PATH TRUE
)

#------------------------------------------------------------------------------
# Configuration

# Provide install directory variables as defined by the GNU Coding Standards:
# https://www.gnu.org/prep/standards/html_node/Directory-Variables.html
include( GNUInstallDirs )
configure_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/config/kernel.json.in
    ${CMAKE_CURRENT_BINARY_DIR}/share/jupyter/kernels/${PROJECT_NAME}/kernel.json
)

#------------------------------------------------------------------------------
# Installation

# Install kernel
install(
    TARGETS ${PROJECT_NAME}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

# Configuration and data directories for jupyter and kernel
set(
    XJUPYTER_DATA_DIR share/jupyter
    CACHE STRING "Jupyter data directory"
)

# Install Jupyter kernelspecs
set( KERNELSPEC_DIR ${CMAKE_CURRENT_BINARY_DIR}/share/jupyter/kernels )
install(
    DIRECTORY   ${KERNELSPEC_DIR}
    DESTINATION ${XJUPYTER_DATA_DIR}
    PATTERN "*.in" EXCLUDE
)
