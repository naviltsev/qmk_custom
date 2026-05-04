RGB_MATRIX_EFFECT(deus_ex)

#ifdef RGB_MATRIX_CUSTOM_EFFECT_IMPLS

static bool deus_ex(effect_params_t* params) {
    RGB_MATRIX_USE_LIMITS(led_min, led_max);

    for (uint8_t i = led_min; i < led_max; i++) {
        rgb_matrix_set_color(i, 0x00, 0x00, 0x00);
    }

    // WASD — light blue
    rgb_matrix_set_color(31, 0x00, 0xC8, 0xFF);  // W
    rgb_matrix_set_color(45, 0x00, 0xC8, 0xFF);  // A
    rgb_matrix_set_color(46, 0x00, 0xC8, 0xFF);  // S
    rgb_matrix_set_color(47, 0x00, 0xC8, 0xFF);  // D

    // F1-F2 — purple
    rgb_matrix_set_color(1,  0x99, 0x00, 0xFF);  // F1
    rgb_matrix_set_color(2,  0x99, 0x00, 0xFF);  // F2

    // F3-F12 — green
    rgb_matrix_set_color(3,  0x00, 0xFF, 0x40);  // F3
    rgb_matrix_set_color(4,  0x00, 0xFF, 0x40);  // F4
    rgb_matrix_set_color(5,  0x00, 0xFF, 0x40);  // F5
    rgb_matrix_set_color(6,  0x00, 0xFF, 0x40);  // F6
    rgb_matrix_set_color(7,  0x00, 0xFF, 0x40);  // F7
    rgb_matrix_set_color(8,  0x00, 0xFF, 0x40);  // F8
    rgb_matrix_set_color(9,  0x00, 0xFF, 0x40);  // F9
    rgb_matrix_set_color(10, 0x00, 0xFF, 0x40);  // F10
    rgb_matrix_set_color(11, 0x00, 0xFF, 0x40);  // F11
    rgb_matrix_set_color(12, 0x00, 0xFF, 0x40);  // F12

    // 1-9, 0 — orange
    rgb_matrix_set_color(15, 0xFF, 0x78, 0x00);  // 1
    rgb_matrix_set_color(16, 0xFF, 0x78, 0x00);  // 2
    rgb_matrix_set_color(17, 0xFF, 0x78, 0x00);  // 3
    rgb_matrix_set_color(18, 0xFF, 0x78, 0x00);  // 4
    rgb_matrix_set_color(19, 0xFF, 0x78, 0x00);  // 5
    rgb_matrix_set_color(20, 0xFF, 0x78, 0x00);  // 6
    rgb_matrix_set_color(21, 0xFF, 0x78, 0x00);  // 7
    rgb_matrix_set_color(22, 0xFF, 0x78, 0x00);  // 8
    rgb_matrix_set_color(23, 0xFF, 0x78, 0x00);  // 9
    rgb_matrix_set_color(24, 0xFF, 0x78, 0x00);  // 0

    // Y — yellow
    rgb_matrix_set_color(35, 0xFF, 0xE0, 0x00);  // Y

    // B — red
    rgb_matrix_set_color(63, 0xFF, 0x00, 0x00);  // B

    return rgb_matrix_check_finished_leds(led_max);
}

#endif // RGB_MATRIX_CUSTOM_EFFECT_IMPLS
