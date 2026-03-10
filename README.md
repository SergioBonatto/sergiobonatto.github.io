# Minimalist Web Engine in C/WebAssembly

A high-performance personal website implementation leveraging C compiled to WebAssembly. The architecture minimizes runtime overhead by adopting patterns typically found in resource-constrained systems, such as build-time asset inlining and non-allocating parsers.

## Performance Metrics (Cold Load)

Metrics captured in Safari (macOS), cache disabled:

| Metric | Value |
| :--- | :--- |
| Domains | 2 |
| HTTP Requests | 6 |
| Uncompressed Resource Size | 128.7 KB |
| Compressed Transfer Size | 80.1 KB |
| Time to Interactive (TTI) | 147 ms |

## System Architecture

### Build-time Asset Inlining
The engine utilizes a static asset packing strategy. A native host utility (`tools/packer.c`) serializes Markdown content into hex-encoded character arrays within a C header (`src/contents_data.h`). This eliminates runtime asynchronous network I/O for content retrieval, reducing latency to memory-access speeds.

### Platform Bridge (Environment Abstraction)
Core logic, state management, and parsing are executed within the WebAssembly linear memory. Interaction with Browser APIs is delegated to a thin JavaScript layer via `EM_JS` macros:
*   **Logic Layer (C/Wasm):** Controls application flow and data processing.
*   **Bridge Layer (JS):** Mediates access to the Document Object Model (DOM) and HTML5 Canvas API.

### Non-Allocating Pointer-Based Parser
The Markdown parser (`src/markdown.c`) is designed for zero dynamic memory allocation (`malloc`/`free`) during runtime:
*   **Raw Pointer Iteration:** The parser traverses the read-only data segment in-place.
*   **Heap View Projection:** Line boundaries are passed as offsets and lengths to the Platform Bridge.
*   **DOM Integration:** JavaScript utilizes `TextDecoder` on a `Uint8Array` view of the WebAssembly heap to project strings into the DOM, minimizing heap fragmentation.

### Execution Model
The engine operates on a hybrid rendering pipeline:
1.  **Synchronous UI Projection:** Page transitions and text rendering are handled via direct DOM element creation.
2.  **Asynchronous Visual Effects:** A C-driven 60 Hz tick loop (`main_tick`) updates a background Canvas, emulating hardware-level visual artifacts (scanlines and flicker).

## Build System

### Requirements
- **Emscripten SDK (`emcc`)**
- **Native C Compiler (`gcc`/`clang`)**
- **Binaryen (`wasm-opt`)**
- **Terser** (JS minification)
- **Brotli** (compression)

### Pipeline Stages
The `build.sh` script automates the following optimization passes:
1.  **Tooling:** Compilation of the host-native `packer`.
2.  **Asset Generation:** Static packing of `contents/` into C source.
3.  **Cross-Compilation:** C to Wasm via `-Oz` and `-flto`.
4.  **Binary Optimization:** `wasm-opt` post-processing.
5.  **Compression:** Brotli encoding of `.js` and `.wasm` artifacts.

```bash
chmod +x build.sh
./build.sh
```

## Implementation Notes and Development Status

Current technical analysis identifies several areas requiring refactoring to align with project standards:
- **AI-Generated Components:** The current implementations of `tools/packer.c` and `src/markdown.c` were generated using AI (GitHub Copilot) during a phase of addressing parsing complexities. These components are slated for total manual refactoring to correct identified bugs and ensure adherence to the project's minimalist kernel-style standards.
- **Abstraction Leaks:** Visual timing constants (`FLICKER_SPEED`, `SCANLINE_SPEED`) are currently duplicated across the C/JS boundary.
- **Build-Tool Reliability:** The `packer.c` utility contains a verified buffer overread (OOB access at `files[count]`) during its output loop.
- **Parser Robustness:** The existing Markdown parser requires refinement for full CommonMark compliance and strict header validation.
