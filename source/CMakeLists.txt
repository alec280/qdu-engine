set(QDU_HEADERS
        grafica/basic_shapes.h
        grafica/easy_shaders.h
        grafica/gpu_shape.h
        grafica/load_shaders.h
        grafica/performance_monitor.h
        grafica/scene_graph.h
        grafica/shape.h
        grafica/simple_eigen.h
        grafica/transformations.h
        grafica/simple_timer.h
        input/input.hpp
        game_object/game_object.hpp
        game_object/input_component.hpp
        game_object/visual_component.hpp
        game_object/attribute_component.hpp
        game_object/audio_component.hpp
        map/map.hpp
        core/vector_2d.hpp
        core/vector_3d.hpp
        core/dictionary.hpp
        scene/scene.hpp
        scene/window.hpp
        scene/navigation_mesh.hpp
        audio/audio.hpp
        audio/audio_source.hpp
        audio/audio_stream.hpp
        application.hpp
        )
set(QDU_SOURCES
        grafica/basic_shapes.cpp
        grafica/easy_shaders.cpp
        grafica/gpu_shape.cpp
        grafica/load_shaders.cpp
        grafica/performance_monitor.cpp
        grafica/scene_graph.cpp
        grafica/shape.cpp
        grafica/transformations.cpp
        input/input.cpp
        game_object/game_object.cpp
        game_object/input_component.cpp
        game_object/visual_component.cpp
        game_object/attribute_component.cpp
        game_object/audio_component.cpp
        map/map.cpp
        core/vector_2d.cpp
        core/vector_3d.cpp
        core/dictionary.cpp
        scene/scene.cpp
        scene/window.cpp
        scene/navigation_mesh.cpp
        audio/audio.cpp
        audio/audio_source.cpp
        audio/audio_stream.cpp
        application.cpp
        )

add_library(QDUEngine STATIC ${QDU_SOURCES} ${QDU_HEADERS} QDUEngine.hpp ${Shaders})
if (MSVC)
    target_compile_options(QDUEngine PUBLIC /wd5033)
endif(MSVC)
target_include_directories(QDUEngine PRIVATE ${THIRD_PARTY_INCLUDE_DIRECTORIES} QDU_INCLUDE_DIRECTORY)
target_link_libraries(QDUEngine PRIVATE ${THIRD_PARTY_LIBRARIES})
set_property(TARGET QDUEngine PROPERTY CXX_STANDARD 20)
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${QDU_SOURCES} ${QDU_HEADERS})