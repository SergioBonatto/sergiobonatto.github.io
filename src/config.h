#ifndef CONFIG_H
#define CONFIG_H

typedef struct {
    float tick_delta;
    float flicker_speed;
    float flicker_base;
    float flicker_amp;
    float scanline_speed;
} TimingConfig;

static const TimingConfig TIMING = {
    .tick_delta     = 0.016f,
    .flicker_speed  = 12.0f,
    .flicker_base   = 0.8f,
    .flicker_amp    = 0.15f,
    .scanline_speed = 120.0f
};

typedef struct {
    const char* bg;
    const char* text;
    const char* scanline;
} Theme;

static const Theme THEME_DARK = {
    .bg       = "#2E3440",
    .text     = "#ECEFF4",
    .scanline = "rgba(236,239,244,0.035)"
};

static const Theme THEME_LIGHT = {
    .bg       = "#ECEFF4",
    .text     = "#2E3440",
    .scanline = "rgba(46,52,64,0.035)"
};

// SSoT: UI Styles managed by C
static const char* CSS_THEME_TOGGLE = 
    "position: fixed; top: 20px; right: 30px; "
    "font-size: 14px; user-select: none; cursor: pointer;";

static const char* CSS_FEED = 
    "padding: 20px; max-width: 900px; margin-top: 180px;";

static const int   HEADER_HEIGHT      = 180;

static const char* MSG_HEADER         = "BONATTO";
static const char* MSG_WHOAMI         = "whoami";

static const char* MSG_BIO =
"Work spans formal methods, functional programming, and operating systems. "
"Main interests include dependent type theory, proof languages, language "
"interpreters, virtual machines, and minimalist kernels.";

#endif
