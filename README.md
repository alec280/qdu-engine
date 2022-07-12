# qdu-engine

C++ Game engine for top-down roguelikes. It also includes an example.

## Dependencies
**qdu-engine** relies on third-party libraries, these can be found under the `thirdParty` folder.
 - [ImGui](https://github.com/ocornut/imgui) for debug GUI.
 - [GLFW](https://www.glfw.org/) to create and manage windows and user inputs.
 - [glad](https://glad.dav1d.de/) to load OpenGL functions. 
 - [glm](https://glm.g-truc.net/0.9.9/index.html) for lineal algebra.
 - [stb\_image](https://github.com/nothings/stb) to load images from disk.
 - [dr_wav](https://mackron.github.io/dr_wav.html) to load .wav sound files from disk.
 - [OpenAL-Soft](https://github.com/kcat/openal-soft) to manage audio.

## Software requirements
Building the engine requires [CMake 3.20+](https://cmake.org/) and OpenGL4.5.

## Building (Windows)
- Clone the repository.
- Build the game engine with Visual Studio 17 2022:
  - `mkdir build`
  - `cd build`
  - `cmake ..`
  - Open the solution: `qdu-engine.sln`
  - Build the solution in Release x64 mode.
- In a separate directory, build OpenAl soft according to the instructions in: https://github.com/kcat/openal-soft
  
### Build in debug mode
Building in debug mode allows to access a visual debugger with F5. This requires to build the first solution in `Debug x64 mode` instead.
  
## Execute demo (Windows)
- From the build folder, go to and execute the demo:
  - `cd Examples/Release`
  - `Demo.exe`
- Add the result of building OpenAL soft to the same directory as `Demo.exe` (will result in `missing .dll file` otherwise).
  
For debug mode, replace `Release` with `Debug`.

## Creating applications with the engine
A new application can be created with CMake by including the engine as a subdirectory of the project.
The project structure should be as follows:
- A folder called `qdu-engine` which contains the repository files.
- A CMakeLists.txt file similar to the one presented below, where `MyGame` is the name of the project, `main.cpp` the main file (should include the `<QDUEngine.hpp>` header), and `MyGameDir` the root folder of the project.
- Additional headers can be specified after `main.cpp`.

```
set(CMAKE_LEGACY_CYGWIN_WIN32 OFF)
get_filename_component(CODE_ROOT ${CMAKE_CURRENT_SOURCE_DIR} DIRECTORY)
cmake_minimum_required(VERSION 3.20)
project(MyGame C CXX)
add_subdirectory(${CODE_ROOT}/MyGameDir/qdu-engine)
link_directories(${CODE_ROOT}/MyGameDir/qdu-engine)
add_executable(MyGame main.cpp)
set_property(TARGET MyGame PROPERTY CXX_STANDARD 20)
target_link_libraries(MyGame PRIVATE QDUEngine)
target_include_directories(MyGame PRIVATE ${QDU_INCLUDE_DIRECTORY} ${THIRD_PARTY_INCLUDE_DIRECTORIES})
```
Note that executables created this way will also require the OpenAL soft files built before.

### Quick API
The engine seeks to provide utilities according to the Entity-Component-System architecture. What follows is a brief description of it's principal classes.

- **Application**: Any application created should extend this class, it contains a scene where to put game objects. It also controls global settings, such as key bindings, window size, and temporary directories. Methods to override:
  - `userStart()` for initialization.
  - `onTransition()` to handle scene changes.

- **GameObject**: Entity in ECS. Can have any (or none) of the following components:

  - **InputComponent**: Allows to handle actions. Methods to override:
    - `onAction()` to handle keyboard keys and joysticks.
    - `onCursorAction()` to handle cursor clicks.
    - `onUpdate()` to handle actions dictated by time.

  - **AudioComponent**: Allows to play audio from WAV files.
  - **VisualComponent**: Allows to represent the object in 3D.

- **Scene**: Contains the active GameObjects. Can be switch, stored, and loaded from JSON files.

- **NavigationMesh**: Allows to handle pathfinding inside a Scene with the use of A*. It can be loaded from a OBJ file (requires triangular faces).

For more information, review the `demo.cpp` example inside the `examples` folder.

## FAQ
Q. `saveGameObject()` and `saveScene()` don't create a file. Why? <br>
A. Those functions don't create folders, the specified dir path must be created beforehand.