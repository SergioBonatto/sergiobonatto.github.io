#include "config.h"

const struct timing_config timing = {
	.tick_delta		= 0.016f
};

const char *nord_palette[16] = {
	"#2E3440", "#3B4252", "#434C5E", "#4C566A",
	"#D8DEE9", "#E5E9F0", "#ECEFF4",
	"#8FBCBB", "#88C0D0", "#81A1C1", "#5E81AC",
	"#BF616A", "#D08770", "#EBCB8B", "#A3BE8C", "#B48EAD"
};

const struct theme theme_dark = {
	.bg          = 0,
	.text        = 6,
	.dim_text    = 9,
	.accent      = 14,
	.code_bg     = 1,
	.code_border = 3
};

const struct theme theme_light = {
	.bg          = 6,
	.text        = 0,
	.dim_text    = 3,
	.accent      = 14,
	.code_bg     = 5,
	.code_border = 4
};

const char *css_theme_toggle =
	"position: fixed; top: 20px; right: 20px; "
	"font-size: 14px; user-select: none; cursor: pointer;";

const char *css_nav_blog =
	"position: fixed; top: 20px; right: 80px; "
	"font-size: 14px; user-select: none; cursor: pointer;";

const char *css_nav_home =
	"position: fixed; top: 20px; right: 140px; "
	"font-size: 14px; user-select: none; cursor: pointer;";

const char *css_feed = "padding: 20px; max-width: 900px; margin-top: 100px; min-height: calc(100vh - 180px - 100px);";

const char *css_footer =
	"width: 100%; padding: 24px 0; display: flex; flex-direction: column; "
	"align-items: center; justify-content: space-between; "
	"background-color: var(--bg-color); color: var(--text-color); "
	"font-size: 14px; margin-top: 40px;";

const char *msg_header = "BONATTO";
const char *msg_whoami = "whoami";
const char *msg_github_url = "https://github.com/SergioBonatto";

const char *msg_bio =
	"Work spans formal methods, functional programming, and operating systems. "
	"Main interests include dependent type theory, proof languages, language "
	"interpreters, virtual machines, and minimalist kernels.";

const char *msg_seo =
	"Software engineer specialized in programming languages, compilers, formal methods, "
	"type systems, virtual machines, operating systems, and systems programming using "
	"C, Haskell, Rust, and TypeScript. Work includes Bend, HVM, Kind, Higher Order "
	"Company tooling, and Agoriz, with emphasis on dependent types, proof systems, "
	"interpreters, runtime architecture, memory management, distributed systems, "
	"functional computation, and minimalist kernel development.";

const char *msg_404_title = "404 - NOT FOUND";
const char *msg_404_body = "The page you are looking for does not exist or has been moved.";

const char *code_block_pre =
	"background:var(--code-bg-color);"
	"padding:16px;"
	"border-radius:4px;"
	"overflow-x:auto;"
	"border:1px solid var(--code-border-color);"
	"margin:20px 0;";

const char *code_block_code =
	"font-family:'Courier New',monospace;"
	"font-size:14px;"
	"line-height:1.5;"
	"color:var(--text-color);";

const char *footer_style =
		"<div style=\"max-width:900px;margin:0 auto;padding:0 20px;\">"
		"<div style=\"display:flex;justify-content:space-between;align-items:center;gap:16px;\">"
		"<div style=\"font-size:14px;display:flex;align-items:center;gap:8px;\">"
		"<span>&copy; %d [Bonatto]</span>"
		"<span style=\"color:var(--dim-text-color)\">&bull;</span>"
		"<span>Vim powered</span>"
		"<span style=\"color:var(--dim-text-color)\">&bull;</span>"
		"<a href=\"%s\" target=\"_blank\" style=\"color:var(--text-color);text-decoration:none;\">GitHub</a>"
		"</div></div></div>";
