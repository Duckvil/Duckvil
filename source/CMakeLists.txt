cmake_minimum_required(VERSION 3.17)

FILE(GLOB children */CMakeLists.txt)

list(APPEND INCLUDES
    "${CMAKE_SOURCE_DIR}/include"
    "${CMAKE_SOURCE_DIR}/__generated_reflection__"
)

list(APPEND STATIC_LIBRARIES "")

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
        )
    endif()
ENDFOREACH()