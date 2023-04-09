
set(LIBRARY_NAME bulk_server_lib)
set(EXAMPLE_EXECUTABLE_NAME bulk_server)

configure_file(version.h.in version.h)

# Configure library
add_library(
        ${LIBRARY_NAME} OBJECT

        App.h
        App.cpp

        server/Server.h
        server/Server.cpp
        server/Connection.h
        server/Connection.cpp
        server/ConnectionManager.h
        server/ConnectionManager.cpp
        server/IRequestHandler.h
        server/IRequestHandlerFactory.h

        server/request-handlers/CommandsRequestHandler.h
        server/request-handlers/CommandsRequestHandler.cpp
        server/request-handlers/CommandsRequestHandlerFactory.h
        server/request-handlers/CommandsRequestHandlerFactory.cpp

        command-bundler/CommandBundler.cpp
        command-bundler/CommandBundler.h
        command-bundler/State.h
        command-bundler/Context.h
        command-bundler/Context.cpp
        command-bundler/Command.cpp
        command-bundler/Command.h

        processors/ThreadSafeQueue.cpp
        processors/ThreadSafeQueue.h
        processors/IBlockProcessor.h
        processors/MultiThreadProcessor.cpp
        processors/MultiThreadProcessor.h
        processors/threads/ThreadProcessor.cpp
        processors/threads/ThreadProcessor.h
        processors/threads/FileThreadProcessor.cpp
        processors/threads/FileThreadProcessor.h
        processors/threads/ConsoleThreadProcessor.cpp
        processors/threads/ConsoleThreadProcessor.h

        types/Block.cpp
        types/Block.h

        utils/Utils.h
        utils/Utils.cpp
        )

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
        ${CMAKE_CURRENT_SOURCE_DIR}/
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
        ${CMAKE_CURRENT_SOURCE_DIR}/
        SYSTEM
        ${PROJECT_DEPS_INCLUDES}
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

install(TARGETS ${EXAMPLE_EXECUTABLE_NAME} RUNTIME DESTINATION bin)