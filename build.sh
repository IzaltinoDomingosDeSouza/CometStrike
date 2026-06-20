mkdir -p bin

OUTPUT_EXE="bin/game_app"

SOURCES="src/game.cpp \
         src/atsin2d/application_layer/application.cpp \
         src/atsin2d/application_layer/application_renderer.cpp \
         src/atsin2d/application_layer/application_resource_manager.cpp"

g++ -std=c++23 $SOURCES -lSDL3 -lSDL3_image -o bin/game_app

if [ $? -eq 0 ]; then
    echo "Compilation successful! Running $OUTPUT_EXE..."
    ./"$OUTPUT_EXE"
else
    echo "Compilation failed!"
    exit 1
fi
