cmake_minimum_required(VERSION 3.16)

FILE(GLOB_RECURSE SRC_FILES *.cpp)

add_library(
    UniTestFramework
    STATIC
    ${SRC_FILES}
)

target_include_directories(
    UniTestFramework
    PRIVATE
)