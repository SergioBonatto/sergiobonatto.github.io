#include <emscripten.h>
#include <stdbool.h>

#include "config.h"
#include "render.h"
#include "ui.h"

static float runtime = 0.0f;
static bool  is_dark = true;
static const Theme* cur_theme;

/* EMSCRIPTEN_KEEPALIVE */
void toggle_theme(void)
{
    is_dark = !is_dark;
    cur_theme = is_dark ? &THEME_DARK : &THEME_LIGHT;

    update_theme_toggle_label(is_dark ? ":light" : ":dark");
    update_theme_colors(cur_theme->bg, cur_theme->text, cur_theme->scanline);
}

void main_tick(void)
{
    runtime += TIMING.tick_delta;
    draw_frame(runtime, MSG_HEADER, cur_theme->text, cur_theme->scanline);
}

int main(void)
{
    // SSoT: Centralized Theme & UI state
    cur_theme = &THEME_DARK;

    init_graphics(cur_theme->bg, cur_theme->text, HEADER_HEIGHT);
    
    // Applying CSS styles directly from C constants
    apply_style("#feed", CSS_FEED);
    add_theme_toggle(":light", CSS_THEME_TOGGLE);

    add_image("pfp.png", NULL, -1, -1);
    add_paragraph(MSG_WHOAMI);
    add_paragraph(MSG_BIO);

    emscripten_set_main_loop(main_tick, 0, 1);

    return 0;
}
