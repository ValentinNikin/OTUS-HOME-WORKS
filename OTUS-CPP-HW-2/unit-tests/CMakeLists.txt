
set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

find_package(GTest REQUIRED)

set(UNIT_TEST_EXECUTABLE_NAME unit_tests)

add_executable(
    ${UNIT_TEST_EXECUTABLE_NAME}
    ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/unit-test-ip.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/unit-test-utils.cpp)

target_include_directories(
    ${UNIT_TEST_EXECUTABLE_NAME} 
    PRIVATE
    ${PROJECT_SOURCE_DIR}/src/
    ${GTEST_INCLUDE_DIRS})

target_link_libraries(
    ${UNIT_TEST_EXECUTABLE_NAME} 
    PRIVATE
    ${MAIN_LIBRARY_NAME}
    ${GTEST_BOTH_LIBRARIES}
    ${CMAKE_THREAD_LIBS_INIT})

add_test(
    NAME test-all
    COMMAND ${UNIT_TEST_EXECUTABLE_NAME}
)