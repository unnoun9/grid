# move out from scripts dir
cd ..

# variables
SRC="src/*.cpp dependencies/imgui/lib/*.o"
OUTPUT="bin/grid"
INCLUDE_PATH="-Idependencies/sfml/include -Idependencies/imgui"
FLAGS="-std=c++17 -Wno-unused-result"
LIB_PATH="-Ldependencies/sfml/lib"
LIBS="-lsfml-graphics -lsfml-window -lsfml-system -lopengl32 -lsfml-main"

# compile, link, and run
g++ $SRC -o $OUTPUT $INCLUDE_PATH $FLAGS $LIB_PATH $LIBS && ./$OUTPUT

# move back into scripts dir
cd scripts
