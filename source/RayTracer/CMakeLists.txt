cmake_minimum_required(VERSION 3.16)

execute_process(COMMAND nvcc -shared ${CMAKE_CURRENT_SOURCE_DIR}/kernel.cu -o ${CMAKE_SOURCE_DIR}/bin/kernel.dll -I ${CMAKE_SOURCE_DIR}/external/glm -I ${CMAKE_SOURCE_DIR}/include/RayTracer)

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
)
SET(includeFiles
    ${CMAKE_SOURCE_DIR}/external/imgui
    "F:/Programs/CUDA Toolkit/v11.6/include"
)
SET(libraries
    glew
    "F:/Programs/CUDA Toolkit/v11.6/lib/x64/cudart.lib"
    "${CONDITIONAL_LIBS}"
)

DUCKVIL_MODULE("RayTracer" "SHARED" "${externalFiles}" "${includeFiles}" "${libraries}")