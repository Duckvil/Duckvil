cmake_minimum_required(VERSION 3.16)

FILE(GLOB_RECURSE SRC_FILES *.cpp)

add_executable(
    TestThread
    ${SRC_FILES}
)

target_include_directories(
    TestThread
    PRIVATE
    ${INCLUDES}
)

target_link_libraries(
    TestThread
    ${GLOBAL_STATIC_LIBRARIES}
    ${LIBS}
)

if(UNIX)
    target_link_libraries(
        TestThread
        pthread
    )
endif()