#!/bin/bash
set -e

mkdir -p build

gcc tools/packer.c -O2 -o tools/packer

./tools/packer contents > src/contents_data.h

emcc \
src/config.c \
src/main.c \
src/render.c \
src/ui.c \
src/markdown.c \
-Oz \
-flto \
-Isrc \
-s WASM=1 \
-s ASSERTIONS=0 \
-s SAFE_HEAP=0 \
-s STACK_OVERFLOW_CHECK=0 \
-s FILESYSTEM=0 \
-s ERROR_ON_UNDEFINED_SYMBOLS=0 \
-s EXPORTED_FUNCTIONS='["_main","_toggle_theme","_switch_page","_render_markdown","_open_article","_malloc","_free"]' \
-s EXPORTED_RUNTIME_METHODS='["UTF8ToString","ccall","cwrap"]' \
-o build/app.js

# Extra optimization passes
wasm-opt -Oz --all-features build/app.wasm -o build/app.wasm

# Minify JS
terser build/app.js -c -m -o build/app.js

# Optional compression
brotli -f -Z build/app.js
brotli -f -Z build/app.wasm

echo "Build successful."