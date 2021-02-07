cmake_minimum_required(VERSION 3.17)

project(Duckvil)

option(GENERATE_REFLECTION "Generate reflection" OFF)
option(DEBUG_MEMORY "Memory debugger" OFF)

find_program(CLANG_TIDY_COMMAND NAMES clang-tidy)

if(NOT CLANG_TIDY_COMMAND)
    message(WARNING "Could not find clang-tidy!")
    set(CMAKE_CXX_CLANG_TIDY "" CACHE STRING "" FORCE)
else()
    message(WARNING "Found clang-tidy")
    set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
endif()

if(WIN32)
    add_definitions(-DDUCKVIL_PLATFORM_WINDOWS)
else()
    if(UNIX)
        add_definitions(-DDUCKVIL_PLATFORM_LINUX)

        SET(GCC_COVERAGE_COMPILE_FLAGS "-g -O0 --coverage")
        SET(GCC_COVERAGE_LINK_FLAGS "--coverage -lgcov")

        SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${GCC_COVERAGE_COMPILE_FLAGS} -fPIC")
        SET(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${GCC_COVERAGE_LINK_FLAGS}")
    endif()
endif()

add_definitions(-DDUCKVIL_OUTPUT="${CMAKE_SOURCE_DIR}/bin")
add_definitions(-DDUCKVIL_SWAP_OUTPUT="${CMAKE_SOURCE_DIR}/bin/Swap")
add_definitions(-DDUCKVIL_CWD="${CMAKE_SOURCE_DIR}")

if(DEBUG_MEMORY)
    add_definitions(-DDUCKVIL_MEMORY_DEBUGGER)
endif()

add_subdirectory(external)
add_subdirectory(source)
list(APPEND CMAKE_CTEST_ARGUMENTS "--output-on-failure")
ENABLE_TESTING()
add_subdirectory(test)

if(GENERATE_REFLECTION)
    message(STATUS "Generating reflection started!")
    execute_process(COMMAND "${CMAKE_SOURCE_DIR}/bin/ReflectionGenerator.exe")
    message(STATUS "Generating reflection done!")
endif()