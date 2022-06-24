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
  Cli11
  GIT_REPOSITORY https://github.com/CLIUtils/CLI11
  GIT_TAG b9be5b9444772324459989177108a6a65b8b2769
)
FetchContent_MakeAvailable( Cli11 )

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
# Linking

target_link_libraries( ${PROJECT_NAME} PRIVATE
    ${PROJECT_LIB}
    CLI11::CLI11
    xeus-static
)

