#pragma once

#include QMK_KEYBOARD_H
#include "battery_indicator.h"  // for set_color_scaled

// Number row (1-0) as a progress bar fed by raw HID from the host, e.g. GPU
// memory usage. Same 12-segment-style scaling as battery_indicator_fkeys,
// but over 10 keys (1-9, 0) instead of 12 (F1-F12).
// - all 10 lit at 100%, none at 0%
// - color is green below 70%, amber 70-89%, red 90%+ (it's a "getting full" warning, not a battery)
static inline void gpu_indicator_numrow(uint8_t percent, uint8_t led_min, uint8_t led_max) {
    if (percent > 100) percent = 100;
    uint8_t lit = (uint8_t)((uint16_t)percent * 10 / 100);  // 0-10 keys to light

    RGB color;
    if (percent >= 90) {
        color = (RGB){0, 255, 0};   // red (GRB)
    } else if (percent >= 70) {
        color = (RGB){140, 255, 0}; // amber (GRB)
    } else {
        color = (RGB){255, 0, 0};   // green (GRB)
    }

    for (uint8_t i = 0; i < 10; i++) {
        uint8_t led = g_led_config.matrix_co[1][i + 1];  // 1=col1 .. 0=col10
        if (led < led_min || led >= led_max) continue;

        if (i < lit) {
            set_color_scaled(led, color.r, color.g, color.b);
        } else {
            set_color_scaled(led, 0, 0, 0);    // off
        }
    }
}
