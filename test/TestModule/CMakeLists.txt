cmake_minimum_required(VERSION 3.18)

FILE(GLOB_RECURSE SRC_FILES *.cpp)

add_executable(
    TestModule
    ${SRC_FILES}
)

target_include_directories(
    TestModule
    PRIVATE
    ${INCLUDES}
)

target_link_libraries(
    TestModule
    ${GLOBAL_STATIC_LIBRARIES}
    ${LIBS}
)