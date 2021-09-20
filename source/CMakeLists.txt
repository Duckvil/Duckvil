cmake_minimum_required(VERSION 3.16)

option(DUCKVIL_TRACY_PROFILER "Tracy profiler" OFF)

FILE(GLOB children */CMakeLists.txt)

list(APPEND INCLUDES
    "${CMAKE_SOURCE_DIR}/include"
    "${CMAKE_SOURCE_DIR}/__generated_reflection__"
    "${CMAKE_SOURCE_DIR}/external"
    ${CMAKE_SOURCE_DIR}/external/glm
    ${CMAKE_SOURCE_DIR}/external/tracy
    "${CMAKE_SOURCE_DIR}/external/xbyak"
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
                if(NOT ${PROJECT_ID} STREQUAL "_tracy")
                    target_link_libraries(
                        ${PROJECT_ID}
                        _tracy
                    )
                endif()

                target_compile_definitions(${PROJECT_ID} PRIVATE TRACY_ENABLE)
                target_compile_definitions(${PROJECT_ID} PRIVATE TRACY_CALLSTACK=40)
                target_compile_options(${PROJECT_ID} PRIVATE
                    $<$<CXX_COMPILER_ID:MSVC>:/Zi>
                    $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:-g>
                )
                target_link_options(${PROJECT_ID} PRIVATE
                    $<$<CXX_COMPILER_ID:MSVC>:/DEBUG:FULL>
                )
                target_link_libraries(${PROJECT_ID} dbghelp)
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