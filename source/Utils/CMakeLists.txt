cmake_minimum_required(VERSION 3.16)

SET(externalFiles
    
)
SET(includeFiles
    ${CMAKE_SOURCE_DIR}/external/json/include
)
SET(libraries

)

SET(DUCKVIL_COMPILE_REFLECTION_SOURCE FALSE)

DUCKVIL_MODULE("Utils" "STATIC" "${externalFiles}" "${includeFiles}" "${libraries}")