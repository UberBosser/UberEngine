#! /bin/bash
echo compiling...
clang++ test.cpp uberengine.cpp \
    -o game.o \
    -DDEBUG \
    -O0 \
    $(sdl2-config --libs --cflags) \
    -lSDL2_image \
    -lSDL2_ttf \
    -lSDL2_mixer \
   && ./game.o
