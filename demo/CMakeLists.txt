# For more information about using CMake with Android Studio, read the
# documentation: https://d.android.com/studio/projects/add-native-code.html

# Sets the minimum version of CMake required to build the native library.

cmake_minimum_required(VERSION 3.4.1)

project(demo)

set(SRC_PATH ${CMAKE_CURRENT_LIST_DIR}/src)
set(EGL_PATH ${CMAKE_CURRENT_LIST_DIR}/src/egl)
set(OPENGL_PATH ${CMAKE_CURRENT_LIST_DIR}/src/opengl)
set(DEMO_PATH ${CMAKE_CURRENT_LIST_DIR}/src/demo)
set(UTIL_PATH ${CMAKE_CURRENT_LIST_DIR}/src/util)

file(GLOB CPP_FILE_PATH
        ${SRC_PATH}/*.cpp
        ${EGL_PATH}/*.cpp
        ${OPENGL_PATH}/*.cpp
        ${UTIL_PATH}/*.cpp
        ${DEMO_PATH}/*.cpp
        )

#message(WARNING "CPP_FILE_PATH:${CPP_FILE_PATH}")

add_library(
        demo
        SHARED
        ${CPP_FILE_PATH})

target_include_directories(demo
        PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        )


target_link_libraries(
        demo
        plog
        glm
        log android GLESv2 EGL
)