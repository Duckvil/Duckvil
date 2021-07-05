cmake_minimum_required(VERSION 3.16)

FILE(GLOB_RECURSE SRC_FILES *.cpp)
FILE(GLOB_RECURSE GENERATED_SRC_FILES ${CMAKE_SOURCE_DIR}/__generated_reflection__/Memory/*.cpp)

list(APPEND SOURCE_FILES ${SRC_FILES})

if(DUCKVIL_COMPILE_REFLECTION_SOURCE)
    list(APPEND SOURCE_FILES ${GENERATED_SRC_FILES})
endif()

add_library(
    Memory
    SHARED
    ${SOURCE_FILES}
)
target_include_directories(
    Memory
    PRIVATE
)
target_compile_definitions(Memory PRIVATE TRACY_DBGHELP_LOCK=DbgHelp)