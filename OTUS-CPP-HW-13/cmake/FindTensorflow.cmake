# Tensorflow_FOUND
# Tensorflow_INCLUDE_DIRS
# Tensorflow_LIBRARIES

set(Tensorflow_FOUND FALSE)

include(FetchContent)
FetchContent_Declare(tensorflow URL "https://storage.googleapis.com/tensorflow/libtensorflow/libtensorflow-cpu-linux-x86_64-2.7.0.tar.gz")
FetchContent_MakeAvailable(tensorflow)
if (NOT tensorflow_POPULATED)
    FetchContent_Populate(tensorflow)
endif()

find_path(Tensorflow_INCLUDE_DIRS tensorflow/c/c_api.h
        PATHS
        /usr/local/include
        ${tensorflow_SOURCE_DIR}/include/ )
if (Tensorflow_INCLUDE_DIRS)
    message(STATUS "Found: ${Tensorflow_INCLUDE_DIRS}")
endif()

find_library(Tensorflow_LIBRARIES
        NAMES libtensorflow.so
        PATHS
        /usr/local/lib
        ${tensorflow_SOURCE_DIR}/lib/)
if (Tensorflow_LIBRARIES)
    message(STATUS "Found: ${Tensorflow_LIBRARIES}")
endif()

if(Tensorflow_INCLUDE_DIRS)
    set(Tensorflow_FOUND TRUE)
    message(STATUS "Found: ${Tensorflow_INCLUDE_DIRS} (${Tensorflow_LIBRARIES})")
else()
    message(FATAL_ERROR "Error! Tensorflow_INCLUDE_DIRS Not found!")
    set(Tensorflow_FOUND FALSE)
endif()

mark_as_advanced(Tensorflow_FOUND)
