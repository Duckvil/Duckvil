cmake_minimum_required(VERSION 3.16)

FILE(GLOB_RECURSE SRC_FILES *.cpp)
FILE(GLOB_RECURSE GENERATED_SRC_FILES ${CMAKE_SOURCE_DIR}/__generated_reflection__/Thread/*.cpp)

add_library(
    Thread
    SHARED
    ${SRC_FILES}
    ${GENERATED_SRC_FILES}
)
target_include_directories(
    Thread
    PRIVATE
)
if(UNIX)
    target_link_libraries(
        Thread
        pthread
    )
endif()