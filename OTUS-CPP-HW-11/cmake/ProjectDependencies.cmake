set(PROJECT_DEPS)
set(PROJECT_DEPS_INCLUDES)

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

list(APPEND PROJECT_DEPS ${CMAKE_THREAD_LIBS_INIT})

find_package(Boost COMPONENTS system regex REQUIRED)
list(APPEND PROJECT_DEPS_INCLUDES ${Boost_INCLUDE_DIR})
list(APPEND PROJECT_DEPS ${Boost_LIBRARIES})

find_package(SQLite3 REQUIRED)
list(APPEND PROJECT_DEPS_INCLUDES ${SQLite3_INCLUDE_DIRS})
list(APPEND PROJECT_DEPS ${SQLite3_LIBRARIES})