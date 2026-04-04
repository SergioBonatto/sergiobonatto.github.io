#include <emscripten.h>
#include <string.h>
#include <stdlib.h>
#include "config.h" 

// From sys.c
EM_JS(void, sys_set_html, (const char *sel_ptr, const char *html_ptr), {
    const sel   = UTF8ToString(sel_ptr);
    const html  = UTF8ToString(html_ptr);
    const el    = document.querySelector(sel);
    if (!el) return;

    el.innerHTML = html;

    if (sel === '#feed') {
        if (!window._imgCache) window._imgCache = new Map();
        el.querySelectorAll('.img-placeholder').forEach(ph => {
            const src = ph.dataset.src;
            let img = window._imgCache.get(src);
            if (!img) {
                img = new Image();
                img.src = src;
                window._imgCache.set(src, img);
            }
            
            img.alt = ph.dataset.alt || "";
            if (ph.dataset.lcp) {
                img.setAttribute('fetchpriority', 'high');
                img.loading = 'eager';
            } else {
                img.loading = 'lazy';
            }
            if (ph.dataset.width) img.width = ph.dataset.width;
            if (ph.dataset.height) img.height = ph.dataset.height;
            
            img.style.maxWidth = '100%';
            img.style.height = 'auto';
            
            if (ph.dataset.scale) {
                const scale = parseFloat(ph.dataset.scale);
                img.style.width = "auto";
                img.onload = () => { img.style.width = (img.naturalWidth * scale) + "px"; };
                if (img.complete) img.onload();
            } else {
                img.style.width = "";
            }

            ph.replaceWith(img);
        });
    }
});

EM_JS(void, sys_set_text, (const char *sel_ptr, const char *text_ptr), {
    const sel   = UTF8ToString(sel_ptr);
    const text  = UTF8ToString(text_ptr);
    const el    = document.querySelector(sel);
    if (el) el.textContent = text;
});

EM_JS(void, sys_set_style, (const char *sel_ptr, const char *css_ptr), {
    const sel = UTF8ToString(sel_ptr);
    const css = UTF8ToString(css_ptr);
    const els = document.querySelectorAll(sel);
    els.forEach(el => el.style.cssText = css);
});

EM_JS(void, sys_append_child, (const char *sel_ptr, const char *tag_ptr, const char *html_ptr), {
    const sel   = UTF8ToString(sel_ptr);
    const tag   = UTF8ToString(tag_ptr);
    const html  = UTF8ToString(html_ptr);
    
    const parent = document.querySelector(sel);
    if (!parent) return;

    const el = document.createElement(tag);
    el.innerHTML = html;
    parent.appendChild(el);
});

EM_JS(void, sys_scroll_to_bottom, (const char *sel_ptr), {
    const sel   = UTF8ToString(sel_ptr);
    const el    = document.querySelector(sel);
    if (el) el.scrollTop = el.scrollHeight;
});

EM_JS(void, sys_init_router, (void), {
    if (!window._imgCache) window._imgCache = new Map();
    document.querySelectorAll('img').forEach(img => {
        if (img.src) window._imgCache.set(img.src, img);
    });

    window.addEventListener('hashchange', () => {
        const hash  = window.location.hash || "#/";
        const len   = lengthBytesUTF8(hash) + 1;
        const ptr   = _malloc(len);
        stringToUTF8(hash, ptr, len);
        if (Module._handle_route) Module._handle_route(ptr);
        _free(ptr);
    });
});

EM_JS(void, sys_update_url, (const char *path_ptr), {
    const path = UTF8ToString(path_ptr);
    if (window.location.hash !== path) {
        history.pushState(null, "", path);
    }
});

EM_JS(void, sys_get_url_hash, (char *buf, size_t max_len), {
    const hash = window.location.hash || "#/";
    stringToUTF8(hash, buf, max_len);
});

EM_JS(void, sys_set_meta, (const char *t_ptr, const char *d_ptr, const char *u_ptr), {
    const title = UTF8ToString(t_ptr);
    const desc  = UTF8ToString(d_ptr);
    const url   = UTF8ToString(u_ptr);
    
    document.title = title;

    const setMeta = (attr, name, content) => {
        let el = document.querySelector(`meta[${attr}="${name}"]`);
        if (!el) {
            el = document.createElement('meta');
            el.setAttribute(attr, name);
            document.head.appendChild(el);
        }
        el.setAttribute('content', content);
    };

    const fullUrl = "https://sergiobonatto.github.io" + (url.startsWith('/') ? url : '/' + url);
    const imgUrl = "https://sergiobonatto.github.io/public/SEO.png";

    setMeta('name', 'description', desc);
    setMeta('property', 'og:title', title);
    setMeta('property', 'og:description', desc);
    setMeta('property', 'og:url', fullUrl);
    setMeta('property', 'og:image', imgUrl);
    setMeta('name', 'twitter:title', title);
    setMeta('name', 'twitter:description', desc);
    setMeta('name', 'twitter:image', imgUrl);
});

EM_JS(void, sys_console_log, (const char *msg_ptr), {
    console.log(UTF8ToString(msg_ptr));
});

EM_JS(double, sys_now, (void), {
    return performance.now();
});


// From render.c
EM_JS(void, update_theme_colors, (const struct theme *t, const char *const *palette), {
	if (!Module.gfx) return;
	
	const rootStyle = document.documentElement.style;
	const getPalette = (idx) => UTF8ToString(HEAP32[(palette >> 2) + idx]);

	/* t layout: bg(0), text(4), dim(8), accent(12), code_bg(16), code_border(20) */
	const bg_idx = HEAP32[t >> 2];
	const tx_idx = HEAP32[(t + 4) >> 2];
	const dm_idx = HEAP32[(t + 8) >> 2];
	const ac_idx = HEAP32[(t + 12) >> 2];
	const cb_idx = HEAP32[(t + 16) >> 2];
	const cr_idx = HEAP32[(t + 20) >> 2];

	const bg = getPalette(bg_idx);
	const text = getPalette(tx_idx);

	rootStyle.setProperty('--bg-color', bg);
	rootStyle.setProperty('--text-color', text);
	rootStyle.setProperty('--dim-text-color', getPalette(dm_idx));
	rootStyle.setProperty('--accent-color', getPalette(ac_idx));
	rootStyle.setProperty('--code-bg-color', getPalette(cb_idx));
	rootStyle.setProperty('--code-border-color', getPalette(cr_idx));

	for (let i = 0; i < 16; i++) {
		rootStyle.setProperty('--nord' + i, getPalette(i));
	}

	Module.gfx.bg = bg;
	Module.gfx.textColor = text;
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
		textColor: ""
	};

	const onResize = () => {
		cvs.width   = window.innerWidth;
		cvs.height  = window.innerHeight;
	};

	window.addEventListener('resize', onResize);
	onResize();
});

EM_JS(void, render_update_strings, (const char *label_ptr, int text_color_idx, const char *const *palette), {
	if (!Module.gfx) return;
	const getPalette = (idx) => UTF8ToString(HEAP32[(palette >> 2) + idx]);

	Module.gfx.label         = UTF8ToString(label_ptr);
	Module.gfx.textColor     = getPalette(text_color_idx);
});

EM_JS(void, apply_style, (const char *selector_cstr, const char *style_cstr), {
	const selector  = UTF8ToString(selector_cstr);
	const style     = UTF8ToString(style_cstr);
	const elements  = document.querySelectorAll(selector);
	elements.forEach(el => el.style.cssText = style);
});

EM_JS(void, draw_frame, (float t), {
	const gfx = Module.gfx;
	if (!gfx?.ctx) return;

	const {ctx, cvs, header_h, bg, label, textColor} = gfx;
	const W = cvs.width;
	const H = cvs.height;

	ctx.fillStyle = bg;
	ctx.fillRect(0, 0, W, H);

	ctx.font 		= "bold 40px 'Courier New', monospace";
	ctx.fillStyle 	= textColor;
	ctx.textAlign 	= "center";
	ctx.fillText(label, W / 2, header_h / 2);
});

// From ui.c
EM_JS(void, add_theme_toggle, (const char *label_cstr, const char *style_cstr), {
	const label = UTF8ToString(label_cstr);
	const style = UTF8ToString(style_cstr);

	const btn 			= document.createElement("div");
	btn.id 				= "theme-toggle";
	btn.textContent 	= label;
	btn.style.cssText 	= style;

	btn.onclick = () => {
		if (Module._ui_toggle_theme) {
			Module._ui_toggle_theme();
		}
	};

	document.body.appendChild(btn);
});

EM_JS(void, add_nav_link, (const char *label_cstr, const char *style_cstr, const char *id_cstr), {
	const label = UTF8ToString(label_cstr);
	const style = UTF8ToString(style_cstr);
	const id 	= UTF8ToString(id_cstr);

	const btn           = document.createElement("div");
	btn.id 			    = id;
	btn.textContent 	= label;
	btn.style.cssText   = style;

	btn.onclick = () => {
		if (Module._switch_page) {
			const isBlog = btn.id === "nav-blog";
			Module._switch_page(isBlog);
		}
	};

	document.body.appendChild(btn);
});

EM_JS(void, update_theme_toggle_label, (const char *label_cstr), {
	const label = UTF8ToString(label_cstr);
	const btn   = document.getElementById("theme-toggle");
	if (btn)
		btn.textContent = label;
});
