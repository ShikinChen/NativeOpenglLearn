add_definitions(-DDEMO_ANDROID)
add_definitions(-DANDROID)
cmake_minimum_required(VERSION 3.4.1)
## CMAKE_CURRENT_LIST_DIR 当前cmake文件所在目录
ADD_SUBDIRECTORY(${CMAKE_CURRENT_LIST_DIR}/app/src/main/cpp/)
