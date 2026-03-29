#include <stdio.h>
#include <string.h>
#include <emscripten.h>
#include "config.h"
#include "state.h"
#include "buffer.h"
#include "js_api.h"
#include "ui.h"

void add_paragraph(const char *text, size_t len) {
	buf_append(&g_html_buf, "<p class=\"para\">> ");
	buf_escape(&g_html_buf, text, len);
	buf_append(&g_html_buf, "</p>");
}

void add_code_block( 
		struct str_view lang,
		struct str_view code) {
	buf_printf(
		&g_html_buf,
		"<pre style=\"%s\">"
		"<code class=\"language-%.*s\" style=\"%s\">", 
		code_block_pre,
		(int)lang.len,
		lang.data,
		code_block_code
		);
	buf_escape(&g_html_buf, code.data, code.len);
	buf_append(&g_html_buf, "</code></pre>");
}

void add_image(const char *path, size_t path_len, const char *alt, size_t alt_len, float scale, int width, int height, int is_lcp) {
	buf_printf(&g_html_buf, "<p class=\"para\"><img src=\"%.*s\" alt=\"%.*s\" %s %s %s style=\"max-width:100%%;height:auto;%s\"",
	           (int)path_len, path, (int)alt_len, alt ? alt : "",
	           is_lcp ? "fetchpriority=\"high\" loading=\"eager\"" : "loading=\"lazy\"",
	           width > 0 ? "width=\"...\"" : "",
	           height > 0 ? "height=\"...\"" : "",
	           (scale > 0 && scale != 1.0f) ? "width:auto;" : "");

	if (scale > 0 && scale != 1.0f) {
		buf_printf(&g_html_buf, " onload=\"this.style.width=(this.naturalWidth*%f)+'px'\"", scale);
	}

	buf_append(&g_html_buf, "></p>");
}

void add_blog_entry(const char *title, const char *date, const char *slug, int index) {
	buf_printf(&g_html_buf,
	           "<div style=\"margin-bottom:20px;display:flex;gap:20px;\">"
	           "<span style=\"color:var(--dim-text-color);min-width:100px;\">%s</span>"
	           "<a href=\"#/post/%s\" onclick=\"Module._open_article(%d);return false;\" style=\"color:var(--text-color);text-decoration:none;\">%s</a>"
	           "</div>",
	           date, slug, index, title);
}

void add_bar(int h, int w, const float *pcts, const char **colors, const float *opacities, const int *styles, int n) {
	buf_printf(&g_html_buf, "<div style=\"width:%dpx;height:%dpx;border:1px solid var(--text-color);display:flex;flex-direction:column;\">", w, h);
	for (int i = 0; i < n; i++) {
		const char *style_str = "";
		if (styles[i] == BAR_SEG_HATCHED) {
			style_str = "background-image:repeating-linear-gradient(45deg,transparent,transparent 2px,var(%s) 2px,var(%s) 3px);";
		} else {
			style_str = "background:var(%s);";
		}

		buf_printf(&g_html_buf, "<div style=\"height:%.1f%%;opacity:%.2f;", pcts[i] * 100.0f, opacities[i]);
		buf_printf(&g_html_buf, style_str, colors[i], colors[i]);
		buf_append(&g_html_buf, "\"></div>");
	}
	buf_append(&g_html_buf, "</div>");
}

void ui_begin_render(void) {
	buf_reset(&g_html_buf);
}

void ui_end_render(void) {
	sys_set_html("#feed", g_html_buf.data);
}

void clear_feed(void) {
	sys_set_html("#feed", "");
}

void add_footer(int year, const char *style, const char *github_url) {
	char footer_html[1024];
	snprintf(footer_html, sizeof(footer_html),
		footer_style, year, github_url);
	sys_append_child("body", "footer", footer_html);
	sys_set_style("footer", style);
}

EMSCRIPTEN_KEEPALIVE
void ui_toggle_theme(void) {
	state.is_dark = !state.is_dark;
	state.theme = state.is_dark ? &theme_dark : &theme_light;
	update_theme_toggle_label(state.is_dark ? ":light" : ":dark");
	update_theme_colors(state.theme, palette);
	render_update_strings(msg_header, state.theme->text, palette);
}

void ui_init_router(void) { sys_init_router(); }
void ui_sync_url(const char *path) { sys_update_url(path); }
void ui_get_current_hash(char *buf, int max_len) { sys_get_url_hash(buf, max_len); }
void update_seo_metadata(const char *t, const char *d, const char *u) { sys_set_meta(t, d, u); }
