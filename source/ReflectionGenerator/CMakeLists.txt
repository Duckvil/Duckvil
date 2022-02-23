cmake_minimum_required(VERSION 3.16)

SET(externalFiles
    
)
SET(includeFiles
    ${CMAKE_SOURCE_DIR}/external/json/include
)
SET(libraries
    nlohmann_json
)

DUCKVIL_MODULE("ReflectionGenerator" "exe" "${externalFiles}" "${includeFiles}" "${libraries}")