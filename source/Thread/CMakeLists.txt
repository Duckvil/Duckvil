cmake_minimum_required(VERSION 3.16)

if(UNIX)
    SET(CONDITIONAL_LIBS pthread)
endif()

SET(externalFiles
    
)
SET(includeFiles
    
)
SET(libraries
    "${CONDITIONAL_LIBS}"
)

DUCKVIL_MODULE("Thread" "SHARED" "${externalFiles}" "${includeFiles}" "${libraries}")