cmake_minimum_required(VERSION 3.16)

SET(externalFiles
    ${CMAKE_SOURCE_DIR}/external/imgui/imgui.cpp
    ${CMAKE_SOURCE_DIR}/external/imgui/imgui_draw.cpp
    ${CMAKE_SOURCE_DIR}/external/imgui/imgui_tables.cpp
    ${CMAKE_SOURCE_DIR}/external/imgui/imgui_widgets.cpp
)
SET(includeFiles
    ${CMAKE_SOURCE_DIR}/external/imgui
    ${CMAKE_SOURCE_DIR}/external/imgui/backends
    ${CMAKE_SOURCE_DIR}/external/SDL2/include
    ${CMAKE_SOURCE_DIR}/external/glew/include
    ${CMAKE_SOURCE_DIR}/external/hex_editor
)
SET(libraries
    
)

DUCKVIL_MODULE("HotReloader" "SHARED" "${externalFiles}" "${includeFiles}" "${libraries}")