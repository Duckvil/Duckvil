cmake_minimum_required(VERSION 3.16)

SET(externalFiles
    
)
SET(includeFiles
    ${CMAKE_SOURCE_DIR}/external/json/include
)
SET(libraries
    
)

DUCKVIL_MODULE("Engine" "SHARED" "${externalFiles}" "${includeFiles}" "${libraries}")