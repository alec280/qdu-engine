# qdu-engine

C++ Game engine for top-down roguelikes

## Building (Windows)

- Clone the repository
- Inside the repository type: `git submodule update --init --recursive`
- Build the game engine with VS 2022
  - `mkdir build`
  - `cd build`
  - `cmake ../src`
  - Open the .sln
  - Build the solution in Release mode
  - Executable is generated at `build/Release/main.exe`
- Relative to the root, build the demo inside in a build folder next to its src folder
  - `cd examples/demo`
  - `mkdir build`
  - `cd build`
  - `cmake ../src`
  - Open the .sln
  - Build the solution in Release or Debug mode
  - Executable is generated at `examples/demo/{Release|Debug}/main.exe`
  
## Execution (Windows)

Relative to the root, execute `examples/demo/{Release|Debug}/main.exe` according to the solution built