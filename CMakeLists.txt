cmake_minimum_required(VERSION 3.20)
set(CMAKE_LEGACY_CYGWIN_WIN32 OFF)

project(qdu-engine C CXX)

find_package(OpenGL REQUIRED)
set(THIRD_PARTY_INCLUDE_DIRECTORIES "${CMAKE_CURRENT_SOURCE_DIR}/thirdParty/glad/include"
        "${CMAKE_CURRENT_SOURCE_DIR}/thirdParty/glfw/include")
set(QDU_INCLUDE_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/source")

if(MSVC)
    set(THIRD_PARTY_LIBRARIES glad glfw ${OPENGL_LIBRARIES})
else()
    set(THIRD_PARTY_LIBRARIES glad glfw ${OPENGL_LIBRARIES})
endif(MSVC)

# Include sub-projects.
add_subdirectory(thirdParty)
add_subdirectory(examples)
add_subdirectory(source)