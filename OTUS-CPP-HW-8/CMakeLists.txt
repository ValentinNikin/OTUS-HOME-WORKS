cmake_minimum_required(VERSION 3.2)

set(EXECUTABLE_NAME bayan)
set(MAIN_LIBRARY_NAME bayan_lib)

set(PATCH_VERSION "1" CACHE INTERNAL "Patch version")
set(PROJECT_VERSION 0.0.${PATCH_VERSION})

option(ENABLED_TESTING OFF)

project(otus-cpp-hw-8 VERSION ${PROJECT_VERSION})

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

find_package(Boost COMPONENTS program_options filesystem regex REQUIRED)

configure_file(
        ${PROJECT_SOURCE_DIR}/version.h.in
        ${PROJECT_SOURCE_DIR}/src/version.h)

add_library(
        ${MAIN_LIBRARY_NAME} OBJECT

        src/utils/ArgumentsReader.cpp
        src/utils/ArgumentsReader.h
        src/utils/Arguments.h

        src/types/FileInfo.h

        src/file-scanner/FileScanner.cpp
        src/file-scanner/FileScanner.h

        src/duplicate-finder/hash/HashCalculator.h

        src/duplicate-finder/DuplicateFinder.cpp
        src/duplicate-finder/DuplicateFinder.h

        src/duplicate-finder/FileBlockHashReader.cpp
        src/duplicate-finder/FileBlockHashReader.h

        src/duplicate-finder/FileComparator.cpp
        src/duplicate-finder/FileComparator.h

        src/duplicate-finder/hash/Crc32Calculator.cpp
        src/duplicate-finder/hash/Crc32Calculator.h

        src/duplicate-finder/hash/DefaultHashCalculator.cpp
        src/duplicate-finder/hash/DefaultHashCalculator.h
)

add_executable(
        ${EXECUTABLE_NAME}
        ${PROJECT_SOURCE_DIR}/src/main.cpp
        $<TARGET_OBJECTS:${MAIN_LIBRARY_NAME}>)

set_target_properties(
        ${EXECUTABLE_NAME} ${MAIN_LIBRARY_NAME} PROPERTIES
        CXX_STANDARD 17
        CXX_STANDARD_REQUIRED ON
)

target_include_directories(
        ${EXECUTABLE_NAME}
        PRIVATE
        ${PROJECT_SOURCE_DIR}/src/
        ${Boost_INCLUDE_DIRS}
)

target_include_directories(
        ${MAIN_LIBRARY_NAME}
        PRIVATE
        ${PROJECT_SOURCE_DIR}/src/
        ${Boost_INCLUDE_DIRS}
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
        ${MAIN_LIBRARY_NAME}
        PRIVATE
        ${Boost_LIBRARIES}
)

target_link_libraries(
        ${EXECUTABLE_NAME}
        PRIVATE
        ${MAIN_LIBRARY_NAME}
        ${CMAKE_THREAD_LIBS_INIT}
        ${Boost_LIBRARIES}
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