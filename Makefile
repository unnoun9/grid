# this file is an alternative to build.sh for building the entire program
# it is a bit efficient, in that it only compiles the files that were changed

# compiler and flags
COMPILER = g++
FLAGS = -std=c++17 -Wno-unused-result
# output executable
OUTPUT = bin/grid
# include paths and libraries
INCLUDE_PATHS = -Idependencies/sfml/include -Idependencies/imgui
LIBS = -Ldependencies/sfml//lib -lsfml-graphics -lsfml-window -lsfml-system -lopengl32 -lsfml-main

# source and object files that are important for building
SRC_FILES = $(wildcard src/*.cpp)
IMGUI_FILES = $(wildcard dependencies/imgui/lib/*.o)
OBJ_FILES = $(SRC_FILES:src/%.cpp=obj/%.o)

# rule to build and run the program
run: $(OUTPUT)
	./$(OUTPUT)

# rule to link compiled object files
$(OUTPUT): obj $(OBJ_FILES) $(IMGUI_FILES)
	$(COMPILER) $(OBJ_FILES) $(IMGUI_FILES) -o $(OUTPUT) $(FLAGS) $(INCLUDE_PATHS) $(LIBS)

# rule to compile source into object files
obj/%.o: src/%.cpp
	$(COMPILER) -c -g $< -o $@ $(FLAGS) $(INCLUDE_PATHS)

# rule to make a directory to store the temporary object files
obj:
	mkdir -p obj

# rule to clean up generated files
clean:
	rm -rf obj $(OUTPUT)

# phony targets
.PHONY: clean
.PHONY: run