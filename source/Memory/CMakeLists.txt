cmake_minimum_required(VERSION 3.16)

FILE(GLOB_RECURSE SRC_FILES *.cpp)
FILE(GLOB_RECURSE GENERATED_SRC_FILES ${CMAKE_SOURCE_DIR}/__generated_reflection__/Memory/*.cpp)

add_library(
    Memory
    SHARED
    ${SRC_FILES}
    ${GENERATED_SRC_FILES}
)
target_include_directories(
    Memory
    PRIVATE
)
target_compile_definitions(Memory PRIVATE TRACY_DBGHELP_LOCK=DbgHelp)