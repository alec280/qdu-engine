set(QDU_HEADERS
        Engine.hpp
        Application.hpp
        Geometry/Shape.hpp
        Geometry/Shader.hpp
        World/World.hpp)
set(QDU_SOURCES
        Application.cpp
        Geometry/Shape.cpp
        Geometry/Shader.cpp
        World/World.cpp)

add_library(QDUEngine STATIC ${QDU_SOURCES} ${QDU_HEADERS} QDUEngine.hpp)
if (MSVC)
    target_compile_options(QDUEngine PUBLIC /wd5033)
endif(MSVC)
target_include_directories(QDUEngine PRIVATE ${THIRD_PARTY_INCLUDE_DIRECTORIES} QDU_INCLUDE_DIRECTORY)
target_link_libraries(QDUEngine PRIVATE ${THIRD_PARTY_LIBRARIES})
set_property(TARGET QDUEngine PROPERTY CXX_STANDARD 20)
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${QDU_SOURCES} ${QDU_HEADERS})
