# For more information about using CMake with Android Studio, read the
# documentation: https://d.android.com/studio/projects/add-native-code.html

# Sets the minimum version of CMake required to build the native library.

cmake_minimum_required(VERSION 3.4.1)

project(demo)

set(SRC_PATH ${CMAKE_CURRENT_LIST_DIR}/src)



file(GLOB PATH_TO_CPP_FILE
        ${SRC_PATH}/*.cpp
        )



add_library(
        demo
        SHARED
        ${PATH_TO_CPP_FILE})

target_include_directories(demo
        PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        )

target_link_libraries(
        demo
)