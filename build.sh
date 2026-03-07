#!/bin/bash

mkdir -p build

emcc \
src/main.c \
src/render.c \
src/ui.c \
-O3 \
-s WASM=1 \
-s EXPORTED_RUNTIME_METHODS='["UTF8ToString"]' \
-o build/app.js
