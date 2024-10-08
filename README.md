# Grid

## Repository Structure
- `src` directory contains all the source code that we will be editing.
    * It has all the .cpp and .h files.
- `bin` directory contains the executable output program and SFML dlls (which are important for dynamic linking).
    * The final executable i.e `grid.exe` will be here along with .dll files that are necessary to run the program.
- `dependencies` directory has all the libraries' header files and lib files.
    * For now, it has SFML and ImGui (ImGuiFileDialog and dirent were also added here for file chooser).
    * In each library directory there is a `lib` and `include` directory. `lib` will typically contain all the function definitions etc for the library. `include` will contain mostly header files (it may contain .cpp files as well (imgui does)) that we'll include in out source code.
- `scripts` directory contains the scripts to build the entire project (some scripts will build part of it).
- `Makefile` is an alternative to `build.sh` from `scripts`.
- `fonts` directory just has a font which may be removed, we'll see.

## Our Source Code (`src` directory)
- `main.cpp` has the `main()` function and the *main* `while` loop. In `main()` a window is created and then in the loop, events are handled, the state of the program is updated and finally the state of the program is rendered / drawn (very similar to a structure of a game).
- `globals.h` has global variables that will be refernced by other .cpp files through the `extern` keyword.
- `vec2.h` declares a Mathematical 2D Vector. This will be used to represent (x, y) positions in the image later on. `vec2.cpp` contains definitions for each function of `vec2`.
- `util.h`and `util.cpp` uses a namespace to declare and define some useful general helper functions
- `gui.h` and `gui.cpp` define a struct that has useful variables that ImGui will modify for us and then we'll use those variables to do useful stuff. They also define very simple functions that take an instance of that variable struct to modify it. These functions are simple and just call the very nice and convenient ImGui functions to display the GUI. And then these functions are called
- Note: ImGui's functions are mostly in `ImGui::` namespace and SFML's functions are mostly in the `sf::` namespace.

## Building and Running the Entire Program
- First run `./imgui.sh` to compile ImGui source code.
    *This is so that ImGui is first compiled from source code to object files and stored in `dependencies/imgui/lib`. These object files are necessary in the linking stage done so by `Makefile` or `build.sh`.
- Then run one of the following to compiler and run the entire program:
    * `make` in the terminal, if you have make installed. `make` uses the `Makefile`. The advantage of `make` is that it does not recompile everything for no reason. It only compiles and links the recently changed files.
    * Or `./build.sh`. On windows, I recommend getting the git bash terminal (or the sh.exe executable) that comes with [git](https://git-scm.com/) to run the scripts. `build.sh` is good and all but it will recompile everything, and so will take longer to run and test the program after tiny changes.
- `imgui.sh` is just thier to automatically compile (only compile not link) the imgui source code. It just automates some manual labour. Probably not that important but is handy.
- `build.sh`(shell script) will compile and link our source code to libraries and run the executable.
- `Makefile` uses `make` to do the exact same thing as `build.sh` but in a more efficient way.
- `bin` directory will contain the final executable along with SFML dll files to dynamically link us to SFML. The program won't run without these dll files.