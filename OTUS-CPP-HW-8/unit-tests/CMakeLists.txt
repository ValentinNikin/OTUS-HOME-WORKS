
set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

find_package(GTest REQUIRED)

set(UNIT_TEST_EXECUTABLE_NAME unit_tests)

add_executable(
    ${UNIT_TEST_EXECUTABLE_NAME}
    ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp
    $<TARGET_OBJECTS:${MAIN_LIBRARY_NAME}>

    Config.cpp
    Config.h

    tests/UnitTest_FileScanner.cpp
    tests/UnitTest_DuplicateFinder.cpp)

target_include_directories(
    ${UNIT_TEST_EXECUTABLE_NAME} 
    PRIVATE
    ${PROJECT_SOURCE_DIR}/src/
    SYSTEM
    ${GTEST_INCLUDE_DIRS}
    ${Boost_INCLUDE_DIRS}
)

target_link_libraries(
    ${UNIT_TEST_EXECUTABLE_NAME} 
    PRIVATE
    ${GTEST_BOTH_LIBRARIES}
    ${CMAKE_THREAD_LIBS_INIT}
    ${Boost_LIBRARIES}
)

add_test(
    NAME test-all
    COMMAND ${UNIT_TEST_EXECUTABLE_NAME}
)