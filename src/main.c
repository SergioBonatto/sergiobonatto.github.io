#include <emscripten.h>

#include "config.h"
#include "render.h"
#include "ui.h"

static float runtime = 0.0f;

void main_tick(void)
{
    runtime += TIMING.tick_delta;

    draw_frame(runtime);
}

int main(void)
{
    add_image("../pfp.png", NULL, -1, -1);
    add_paragraph(MSG_WHOAMI);
    add_paragraph(MSG_BIO);

    emscripten_set_main_loop(main_tick, 0, 1);

    return 0;
}
