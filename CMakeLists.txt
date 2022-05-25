cmake_minimum_required(VERSION 3.20)
set(CMAKE_LEGACY_CYGWIN_WIN32 OFF)

project(qdu-engine C CXX)

find_package(OpenGL REQUIRED)
add_definitions(-D_USE_MATH_DEFINES)

set(THIRD_PARTY_INCLUDE_DIRECTORIES "${CMAKE_CURRENT_SOURCE_DIR}/thirdParty/glad/include"
        "${CMAKE_CURRENT_SOURCE_DIR}/thirdParty/glfw/include"
        "${CMAKE_CURRENT_SOURCE_DIR}/thirdParty/eigen"
        "${CMAKE_CURRENT_SOURCE_DIR}/thirdParty/glm"
        "${CMAKE_CURRENT_SOURCE_DIR}/thirdParty/stb"
        "${CMAKE_CURRENT_SOURCE_DIR}/thirdParty/json/include")
set(QDU_INCLUDE_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/source")

if(MSVC)
    set(THIRD_PARTY_LIBRARIES glad glfw ${OPENGL_LIBRARIES})
else()
    set(THIRD_PARTY_LIBRARIES glad glfw ${OPENGL_LIBRARIES})
endif(MSVC)

# Include sub-projects.
configure_file(CMakeConfigFiles/root_directory.h.in "${CMAKE_SOURCE_DIR}/source/grafica/root_directory.h")
add_subdirectory(thirdParty)
add_subdirectory(examples)
add_subdirectory(source)