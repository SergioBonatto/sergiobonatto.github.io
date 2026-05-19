#include <emscripten.h>
#include <string.h>

#include "state.h"
#include "render.h"
#include "ui.h"
#include "router.h"
#include "pages.h"
#include "sys.h"

struct site_state state = {
	.runtime	= 0.0f,
	.is_dark	= false,
	.theme		= &theme_light,
	.page		= PAGE_INITIAL
};

int main(void) {
	char initial_hash[256];

	state.is_dark = sys_load_theme() == 1;
	state.theme = state.is_dark ? &theme_dark : &theme_light;

	init_graphics(state.theme, UI_HEADER_HEIGHT);
	update_theme_colors(state.theme, palette);
	render_update_strings(msg_header, state.theme->text, palette);

	apply_style("#feed", css_feed);
	add_theme_toggle(state.is_dark ? ":light" : ":dark", css_theme_toggle);
	add_nav_link(":blog", css_nav_blog, "nav-blog");
	add_nav_link(":home", css_nav_home, "nav-home");

	ui_init_router();
	ui_get_current_hash(initial_hash, sizeof(initial_hash));

	if (initial_hash[0] != '\0' && strcmp(initial_hash, "#/") != 0) {
		handle_route(initial_hash);
	} else {
		switch_page(false);
	}

	page_add_footer();
	draw_frame();

	return 0;
}
