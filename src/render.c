#include <emscripten.h>
#include "render.h"

EM_JS(void, init_graphics, (const char* bg_cstr, const char* text_cstr, int header_h), {
    
    // Convert C/WASM strings to JS strings
    const bg    = UTF8ToString(bg_cstr);
    const text  = UTF8ToString(text_cstr);

    // Inject colors into global CSS variables
    document.documentElement.style.setProperty('--bg-color', bg);
    document.documentElement.style.setProperty('--text-color', text);

    const cvs = document.getElementById("screen");
    if (!cvs) return;

    // Persist references in the Module object (Emscripten scope)
    Module.gfx = {
        cvs: cvs,
        ctx: cvs.getContext("2d", { alpha: false }),
        height: header_h
    };

    const onResize = () => {
        Module.gfx.cvs.width = window.innerWidth;
        Module.gfx.cvs.height = Module.gfx.height;
    };

    window.addEventListener('resize', onResize);
    onResize();
});

EM_JS(void, draw_frame, (float t, const char* label_cstr, const char* text_color_cstr, const char* scanline_color_cstr), {
    
    if (!Module.gfx || !Module.gfx.ctx) return;

    const ctx = Module.gfx.ctx;
    const W = Module.gfx.cvs.width;
    const H = Module.gfx.cvs.height;

    const label = UTF8ToString(label_cstr);
    const textColor = UTF8ToString(text_color_cstr);
    const scanlineColor = UTF8ToString(scanline_color_cstr);

    // Animation parameters 
    // TODO: move to config
    const flickerSpeed  = 12.0;
    const flickerBase   = 0.8;
    const flickerAmp    = 0.15;
    const scanlineSpeed = 120.0;

    // Background
    ctx.fillStyle = getComputedStyle(document.documentElement).getPropertyValue('--bg-color');
    ctx.fillRect(0, 0, W, H);

    // Text with Flicker effect
    ctx.font = "bold 40px 'Courier New', monospace";
    ctx.fillStyle   = textColor;
    ctx.textAlign   = "center";
    ctx.globalAlpha = flickerBase + Math.sin(t * flickerSpeed) * flickerAmp;
    
    ctx.fillText(label, W/2, H/2);

    // Scanline effect
    ctx.globalAlpha = 1.0;
    ctx.fillStyle   = scanlineColor;
    const scanlineY = (t * scanlineSpeed) % H;
    ctx.fillRect(0, scanlineY, W, 2);
});
