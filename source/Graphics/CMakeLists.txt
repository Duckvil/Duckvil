cmake_minimum_required(VERSION 3.16)

FILE(GLOB_RECURSE SRC_FILES *.cpp)
FILE(GLOB_RECURSE GENERATED_SRC_FILES ${CMAKE_SOURCE_DIR}/__generated_reflection__/Graphics/*.cpp)

list(APPEND SOURCE_FILES ${SRC_FILES})

if(DUCKVIL_COMPILE_REFLECTION_SOURCE)
    list(APPEND SOURCE_FILES ${GENERATED_SRC_FILES})
endif()

add_library(
    Graphics
    SHARED
    ${SOURCE_FILES}
)
target_include_directories(
    Graphics
    PRIVATE
    ${CMAKE_SOURCE_DIR}/external
)
target_link_libraries(
    Graphics
    glew
    SDL2
    assimp
)

if(WIN32)
    target_link_libraries(
        Graphics
        OpenGL32
    )
else()
    if(UNIX)
        target_link_libraries(
            Graphics
            GL
        )
    endif()
endif()