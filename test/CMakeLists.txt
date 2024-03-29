cmake_minimum_required(VERSION 3.16)

FILE(GLOB_RECURSE children */CMakeLists.txt)

set(DUCKVIL_TEST_OUTPUT ${CMAKE_CURRENT_SOURCE_DIR}/Output)
add_definitions(-DDUCKVIL_TEST_OUTPUT="${DUCKVIL_TEST_OUTPUT}")

list(APPEND INCLUDES
    "${CMAKE_SOURCE_DIR}/include"
    "${CMAKE_SOURCE_DIR}/__generated_reflection__"
    "${CMAKE_SOURCE_DIR}"
    "${CMAKE_SOURCE_DIR}/external"
)

FOREACH(child ${children})
    get_filename_component(PARENT_DIR ${child} DIRECTORY)
    add_subdirectory(${PARENT_DIR})
    get_filename_component(PROJECT_ID ${PARENT_DIR} NAME)
    set_target_properties(${PROJECT_ID} PROPERTIES
        CXX_STANDARD 20
        LIBRARY_OUTPUT_DIRECTORY ${DUCKVIL_TEST_OUTPUT}
        ARCHIVE_OUTPUT_DIRECTORY ${DUCKVIL_TEST_OUTPUT}
        RUNTIME_OUTPUT_DIRECTORY ${DUCKVIL_TEST_OUTPUT}
        LIBRARY_OUTPUT_DIRECTORY_DEBUG ${DUCKVIL_TEST_OUTPUT}
        ARCHIVE_OUTPUT_DIRECTORY_DEBUG ${DUCKVIL_TEST_OUTPUT}
        RUNTIME_OUTPUT_DIRECTORY_DEBUG ${DUCKVIL_TEST_OUTPUT}
        LIBRARY_OUTPUT_DIRECTORY_RELEASE ${DUCKVIL_TEST_OUTPUT}
        ARCHIVE_OUTPUT_DIRECTORY_RELEASE ${DUCKVIL_TEST_OUTPUT}
        RUNTIME_OUTPUT_DIRECTORY_RELEASE ${DUCKVIL_TEST_OUTPUT}
        DEBUG_POSTFIX ""
        PREFIX "run"
        FOLDER tests
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
    if(WIN32)
        target_link_libraries(${PROJECT_ID} dbghelp)
    endif()
    add_test(NAME ${PROJECT_ID} COMMAND ${PROJECT_ID})
ENDFOREACH()

set_target_properties(TestType PROPERTIES FOLDER tests)