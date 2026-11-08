// Copyright 2026 Nikolay Aviltsev
// SPDX-License-Identifier: GPL-2.0-or-later

#pragma once

// keyboard.json sets 5; the Caps Lock cursor layer (_CUR) is layer 5, so VIA's
// EEPROM keymap needs room for 6 or that layer would read back as KC_NO.
#undef DYNAMIC_KEYMAP_LAYER_COUNT
#define DYNAMIC_KEYMAP_LAYER_COUNT 6

// Key backlight defaults (keyboard.json: cycle_left_right, val 96): static red
// at half of max_brightness (160). Only applied when the EEPROM is reset.
#undef RGB_MATRIX_DEFAULT_MODE
#define RGB_MATRIX_DEFAULT_MODE RGB_MATRIX_SOLID_COLOR
#undef RGB_MATRIX_DEFAULT_HUE
#define RGB_MATRIX_DEFAULT_HUE 0
#undef RGB_MATRIX_DEFAULT_SAT
#define RGB_MATRIX_DEFAULT_SAT 255
#undef RGB_MATRIX_DEFAULT_VAL
#define RGB_MATRIX_DEFAULT_VAL 80

// Front side light brightness step after reset, 0..4 (fork default 4 =
// brightest). 1 = second-dimmest, the lowest where BT3 amber reads as amber.
#define HS_RL_DEFAULT_VAL 1
// Front side light effect after reset: 12 = off (fork default 0 = rainbow).
#define HS_RL_DEFAULT_MODE 12

// Fn+Space: no battery gauge on keys 1..0 -- the front strip shows it instead
// (keymap.c strip_battery()); the keys just go dark while it's held.
#define HS_BATQ_NO_KEY_GAUGE
