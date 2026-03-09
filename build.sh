#!/bin/bash

# Build script for the WASM-based static site.
# Operates in a Research -> Strategy -> Execution lifecycle.

set -e

mkdir -p build

# Compile the native packaging utility
gcc tools/packer.c -o tools/packer

# Generate the C header containing all embedded markdown contents
./tools/packer contents > src/contents_data.h

# Compile the site using Emscripten with O3 optimization
emcc \
src/config.c \
src/main.c \
src/render.c \
src/ui.c \
src/markdown.c \
-O3 \
-Isrc \
-s WASM=1 \
-s EXPORTED_FUNCTIONS='["_main", "_toggle_theme", "_switch_page", "_render_markdown", "_open_article", "_malloc", "_free"]' \
-s EXPORTED_RUNTIME_METHODS='["UTF8ToString", "ccall", "cwrap"]' \
-o build/app.js

echo "Build successful: Contents embedded in WASM binary."
