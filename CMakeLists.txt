cmake_minimum_required(VERSION 3.17)

project(Duckvil)

if(WIN32)
    add_definitions(-DDUCKVIL_PLATFORM_WINDOWS)
else()
    if(UNIX)
        add_definitions(-DDUCKVIL_PLATFORM_LINUX)
    endif()
endif()

add_subdirectory(source)
ENABLE_TESTING()
add_subdirectory(test)