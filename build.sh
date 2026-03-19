#!/bin/bash
set -e

mkdir -p build

gcc tools/packer.c -O2 -o tools/packer

./tools/packer contents > src/contents_data.h

# Compila individualmente para gerar o compile_commands.json (apenas para o linter)
echo "[" > compile_commands.json
for file in src/*.c; do
    emcc "$file" -Isrc -DEMSCRIPTEN -MJ "$file.json" -c -o "build/$(basename "$file" .c).o"
    cat "$file.json" >> compile_commands.json
    rm "$file.json"
done
# Remove a última vírgula (emcc -MJ adiciona vírgulas no final de cada objeto)
# e fecha o array. O sed remove a última vírgula do arquivo.
sed -i '' '$ s/,$//' compile_commands.json
echo "]" >> compile_commands.json

# Build final (o comando original sem o bear)
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
-s EXPORTED_FUNCTIONS='["_main","_toggle_theme","_switch_page","_render_markdown","_open_article","_handle_route","_malloc","_free"]' \
-s EXPORTED_RUNTIME_METHODS='["UTF8ToString","ccall","cwrap"]' \
-o build/app.js

# Comandos de pós-processamento (ignora falhas se as ferramentas não existirem)
wasm-opt -Oz --all-features build/app.wasm -o build/app.wasm || echo "wasm-opt skipped"
terser build/app.js -c -m -o build/app.js || echo "terser skipped"
brotli -f -Z build/app.js || echo "brotli skipped"
brotli -f -Z build/app.wasm || echo "brotli skipped"

echo "Build successful."
