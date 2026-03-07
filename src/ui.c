#include <emscripten.h>
#include "ui.h"

EM_JS(void, add_paragraph, (const char *cstr), {
	try {
		const text = UTF8ToString(cstr);

		// Cache feed reference to avoid repeated DOM lookups
		if (!Module.ui_feed) {
			Module.ui_feed = document.getElementById("feed");
		}

		const feed = Module.ui_feed;
		if (!feed) {
			console.error("feed element not found");
			return;
		}

		const p	      = document.createElement("p");
		p.className   = "para";
		p.textContent = "> " + text;

		feed.appendChild(p);
		feed.scrollTop = feed.scrollHeight;

	} catch (e) {
		console.error("add_paragraph failed:", e);
	}
});

EM_JS(void, add_image, (const char *path_cstr, const char *alt_cstr, int width, int height), {
	try {
		const url = UTF8ToString(path_cstr);
		const alt = UTF8ToString(alt_cstr);

		if (!Module.ui_feed) {
			Module.ui_feed = document.getElementById("feed");
		}

		const feed = Module.ui_feed;
		if (!feed) {
			console.error("feed element not found");
			return;
		}

		const container	    = document.createElement("div");
		container.className = "para";

		const img = document.createElement("img");
		img.src	  = url;
		img.alt	  = alt;

		if (width > 0)
			img.style.width = width + "px";
		if (height > 0)
			img.style.height = height + "px";
		if (width <= 0 && height <= 0)
			img.style.maxWidth = "100%";

		container.appendChild(img);
		feed.appendChild(container);
		feed.scrollTop = feed.scrollHeight;

	} catch (e) {
		console.error("add_image failed:", e);
	}
});

EM_JS(void, add_theme_toggle, (const char *label_cstr, const char *style_cstr), {
	try {
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
				console.error("toggle_theme not found in Module");
			}
		};

		document.body.appendChild(btn);

	} catch (e) {
		console.error("add_theme_toggle failed:", e);
	}
});

EM_JS(void, update_theme_toggle_label, (const char *label_cstr), {
	try {
		const label = UTF8ToString(label_cstr);
		const btn   = document.getElementById("theme-toggle");
		if (btn)
			btn.textContent = label;
	} catch (e) {
		console.error("update_theme_toggle_label failed:", e);
	}
});
