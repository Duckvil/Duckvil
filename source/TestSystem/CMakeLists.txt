cmake_minimum_required(VERSION 3.16)

FILE(GLOB_RECURSE SRC_FILES *.cpp)
FILE(GLOB_RECURSE GENERATED_SRC_FILES ${CMAKE_SOURCE_DIR}/__generated_reflection__/TestSystem/*.cpp)

add_library(
    TestSystem
    SHARED
    ${SRC_FILES}
    ${GENERATED_SRC_FILES}
    "${CMAKE_SOURCE_DIR}/external/imgui/imgui.cpp"
    "${CMAKE_SOURCE_DIR}/external/imgui/imgui_draw.cpp"
    "${CMAKE_SOURCE_DIR}/external/imgui/imgui_tables.cpp"
    "${CMAKE_SOURCE_DIR}/external/imgui/imgui_widgets.cpp"
)
target_include_directories(
    TestSystem
    PRIVATE
    "${CMAKE_SOURCE_DIR}/external/imgui"
)