cmake_minimum_required(VERSION 3.25)

SET(externalFiles
    ${CMAKE_SOURCE_DIR}/external/imgui/imgui.cpp
    ${CMAKE_SOURCE_DIR}/external/imgui/imgui_draw.cpp
    ${CMAKE_SOURCE_DIR}/external/imgui/imgui_tables.cpp
    ${CMAKE_SOURCE_DIR}/external/imgui/imgui_widgets.cpp
    ${CMAKE_SOURCE_DIR}/external/imgui/backends/imgui_impl_opengl3.cpp
    ${CMAKE_SOURCE_DIR}/external/imgui/backends/imgui_impl_sdl.cpp
)
SET(includeFiles
    "${CMAKE_SOURCE_DIR}/external/mono/msvc/include"
    "${CMAKE_SOURCE_DIR}/external/json/include"
    ${CMAKE_SOURCE_DIR}/external/imgui
    ${CMAKE_SOURCE_DIR}/external/imgui/backends
    ${CMAKE_SOURCE_DIR}/external/SDL2/include
    ${CMAKE_SOURCE_DIR}/external/glew/include
    ${CMAKE_SOURCE_DIR}/external/hex_editor
    ${CMAKE_SOURCE_DIR}/external/json/include
    ${CMAKE_SOURCE_DIR}/external/ImGuizmo
)
SET(libraries
    "${CMAKE_SOURCE_DIR}/external/mono/msvc/build/sgen/x64/lib/Debug/libmono-static-sgen.lib"
    "${CMAKE_SOURCE_DIR}/external/mono/msvc/build/sgen/x64/lib/Debug/libmonoruntime-sgen.lib"
    "${CMAKE_SOURCE_DIR}/external/mono/msvc/build/sgen/x64/lib/Debug/libmonoutils.lib"
    "${CMAKE_SOURCE_DIR}/external/mono/msvc/build/sgen/x64/lib/Debug/mono-2.0-sgen.lib"
    winmm
    version
    "${CMAKE_SOURCE_DIR}/external/mono/msvc/build/sgen/x64/lib/Debug/eglib.lib"
    bcrypt
    Ws2_32
    glew
    SDL2
    ImGuizmo
)

DUCKVIL_MODULE("CSharp" "SHARED" "${externalFiles}" "${includeFiles}" "${libraries}")