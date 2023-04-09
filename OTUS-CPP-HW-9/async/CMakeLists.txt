
set(LIBRARY_NAME async)
set(EXAMPLE_EXECUTABLE_NAME otus-9-example)

configure_file(src/version.h.in version.h)

# Configure library
add_library(
        ${LIBRARY_NAME} SHARED

        include/async.h

        src/async.cpp
        src/Server.h
        src/Server.cpp
        src/command-bundler/CommandBundler.cpp
        src/command-bundler/CommandBundler.h

        src/command-bundler/State.h
        src/command-bundler/Context.h
        src/command-bundler/Context.cpp
        src/processors/ThreadSafeQueue.cpp
        src/processors/ThreadSafeQueue.h

        src/processors/IBlockProcessor.h

        src/processors/MultiThreadProcessor.cpp
        src/processors/MultiThreadProcessor.h

        src/processors/threads/ThreadProcessor.cpp
        src/processors/threads/ThreadProcessor.h
        src/processors/threads/FileThreadProcessor.cpp
        src/processors/threads/FileThreadProcessor.h
        src/processors/threads/ConsoleThreadProcessor.cpp
        src/processors/threads/ConsoleThreadProcessor.h

        src/command-bundler/Command.cpp
        src/command-bundler/Command.h
        src/types/Block.cpp
        src/types/Block.h

        src/utils/Utils.h
        src/utils/Utils.cpp
        )


set_target_properties(${LIBRARY_NAME} PROPERTIES
        VERSION ${MAJOR_VERSION}.${MINOR_VERSION}.${BUILD_VERSION}
        SOVERSION 1
        PUBLIC_HEADER include/async.h)

set_target_properties(
        ${LIBRARY_NAME} PROPERTIES
        CXX_STANDARD 17
        CXX_STANDARD_REQUIRED ON
)

target_link_libraries(
        ${LIBRARY_NAME}
        PRIVATE
        ${PROJECT_DEPS}
)

target_include_directories(
        ${LIBRARY_NAME}
        PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${CMAKE_CURRENT_SOURCE_DIR}/src
        SYSTEM
        ${PROJECT_DEPS_INCLUDES}
)

# Configure executable
add_executable(${EXAMPLE_EXECUTABLE_NAME} main.cpp)

set_target_properties(
        ${EXAMPLE_EXECUTABLE_NAME} PROPERTIES
        CXX_STANDARD 17
        CXX_STANDARD_REQUIRED ON
)

target_include_directories(
        ${EXAMPLE_EXECUTABLE_NAME}
        PRIVATE
        ${PROJECT_DEPS_INCLUDES}
        ${CMAKE_CURRENT_SOURCE_DIR}/include/
)

target_link_libraries(
        ${EXAMPLE_EXECUTABLE_NAME}
        PRIVATE
        ${LIBRARY_NAME}
        ${PROJECT_DEPS}
)

if (BUILD_TESTS)
    enable_testing()
    add_subdirectory(unit-tests)
endif()

include(GNUInstallDirs)

install(TARGETS ${LIBRARY_NAME}
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

install(TARGETS ${EXAMPLE_EXECUTABLE_NAME} RUNTIME DESTINATION bin)