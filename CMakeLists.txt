cmake_minimum_required(VERSION 3.15)

# Policies
# The pre-defined value will not be replaced by default value from `option`.
cmake_policy(SET CMP0077 NEW)

project(psnr
        VERSION 0.0.0.1
        LANGUAGES CXX
)

# Compile Options
include(CMakeDependentOption)

option(PSNR_HEADER_ONLY "Enable the header-only mode" OFF)
cmake_dependent_option(PSNR_BUILD_SHARED_LIBS "Specifies the type of PSNR to build" ON
        "NOT PSNR_HEADER_ONLY AND BUILD_SHARED_LIBS" OFF)

# Helper
function(copy_dlls_if_needed name)
    if (WIN32)
        add_custom_command(TARGET ${name} POST_BUILD COMMAND ${CMAKE_COMMAND} -E
                copy_if_different $<TARGET_RUNTIME_DLLS:${name}> $<TARGET_FILE:${name}> $<TARGET_FILE_DIR:${name}> COMMAND_EXPAND_LISTS)
    endif ()
endfunction()

set(PSNR_COMPILE_INFO "<tag:${PROJECT_VERSION}> by [${CMAKE_CXX_COMPILER_ID}-${CMAKE_CXX_COMPILER_VERSION} (${CMAKE_SYSTEM_PROCESSOR})]")

set(PSNR_CONFIGURE_DIR "${PROJECT_SOURCE_DIR}/src/include/psnr/common")
configure_file("${PSNR_CONFIGURE_DIR}/cmake.h.in" "${PSNR_CONFIGURE_DIR}/cmake.h" @ONLY)

# Dependencies
include(FetchContent)

FetchContent_Declare(argparse
        GIT_REPOSITORY https://github.com/p-ranav/argparse.git
        GIT_TAG v3.1
)
FetchContent_MakeAvailable(argparse)

FetchContent_Declare(fmt
        GIT_REPOSITORY https://github.com/fmtlib/fmt.git
        GIT_TAG 11.0.2
)
FetchContent_MakeAvailable(fmt)

# Sources
add_subdirectory(src)
