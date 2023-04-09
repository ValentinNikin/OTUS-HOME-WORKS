
set(LIBRARY_NAME mapreduce_lib)
set(EXAMPLE_EXECUTABLE_NAME mapreduce)

configure_file(version.h.in version.h)

# Configure library
add_library(
        ${LIBRARY_NAME} OBJECT

        map-reducer/include/IMapper.h
        map-reducer/include/IReducer.h
        map-reducer/include/MapReducer.h

        map-reducer/src/Range.h
        map-reducer/src/Splitter.h
        map-reducer/src/Splitter.cpp

        map-reducer/src/MapperTask.h
        map-reducer/src/MapperTask.cpp

        map-reducer/src/ReducerTask.h
        map-reducer/src/ReducerTask.cpp

        map-reducer/src/MapReducer.cpp

        map-reducer/src/Shuffler.h
        map-reducer/src/Shuffler.cpp

        map-reducer/src/FileLinesReader.h
        map-reducer/src/FileLinesReader.cpp

        UserMapper.h
        UserMapper.cpp
        UserReducer.h
        UserReducer.cpp
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