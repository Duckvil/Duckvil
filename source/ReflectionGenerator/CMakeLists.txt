cmake_minimum_required(VERSION 3.16)

FILE(GLOB_RECURSE SRC_FILES *.cpp)

add_executable(
    ReflectionGenerator
    ${SRC_FILES}
)

target_include_directories(
    ReflectionGenerator
    PRIVATE
)