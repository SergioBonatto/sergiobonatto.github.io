#include <emscripten.h>
#include <stdbool.h>

#include "config.h"
#include "render.h"
#include "ui.h"

static float runtime = 0.0f;
static bool  is_dark = true;

static const char* cur_bg;
static const char* cur_text;
static const char* cur_scanline;

static const char* SCANLINE_DARK = "rgba(236,239,244,0.035)";
static const char* SCANLINE_LIGHT = "rgba(46,52,64,0.035)";

EMSCRIPTEN_KEEPALIVE
void toggle_theme(void)
{
    is_dark = !is_dark;

    if (is_dark) {
        cur_bg       = COLOR_BG;
        cur_text     = COLOR_TEXT_PRIMARY;
        cur_scanline = SCANLINE_DARK;
        update_theme_toggle_label(":light");
    } else {
        cur_bg       = COLOR_TEXT_PRIMARY;
        cur_text     = COLOR_BG;
        cur_scanline = SCANLINE_LIGHT;
        update_theme_toggle_label(":dark");
    }

    update_theme_colors(cur_bg, cur_text, cur_scanline);
}

void main_tick(void)
{
    runtime += TIMING.tick_delta;

    draw_frame(runtime, MSG_HEADER, cur_text, cur_scanline);
}

int main(void)
{
    // Initial theme state
    cur_bg       = COLOR_BG;
    cur_text     = COLOR_TEXT_PRIMARY;
    cur_scanline = SCANLINE_DARK;

    // SSoT: Centralized style initialization from C
    init_graphics(cur_bg, cur_text, HEADER_HEIGHT);

    add_theme_toggle(":light");

    add_image("pfp.png", NULL, -1, -1);
    add_paragraph(MSG_WHOAMI);
    add_paragraph(MSG_BIO);

    emscripten_set_main_loop(main_tick, 0, 1);

    return 0;
}
