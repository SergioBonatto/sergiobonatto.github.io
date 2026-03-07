#include <emscripten.h>

#include "config.h"
#include "render.h"
#include "ui.h"

static float runtime = 0.0f;

void main_tick(void)
{
    runtime += TIMING.tick_delta;

    draw_frame(runtime, MSG_HEADER, COLOR_TEXT_PRIMARY, COLOR_SCANLINE);
}

int main(void)
{
    // SSoT: Centralized style initialization from C
    init_graphics(COLOR_BG, COLOR_TEXT_PRIMARY, HEADER_HEIGHT);

    add_image("pfp.png", NULL, -1, -1);
    add_paragraph(MSG_WHOAMI);
    add_paragraph(MSG_BIO);

    emscripten_set_main_loop(main_tick, 0, 1);

    return 0;
}
