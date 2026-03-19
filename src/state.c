#include <emscripten.h>
#include "state.h"
#include "ui.h"
#include "render.h"

struct site_state state = {
	.runtime = 0.0f,
	.is_dark = true,
	.theme = &theme_dark,
	.page = PAGE_INITIAL
};

EMSCRIPTEN_KEEPALIVE
void toggle_theme(void)
{
	state.is_dark = !state.is_dark;
	state.theme = state.is_dark ? &theme_dark : &theme_light;

	update_theme_toggle_label(state.is_dark ? ":light" : ":dark");
	update_theme_colors(state.theme);
}

void main_tick(void)
{
	state.runtime += timing.tick_delta;
	draw_frame(state.runtime, msg_header, state.theme->text,
		   state.theme->scanline);
}
