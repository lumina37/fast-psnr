cmake_minimum_required(VERSION 3.15)

project(psnr
        VERSION 0.0.0.1
        LANGUAGES CXX
)

# Helper
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

# Sources
add_subdirectory(src)
