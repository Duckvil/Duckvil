cmake_minimum_required(VERSION 3.17)

FILE(GLOB_RECURSE SRC_FILES *.cpp)

add_library(
    Memory
    SHARED
    ${SRC_FILES}
)
target_include_directories(
    Memory
    PRIVATE
)