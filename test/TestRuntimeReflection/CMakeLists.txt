cmake_minimum_required(VERSION 3.16)

FILE(GLOB SRC_FILES *.cpp)
FILE(GLOB TEST_TYPE_SRC_FILES TestType/*.cpp)

add_executable(
    TestRuntimeReflection
    ${SRC_FILES}
)

add_library(
    TestType
    SHARED
    ${TEST_TYPE_SRC_FILES}
)

target_include_directories(
    TestType
    PRIVATE
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/external
)

target_link_libraries(
    TestType
    ${GLOBAL_STATIC_LIBRARIES}
    ${LIBS}
)

if(WIN32)
target_link_libraries(
    TestType
    dbghelp
)
endif()

set_target_properties(TestType PROPERTIES
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
    PREFIX ""
)

target_include_directories(
    TestRuntimeReflection
    PRIVATE
    ${INCLUDES}
)

target_link_libraries(
    TestRuntimeReflection
    ${GLOBAL_STATIC_LIBRARIES}
    ${LIBS}
)