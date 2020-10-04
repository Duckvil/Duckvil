cmake_minimum_required(VERSION 3.17)

FILE(GLOB_RECURSE SRC_FILES *.cpp)

add_executable(
    TestRuntimeReflection
    ${SRC_FILES}
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