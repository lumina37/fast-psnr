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

set(PSNR_ARGPARSE_PATH "https://github.com/p-ranav/argparse/archive/refs/tags/v3.1.tar.gz" CACHE STRING
        "Specifies the path of argparse")
FetchContent_Declare(
        argparse
        DOWNLOAD_EXTRACT_TIMESTAMP ON
        URL ${PSNR_ARGPARSE_PATH}
)
FetchContent_MakeAvailable(argparse)

include(CTest)

FetchContent_Declare(
        doctest
        GIT_REPOSITORY https://github.com/doctest/doctest.git
        GIT_TAG v2.4.11
)
FetchContent_MakeAvailable(doctest)
include(${doctest_SOURCE_DIR}/scripts/cmake/doctest.cmake)

# Sources
add_subdirectory(src)
add_subdirectory(tests)
