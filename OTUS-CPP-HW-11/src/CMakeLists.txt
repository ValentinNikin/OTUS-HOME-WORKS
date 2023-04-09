
set(LIBRARY_NAME join_server_lib)
set(EXAMPLE_EXECUTABLE_NAME join_server)

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
        server/request-handlers/RequestParser.h
        server/request-handlers/RequestParser.cpp
        server/request-handlers/TablesManagerRequestHandler.h
        server/request-handlers/TablesManagerRequestHandler.cpp
        server/request-handlers/TablesManagerRequestHandlerFactory.h
        server/request-handlers/TablesManagerRequestHandlerFactory.cpp

        db/DatabaseContext.h
        db/DatabaseContext.cpp
        db/DBTable.h
        db/DBTable.cpp
        db/DBBuilder.h
        db/DBBuilder.cpp

        tables-manager/ITablesManager.h
        tables-manager/DBTablesManager.h
        tables-manager/DBTablesManager.cpp

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