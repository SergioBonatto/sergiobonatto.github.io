#include "config.h"

const struct timing_config timing = {
	.tick_delta		= 0.016f,
	.flicker_speed	= 12.0f,
	.flicker_base	= 0.8f,
	.flicker_amp	= 0.15f,
	.scanline_speed = 120.0f
};

const struct theme theme_dark = {
	.bg			= "#2E3440",
	.text		= "#ECEFF4",
	.dim_text	= "#81A1C1",
	.scanline	= "rgba(236,239,244,0.035)",
	.accent		= "#A3BE8C",

	.nord0		= "#2E3440",
	.nord1		= "#3B4252",
	.nord2		= "#434C5E",
	.nord3		= "#4C566A",
	.nord4		= "#D8DEE9",
	.nord5		= "#E5E9F0",
	.nord6		= "#ECEFF4",
	.nord7		= "#8FBCBB",
	.nord8		= "#88C0D0",
	.nord9		= "#81A1C1",
	.nord10		= "#5E81AC",
	.nord11		= "#BF616A",
	.nord12		= "#D08770",
	.nord13		= "#EBCB8B",
	.nord14		= "#A3BE8C",
	.nord15		= "#B48EAD"
};

const struct theme theme_light = {
	.bg			= "#ECEFF4",
	.text		= "#2E3440",
	.dim_text	= "#4C566A",
	.scanline	= "rgba(46,52,64,0.035)",
	.accent		= "#A3BE8C",

	.nord0		= "#2E3440",
	.nord1		= "#3B4252",
	.nord2		= "#434C5E",
	.nord3		= "#4C566A",
	.nord4		= "#D8DEE9",
	.nord5		= "#E5E9F0",
	.nord6		= "#ECEFF4",
	.nord7		= "#8FBCBB",
	.nord8		= "#88C0D0",
	.nord9		= "#81A1C1",
	.nord10		= "#5E81AC",
	.nord11		= "#BF616A",
	.nord12		= "#D08770",
	.nord13		= "#EBCB8B",
	.nord14		= "#A3BE8C",
	.nord15		= "#B48EAD"
};


const char *css_theme_toggle =
	"position: fixed; top: 20px; right: 30px; "
	"font-size: 14px; user-select: none; cursor: pointer;";

const char *css_nav_blog =
	"position: fixed; top: 20px; right: 90px; "
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

/* 
 * NOTE: The 'posts' array and 'posts_count' are now auto-generated
 * by tools/packer.c and reside in src/contents_data.h
 */

const char *msg_header = "BONATTO";
const char *msg_whoami = "whoami";

const char *msg_bio =
	"Work spans formal methods, functional programming, and operating systems. "
	"Main interests include dependent type theory, proof languages, language "
	"interpreters, virtual machines, and minimalist kernels.";

const char *msg_404_title = "404 - NOT FOUND";
const char *msg_404_body = "The page you are looking for does not exist or has been moved.";
