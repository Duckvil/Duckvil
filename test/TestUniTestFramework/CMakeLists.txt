cmake_minimum_required(VERSION 3.17)

FILE(GLOB_RECURSE SRC_FILES *.cpp)

add_executable(
    TestUniTestFramework
    ${SRC_FILES}
)

target_include_directories(
    TestUniTestFramework
    PRIVATE
    ${INCLUDES}
)

target_link_libraries(
    TestUniTestFramework
    ${GLOBAL_STATIC_LIBRARIES}
    ${LIBS}
)