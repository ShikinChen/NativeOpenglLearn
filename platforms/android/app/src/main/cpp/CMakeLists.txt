# For more information about using CMake with Android Studio, read the
# documentation: https://d.android.com/studio/projects/add-native-code.html

# Sets the minimum version of CMake required to build the native library.

cmake_minimum_required(VERSION 3.4.1)

# Declares and names the project.

project("NativeOpenglLearn")


file(GLOB CPP_FILE_PATH
        ${CMAKE_CURRENT_LIST_DIR}/*.cpp
        )

add_library(
        demo_android
        SHARED
        ${CPP_FILE_PATH})

find_library(log-lib log)
find_library(android-lib android)
find_library(glesv2-lib GLESv2)
find_library(egl-lib EGL)

target_link_libraries(
        demo_android
        ${log-lib} ${android-lib} ${glesv2-lib} ${egl-lib})