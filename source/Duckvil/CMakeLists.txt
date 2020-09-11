cmake_minimum_required(VERSION 3.17)

FILE(GLOB_RECURSE SRC_FILES *.cpp)

add_executable(
    Duckvil
    ${SRC_FILES}
)

target_include_directories(
    Duckvil
    PRIVATE
)