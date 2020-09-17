cmake_minimum_required(VERSION 3.17)

project(Duckvil)

find_program(CLANG_TIDY_COMMAND NAMES clang-tidy)

if(NOT CLANG_TIDY_COMMAND)
    message(WARNING "Could not find clang-tidy!")
    set(CMAKE_CXX_CLANG_TIDY "" CACHE STRING "" FORCE)
else()
    set(CLANG_TIDY_CHECKS "-*,modernize-*")
    set(CMAKE_CXX_CLANG_TIDY "${CLANG_TIDY_COMMAND};-checks=${CLANG_TIDY_CHECKS};-header-filter='${CMAKE_SOURCE_DIR}/source/*'")
endif()

if(WIN32)
    add_definitions(-DDUCKVIL_PLATFORM_WINDOWS)
else()
    if(UNIX)
        add_definitions(-DDUCKVIL_PLATFORM_LINUX)

        SET(GCC_COVERAGE_COMPILE_FLAGS "-g -O0 -coverage -fprofile-arcs -ftest-coverage")
        SET(GCC_COVERAGE_LINK_FLAGS "-coverage -lgcov")

        SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${GCC_COVERAGE_COMPILE_FLAGS}")
        SET(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${GCC_COVERAGE_LINK_FLAGS}")
    endif()
endif()

add_definitions(-DDUCKVIL_OUTPUT="${CMAKE_SOURCE_DIR}/bin")

add_subdirectory(source)
list(APPEND CMAKE_CTEST_ARGUMENTS "--output-on-failure")
ENABLE_TESTING()
add_subdirectory(test)