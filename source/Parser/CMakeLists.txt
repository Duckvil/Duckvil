cmake_minimum_required(VERSION 3.16)

set(DUCKVIL_CLANG_LIB_PATH CACHE FILEPATH "Clang lib path")

SET(externalFiles
    
)
SET(includeFiles
    ${CMAKE_SOURCE_DIR}/external/cppast/include
)
SET(libraries
    cppast
    ${DUCKVIL_CLANG_LIB_PATH}
)

DUCKVIL_MODULE("Parser" "SHARED" "${externalFiles}" "${includeFiles}" "${libraries}")