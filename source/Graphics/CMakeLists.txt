cmake_minimum_required(VERSION 3.16)

if(WIN32)
    SET(CONDITIONAL_LIBS OpenGL32)
elseif(UNIX)
    SET(CONDITIONAL_LIBS GL)
endif()

SET(externalFiles
    
)
SET(includeFiles
    
)
SET(libraries
    glew
    SDL2
    assimp
    "${CONDITIONAL_LIBS}"
)

DUCKVIL_MODULE("Graphics" "SHARED" "${externalFiles}" "${includeFiles}" "${libraries}")