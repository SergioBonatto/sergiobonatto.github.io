#!/bin/bash

mkdir -p build

emcc \
src/config.c \
src/main.c \
src/render.c \
src/ui.c \
src/markdown.c \
-O3 \
-s WASM=1 \
-s EXPORTED_FUNCTIONS='["_main", "_toggle_theme", "_switch_page", "_render_markdown", "_open_article", "_malloc", "_free"]' \
-s EXPORTED_RUNTIME_METHODS='["UTF8ToString", "ccall", "cwrap"]' \
-o build/app.js
