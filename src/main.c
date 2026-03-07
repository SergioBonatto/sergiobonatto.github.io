#include <emscripten.h>
#include <stdbool.h>
#include <time.h>

#include "config.h"
#include "render.h"
#include "ui.h"

static float runtime = 0.0f;
static bool is_dark = true;
static const struct theme *cur_theme;

EMSCRIPTEN_KEEPALIVE
void toggle_theme(void) {
	is_dark = !is_dark;
	cur_theme = is_dark ? &theme_dark : &theme_light;

	update_theme_toggle_label(is_dark ? ":light" : ":dark");
	update_theme_colors(cur_theme->bg, cur_theme->text,
			    cur_theme->dim_text, cur_theme->scanline);
}

void main_tick(void) {
	runtime += timing.tick_delta;
	draw_frame(runtime, msg_header, cur_theme->text, cur_theme->scanline);
}

int main(void) {
	cur_theme = &theme_dark;

	init_graphics(cur_theme->bg, cur_theme->text, cur_theme->dim_text, UI_HEADER_HEIGHT);

	apply_style("#feed", css_feed);
	add_theme_toggle(":light", css_theme_toggle);

	add_image("pfp.png", NULL, 1.0f);
	add_paragraph(msg_whoami);
	add_paragraph(msg_bio);

	time_t t = time(NULL);
	struct tm tm = *localtime(&t);
	int year = tm.tm_year + 1900;

	add_footer(year, css_footer);

	emscripten_set_main_loop(main_tick, 0, 1);

	return 0;
}
