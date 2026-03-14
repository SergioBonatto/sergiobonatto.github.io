#include <emscripten.h>
#include "ui.h"
#include "graph.h"

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

EM_JS(void, add_image, (const char *path_ptr, size_t path_len, const char *alt_ptr, size_t alt_len, float scale), {
	ui_init_internal();
	
	const decoder = new TextDecoder("utf-8");
	const url = decoder.decode(HEAPU8.subarray(path_ptr, path_ptr + path_len));
	
	let alt = "";
	if (alt_ptr && alt_len > 0) {
		alt = decoder.decode(HEAPU8.subarray(alt_ptr, alt_ptr + alt_len));
	}

	const img 			= document.createElement("img");
	img.src 			= url;
	img.alt 			= alt;
	img.style.maxWidth 	= "100%";

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
		if (Module._toggle_theme) {
			Module._toggle_theme();
		}
	};

	document.body.appendChild(btn);
});

EM_JS(void, add_footer, (int year, const char *style_cstr), {
	const style = UTF8ToString(style_cstr);

	const footer = document.createElement("footer");
	footer.id = "main-footer";
	footer.style.cssText = style;

	footer.innerHTML = `
		<div style="max-width: 1280px; margin: 0 auto; padding: 0 8px;">
			<div style="display: flex; flex-direction: column; align-items: center; justify-content: space-between; gap: 16px;">
				<div style="display: flex; align-items: center; gap: 12px;">
					<div style="font-size: 14px; display: flex; align-items: center; gap: 8px;">
						<span>&copy; ${Number(year)}</span>
						<span style="color: var(--dim-text-color)">&bull;</span>
						<span>[Bonatto]</span>
						<span style="color: var(--dim-text-color)">&bull;</span>
						<span>Vim powered</span>
					</div>
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
	const id 	= UTF8ToString(id_cstr);

	const btn 			= document.createElement("div");
	btn.id 				= id;
	btn.textContent 	= label;
	btn.style.cssText 	= style;

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

EM_JS(void, add_bar, (int height, int width, const struct bar_segment *segs, int n), {
	ui_init_internal();

	const container = document.createElement("div");
	container.style.width 			= width + "px";
	container.style.height 			= height + "px";
	container.style.border 			= "1px solid var(--text-color)";
	container.style.display 		= "flex";
	container.style.flexDirection 	= "column";

	/* 
	 * WASM32 memory layout for struct bar_segment:
	 * +0  pct        (float, 4 bytes)
	 * +4  color_var  (ptr,   4 bytes)
	 * +8  opacity    (float, 4 bytes)
	 * +12 style      (int,   4 bytes)
	 * Stride = 16 bytes.
	 */
	const STRIDE = 16;

	for (let i = 0; i < n; i++) {
		const base = segs + (i * STRIDE);

		const pct 		= HEAPF32[base >> 2];
		const color_ptr = HEAP32[(base + 4) >> 2];
		const opacity 	= HEAPF32[(base + 8) >> 2];
		const style 	= HEAP32[(base + 12) >> 2];

		const color_var = UTF8ToString(color_ptr);

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
