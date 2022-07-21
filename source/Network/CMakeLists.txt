cmake_minimum_required(VERSION 3.16)

SET(externalFiles
    
)
SET(includeFiles
    "${CMAKE_SOURCE_DIR}/external/asio/asio/include"
)
SET(libraries
    
)

DUCKVIL_MODULE("Network" "SHARED" "${externalFiles}" "${includeFiles}" "${libraries}")