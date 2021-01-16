cmake_minimum_required(VERSION 3.17)

FILE(GLOB_RECURSE SRC_FILES *.cpp)
FILE(GLOB_RECURSE GENERATED_SRC_FILES ${CMAKE_SOURCE_DIR}/__generated_reflection__/HotReloader/*.cpp)

add_library(
    HotReloader
    SHARED
    ${SRC_FILES}
    ${GENERATED_SRC_FILES}
)
target_include_directories(
    HotReloader
    PRIVATE
)