#include <emscripten.h>
#include "render.h"

EM_JS(void, init_graphics, (const char *bg_cstr, const char *text_cstr, const char *dim_cstr, int header_h), {
	const bg	= UTF8ToString(bg_cstr);
	const text	= UTF8ToString(text_cstr);
	const dim	= UTF8ToString(dim_cstr);
	const rootStyle = document.documentElement.style;

	rootStyle.setProperty('--bg-color', bg);
	rootStyle.setProperty('--text-color', text);
	rootStyle.setProperty('--dim-text-color', dim);

	const cvs = document.getElementById("screen");
	if (!cvs)
		return;

	Module.gfx = {
		cvs,
		ctx: cvs.getContext("2d", {alpha: false}),
		header_h,
		bg,
		lastLabelPtr: 0,
		cachedLabel: "",
		lastTextColorPtr: 0,
		cachedTextColor: "",
		lastScanlinePtr: 0,
		cachedScanline: ""
	};

	const onResize = () => {
		cvs.width  = window.innerWidth;
		cvs.height = window.innerHeight;
	};

	window.addEventListener('resize', onResize);
	onResize();
});

EM_JS(void, apply_style, (const char *selector_cstr, const char *style_cstr), {
	const selector = UTF8ToString(selector_cstr);
	const style    = UTF8ToString(style_cstr);
	const elements = document.querySelectorAll(selector);
	elements.forEach(el => el.style.cssText = style);
});

EM_JS(void, update_theme_colors, (const char *bg_cstr, const char *text_cstr, const char *dim_cstr, const char *scanline_cstr), {
	const bg	= UTF8ToString(bg_cstr);
	const text	= UTF8ToString(text_cstr);
	const dim	= UTF8ToString(dim_cstr);
	const rootStyle = document.documentElement.style;

	rootStyle.setProperty('--bg-color', bg);
	rootStyle.setProperty('--text-color', text);
	rootStyle.setProperty('--dim-text-color', dim);

	if (Module.gfx) {
		Module.gfx.bg = bg;
	}
});

EM_JS(void, draw_frame, (float t, const char *label_ptr, const char *text_color_ptr, const char *scanline_color_ptr), {
	const gfx = Module.gfx;
	if (!gfx?.ctx)
		return;

	const {ctx, cvs, header_h, bg} = gfx;
	const W			       = cvs.width;
	const H			       = cvs.height;

	if (label_ptr !== gfx.lastLabelPtr) {
		gfx.cachedLabel  = UTF8ToString(label_ptr);
		gfx.lastLabelPtr = label_ptr;
	}
	if (text_color_ptr !== gfx.lastTextColorPtr) {
		gfx.cachedTextColor  = UTF8ToString(text_color_ptr);
		gfx.lastTextColorPtr = text_color_ptr;
	}
	if (scanline_color_ptr !== gfx.lastScanlinePtr) {
		gfx.cachedScanline  = UTF8ToString(scanline_color_ptr);
		gfx.lastScanlinePtr = scanline_color_ptr;
	}

	const FLICKER_SPEED  = 12.0;
	const FLICKER_BASE   = 0.8;
	const FLICKER_AMP    = 0.15;
	const SCANLINE_SPEED = 120.0;

	ctx.fillStyle = bg;
	ctx.fillRect(0, 0, W, H);

	ctx.font	= "bold 40px 'Courier New', monospace";
	ctx.fillStyle	= gfx.cachedTextColor;
	ctx.textAlign	= "center";
	ctx.globalAlpha =
		FLICKER_BASE + Math.sin(t * FLICKER_SPEED) * FLICKER_AMP;
	ctx.fillText(gfx.cachedLabel, W / 2, header_h / 2);

	ctx.globalAlpha = 1.0;
	ctx.fillStyle	= gfx.cachedScanline;
	ctx.fillRect(0, (t * SCANLINE_SPEED) % H, W, 2);
});
