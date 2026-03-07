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
    .tick_delta = 0.016f,
    .flicker_speed = 10.0f,
    .flicker_base = 0.7f,
    .flicker_amp = 0.3f,
    .scanline_speed = 100.0f
};

static const char* MSG_WHOAMI = "whoami";

static const char* MSG_BIO =
"Work spans formal methods, functional programming, and operating systems. "
"Main interests include dependent type theory, proof languages, language "
"interpreters, virtual machines, and minimalist kernels.";

#endif
