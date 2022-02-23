cmake_minimum_required(VERSION 3.16)

SET(externalFiles
    
)
SET(includeFiles
    
)
SET(libraries
    
)

DUCKVIL_MODULE("Memory" "SHARED" "${externalFiles}" "${includeFiles}" "${libraries}")

target_compile_definitions(Memory PRIVATE TRACY_DBGHELP_LOCK=DbgHelp)