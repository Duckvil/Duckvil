cmake_minimum_required(VERSION 3.16)

if(WIN32)
    set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS TRUE)
    set(BUILD_SHARED_LIBS TRUE)
endif()

add_library(
    _tracy
    SHARED
    ${CMAKE_SOURCE_DIR}/external/tracy/TracyClient.cpp
)