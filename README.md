# qdu-engine

C++ Game engine for top-down roguelikes.

## Building (Windows)

- Clone the repository.
- Inside the repository type: `git submodule update --init --recursive`
- Build the game engine with Visual Studio 17 2022:
  - `mkdir build`
  - `cd build`
  - `cmake ..`
  - Open the solution: `qdu-engine.sln`
  - Build the solution in Release x64 mode.
  
## Execute demo (Windows)

- From the build folder, go to and execute the demo:
  - `cd Examples/Release`
  - `Demo.exe`