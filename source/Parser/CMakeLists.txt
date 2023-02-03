cmake_minimum_required(VERSION 3.16)

SET(externalFiles
    
)
SET(includeFiles
    ${CMAKE_SOURCE_DIR}/external/cppast/include
)
SET(libraries
    cppast
    "F:/Programs/LLVM/lib/libclang.lib"
)

DUCKVIL_MODULE("Parser" "SHARED" "${externalFiles}" "${includeFiles}" "${libraries}")