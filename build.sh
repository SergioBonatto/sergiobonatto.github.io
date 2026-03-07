#!/bin/bash

mkdir -p build

emcc \
src/config.c \
src/main.c \
src/render.c \
src/ui.c \
-O3 \
-s WASM=1 \
-s EXPORTED_FUNCTIONS='["_main", "_toggle_theme"]' \
-s EXPORTED_RUNTIME_METHODS='["UTF8ToString", "ccall", "cwrap"]' \
-o build/app.js
