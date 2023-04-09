cmake_minimum_required(VERSION 3.2)

set(OTUS_CPP_HW_1_EXECUTABLE_NAME helloworld_cli)
set(OTUS_CPP_HW_1_LIBRARY_NAME helloworld)

set(PATCH_VERSION "1" CACHE INTERNAL "Patch version")
set(PROJECT_VESRION 0.0.${PATCH_VERSION})

option(ENABLED_TESTING ON)

project(otus-cpp-hw-1 VERSION ${PROJECT_VESRION})

configure_file(
    ${PROJECT_SOURCE_DIR}/lib/version.h.in 
    ${PROJECT_SOURCE_DIR}/lib/version.h)

add_executable(
    ${OTUS_CPP_HW_1_EXECUTABLE_NAME}
    ${PROJECT_SOURCE_DIR}/src/main.cpp)
add_library(
    ${OTUS_CPP_HW_1_LIBRARY_NAME} 
    ${PROJECT_SOURCE_DIR}/lib/lib.h
    ${PROJECT_SOURCE_DIR}/lib/lib.cpp)

set_target_properties(
    ${OTUS_CPP_HW_1_EXECUTABLE_NAME} ${OTUS_CPP_HW_1_LIBRARY_NAME} PROPERTIES
    CXX_STANDARD 14
    CXX_STANDARD_REQUIRED ON
)

target_include_directories(
    ${OTUS_CPP_HW_1_EXECUTABLE_NAME} 
    PRIVATE 
    ${PROJECT_SOURCE_DIR}/lib/
)

target_include_directories(
    ${OTUS_CPP_HW_1_LIBRARY_NAME} 
    PRIVATE 
    ${PROJECT_SOURCE_DIR}/lib/
)

target_link_libraries(
    ${OTUS_CPP_HW_1_EXECUTABLE_NAME} 
    PRIVATE
    ${OTUS_CPP_HW_1_LIBRARY_NAME}
)

target_compile_options(
    ${OTUS_CPP_HW_1_EXECUTABLE_NAME} 
    PRIVATE
    -Wall -Wextra -pedantic -Werror
)
target_compile_options(
    ${OTUS_CPP_HW_1_LIBRARY_NAME} 
    PRIVATE
    -Wall -Wextra -pedantic -Werror
)

if (ENABLED_TESTING) 
    enable_testing()
    add_subdirectory(unit-tests)
endif()

install(TARGETS ${OTUS_CPP_HW_1_EXECUTABLE_NAME} RUNTIME DESTINATION bin)

set(CPACK_GENERATOR DEB)
set(CPACK_PACKAGE_VERSION_MAJOR "${PROJECT_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${PROJECT_VERSION_MINOR}")
set(CPACK_PACKAGE_VERSION_PATCH "${PROJECT_VERSION_PATCH}")
set(CPACK_PACKAGE_CONTACT example@example.com)
include(CPack)
