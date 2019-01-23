Requirements:
- CMake
- GCC or Clang

Build instruction:
- run ./run-cmake.sh
  - you must specify debug/release and gcc/clang
  - to build for release and gcc, run:
    ./run-cmake.sh release gcc

- cd into either build-release or build-debug
- run make
- run the program
  - release builds are under bin/
  - debug builds are under bin/debug/

- program args:
  - SIZE (int): the size of the board
  - GENERATIONS (int): the maximum number of generations to simulate
  - CHANCE (float): number on [0, 1], the chance of a cell being
    alive or dead upon initialization
