cmake_minimum_required(VERSION 3.5)

include_directories(${PROJECT_SOURCE_DIR}/include)

file(GLOB_RECURSE CORE_SOURCES "*.cpp")
add_library(core ${CORE_SOURCES})
