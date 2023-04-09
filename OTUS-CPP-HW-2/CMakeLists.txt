cmake_minimum_required(VERSION 3.2)

set(EXECUTABLE_NAME ip_filter)
set(MAIN_LIBRARY_NAME ip_filter_lib)

set(PATCH_VERSION "1" CACHE INTERNAL "Patch version")
set(PROJECT_VERSION 0.0.${PATCH_VERSION})

option(ENABLED_TESTING ON)

project(otus-cpp-hw-2 VERSION ${PROJECT_VERSION})

configure_file(
    ${PROJECT_SOURCE_DIR}/version.h.in 
    ${PROJECT_SOURCE_DIR}/src/version.h)

add_executable(
    ${EXECUTABLE_NAME}
    ${PROJECT_SOURCE_DIR}/src/main.cpp
)

add_library(
    ${MAIN_LIBRARY_NAME}
    ${PROJECT_SOURCE_DIR}/src/utils.h
    ${PROJECT_SOURCE_DIR}/src/utils.cpp
    ${PROJECT_SOURCE_DIR}/src/ip.h
    ${PROJECT_SOURCE_DIR}/src/ip.cpp
)

set_target_properties(
    ${EXECUTABLE_NAME} ${MAIN_LIBRARY_NAME} PROPERTIES
    CXX_STANDARD 14
    CXX_STANDARD_REQUIRED ON
)

target_include_directories(
    ${EXECUTABLE_NAME} 
    PRIVATE 
    ${PROJECT_SOURCE_DIR}/src/
)

target_include_directories(
    ${MAIN_LIBRARY_NAME}
    PRIVATE 
    ${PROJECT_SOURCE_DIR}/src/
)

target_compile_options(
    ${EXECUTABLE_NAME} 
    PRIVATE
    -Wall -Wextra -pedantic -Werror
)

target_compile_options(
    ${MAIN_LIBRARY_NAME} 
    PRIVATE
    -Wall -Wextra -pedantic -Werror
)

target_link_libraries(
    ${EXECUTABLE_NAME} 
    PRIVATE
    ${MAIN_LIBRARY_NAME}
)

if (ENABLED_TESTING) 
    enable_testing()
    add_subdirectory(unit-tests)
endif()

install(TARGETS ${EXECUTABLE_NAME} RUNTIME DESTINATION bin)

set(CPACK_GENERATOR DEB)
set(CPACK_PACKAGE_VERSION_MAJOR "${PROJECT_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${PROJECT_VERSION_MINOR}")
set(CPACK_PACKAGE_VERSION_PATCH "${PROJECT_VERSION_PATCH}")
set(CPACK_PACKAGE_CONTACT example@example.com)
include(CPack)
