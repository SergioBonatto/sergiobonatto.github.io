#include <emscripten.h>
#include "render.h"

/* 
 * Sincroniza a struct theme (C) com as CSS Variables (JS) 
 * e atualiza o cache do renderizador.
 */
EM_JS(void, update_theme_colors, (const struct theme *t, const char *const *palette), {
	if (!Module.gfx) return;
	
	const rootStyle = document.documentElement.style;
	const getPalette = (idx) => UTF8ToString(HEAP32[(palette >> 2) + idx]);

	/* t layout: bg(0), text(4), dim(8), accent(12), scanline(16) */
	const bg_idx = HEAP32[t >> 2];
	const tx_idx = HEAP32[(t + 4) >> 2];
	const dm_idx = HEAP32[(t + 8) >> 2];
	const ac_idx = HEAP32[(t + 12) >> 2];
	const sc_idx = HEAP32[(t + 16) >> 2];

	const bg = getPalette(bg_idx);
	const text = getPalette(tx_idx);

	rootStyle.setProperty('--bg-color', bg);
	rootStyle.setProperty('--text-color', text);
	rootStyle.setProperty('--dim-text-color', getPalette(dm_idx));
	rootStyle.setProperty('--scanline-color', getPalette(sc_idx));
	rootStyle.setProperty('--accent-color', getPalette(ac_idx));

	for (let i = 0; i < 16; i++) {
		rootStyle.setProperty('--nord' + i, getPalette(i));
	}

	/* Atualiza cache de renderização */
	Module.gfx.bg = bg;
	Module.gfx.textColor = text;
	Module.gfx.scanlineColor = getPalette(sc_idx);
});

EM_JS(void, init_graphics, (const struct theme *t, int header_h), {
	const cvs = document.getElementById("screen");
	if (!cvs) return;

	Module.gfx = {
		cvs,
		ctx: cvs.getContext("2d", {alpha: false}),
		header_h,
		bg: "",
		label: "",
		textColor: "",
		scanlineColor: ""
	};

	const onResize = () => {
		cvs.width = window.innerWidth;
		cvs.height = window.innerHeight;
	};

	window.addEventListener('resize', onResize);
	onResize();
});

EM_JS(void, render_update_strings, (const char *label_ptr, int text_color_idx, int scanline_color_idx, const char *const *palette), {
	if (!Module.gfx) return;
	const getPalette = (idx) => UTF8ToString(HEAP32[(palette >> 2) + idx]);

	Module.gfx.label         = UTF8ToString(label_ptr);
	Module.gfx.textColor     = getPalette(text_color_idx);
	Module.gfx.scanlineColor = getPalette(scanline_color_idx);
});

EM_JS(void, apply_style, (const char *selector_cstr, const char *style_cstr), {
	const selector = UTF8ToString(selector_cstr);
	const style = UTF8ToString(style_cstr);
	const elements = document.querySelectorAll(selector);
	elements.forEach(el => el.style.cssText = style);
});

EM_JS(void, draw_frame, (float t), {
	const gfx = Module.gfx;
	if (!gfx?.ctx) return;

	const {ctx, cvs, header_h, bg, label, textColor, scanlineColor} = gfx;
	const W = cvs.width;
	const H = cvs.height;

	const FLICKER_SPEED 	= 12.0;
	const FLICKER_BASE 		= 0.8;
	const FLICKER_AMP 		= 0.15;
	const SCANLINE_SPEED 	= 120.0;

	ctx.fillStyle = bg;
	ctx.fillRect(0, 0, W, H);

	ctx.font 		= "bold 40px 'Courier New', monospace";
	ctx.fillStyle 	= textColor;
	ctx.textAlign 	= "center";
	ctx.globalAlpha = FLICKER_BASE + Math.sin(t * FLICKER_SPEED) * FLICKER_AMP;
	ctx.fillText(label, W / 2, header_h / 2);

	/* Scanline com opacidade programática */
	ctx.globalAlpha = 0.035;
	ctx.fillStyle 	= scanlineColor;
	ctx.fillRect(0, (t * SCANLINE_SPEED) % H, W, 2);
	ctx.globalAlpha = 1.0;
});
