# Project: Blog WASM Engine
TAGS: #Project #WASM #C #Systems #Web

- Focus: Minimalist high-performance engine for personal blog.
- Status: Optimizing for 100/100 Lighthouse

## Technical Overview
Minimalist blog engine implemented in C, cross-compiled to WebAssembly (WASM) via Emscripten. The system prioritizes high-performance metrics and zero-latency content delivery by eliminating runtime asynchronous I/O.

## System Architecture

### 1. Build-time Asset Pipeline
- **Host Tooling**: `tools/packer.c` (native C) scans the `contents/` directory for Markdown files.
- **Serialization**: Extracts metadata (title, date, slug) and converts Markdown bodies into hex-encoded C arrays.
- **Header Generation**: Produces `generated/contents_data.h`, embedding all site content directly into the WASM binary data segment.
- **Cache Management**: `build.sh` implements content hashing for JS/WASM artifacts and assets (AVIF) to ensure aggressive caching.

### 2. Memory and Execution Model
- **Allocation Strategy**: Primarily utilizes fixed-capacity buffers (e.g., `BUFFER_CAPACITY` at 512KB) to minimize dynamic memory overhead and heap fragmentation.
- **Parser Design**: Non-allocating, pointer-based Markdown parser (`src/markdown.c`) operating in-place on the embedded data segment.
- **Execution Loop**: Implements a 60Hz tick loop via `emscripten_set_main_loop` for visual effects and state synchronization.

### 3. Rendering Engine
- **Hybrid Pipeline**:
    - **DOM Bridge**: C logic interacts with the DOM via `EM_JS` macros (`src/js_api.c`) for text and UI layout.
    - **Canvas Layer**: Background HTML5 Canvas handles low-level visual artifacts (scanlines, flicker) driven by the C engine.
- **Mathematical Rendering**: Custom LaTeX to MathML parser (`src/math.c`) for lightweight, browser-native formula display.

### 4. Platform Abstraction Layer
- **System Interface**: `include/sys.h` and `src/js_api.c` define the boundary between C logic and Browser APIs (Routing, SEO, Storage).
- **UI Management**: State-driven navigation (`src/router.c`) and theme management (Dark/Light palettes in `src/config.c`).

## Current Feature Set
- **Markdown Parsing**: Pointer-based implementation with support for:
    - Headers (H1-H6) and paragraphs.
    - Code blocks with language identification for CSS.
    - Image inlining with `fetchpriority` (LCP) and `loading="lazy"`.
    - Shortcodes for segmented bar graphs (`[[graph:...]]`).
- **LaTeX/Math Integration**: Internal LaTeX to MathML translator supporting fractions, roots, Greek letters, and basic operators.
- **State & Routing**:
    - SPA (Single Page Application) with hash routing (`#/`, `#/blog`, `#/post/`).
    - C-managed state (Dark/Light mode, page transitions).
    - SEO metadata synchronization (Title, Description, OpenGraph).

## Roadmap
- [ ] **Refactoring & Safety**:
    - [ ] Manual rewrite of `src/markdown.c` and `tools/packer.c` to replace AI-generated logic.
    - [ ] Fix verified buffer overread in `packer.c` output loop.
    - [ ] Implement robust error handling for buffer overflows in `src/buffer.c`.
- [ ] **Parser Enhancements**:
    - [ ] Align parser with CommonMark specification.
    - [ ] Add support for tables and nested lists.
    - [ ] Improve MathML parser robustness for complex expressions.
- [ ] **Visuals & CRT Emulation**:
    - [ ] Implement dynamic scanlines and flicker in `draw_frame`.
    - [ ] Unify timing constants (`FLICKER_SPEED`, etc.) across C/JS boundary.
- [ ] **DX (Developer Experience)**:
    - [ ] Replace `sed` in build script with a robust template generator.
    - [ ] Enhance WASM compiler error messaging for development.
