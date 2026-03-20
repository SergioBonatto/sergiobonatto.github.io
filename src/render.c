#include <emscripten.h>
#include "render.h"

/* 
 * Sincroniza a struct theme (C) com as CSS Variables (JS) 
 * e atualiza o cache do renderizador.
 */
EM_JS(void, update_theme_colors, (const struct theme *t), {
	if (!t || !Module.gfx) return;
	
	const rootStyle = document.documentElement.style;
	const getStr = (offset) => UTF8ToString(HEAP32[(t + offset) >> 2]);

	const bg = getStr(0);
	const text = getStr(4);
	const scanline = getStr(12);

	rootStyle.setProperty('--bg-color', bg);
	rootStyle.setProperty('--text-color', text);
	rootStyle.setProperty('--dim-text-color', getStr(8));
	rootStyle.setProperty('--scanline-color', scanline);
	rootStyle.setProperty('--accent-color', getStr(16));

	for (let i = 0; i < 16; i++) {
		rootStyle.setProperty('--nord' + i, getStr(20 + i * 4));
	}

	/* Atualiza cache de renderização */
	Module.gfx.bg = bg;
	Module.gfx.textColor = text;
	Module.gfx.scanlineColor = scanline;
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

EM_JS(void, render_update_strings, (const char *label_ptr, const char *text_color_ptr, const char *scanline_color_ptr), {
	if (!Module.gfx) return;
	Module.gfx.label         = UTF8ToString(label_ptr);
	Module.gfx.textColor     = UTF8ToString(text_color_ptr);
	Module.gfx.scanlineColor = UTF8ToString(scanline_color_ptr);
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

	ctx.globalAlpha = 1.0;
	ctx.fillStyle 	= scanlineColor;
	ctx.fillRect(0, (t * SCANLINE_SPEED) % H, W, 2);
});
