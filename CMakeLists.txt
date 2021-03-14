cmake_minimum_required(VERSION 3.11)

include(FetchContent)

project(cabaret-tetrahedral)

set(CMAKE_CXX_STANDARD 17)
add_compile_options(-Wall -Wextra -pedantic -Werror)

# eigen library begin
FetchContent_Declare(
        eigen
        GIT_REPOSITORY https://gitlab.com/libeigen/eigen.git
        GIT_TAG        3.3.9
)
FetchContent_GetProperties(eigen)
if(NOT eigen)
    FetchContent_Populate(eigen)
    include_directories(SYSTEM ${eigen_SOURCE_DIR})
endif()
# eigen library end

# googletest library begin
FetchContent_Declare(
        googletest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG        release-1.10.0
)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_GetProperties(googletest)
if(NOT googletest)
    FetchContent_Populate(googletest)
    add_subdirectory(
            ${googletest_SOURCE_DIR}
            ${googletest_BINARY_DIR}
            EXCLUDE_FROM_ALL
    )
endif()
# googletest library end

# cabaret start
add_subdirectory(src/core)
add_subdirectory(src/parser)
add_subdirectory(src/transfer)

add_executable(cabaret src/cabaret.cpp)

target_include_directories(cabaret PRIVATE include)

target_link_libraries(cabaret core)
target_link_libraries(cabaret parser)
target_link_libraries(cabaret transfer)
# cabaret end

# unit tests begin
enable_testing()

file(GLOB TEST_SOURCES test/*.cpp)

add_executable(unit_tests ${TEST_SOURCES})

target_include_directories(unit_tests PRIVATE include)

target_link_libraries(unit_tests core)
target_link_libraries(unit_tests gtest_main)

add_test(test_all unit_tests)
# unit tests end