#pragma once

#include QMK_KEYBOARD_H

//
// IMPORTANT:
// Most likely it's specific to Kick75, but LED driver
// treats 1st field as G, 2nd as R, and 3rd as B
// when constructing color through RGB type/struct and then passing color
// components to eg. rgb_matrix_set_color().
// So rgb_matrix_set_color(led, color.r, color.g, color.b) - 
// it's where the R and G swapping happens on Kick75.
// If you pass values directly, eg. rgb_matrix_set_color(led, 255, 0, 0) - it's ok,
// the color will be red.
//

// ESC key shows a single color based on battery level, 3 colors:
// green (>=30%), orange (>=10%), red (<10%)
static inline void battery_indicator_simple(uint8_t level, uint8_t led_min, uint8_t led_max) {
    uint8_t esc_led = g_led_config.matrix_co[0][0];
    if (esc_led < led_min || esc_led >= led_max) return;

    RGB color;
    if (level >= 30) {
        color = (RGB){255, 0, 0};   // green (GRB)
    } else if (level >= 10) {
        color = (RGB){140, 255, 0}; // orange (GRB)
    } else {
        color = (RGB){0, 255, 0};   // red (GRB)
    }

    rgb_matrix_set_color(esc_led, color.r, color.g, color.b);
}

// F1-F12 keys as a progress bar:
// - all 12 lit at 100%, none at 0%
// - each key represents ~8.33% (100/12)
// - color is always green
static inline void battery_indicator_fkeys(uint8_t level, uint8_t led_min, uint8_t led_max) {
    uint8_t lit = (uint8_t)((uint16_t)level * 12 / 100);  // 0-12 keys to light

    for (uint8_t i = 0; i < 12; i++) {
        uint8_t led = g_led_config.matrix_co[0][i + 1];  // F1=col1 .. F12=col12
        if (led < led_min || led >= led_max) continue;

        if (i < lit) {
            rgb_matrix_set_color(led, 0, 255, 0);  // green
        } else {
            rgb_matrix_set_color(led, 0, 0, 0);    // off
        }
    }
}

// ESC shows a rainbow color based on battery level, 7 colors:
// red (0-10%), orange (10-20%), yellow (20-30%), green (30-40%), cyan (40-50%), blue (50-60%), purple (60-100%)
static inline void battery_indicator_rainbow(uint8_t level, uint8_t led_min, uint8_t led_max) {
    uint8_t esc_led = g_led_config.matrix_co[0][0];
    if (esc_led < led_min || esc_led >= led_max) return;

    RGB color;
    if (level >= 60) {
        color = (RGB){0, 128, 128};  // purple (GRB)
    } else if (level >= 50) {
        color = (RGB){0, 0, 255};    // blue (GRB)
    } else if (level >= 40) {
        color = (RGB){255, 0, 255};  // cyan (GRB)
    } else if (level >= 30) {
        color = (RGB){255, 0, 0};    // green (GRB)
    } else if (level >= 20) {
        color = (RGB){255, 255, 0};  // yellow (GRB)
    } else if (level >= 10) {
        color = (RGB){140, 255, 0};  // orange (GRB)
    } else {
        color = (RGB){0, 255, 0};    // red (GRB)
    }

    rgb_matrix_set_color(esc_led, color.r, color.g, color.b);
}
