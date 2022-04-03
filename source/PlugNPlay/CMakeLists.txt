cmake_minimum_required(VERSION 3.16)

if(UNIX)
    SET(CONDITIONAL_LIBS dl)
endif()

SET(externalFiles
    
)
SET(includeFiles
    
)
SET(libraries
    "${CONDITIONAL_LIBS}"
)

DUCKVIL_MODULE("PlugNPlay" "STATIC" "${externalFiles}" "${includeFiles}" "${libraries}")