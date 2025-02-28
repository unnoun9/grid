# this script only compiles imgui source code

# move out from scripts dir
cd ..

mkdir -p dependencies/imgui/lib

# variables
FLAGS="-std=c++17 -O3 -c"
INCLUDE_PATH="-Idependencies/imgui -Idependencies/sfml/include"

for file in dependencies/imgui/*.cpp; do
    g++ "$file" -o "dependencies/imgui/lib/$(basename "$file" .cpp).o" $INCLUDE_PATH $FLAGS 
done

# move back into scripts dir
cd scripts