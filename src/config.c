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
	.dim_text 	= "#81A1C1",
	.scanline 	= "rgba(236,239,244,0.035)"
};

const struct theme theme_light = {
	.bg	  		= "#ECEFF4",
	.text	  	= "#2E3440",
	.dim_text 	= "#4C566A",
	.scanline 	= "rgba(46,52,64,0.035)"
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

const struct blog_post posts[] = {
	{
		.title = "A bíblia e o argumento circular da autoridade: uma análise crítica",
		.date  = "2026-03-04",
		.slug  = "post1"
	},
	{
		.title = "DeepFake e a desumanização na época das IAs",
		.date  = "2026-01-02",
		.slug  = "post2"
	},
	{
		.title = "O Estado Como Motor da Economia: Uma Falácia Sustentada pela Tecnologia Descentralizada",
		.date  = "2025-12-30",
		.slug  = "o_estado_como_motor_da_economia"
	},
	{
		.title = "Minha Noite Estrelada",
		.date  = "2025-12-09",
		.slug  = "minha-noite-estrelada"
	},
	{
		.title = "Como entender a existência da morte antes do pecado de Adão?",
		.date  = "2025-09-05",
		.slug  = "Criacionismo-Evolutivo-1"
	}
};

const int posts_count = sizeof(posts) / sizeof(posts[0]);

const char *msg_header = "BONATTO";
const char *msg_whoami = "whoami";

const char *msg_bio =
	"Work spans formal methods, functional programming, and operating systems. "
	"Main interests include dependent type theory, proof languages, language "
	"interpreters, virtual machines, and minimalist kernels.";
