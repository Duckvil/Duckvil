cmake_minimum_required(VERSION 3.16)

if(WIN32)
    SET(CONDITIONAL_LIBS OpenGL32)
elseif(UNIX)
    SET(CONDITIONAL_LIBS GL)
endif()

SET(externalFiles
    ${CMAKE_SOURCE_DIR}/external/imgui/imgui.cpp
    ${CMAKE_SOURCE_DIR}/external/imgui/imgui_draw.cpp
    ${CMAKE_SOURCE_DIR}/external/imgui/imgui_tables.cpp
    ${CMAKE_SOURCE_DIR}/external/imgui/imgui_widgets.cpp
    ${CMAKE_SOURCE_DIR}/external/imgui/backends/imgui_impl_opengl3.cpp
    ${CMAKE_SOURCE_DIR}/external/imgui/backends/imgui_impl_sdl.cpp
)
SET(includeFiles
    ${CMAKE_SOURCE_DIR}/external/json/include
    ${CMAKE_SOURCE_DIR}/external/imgui
    ${CMAKE_SOURCE_DIR}/external/imgui/backends
)
SET(libraries
    glew
    SDL2
    assimp
    "${CONDITIONAL_LIBS}"
)

DUCKVIL_MODULE("Graphics" "SHARED" "${externalFiles}" "${includeFiles}" "${libraries}")