cmake_minimum_required(VERSION 3.4.1)

project(NativeOpenglLearn)
if (NOT DEMO_PLATFORM)
    string(TOLOWER "${CMAKE_SYSTEM_NAME}" SYSTEM_NAME)
    if (${SYSTEM_NAME} STREQUAL "darwin")
        set(DEMO_PLATFORM "osx")
    else ()
        set(DEMO_PLATFORM ${SYSTEM_NAME})
    endif ()
endif ()

set(DEMO_PLATFORM_CONFIG_FILE "platforms/${DEMO_PLATFORM}/config.cmake")
include(${DEMO_PLATFORM_CONFIG_FILE})
add_subdirectory(demo)

