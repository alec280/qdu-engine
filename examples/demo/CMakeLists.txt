cmake_minimum_required(VERSION 3.20)
set(CMAKE_LEGACY_CYGWIN_WIN32 OFF)

project(demo C CXX)

cmake_path(GET CMAKE_SOURCE_DIR PARENT_PATH EXAMPLE_DIR)
cmake_path(GET EXAMPLE_DIR PARENT_PATH REPO_DIR)

add_executable(main "main.cpp")
set_property(TARGET main PROPERTY CXX_STANDARD 20)
target_link_libraries(main PRIVATE
    "${REPO_DIR}/libs/glad/opengl3.3/build/Release/glad.lib"
    "${REPO_DIR}/libs/glfw/build/src/Release/glfw3.lib"
)
target_include_directories(main PRIVATE
    "${REPO_DIR}/libs/glad/opengl3.3/source/glad/include"
    "${REPO_DIR}/libs/glfw/source/glfw/include"
)