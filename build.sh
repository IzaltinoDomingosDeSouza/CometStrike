mkdir -p bin

OUTPUT_EXE="bin/game_app"

SOURCES="src/game.cpp \
         src/sdl/application.cpp \
         src/sdl/application_renderer.cpp \
         src/sdl/application_resource_manager.cpp"

g++ -std=c++23 $SOURCES -lSDL3 -lSDL3_image -o bin/game_app

if [ $? -eq 0 ]; then
    echo "Compilation successful! Running $OUTPUT_EXE..."
    ./"$OUTPUT_EXE"
else
    echo "Compilation failed!"
    exit 1
fi
