cmake_minimum_required(VERSION 3.17)

project(Duckvil)

if(WIN32)
    add_definitions(-DDUCKVIL_PLATFORM_WINDOWS)
else()
    if(UNIX)
        add_definitions(-DDUCKVIL_PLATFORM_LINUX)
    endif()
endif()

add_definitions(-DDUCKVIL_OUTPUT="${CMAKE_SOURCE_DIR}/bin")

add_subdirectory(source)
list(APPEND CMAKE_CTEST_ARGUMENTS "--output-on-failure")
ENABLE_TESTING()
add_subdirectory(test)