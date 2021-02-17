cmake_minimum_required(VERSION 3.17)

FILE(GLOB_RECURSE SRC_FILES *.cpp)
FILE(GLOB_RECURSE GENERATED_SRC_FILES ${CMAKE_SOURCE_DIR}/__generated_reflection__/Editor/*.cpp)

add_library(
    Editor
    SHARED
    ${SRC_FILES}
    ${GENERATED_SRC_FILES}
    "${CMAKE_SOURCE_DIR}/external/imgui/imgui.cpp"
    "${CMAKE_SOURCE_DIR}/external/imgui/imgui_draw.cpp"
    "${CMAKE_SOURCE_DIR}/external/imgui/imgui_tables.cpp"
    "${CMAKE_SOURCE_DIR}/external/imgui/imgui_widgets.cpp"
    "${CMAKE_SOURCE_DIR}/external/imgui/backends/imgui_impl_opengl3.cpp"
    "${CMAKE_SOURCE_DIR}/external/imgui/backends/imgui_impl_sdl.cpp"
)
target_include_directories(
    Editor
    PRIVATE
    "${CMAKE_SOURCE_DIR}/external/imgui"
    "${CMAKE_SOURCE_DIR}/external/imgui/backends"
    "${CMAKE_SOURCE_DIR}/external/SDL2/include"
    "${CMAKE_SOURCE_DIR}/external/glew/include"
    "${CMAKE_SOURCE_DIR}/external/hex_editor"
)
target_link_libraries(
    Editor
    glew_s
)
target_link_libraries(
    Editor
    glew
)
target_link_libraries(
    Editor
    SDL2
)