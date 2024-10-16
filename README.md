# Grid

# PLEASE READ THIS. IT GETS UPDATED ALMOST EVERY PUSH AND IT EXPLAINS A LOT OF STUFF!

## Repository Structure
- `src` directory contains all the source code that we will be editing.
    * It has all the .cpp and .h files.
    * `structures` directory here contains files for implementation of our own data structures that will be needed. For now, we are using STL's data structure. Will change once these are properly implemented.
- `bin` directory contains the executable output program and SFML dlls (which are important for dynamic linking).
    * The final executable i.e `grid.exe` will be here along with .dll files that are necessary to run the program.
- `dependencies` directory has all the libraries' header files and lib files.
    * For now, it has SFML and ImGui (ImGuiFileDialog and dirent were also added here for file chooser).
    * In each library directory there is a `lib` and `include` directory. `lib` will typically contain all the function definitions etc for the library. `include` will contain mostly header files (it may contain .cpp files as well (imgui does)) that we'll include in out source code.
- `scripts` directory contains the scripts to build the entire project (some scripts will build part of it).
- `Makefile` is an alternative to `build.sh` from `scripts`.
- `fonts` directory just has a font which may be removed, we'll see.
- `.vscode` folder just has the `launch.json` file that helps in launching `gdb` debugger (if it is installed on your machine). First the `bin/grid.exe` must be present, then will the `launch.json` start the debugger (F5 is the shortcut in VS Code, I believe).

## Our Source Code (`src` directory)
- `main.cpp` has the `main()` function and the *main* `while` loop. In `main()` a window is created and then in the loop, events are handled, the state of the program is updated and finally the state of the program is rendered / drawn (very similar to a structure of a game).
    - `#define DEBUG 1` is a macro which I intend to use only where I'm outputting some information that may be helpful in catching some bugs. Changing it's value to 0 should disable those output information.
    - Here bunch of important variables and functions are defined.
    - `window` represents our main *SFML* window where gui and canvas will be drawn, `clock` represents *SFML*'s internal timing system which for now only *ImGui* needs, `running` is a flag variable for our main loop, and `window_size` is the intial size of the window (may need to update if the window is resized but forget for now).
    - `action_map` is a hash map that maps sequence of keys (keyboard keys or shortcuts) which are doubly linked lists of *int*s (*SFML* stores keys as enums which are just *int*s) to an action's name (see *Action.h* and *Action.cpp*), and `currently_pressed_keys` is also a linked list of *int*s representing the currently pressed sequence of keys to remember and compare which keys are being pressed right now in order to properly handle events.
    - `vars` contains some useful variables that either *Actions* or *ImGui*'s interface change in order for us to do some logic with them.
    - `quit()` and `drawline()` are pretty self-explanatory.
- **Note:** ImGui's functions are mostly in `ImGui::` namespace and SFML's functions are mostly in the `sf::` namespace.
- `int.h` contains some `typedef`s for integers. Please use these as integers not the built-in or the other ones. See comments for more detail.
- `vec2t.h` declares a Mathematical 2D Vector. This is a template class. *float*, *int*, and *unsigned int* versions of it are *typedef*-ed at the end of the file These will be used to represent *(x, y)* positions and *(w, h)* size of/in an image. The `vec2` is the most important of these.
- `Variables.h` contains just a struct that holds some important variables that the gui or events modify for us.
- `util.hpp` contains some useful helper functions. More functions like those will be added here.
- `Action.h` and `Action.cpp` define an action that just holds some basic data in order for us to do proper event handling. `do_action()` uses this extensively.
    * `register_action()` just stores a keyboard sequence (key or a shortcut) in the `action_map` to the corresponding action's name.
    * `do_action()` does basic logic related to actions if an action that exists in the action map is called upon by the user through pressing a key or a shortcut.
- `gui.h` and `gui.cpp` define very simple functions that take an instance of the variable struct (defined in *main.cpp*) to modify it. These functions are simple and just call the very nice and convenient ImGui functions to display the GUI
- `Image.h` and `Image.cpp` specify how a raster image is stored with loading, saving, accessing, uploading to texture, etc functionality.
- `Layer.h` specifies a layer that wraps a bunch of data that a layer needs including a pointer to some graphic which for now is only the Image struct, but will later be used for vector graphics, texts, paths, etc.
- `Canvas.h` and `Canvas.cpp` specify the actual canvas that holds all the layers, and some data related to zooming and panning. Currently, it only draws a checker pattern (the one that says "hey a bunch of pixels here are transparent") and the layers are of `RASTER` type. Will expand more on this...
- `filters.h` and `filters.cpp` will contain filter functions that use the CPU (for now) to edit the pixels. Currently it only contains the grayscale filter. More (with shaders) will be added once the canvas if finished.

## TODOs
- `TODOs` are listed below here. Do look at them. Note that `[[[x]]]` in a TODO just gives a unique identifier. Through out the code that same `[[[x]]]` will be scene, which just means that the `x` TODO is belongs/relates to that code somehow.
    * `[[[0]]]` Make template data structures and helper classes:
          - static array
          - dynamic array
          - linked list
          - stack
          - queue
          - hash map
          - string class (maybe)
    * `[[[1]]]` Complete image canvas, storing the image in a suitable data structure (probably a dynamic array), and image layers.
    * `[[[2]]]` Test basic filters and tools and try saving the image. Then implement them using shaders if possible.
    * `[[[3]]]` Start implementing actual features - filters, tools, zoom, pan, layers, etc.

## Building and Running the Entire Program
- First run `./imgui.sh` to compile ImGui source code.
    * This is so that ImGui is first compiled from source code to object files and stored in `dependencies/imgui/lib`. These object files are necessary in the linking stage done so by `Makefile` or `build.sh`.
- Then run one of the following to compiler and run the entire program:
    * `make` in the terminal, if you have make installed. `make` uses the `Makefile`. The advantage of `make` is that it does not recompile everything for no reason. It only compiles and links the recently changed files.
    * Or `./build.sh`. On windows, I recommend getting the git bash terminal (or the sh.exe executable) that comes with [git](https://git-scm.com/) to run the scripts. `build.sh` is good and all but it will recompile everything, and so will take longer to run and test the program after tiny changes.
    * `Note:` Sometimes `Makefile` will not recompile the header files. So when you make a change in the header files, and after running `make` you notice no change, then first `make clean` then run `make` again to build freshly.
- `imgui.sh` is just thier to automatically compile (only compile not link) the imgui source code. It just automates some manual labour. Probably not that important but is handy.
- `build.sh`(shell script) will compile and link our source code to libraries and run the executable.
- `Makefile` uses `make` to do the exact same thing as `build.sh` but in a more efficient way.
- `bin` directory will contain the final executable along with SFML dll files to dynamically link us to SFML. The program won't run without these dll files.
- Once we have `grid.exe` in `bin/`, we can either run it directly or start debugging.