cmake_minimum_required(VERSION 3.16)

FILE(GLOB_RECURSE SRC_FILES *.cpp)

add_executable(
    TestUtils
    ${SRC_FILES}
)

target_include_directories(
    TestUtils
    PRIVATE
    ${INCLUDES}
)

target_link_libraries(
    TestUtils
    ${GLOBAL_STATIC_LIBRARIES}
    ${LIBS}
)