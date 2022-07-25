cmake_minimum_required(VERSION 3.16)

project(Duckvil)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)
set_property(GLOBAL PROPERTY PREDEFINED_TARGETS_FOLDER "_CMake")

option(DUCKVIL_GENERATE_REFLECTION "Generate reflection" OFF)
option(DUCKVIL_COMPILE_REFLECTION_SOURCE "Compile with generated .cpp reflection files" ON)
option(DUCKVIL_DEBUG_MEMORY "Memory debugger" OFF)
option(DUCKVIL_CICD "CI/CD" OFF)
option(DUCKVIL_HOT_RELOADING "Hot reloading" ON)
option(DUCKVIL_TRACY_PROFILER "Tracy profiler" OFF)
option(DUCKVIL_HEADLESS_SERVER "Headless server" OFF)
set(DUCKVIL_PROJECTS_PATH CACHE PATH "Projects path")

find_program(DUCKVIL_MSBUILD_PATH "MSBuild.exe")
find_program(DUCKVIL_VCVARS_PATH "vcvarsall.bat")

add_definitions(-DDUCKVIL_MSBUILD_PATH="${DUCKVIL_MSBUILD_PATH}")
add_definitions(-DDUCKVIL_VCVARS_PATH="${DUCKVIL_VCVARS_PATH}")
add_definitions(-DDUCKVIL_PROJECTS_PATH="${DUCKVIL_PROJECTS_PATH}")

if(NOT DUCKVIL_COMPILE_REFLECTION_SOURCE)
    add_definitions(-DDUCKVIL_GENERATED_BODY="")
else()
    add_definitions(-DDUCKVIL_COMPILE_REFLECTION_SOURCE)
endif()

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
        SET(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -Wl,--no-undefined")
        SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fPIC")
        SET(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${GCC_COVERAGE_LINK_FLAGS} -Wl,--no-undefined")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-invalid-offsetof")
    endif()
endif()

if(MSVC)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /MP /arch:AVX2")
elseif(UNIX)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -msse4.1")
endif()

add_definitions(-DDUCKVIL_OUTPUT="${CMAKE_SOURCE_DIR}/bin")
add_definitions(-DDUCKVIL_SWAP_OUTPUT="${CMAKE_SOURCE_DIR}/bin/Swap")
add_definitions(-DDUCKVIL_CWD="${CMAKE_SOURCE_DIR}")
add_definitions(-DGLM_FORCE_DEFAULT_ALIGNED_GENTYPES)
add_definitions(-DGLM_FORCE_AVX2)

if(DUCKVIL_HEADLESS_SERVER)
    add_definitions(-DDUCKVIL_HEADLESS_SERVER)
endif()

if(DUCKVIL_DEBUG_MEMORY)
    add_definitions(-DDUCKVIL_MEMORY_DEBUGGER)
endif()

if(DUCKVIL_HOT_RELOADING)
    add_definitions(-DDUCKVIL_HOT_RELOADING)
endif()

add_subdirectory(external)

if(DUCKVIL_GENERATE_REFLECTION)
    message(STATUS "Generating reflection started!")
    execute_process(COMMAND ${CMAKE_SOURCE_DIR}/bin/ReflectionGenerator.exe -CWD ${CMAKE_SOURCE_DIR})
    message(STATUS "Generating reflection done!")
endif()

add_subdirectory(source)
list(APPEND CMAKE_CTEST_ARGUMENTS "--output-on-failure")
ENABLE_TESTING()
add_subdirectory(test)