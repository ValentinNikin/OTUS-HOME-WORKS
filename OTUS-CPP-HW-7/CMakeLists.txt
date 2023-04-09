cmake_minimum_required(VERSION 3.2)

set(EXECUTABLE_NAME bulk)
set(MAIN_LIBRARY_NAME bulk_lib)

set(PATCH_VERSION "1" CACHE INTERNAL "Patch version")
set(PROJECT_VERSION 0.0.${PATCH_VERSION})

option(ENABLED_TESTING OFF)

project(otus-cpp-hw-7 VERSION ${PROJECT_VERSION})

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

configure_file(
        ${PROJECT_SOURCE_DIR}/version.h.in
        ${PROJECT_SOURCE_DIR}/src/version.h)

add_library(
        ${MAIN_LIBRARY_NAME} OBJECT

        src/CommandBundler.cpp
        src/CommandBundler.h

        src/CommandReader.cpp
        src/CommandReader.h

        src/processors/ICommandProcessor.h

        src/commands/Command.cpp
        src/commands/Command.h
        src/commands/Block.cpp
        src/commands/Block.h

        src/processors/ConsoleProcessor.cpp
        src/processors/ConsoleProcessor.h
        src/processors/FileProcessor.cpp
        src/processors/FileProcessor.h
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
        ${CMAKE_THREAD_LIBS_INIT}
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