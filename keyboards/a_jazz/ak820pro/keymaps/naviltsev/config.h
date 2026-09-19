#pragma once

// VIA defaults this to 4 (set by generated info_config.h); we added a 5th
// layer (CURSOR) and a 6th (MENU, for the on-device settings menu -- its own
// persistent layer so the encoder resolves to menu-aware keycodes regardless
// of whether Fn is still held, since ENCODER_MAP_ENABLE resolves turns from
// encoder_map[current_layer], not encoder_update_user()). This keymap
// config.h is included after the generated config, so undef before
// redefining.
#ifdef DYNAMIC_KEYMAP_LAYER_COUNT
#    undef DYNAMIC_KEYMAP_LAYER_COUNT
#endif
#define DYNAMIC_KEYMAP_LAYER_COUNT 6

// The 5th layer no longer fit in the default 1KB emulated EEPROM (4 layers
// at MATRIX_ROWS*MATRIX_COLS*2 = 180 bytes each barely fit already). Firmware
// only uses ~75KB of the 256KB flash budget, so double the wear-leveling
// backing store (-> 2KB logical) for headroom. Plenty of room left for the
// 6th layer too (~184 bytes each).
#define WEAR_LEVELING_BACKING_SIZE 4096
