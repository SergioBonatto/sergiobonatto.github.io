#include <emscripten.h>
#include <string.h>

#include "config.h"
#include "render.h"
#include "ui.h"
#include "router.h"
#include "pages.h"

struct site_state state = {
	.runtime = 0.0f,
	.is_dark = true,
	.theme = &theme_dark,
	.page = PAGE_INITIAL
};

int main(void)
{
	char initial_hash[256];

	init_graphics(state.theme, UI_HEADER_HEIGHT);

	apply_style("#feed", css_feed);
	add_theme_toggle(":light", css_theme_toggle);
	add_nav_link(":blog", css_nav_blog, "nav-blog");
	add_nav_link(":home", css_nav_home, "nav-home");

	ui_init_router();
	ui_get_current_hash(initial_hash, sizeof(initial_hash));

	if (initial_hash[0] != '\0' && strcmp(initial_hash, "#/") != 0)
		handle_route(initial_hash);
	else
		switch_page(false);

	page_add_footer();

	emscripten_set_main_loop(render_tick, 0, 1);

	return 0;
}
