#include <emscripten.h>
#include "ui.h"

/*
 * Internal JS helpers to reduce boilerplate and follow "Dry" principles.
 * These are attached to Module to be accessible across EM_JS blocks.
 */
EM_JS(void, ui_init_internal, (void), {
	if (Module.ui_initialized) return;

	Module.ui_get_feed = () => {
		if (!Module.ui_feed) {
			Module.ui_feed = document.getElementById("feed");
			if (!Module.ui_feed) console.error("UI: #feed not found");
		}
		return Module.ui_feed;
	};

	Module.ui_append_para = (content) => {
		const feed = Module.ui_get_feed();
		if (!feed) return;

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

EM_JS(void, add_paragraph, (const char *cstr), {
	ui_init_internal();
	let text = UTF8ToString(cstr);
	// Basic cleanup for carriage returns avoiding regex literals to bypass C preprocessor
	text = text.split(String.fromCharCode(13)).join("");
	Module.ui_append_para(text);
});

EM_JS(void, add_image, (const char *path_cstr, const char *alt_cstr, float scale), {
	ui_init_internal();
	const url = UTF8ToString(path_cstr);
	const alt = UTF8ToString(alt_cstr);

	const img = document.createElement("img");
	img.src	  = url;
	img.alt	  = alt;
	img.style.maxWidth = "100%";

	// Scale image proportionally once loaded
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

	const btn	  = document.createElement("div");
	btn.id		  = "theme-toggle";
	btn.textContent	  = label;
	btn.style.cssText = style;

	btn.onclick = () => {
		if (Module._toggle_theme) {
			Module._toggle_theme();
		} else {
			console.warn("UI: toggle_theme symbol not exported");
		}
	};

	document.body.appendChild(btn);
});

EM_JS(void, add_footer, (int year, const char *style_cstr), {
	const style = UTF8ToString(style_cstr);

	const footer = document.createElement("footer");
	footer.id    = "main-footer";
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
	if (feed) feed.innerHTML = "";
});

EM_JS(void, add_nav_link, (const char *label_cstr, const char *style_cstr, const char *id_cstr), {
	const label = UTF8ToString(label_cstr);
	const style = UTF8ToString(style_cstr);
	const id	= UTF8ToString(id_cstr);

	const btn	  = document.createElement("div");
	btn.id		  = id;
	btn.textContent	  = label;
	btn.style.cssText = style;

	btn.onclick = () => {
		if (Module._switch_page) {
			// Navigate based on button ID
			const isBlog = btn.id === "nav-blog";
			Module._switch_page(isBlog);
		}
	};

	document.body.appendChild(btn);
});

EM_JS(void, add_blog_entry, (const char *title_cstr, const char *date_cstr, int index), {
	ui_init_internal();
	const title = UTF8ToString(title_cstr);
	const date  = UTF8ToString(date_cstr);

	const container = document.createElement("div");
	container.style.marginBottom = "20px";
	container.style.display = "flex";
	container.style.gap = "20px";

	const dateEl = document.createElement("span");
	dateEl.style.color = "var(--dim-text-color)";
	dateEl.style.minWidth = "100px";
	dateEl.textContent = date;

	const link = document.createElement("a");
	link.href = "#";
	link.textContent = title;
	link.style.color = "var(--text-color)";
	link.style.textDecoration = "none";
	link.style.cursor = "pointer";
	link.onmouseover = () => link.style.textDecoration = "underline";
	link.onmouseout = () => link.style.textDecoration = "none";

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
	const btn   = document.getElementById("theme-toggle");
	if (btn) btn.textContent = label;
});
