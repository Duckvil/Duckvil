cmake_minimum_required(VERSION 3.16)

FILE(GLOB_RECURSE SRC_FILES *.cpp)
FILE(GLOB_RECURSE GENERATED_SRC_FILES ${CMAKE_SOURCE_DIR}/__generated_reflection__/HotReloader/*.cpp)

list(APPEND SOURCE_FILES ${SRC_FILES})

if(DUCKVIL_COMPILE_REFLECTION_SOURCE)
    list(APPEND SOURCE_FILES ${GENERATED_SRC_FILES})
endif()

add_library(
    HotReloader
    SHARED
    ${SOURCE_FILES}
    "${CMAKE_SOURCE_DIR}/external/imgui/imgui.cpp"
    "${CMAKE_SOURCE_DIR}/external/imgui/imgui_draw.cpp"
    "${CMAKE_SOURCE_DIR}/external/imgui/imgui_tables.cpp"
    "${CMAKE_SOURCE_DIR}/external/imgui/imgui_widgets.cpp"
)
target_include_directories(
    HotReloader
    PRIVATE
    "${CMAKE_SOURCE_DIR}/external/imgui"
    "${CMAKE_SOURCE_DIR}/external/imgui/backends"
    "${CMAKE_SOURCE_DIR}/external/SDL2/include"
    "${CMAKE_SOURCE_DIR}/external/glew/include"
    "${CMAKE_SOURCE_DIR}/external/hex_editor"
)