cmake_minimum_required(VERSION 3.20)
set(CMAKE_LEGACY_CYGWIN_WIN32 OFF)

project(qdu-engine C CXX)

add_library(math_lib STATIC
    "src/math/vector3.h"
    "src/math/vector3.cpp"
)

add_executable(main "src/main.cpp")
set_property(TARGET main PROPERTY CXX_STANDARD 20)

target_include_directories(main PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(main PRIVATE math_lib)