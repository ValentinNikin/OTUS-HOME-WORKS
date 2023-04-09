
set(UNIT_TEST_EXECUTABLE_NAME unit_tests)

find_package(GTest REQUIRED)

add_executable(
    ${UNIT_TEST_EXECUTABLE_NAME}
    main.cpp

    UnitTest_CommandBundler.cpp)

target_include_directories(
    ${UNIT_TEST_EXECUTABLE_NAME}
    PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/../include
    ${CMAKE_CURRENT_SOURCE_DIR}/../src
    SYSTEM
    ${PROJECT_DEPS_INCLUDES}
    ${GTEST_INCLUDE_DIRS}
)

target_link_libraries(
    ${UNIT_TEST_EXECUTABLE_NAME} 
    PRIVATE
    ${LIBRARY_NAME}
    ${PROJECT_DEPS}
    ${GTEST_BOTH_LIBRARIES}
)

add_test(
    NAME test-all
    COMMAND ${UNIT_TEST_EXECUTABLE_NAME}
)