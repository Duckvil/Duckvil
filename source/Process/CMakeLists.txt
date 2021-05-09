cmake_minimum_required(VERSION 3.16)

FILE(GLOB_RECURSE SRC_FILES *.cpp)
FILE(GLOB_RECURSE GENERATED_SRC_FILES ${CMAKE_SOURCE_DIR}/__generated_reflection__/Process/*.cpp)

add_library(
    Process
    SHARED
    ${SRC_FILES}
    ${GENERATED_SRC_FILES}
)
target_include_directories(
    Process
    PRIVATE
)