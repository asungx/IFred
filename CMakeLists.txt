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
)
