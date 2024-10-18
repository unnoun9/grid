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

## Preferred Style for this Project (PLEASE FOLLOW THIS)
- `Note:` I could not mention everything here, so observe the examples below and the code in `src` directory for stuff not mentioned.
- Commit messages should be lowercased, follow english punchuation rules to some extent, and sentences in them should be separated by double hypher `--` that has a space on each side of it. See the blow Examples.
    ```shell
    git commit -m "a very usefull commit message -- some more details here -- bug is finaly fixed!! -- more TODOS added"
    git commit -m "shaders are finally added, but their effect is not to be seen anywhere when the final image is exported -- canvas is finaly perfected!"
    ``` 
- Comments in the code should be in lowercase. Each `//` must follow with a space. Sentences must be separated with a newline comment or a `;`. See the below Examples.
    ```cpp
    // sphinx of the black quartz, judge my vow!; what the bugha?!; the quick brown fox does some crazy shit...

    // sphinx of the black quartz, judge my vow!
    // what the bugha?!
    // the quick brown fox does some crazy shit...
    ```
   * If any function or structs/classes have declarations in a file, and there is a need to document/explain it through comments, then those comments should be in the declarations not in the definitions, unless again there is no declaration.
   * For super large files, follow the convention of separating **functions** or **logic** with:
      ```cpp
      //..................................................................................................
      ```
   * Or if deemed necessary, with this:
      ```cpp
      //................................................. SOME DESCRIPTIVE HEADING .................................................
      ```
- Identifiers' names (variables, function names, custom types, etc) must be in these styles: `flatcase / lowercase`, `snake_case / underscore_method`, `UPPERCASE / SCREAMINGCASE`, `SCREAMING_SNAKE_CASE`, `Pascal_Snake_Case / Tile_Case`, `First_capital_case / Semi_title_case`.
    * Structs, classes and enums must be named in either `Pascal_Snake_Case` if possible, otherwise `First_capital_case`.
    * Variables, and function must be named using `flatcase` if possible, otherwise `snake_case`.
    * Macros must use `SCREAMINGCASE` if possible, otherwise `SCREAMING_SNAKE_CASE`.
    * No other styles are acceptable as they look ugly, especially `SnakeCase / snakeCase` which is quite common and super ugly.
    * See some examples below.
        ``` cpp
        float some_value;  // good and preferred
        float somevalue;   // good only if readable at first glance
        float someValue;   // aweful

        #define NUM_PIXELS 727 * 272  // good and preferred
        #define NUMPIXELS  727 * 272  // good only if readable at first glance
        #define num_pixles 727 * 272  // bad
        #define Num_Pixels 727 * 272  // bad
        #define Num_pixels 727 * 272  // bad
        #define numPixels  727 * 272  // aweful
        #define NumPixels  727 * 272  // aweful

        void load_from_file(const std::string filepath); // good
        void loadfromfile(const std::string& file_path); // good only if readable at first glance
        void loadFromFile(const std::string& FilePath);  // aweful
        void LoadFromFile(const std::string& filePath);  // aweful

        struct dynamic_array; // good and preferred only if it is in a namespace
        struct Dynamic_Array; // good and preferred
        struct Dynamic_array; // good
        struct DynamicArray;  // aweful

        ```
- Use `Allman` style indentation, with 4 tab spaces width as spaces `' '` not tabs `\t`, for control structure blocks, functions, structs, and classes.
    ``` cpp
    void foo()
    {
        while (true)
        {
            if (condition)
               break;
            // ...
        }
    }
    ```
   * `K&R` style indentation must only be used for initializing through the **initializer list** and  **namespaces**.
       ``` cpp
       namespace ds {
   
           // ...
   
       } // end namespace
       ```
       ``` cpp
       float array[SIZE] = { /* elements */  };

       ui8 pixels[W * H] = {
           // a lot of elements (perhaps the array is multi-dimensional)
       };
       ```
- Use `struct` rather than `class`.
    * Again, a struct's declaration must have comments, if necessary, about what its variables represent and what its functions do. The definition of functions will only contain comments, if needed, inside the function to explain some tricky thing happening in there.
    * If there are private members, a struct must have only one `private:` sub-section at the end, not at the beginning. There will be no `public:` keyword used.
    * Variables must be at the top in a section of a struct and functions at the bottom.
    * Example of a struct with a private member:
        ```cpp
        namespace stl {
        
        template <typename T>
        struct vector
        {
            vector();                     // creates a vector of size 2 by default
            vector(const vector& other);  // specifies how vectors will be deep copied
            vector(vector&& other);       // specifies how vectors will be "moved"
            vector(ui32 capacity);        // creates a vector of given capacity
            ~vector();                    // clears everything

            // get the size
            inline ui32 size() const;

            // access the actual data
            Type* data() const;

            // push an element at the end of the vector
            void append(const Type& element);
            
            // more functions...
            // ...

            // to access the i-th element
            const Type& operator[] (ui32 index) const;

            // to modify the i-th element
            Type& operator[] (ui32 index);

            // more operators...
            // ...

        private:
            T* m_data{};            // the actual data
            ui32 m_size = 0;        // to track the current size
            ui32 m_capacity = 2;    // the actual space allocated in terms of number of `T` that can be appended

            // allocator; allocates memory and moves everything to that memory
            void reallocate(ui32 new_capacity);
        };

        //..................................................................................................
        // constructors and other functions...
        // ...

        //..................................................................................................
        template <typename T>
        void vector<T>::reallocate(ui32 new_capacity)
        {
            T* new_block = new T[new_capacity];

            // what if we are decreasing the capacity
            if (new_capacity < m_size) m_size = new_capacity;

            // move the data to new block
            for (ui32 i = 0; i < m_size; i++)
            {
                new_block[i] = m_data[i];
            }

            // update internal variables
            delete[] m_data;
            m_data = new_block;
            m_capacity = new_capacity;
        }

        //..................................................................................................
        // more stuff...
        // ...
        
        } // end stl namespace
        ```

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
- `TODOs` are listed below here. Do look at them. Note that `[[[x]]]` in a TODO just gives a unique identifier. Through out the code that same `[[[x]]]` may (or may not) be scene, which just means that the `x` TODO is belongs/relates to that code somehow.
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
