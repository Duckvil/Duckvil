cmake_minimum_required(VERSION 3.17)

FILE(GLOB SRC_FILES *.cpp)
FILE(GLOB TEST_TYPE_SRC_FILES TestType/*.cpp)

add_definitions(-DDUCKVIL_TEST_OUTPUT="${CMAKE_SOURCE_DIR}/test/Debug")

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
)

set_target_properties(TestType PROPERTIES
    CXX_STANDARD 20
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/test"
    ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/test"
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/test"
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