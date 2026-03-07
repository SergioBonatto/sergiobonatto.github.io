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

// Style Configuration (SSoT)
static const char* COLOR_BG           = "#2E3440";
static const char* COLOR_TEXT_PRIMARY = "#ECEFF4";
static const char* COLOR_SCANLINE     = "rgba(236,239,244,0.035)";

static const int   HEADER_HEIGHT      = 180;

static const char* MSG_HEADER         = "BONATTO";
static const char* MSG_WHOAMI         = "whoami";

static const char* MSG_BIO =
"Work spans formal methods, functional programming, and operating systems. "
"Main interests include dependent type theory, proof languages, language "
"interpreters, virtual machines, and minimalist kernels.";

#endif
