cmake_minimum_required(VERSION 3.16)

if(UNIX)
    SET(CONDITIONAL_LIBS pthread)
endif()

if(WIN32)
    set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS TRUE)
    set(BUILD_SHARED_LIBS TRUE)
endif()

SET(externalFiles
    ${CMAKE_SOURCE_DIR}/external/tracy/TracyClient.cpp
)
SET(includeFiles
    
)
SET(libraries
    "${CONDITIONAL_LIBS}"
)

DUCKVIL_MODULE("_tracy" "SHARED" "${externalFiles}" "${includeFiles}" "${libraries}")