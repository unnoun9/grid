# Grid

## Repository Structure
- `src` directory contains all the source code.
- `bin` directory contains the executable output program.
- `dependencies` directory has all the libraries' header files and lib files.
- `scripts` directory contains the scripts to build the entire project (some scripts will build part of it).
- `fonts` directory just has a font which may be removed, we'll see.
- `config.txt` is irrelevant to our image manipulator project and will be removed.

## Building the project
- First run `./imgui.sh` to compile ImGui source code, then run `./build.sh`. For windows, I recommend installing the git bash terminal (or the sh.exe executable) that comes with [git](https://git-scm.com/) to run the scripts.
- `build.sh`(shell script) will compile and link our source code to libraries and run the executable.
- `imgui.sh` is just thier to automatically compile (only compile not link) the imgui source code. It just automates some manual labour. Probably not that important but is handy.
- `bin` folder will contain the final executable along with SFML dll files to dynamically link us to SFML. The program won't run without these dll files.