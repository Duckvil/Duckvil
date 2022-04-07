cmake_minimum_required(VERSION 3.16)

option(DUCKVIL_TRACY_PROFILER "Tracy profiler" OFF)

macro(DUCKVIL_MODULE moduleName moduleType moduleExternalFiles moduleIncludeFiles moduleLinkLibraries)
    FILE(GLOB_RECURSE SOURCE_FILES *.cpp)
    FILE(GLOB_RECURSE REFLECTION_SOURCE_FILES ${CMAKE_SOURCE_DIR}/__generated_reflection__/${moduleName}/*.cpp)
    FILE(GLOB_RECURSE REFLECTION_HEADER_FILES ${CMAKE_SOURCE_DIR}/__generated_reflection__/${moduleName}/*.h)
    FILE(GLOB_RECURSE HEADER_FILES ${CMAKE_SOURCE_DIR}/include/${moduleName}/*.h)

    SET(REFLECTION_SOURCE)
    SET(REFLECTION_HEADER)

    if(DUCKVIL_COMPILE_REFLECTION_SOURCE)
        list(APPEND REFLECTION_SOURCE ${REFLECTION_SOURCE_FILES})
        list(APPEND REFLECTION_HEADER ${REFLECTION_HEADER_FILES})
    endif()

    if(NOT ${moduleType} STREQUAL "exe")
        add_library(
            ${moduleName}
            ${moduleType}
            ${SOURCE_FILES}
            ${HEADER_FILES}
            ${REFLECTION_SOURCE}
            ${REFLECTION_HEADER}
            ${moduleExternalFiles}
        )
    else()
        add_executable(
            ${moduleName}
            ${SOURCE_FILES}
            ${HEADER_FILES}
            ${REFLECTION_SOURCE}
            ${REFLECTION_HEADER}
            ${moduleExternalFiles}
        )
    endif()

    target_include_directories(
        ${moduleName}
        PRIVATE
        ${moduleIncludeFiles}
    )

    target_link_libraries(
        ${moduleName}
        ${moduleLinkLibraries}
    )

    source_group(TREE ${CMAKE_SOURCE_DIR}/include/${moduleName} PREFIX "Header Files" FILES ${HEADER_FILES})
    source_group(TREE ${CMAKE_SOURCE_DIR}/__generated_reflection__/${moduleName} PREFIX "Reflection Source Files" FILES ${REFLECTION_SOURCE})
    source_group(TREE ${CMAKE_SOURCE_DIR}/__generated_reflection__/${moduleName} PREFIX "Reflection Header Files" FILES ${REFLECTION_HEADER})
    source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} PREFIX "Source Files" FILES ${SOURCE_FILES})
    source_group("External Files" FILES ${moduleExternalFiles})
endmacro(DUCKVIL_MODULE)

FILE(GLOB children */CMakeLists.txt)

list(APPEND INCLUDES
    "${CMAKE_SOURCE_DIR}/include"
    "${CMAKE_SOURCE_DIR}/__generated_reflection__"
    "${CMAKE_SOURCE_DIR}/external"
    ${CMAKE_SOURCE_DIR}/external/glm
    ${CMAKE_SOURCE_DIR}/external/tracy
    "${CMAKE_SOURCE_DIR}/external/xbyak"
    "${CMAKE_SOURCE_DIR}/external/assimp/include"
)

list(APPEND STATIC_LIBRARIES "")
list(APPEND PROJECTS "")

macro(UTIL_CICD AN_ARGUMENT)
    add_subdirectory(${AN_ARGUMENT})
    get_filename_component(PROJECT_ID ${AN_ARGUMENT} NAME)
    get_target_property(
        TARGET_TYPE ${PROJECT_ID} TYPE
    )
    target_include_directories(
        ${PROJECT_ID} PRIVATE
        ${INCLUDES}
    )
    set_target_properties(${PROJECT_ID} PROPERTIES
        CXX_STANDARD 20
        LIBRARY_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin"
        ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin"
        RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin"
        LIBRARY_OUTPUT_DIRECTORY_DEBUG "${CMAKE_SOURCE_DIR}/bin"
        ARCHIVE_OUTPUT_DIRECTORY_DEBUG "${CMAKE_SOURCE_DIR}/bin"
        RUNTIME_OUTPUT_DIRECTORY_DEBUG "${CMAKE_SOURCE_DIR}/bin"
        LIBRARY_OUTPUT_DIRECTORY_RELEASE "${CMAKE_SOURCE_DIR}/bin"
        ARCHIVE_OUTPUT_DIRECTORY_RELEASE "${CMAKE_SOURCE_DIR}/bin"
        RUNTIME_OUTPUT_DIRECTORY_RELEASE "${CMAKE_SOURCE_DIR}/bin"
        DEBUG_POSTFIX ""
        PREFIX ""
    )
    if(${TARGET_TYPE} STREQUAL "STATIC_LIBRARY")
        list(APPEND STATIC_LIBRARIES ${PROJECT_ID})
    else()
        list(APPEND PROJECTS ${PROJECT_ID})
    endif()
endmacro()

if(NOT DUCKVIL_CICD)
    FOREACH(child ${children})
        get_filename_component(PARENT_DIR ${child} DIRECTORY)
        add_subdirectory(${PARENT_DIR})
        get_filename_component(PROJECT_ID ${PARENT_DIR} NAME)
        get_target_property(
            TARGET_TYPE ${PROJECT_ID} TYPE
        )
        target_include_directories(
            ${PROJECT_ID} PRIVATE
            ${INCLUDES}
        )
        set_target_properties(${PROJECT_ID} PROPERTIES
            CXX_STANDARD 20
            LIBRARY_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin"
            ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin"
            RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin"
            LIBRARY_OUTPUT_DIRECTORY_DEBUG "${CMAKE_SOURCE_DIR}/bin"
            ARCHIVE_OUTPUT_DIRECTORY_DEBUG "${CMAKE_SOURCE_DIR}/bin"
            RUNTIME_OUTPUT_DIRECTORY_DEBUG "${CMAKE_SOURCE_DIR}/bin"
            LIBRARY_OUTPUT_DIRECTORY_RELEASE "${CMAKE_SOURCE_DIR}/bin"
            ARCHIVE_OUTPUT_DIRECTORY_RELEASE "${CMAKE_SOURCE_DIR}/bin"
            RUNTIME_OUTPUT_DIRECTORY_RELEASE "${CMAKE_SOURCE_DIR}/bin"
            DEBUG_POSTFIX ""
            PREFIX ""
        )
        if(${TARGET_TYPE} STREQUAL "STATIC_LIBRARY")
            list(APPEND STATIC_LIBRARIES ${PROJECT_ID})
            if(${DUCKVIL_TRACY_PROFILER})
                target_compile_definitions(${PROJECT_ID} PRIVATE TRACY_ENABLE)
            endif()
        endif()
    ENDFOREACH()

    SET(GLOBAL_STATIC_LIBRARIES  ${STATIC_LIBRARIES} CACHE INTERNAL "Engine static libs" FORCE)

    FOREACH(child ${children})
        get_filename_component(PARENT_DIR ${child} DIRECTORY)
        get_filename_component(PROJECT_ID ${PARENT_DIR} NAME)
        get_target_property(
            TARGET_TYPE ${PROJECT_ID} TYPE
        )
        if(${TARGET_TYPE} STREQUAL "EXECUTABLE" OR ${TARGET_TYPE} STREQUAL "SHARED_LIBRARY")
            target_link_libraries(
                ${PROJECT_ID}
                ${STATIC_LIBRARIES}
                ${LIBS}
                flecs_static
            )

            if(${DUCKVIL_TRACY_PROFILER})
                target_link_libraries(
                    ${PROJECT_ID}
                    TracyClient
                )

                target_compile_definitions(${PROJECT_ID} PRIVATE TRACY_ENABLE)
                target_compile_definitions(${PROJECT_ID} PRIVATE TRACY_CALLSTACK=40)
                target_compile_options(${PROJECT_ID} PRIVATE
                    $<$<CXX_COMPILER_ID:MSVC>:/Zi>
                    $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:-g>
                )
                target_link_options(${PROJECT_ID} PRIVATE
                    $<$<CXX_COMPILER_ID:MSVC>:/DEBUG:FULL>
                )
                if(WIN32)
                    target_link_libraries(${PROJECT_ID} dbghelp)
                endif()
                target_compile_options(${PROJECT_ID} PRIVATE
                    $<$<CXX_COMPILER_ID:MSVC>:/GF>
                )
            endif()
        endif()
    ENDFOREACH()
else()
    util_cicd("Utils")
    util_cicd("Memory")
    util_cicd("PlugNPlay")
    util_cicd("RuntimeReflection")
    util_cicd("UniTestFramework")

    SET(GLOBAL_STATIC_LIBRARIES  ${STATIC_LIBRARIES} CACHE INTERNAL "Engine static libs" FORCE)

    FOREACH(project ${PROJECTS})
        target_link_libraries(
            ${project}
            ${STATIC_LIBRARIES}
            ${LIBS}
        )
    ENDFOREACH()
endif()