cmake_minimum_required(VERSION 3.16)

FILE(GLOB_RECURSE SRC_FILES *.cpp)

add_library(
    PlugNPlay
    STATIC
    ${SRC_FILES}
)

target_include_directories(
    PlugNPlay
    PRIVATE
)

if(UNIX)
    find_library(LIB_LOCATION libdl.so)

    target_link_libraries(
        PlugNPlay
        ${LIB_LOCATION}
    )
endif()