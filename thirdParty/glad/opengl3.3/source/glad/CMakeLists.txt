cmake_minimum_required(VERSION 3.10)
set(CMAKE_LEGACY_CYGWIN_WIN32 OFF)

project(glad C)

add_library(glad STATIC "src/glad.c")
target_include_directories(glad PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/include")