#include <emscripten.h>
#include "ui.h"
#include "state.h"
#include "render.h"

EM_JS(void, ui_init_internal, (void), {
	if (Module.ui_initialized)
		return;

	Module.ui_get_feed = () => {
		if (!Module.ui_feed) {
			Module.ui_feed = document.getElementById("feed");
		}
		return Module.ui_feed;
	};

	Module.ui_append_para = (content) => {
		const feed = Module.ui_get_feed();
		if (!feed)
			return;

		const p = document.createElement("p");
		p.className = "para";
		if (typeof content === "string") {
			p.textContent = "> " + content;
		} else {
			p.appendChild(content);
		}

		feed.appendChild(p);
		feed.scrollTop = feed.scrollHeight;
		return p;
	};

	Module.ui_initialized = true;
});

EM_JS(void, add_paragraph, (const char *ptr, size_t len), {
	ui_init_internal();
	
	const view 		= HEAPU8.subarray(ptr, ptr + len);
	const decoder 	= new TextDecoder("utf-8");
	const text 		= decoder.decode(view);
	
	Module.ui_append_para(text);
});

EM_JS(void, add_code_block, (const char *lang_ptr, size_t lang_len, const char *code_ptr, size_t code_len), {
	ui_init_internal();

	const decoder = new TextDecoder("utf-8");
	const lang = decoder.decode(HEAPU8.subarray(lang_ptr, lang_ptr + lang_len));
	const code = decoder.decode(HEAPU8.subarray(code_ptr, code_ptr + code_len));

	const pre = document.createElement("pre");
	pre.style.background = "var(--nord1)";
	pre.style.padding = "16px";
	pre.style.borderRadius = "4px";
	pre.style.overflowX = "auto";
	pre.style.border = "1px solid var(--nord3)";
	pre.style.margin = "20px 0";

	const code_el = document.createElement("code");
	code_el.className = "language-" + lang;
	code_el.textContent = code;
	code_el.style.fontFamily = "'Courier New', monospace";
	code_el.style.fontSize = "14px";
	code_el.style.lineHeight = "1.5";
	code_el.style.color = "var(--text-color)";

	pre.appendChild(code_el);
	Module.ui_append_para(pre);
});

EM_JS(void, add_image, (const char *path_ptr, size_t path_len, const char *alt_ptr, size_t alt_len, float scale, int width, int height, int is_lcp), {
	ui_init_internal();
	
	const decoder = new TextDecoder("utf-8");
	const url = decoder.decode(HEAPU8.subarray(path_ptr, path_ptr + path_len));
	
	let alt = "";
	if (alt_ptr && alt_len > 0) {
		alt = decoder.decode(HEAPU8.subarray(alt_ptr, alt_ptr + alt_len));
	}

	const img 		= document.createElement("img");
	img.src 			= url;
	img.alt 			= alt;
	
	if (is_lcp) {
		img.fetchPriority = "high";
		img.loading = "eager";
	} else {
		img.loading = "lazy";
	}

	if (width > 0) img.width = width;
	if (height > 0) img.height = height;

	img.style.maxWidth 	= "100%";
	img.style.height 	= "auto";

	if (scale > 0 && scale !== 1.0) {
		img.onload = () => {
			img.style.width = (img.naturalWidth * scale) + "px";
		};
	}

	Module.ui_append_para(img);
});

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

EM_JS(void, add_footer, (int year, const char *style_cstr, const char *github_url_cstr), {
	const style = UTF8ToString(style_cstr);
	const github_url = UTF8ToString(github_url_cstr);

	const footer = document.createElement("footer");
	footer.id = "main-footer";
	footer.style.cssText = style;

	footer.innerHTML = `
		<div style="max-width: 900px; margin: 0 auto; padding: 0 20px;">
			<div style="display: flex; justify-content: space-between; align-items: center; gap: 16px;">
				<div style="font-size: 14px; display: flex; align-items: center; gap: 8px;">
					<span>&copy; ${Number(year)} [Bonatto]</span>
					<span style="color: var(--dim-text-color)">&bull;</span>
					<span>Vim powered</span>
					<span style="color: var(--dim-text-color)">&bull;</span>
					<a href="${github_url}" target="_blank" style="color: var(--text-color); text-decoration: none; font-size: 14px;">GitHub</a>
				</div>
			</div>
		</div>
	`;

	document.body.appendChild(footer);
});

EM_JS(void, clear_feed, (void), {
	const feed = (Module.ui_get_feed) ? Module.ui_get_feed() : document.getElementById("feed");
	if (feed)
		feed.innerHTML = "";
});

EM_JS(void, add_nav_link, (const char *label_cstr, const char *style_cstr, const char *id_cstr), {
	const label = UTF8ToString(label_cstr);
	const style = UTF8ToString(style_cstr);
	const id 		= UTF8ToString(id_cstr);

	const btn 				= document.createElement("div");
	btn.id 						= id;
	btn.textContent 	= label;
	btn.style.cssText = style;

	btn.onclick = () => {
		if (Module._switch_page) {
			const isBlog = btn.id === "nav-blog";
			Module._switch_page(isBlog);
		}
	};

	document.body.appendChild(btn);
});

EM_JS(void, add_blog_entry, (const char *title_cstr, const char *date_cstr, int index), {
	ui_init_internal();
	const title = UTF8ToString(title_cstr);
	const date = UTF8ToString(date_cstr);

	const container 				= document.createElement("div");
	container.style.marginBottom 	= "20px";
	container.style.display 		= "flex";
	container.style.gap 			= "20px";

	const dateEl 				= document.createElement("span");
	dateEl.style.color 			= "var(--dim-text-color)";
	dateEl.style.minWidth 		= "100px";
	dateEl.textContent 			= date;

	const link 					= document.createElement("a");
	link.href 					= "#";
	link.textContent 			= title;
	link.style.color 			= "var(--text-color)";
	link.style.textDecoration 	= "none";
	link.style.cursor 			= "pointer";
	link.onmouseover 			= () => link.style.textDecoration = "underline";
	link.onmouseout 			= () => link.style.textDecoration = "none";

	link.onclick = (e) => {
		e.preventDefault();
		if (Module._open_article) {
			Module._open_article(index);
		}
	};

	container.appendChild(dateEl);
	container.appendChild(link);

	Module.ui_append_para(container);
});

EM_JS(void, update_theme_toggle_label, (const char *label_cstr), {
	const label = UTF8ToString(label_cstr);
	const btn = document.getElementById("theme-toggle");
	if (btn)
		btn.textContent = label;
});

EM_JS(void, ui_init_router, (void), {
	window.addEventListener('hashchange', () => {
		if (Module._handle_route) {
			const hash 		= window.location.hash || "#/";
			const length 	= lengthBytesUTF8(hash) + 1;
			const pathPtr 	= _malloc(length);
			stringToUTF8(hash, pathPtr, length);
			Module._handle_route(pathPtr);
			_free(pathPtr);
		}
	});
});

EM_JS(void, ui_sync_url, (const char *path_cstr), {
	const path = UTF8ToString(path_cstr);
	if (window.location.hash !== path) {
		history.pushState(null, "", path);
	}
});

EM_JS(void, ui_get_current_hash, (char *buf, int max_len), {
	const hash = window.location.hash || "#/";
	stringToUTF8(hash, buf, max_len);
});

EM_JS(void, update_seo_metadata, (const char *title_ptr, const char *desc_ptr, const char *url_ptr), {
	const title 	= UTF8ToString(title_ptr);
	const desc 		= UTF8ToString(desc_ptr);
	const url 		= UTF8ToString(url_ptr);
	const baseUrl 	= "https://sergiobonatto.github.io";
	const imgUrl 	= baseUrl + "/public/SEO.png";
	const fullUrl 	= baseUrl + (url.startsWith('/') ? url : '/' + url);

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

	setMeta('name', 'description', desc);
	setMeta('property', 'og:title', title);
	setMeta('property', 'og:description', desc);
	setMeta('property', 'og:url', fullUrl);
	setMeta('property', 'og:image', imgUrl);
	setMeta('property', 'og:site_name', 'Sergio Bonatto');
	setMeta('name', 'twitter:title', title);
	setMeta('name', 'twitter:description', desc);
	setMeta('name', 'twitter:image', imgUrl);
	setMeta('name', 'twitter:card', 'summary_large_image');
	setMeta('name', 'twitter:site', '@fibonatto');
	setMeta('name', 'twitter:creator', '@fibonatto');

	let canonical = document.querySelector('link[rel="canonical"]');
	if (!canonical) {
		canonical = document.createElement('link');
		canonical.setAttribute('rel', 'canonical');
		document.head.appendChild(canonical);
	}
	canonical.setAttribute('href', fullUrl);
});

EM_JS(void, add_bar, (int height, int width, const float *pcts, const char **colors, const float *opacities, const int *styles, int n), {
	ui_init_internal();

	const container = document.createElement("div");
	container.style.width 			= width + "px";
	container.style.height 			= height + "px";
	container.style.border 			= "1px solid var(--text-color)";
	container.style.display 		= "flex";
	container.style.flexDirection 	= "column";

	for (let i = 0; i < n; i++) {
		const pct 		= HEAPF32[(pcts >> 2) + i];
		const colorPtr 	= HEAP32[(colors >> 2) + i];
		const opacity 	= HEAPF32[(opacities >> 2) + i];
		const style 	= HEAP32[(styles >> 2) + i];

		const color_var = UTF8ToString(colorPtr);

		const seg 			= document.createElement("div");
		seg.style.height 	= (pct * 100) + "%";
		seg.style.opacity 	= opacity;

		if (style === 0) { /* BAR_SEG_SOLID */
			seg.style.background = "var(" + color_var + ")";
		} else if (style === 1) { /* BAR_SEG_HATCHED */
			const c = "var(" + color_var + ")";
			seg.style.backgroundImage =
				"repeating-linear-gradient(45deg, " +
				"transparent, transparent 2px, " +
				c + " 2px, " + c + " 3px)";
		}

		container.appendChild(seg);
	}

	Module.ui_append_para(container);
});

EMSCRIPTEN_KEEPALIVE
void ui_toggle_theme(void)
{
	state.is_dark = !state.is_dark;
	state.theme = state.is_dark ? &theme_dark : &theme_light;

	update_theme_toggle_label(state.is_dark ? ":light" : ":dark");
	update_theme_colors(state.theme, nord_palette);
	render_update_strings(msg_header, state.theme->text, nord_palette);
}
