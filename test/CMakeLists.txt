cmake_minimum_required(VERSION 3.17)

FILE(GLOB_RECURSE children */CMakeLists.txt)

list(APPEND INCLUDES
    "${CMAKE_SOURCE_DIR}/include"
    "${CMAKE_SOURCE_DIR}/__generated_reflection__"
    "${CMAKE_SOURCE_DIR}"
)

FOREACH(child ${children})
    get_filename_component(PARENT_DIR ${child} DIRECTORY)
    add_subdirectory(${PARENT_DIR})
    get_filename_component(PROJECT_ID ${PARENT_DIR} NAME)
    get_target_property(
        TARGET_TYPE ${PROJECT_ID} TYPE
    )
    set_target_properties(${PROJECT_ID} PROPERTIES
        CXX_STANDARD 20
        LIBRARY_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}"
        ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}"
        RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}"
        DEBUG_POSTFIX ""
        PREFIX "run"
    )
    target_include_directories(
        ${PROJECT_ID} PRIVATE
        ${INCLUDES}
    )
    target_link_libraries(
        ${PROJECT_ID}
        ${GLOBAL_STATIC_LIBRARIES}
        ${LIBS}
    )
    add_test(NAME ${PARENT_DIR} COMMAND ${CMAKE_COMMAND} -E ENVIRONMENT CTEST_OUTPUT_ON_FAILURE=1)
ENDFOREACH()