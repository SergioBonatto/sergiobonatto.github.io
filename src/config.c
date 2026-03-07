#include "config.h"

const struct timing_config timing = {
	.tick_delta		= 0.016f,
	.flicker_speed	= 12.0f,
	.flicker_base	= 0.8f,
	.flicker_amp	= 0.15f,
	.scanline_speed = 120.0f
};

const struct theme theme_dark = {
	.bg	  		= "#2E3440",
	.text	  	= "#ECEFF4",
	.scanline 	= "rgba(236,239,244,0.035)"
};

const struct theme theme_light = {
	.bg	  		= "#ECEFF4",
	.text	  	= "#2E3440",
	.scanline 	= "rgba(46,52,64,0.035)"
};

const char *css_theme_toggle =
	"position: fixed; top: 20px; right: 30px; "
	"font-size: 14px; user-select: none; cursor: pointer;";

const char *css_feed = "padding: 20px; max-width: 900px; margin-top: 180px;";

const char *msg_header = "BONATTO";
const char *msg_whoami = "whoami";

const char *msg_bio =
	"Work spans formal methods, functional programming, and operating systems. "
	"Main interests include dependent type theory, proof languages, language "
	"interpreters, virtual machines, and minimalist kernels.";
