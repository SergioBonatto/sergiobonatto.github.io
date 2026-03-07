#include <emscripten.h>
#include "render.h"

EM_JS(void, draw_frame, (float t), {

    const CONFIG = {
        headerHeight: 180,

        colors: {
            // Polar Night (backgrounds)
            bg      : "#2E3440",        // nord0
            bgSoft  : "#3B4252",        // nord1
            bgPanel : "#434C5E",        // nord2

            // Snow Storm (text)
            textPrimary : "#ECEFF4",    // nord6
            textSoft    : "#E5E9F0",    // nord5
            textMuted   : "#D8DEE9",    // nord4

            // other efects
            scanline: "rgba(236,239,244,0.035)",
            glow    : "rgba(236,239,244,0.08)"
        },

        flickerSpeed    : 10,
        flickerBase     : 0.82,
        flickerAmp      : 0.12,
        scanlineSpeed   : 100
    };

    const cvs = document.getElementById("screen");
    if (!cvs) return;

    const ctx = cvs.getContext("2d");

    cvs.width = window.innerWidth;
    cvs.height = CONFIG.headerHeight;

    const W = cvs.width;
    const H = cvs.height;

    ctx.fillStyle = CONFIG.colors.bg;
    ctx.fillRect(0,0,W,H);

    ctx.font = "bold 40px 'Courier New', monospace";
    ctx.fillStyle = CONFIG.colors.textPrimary;
    ctx.textAlign = "center";

    ctx.globalAlpha =
        CONFIG.flickerBase +
        Math.sin(t * CONFIG.flickerSpeed) * CONFIG.flickerAmp;

    ctx.fillText("BONATTO", W/2, H/2);

    ctx.globalAlpha = 1.0;

    ctx.fillStyle = CONFIG.colors.scanline;

    const scanlineY = (t * CONFIG.scanlineSpeed) % H;

    ctx.fillRect(0, scanlineY, W, 2);
});
