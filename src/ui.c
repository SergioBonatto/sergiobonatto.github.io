#include <emscripten.h>
#include "ui.h"

EM_JS(void, add_paragraph, (const char* cstr), {

    try {

        const text =
            (typeof UTF8ToString !== 'undefined')
            ? UTF8ToString(cstr)
            : Module.UTF8ToString(cstr);

        const feed = document.getElementById("feed");

        if (!feed) {
            console.error("feed element not found");
            return;
        }

        const p = document.createElement("p");

        p.className = "para";
        p.textContent = "> " + text;

        feed.appendChild(p);

        feed.scrollTop = feed.scrollHeight;

    } catch (e) {

        console.error("add_paragraph failed:", e);

    }

});

EM_JS(void, add_image, (const char* path_cstr, const char* alt_cstr, int width, int height), {

    try {

        const url =
            (typeof UTF8ToString !== 'undefined')
            ? UTF8ToString(path_cstr)
            : Module.UTF8ToString(path_cstr);

        const alt =
            (typeof UTF8ToString !== 'undefined')
            ? UTF8ToString(alt_cstr)
            : Module.UTF8ToString(alt_cstr);

        const feed = document.getElementById("feed");

        if (!feed) {
            console.error("feed element not found");
            return;
        }

        const container = document.createElement("div");

        container.className = "para";

        const img = document.createElement("img");

        img.src = url;
        img.alt = alt;

        if (width > 0) img.style.width = width + "px";
        if (height > 0) img.style.height = height + "px";
        if (width <= 0 && height <= 0) img.style.maxWidth = "100%";

        container.appendChild(img);
        feed.appendChild(container);
        feed.scrollTop = feed.scrollHeight;

    } catch (e) {

        console.error("add_image failed:", e);

    }

});
