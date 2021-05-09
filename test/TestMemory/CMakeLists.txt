cmake_minimum_required(VERSION 3.16)

FILE(GLOB_RECURSE SRC_FILES *.cpp)

add_executable(
    TestMemory
    ${SRC_FILES}
)

target_include_directories(
    TestMemory
    PRIVATE
    ${INCLUDES}
)

target_link_libraries(
    TestMemory
    ${GLOBAL_STATIC_LIBRARIES}
    ${LIBS}
)