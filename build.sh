mkdir -p bin

OUTPUT_EXE="bin/game_app"

g++ -std=c++23 src/game.cpp -lSDL3 -lSDL3_image -o bin/game_app

if [ $? -eq 0 ]; then
    echo "Compilation successful! Running $OUTPUT_EXE..."
    ./"$OUTPUT_EXE"
else
    echo "Compilation failed!"
    exit 1
fi
