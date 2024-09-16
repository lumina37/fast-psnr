cmake_minimum_required(VERSION 3.15)

include(cmake/policies.cmake)

project(PSNR VERSION 0.0.1 LANGUAGES CXX)

include(cmake/helper.cmake)
include(cmake/dependencies.cmake)

add_subdirectory(src)
