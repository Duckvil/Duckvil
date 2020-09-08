cmake_minimum_required(VERSION 3.18)

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