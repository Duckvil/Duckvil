cmake_minimum_required(VERSION 3.16)

SET(externalFiles
    ${CMAKE_SOURCE_DIR}/external/imgui/imgui.cpp
    ${CMAKE_SOURCE_DIR}/external/imgui/imgui_draw.cpp
    ${CMAKE_SOURCE_DIR}/external/imgui/imgui_tables.cpp
    ${CMAKE_SOURCE_DIR}/external/imgui/imgui_widgets.cpp
)
SET(includeFiles
    ${CMAKE_SOURCE_DIR}/external/imgui
)
SET(libraries
    
)

DUCKVIL_MODULE("TestSystem" "SHARED" "${externalFiles}" "${includeFiles}" "${libraries}")