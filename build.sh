#!/bin/bash
set -e

mkdir -p build

gcc tools/packer.c -O2 -o tools/packer
./tools/packer contents > generated/contents_data.h

# 1. Static Assets Hashing
PFP_SRC="public/pfp.avif"
PFP_HASH=$(shasum -a 256 "$PFP_SRC" | cut -c 1-8)
PFP_DIST="public/pfp.$PFP_HASH.avif"

# Cleanup old versions
rm -f public/pfp.*.avif
cp "$PFP_SRC" "$PFP_DIST"
echo "#define ASSET_PFP \"$PFP_DIST\"" > generated/assets.h

# 2. Compilation
echo "Compiling WASM..."
emcc \
src/*.c \
-Oz -flto -Iinclude -Igenerated -s WASM=1 -s ASSERTIONS=0 -s SAFE_HEAP=0 -s STACK_OVERFLOW_CHECK=0 \
-s FILESYSTEM=0 -s ERROR_ON_UNDEFINED_SYMBOLS=0 \
-s EXPORTED_FUNCTIONS='["_main","_ui_toggle_theme","_switch_page","_render_markdown","_handle_route","_handle_current_route","_draw_frame"]' \
-s EXPORTED_RUNTIME_METHODS='["UTF8ToString","ccall","cwrap"]' \
-o build/app.js

# Generate compile_commands.json
echo "[" > compile_commands.json
FILES=(src/*.c)
for i in "${!FILES[@]}"; do
    file="${FILES[$i]}"
    emcc "$file" -Iinclude -Igenerated -DEMSCRIPTEN -MJ "$file.json" -c -o "build/$(basename "$file" .c).o"
    # Remove any trailing comma and newline to control the structure manually
    cat "$file.json" >> compile_commands.json
    rm "$file.json"
done
# Remove last comma if exists and close array
sed -i '' '$ s/,$//' compile_commands.json
echo "]" >> compile_commands.json

# 3. Post-processing and JS/WASM Hashing
wasm-opt -Oz --all-features build/app.wasm -o build/app.wasm || echo "wasm-opt skipped"
terser build/app.js -c -m -o build/app.js || echo "terser skipped"

JS_HASH=$(shasum -a 256 build/app.js | cut -c 1-8)
WASM_HASH=$(shasum -a 256 build/app.wasm | cut -c 1-8)

# Cleanup old builds before renaming
rm -f build/app.*.js build/app.*.wasm build/app.*.br

mv build/app.wasm "build/app.$WASM_HASH.wasm"
mv build/app.js "build/app.$JS_HASH.js"

# Patch JS to point to new .wasm
sed -i '' "s/app.wasm/app.$WASM_HASH.wasm/g" "build/app.$JS_HASH.js"

# 4. Generate final index.html
sed "s|{{PFP}}|$PFP_DIST|g; s|{{JS}}|build/app.$JS_HASH.js|g" index.template.html > index.html

# 5. Compression (optional .br files)
brotli -f -Z "build/app.$JS_HASH.js" || echo "brotli skipped"
brotli -f -Z "build/app.$WASM_HASH.wasm" || echo "brotli skipped"

echo "Build successful: index.html updated."
