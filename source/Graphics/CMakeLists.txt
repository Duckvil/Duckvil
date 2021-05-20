cmake_minimum_required(VERSION 3.16)

FILE(GLOB_RECURSE SRC_FILES *.cpp)
FILE(GLOB_RECURSE GENERATED_SRC_FILES ${CMAKE_SOURCE_DIR}/__generated_reflection__/Graphics/*.cpp)

add_library(
    Graphics
    SHARED
    ${SRC_FILES}
    ${GENERATED_SRC_FILES}
)
target_include_directories(
    Graphics
    PRIVATE
    ${CMAKE_SOURCE_DIR}/external
)
target_link_libraries(
    Graphics
    OpenGL32
    glew
    SDL2
)