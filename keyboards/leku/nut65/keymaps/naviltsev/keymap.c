// Copyright 2024 sdk66 (@sdk66)
// Copyright 2026 Nikolay Aviltsev
// SPDX-License-Identifier: GPL-2.0-or-later
//
// Starting point copied from the keyboard's own default keymap
// (keyboards/leku/nut65/keymaps/default/keymap.c in the weikav fork), minus
// the encoder map -- this board has no knob. The key layout lives in EEPROM
// once VIA has touched it; keymaps[] below only seeds it after an EEPROM reset.

#include QMK_KEYBOARD_H
#include "rgb_record/rgb_record.h"
#include "wireless.h"
#include "usb_main.h"
#include "rgb_matrix_blink.h"

enum layers {
    _BL = 0,
    _FL,
    _MBL,
    _MFL,
    _DEFA,
    _CUR, // held on Caps Lock: WASD arrows, Esc = `, 1..= = F1..F12
};

// clang-format off
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [_BL] = LAYOUT( /* win Base */
        KC_ESC,   KC_1,       KC_2,       KC_3,     KC_4,     KC_5,     KC_6,     KC_7,     KC_8,     KC_9,     KC_0,       KC_MINS,  KC_EQL,   KC_BSPC,   KC_INS,
        KC_TAB,   KC_Q,       KC_W,       KC_E,     KC_R,     KC_T,     KC_Y,     KC_U,     KC_I,     KC_O,     KC_P,       KC_LBRC,  KC_RBRC,  KC_BSLS,   KC_DEL,
        MO(_CUR), KC_A,       KC_S,       KC_D,     KC_F,     KC_G,     KC_H,     KC_J,     KC_K,     KC_L,     KC_SCLN,    KC_QUOT,            KC_ENT,    KC_PGUP,
        KC_LSFT,              KC_Z,       KC_X,     KC_C,     KC_V,     KC_B,     KC_N,     KC_M,     KC_COMM,  KC_DOT,     KC_SLSH,  KC_RSFT,  KC_UP,     KC_PGDN,
        KC_LCTL,  KC_LCMD,    KC_LALT,                        KC_SPC,                                           KC_RALT,    MO(_FL),  KC_LEFT,  KC_DOWN,   KC_RGHT,
        KC_NO,    KC_NO,      KC_NO,      KC_NO,    KC_NO,    KC_NO,    KC_NO,    KC_NO,    KC_NO,    KC_NO,    KC_NO,      KC_NO,    KC_NO,    KC_NO,     KC_NO
        ),

    [_FL] = LAYOUT( /* win FN */
        KC_GRV,   KC_F1,      KC_F2,      KC_F3,    KC_F4,    KC_F5,    KC_F6,    KC_F7,    KC_F8,    KC_F9,    KC_F10,     KC_F11,   KC_F12,   RL_MOD,    MUS_STA,
        KC_GRV,   KC_BT1,     KC_BT2,     KC_BT3,   KC_2G4,   KC_USB,   _______,  _______,  _______,  _______,  _______,    _______,  _______,  RGB_MOD,   MUS_CUT,
        _______,  _______,    _______,    DEB_TOG,  _______,  _______,  KC_SCRL,  KC_PAUS,  KC_HOME,  KC_END,   _______,    _______,            RGB_HUI,   RL_VAI,
        _______,              _______,    _______,  _______,  _______,  _______,  NK_TOGG,  SYS_CUT,  _______,  _______,    _______,  MO(_DEFA),RGB_VAI,   RL_VAD,
        LED_TOG,  GU_TOGG,    HS_FREQ,                        HS_BATQ,                                          KC_FTOG,    _______,  RGB_SPD,  RGB_VAD,   RGB_SPI,
        KC_NO,    KC_NO,      KC_NO,      KC_NO,    KC_NO,    KC_NO,    KC_NO,    KC_NO,    KC_NO,    KC_NO,    KC_NO,      KC_NO,    KC_NO,    KC_NO,     KC_NO
        ),

    [_MBL] = LAYOUT(  /* mac Base */
        KC_ESC,   KC_1,       KC_2,       KC_3,     KC_4,     KC_5,     KC_6,     KC_7,     KC_8,     KC_9,     KC_0,       KC_MINS,  KC_EQL,   KC_BSPC,   KC_INS,
        KC_TAB,   KC_Q,       KC_W,       KC_E,     KC_R,     KC_T,     KC_Y,     KC_U,     KC_I,     KC_O,     KC_P,       KC_LBRC,  KC_RBRC,  KC_BSLS,   KC_DEL,
        MO(_CUR), KC_A,       KC_S,       KC_D,     KC_F,     KC_G,     KC_H,     KC_J,     KC_K,     KC_L,     KC_SCLN,    KC_QUOT,            KC_ENT,    KC_PGUP,
        KC_LSFT,              KC_Z,       KC_X,     KC_C,     KC_V,     KC_B,     KC_N,     KC_M,     KC_COMM,  KC_DOT,     KC_SLSH,  KC_RSFT,  KC_UP,     KC_PGDN,
        KC_LCTL,  KC_LALT,    KC_LCMD,                        KC_SPC,                                           KC_RCMD,    MO(_MFL), KC_LEFT,  KC_DOWN,   KC_RGHT,
        KC_NO,    KC_NO,      KC_NO,      KC_NO,    KC_NO,    KC_NO,    KC_NO,    KC_NO,    KC_NO,    KC_NO,    KC_NO,      KC_NO,    KC_NO,    KC_NO,     KC_NO
        ),

    [_MFL] = LAYOUT(  /*mac FN */
        KC_GRV,   KC_F1,      KC_F2,      KC_F3,    KC_F4,    KC_F5,    KC_F6,    KC_F7,    KC_F8,    KC_F9,    KC_F10,     KC_F11,   KC_F12,   RL_MOD,    MUS_STA,
        KC_GRV,   KC_BT1,     KC_BT2,     KC_BT3,   KC_2G4,   KC_USB,   _______,  _______,  _______,  _______,  _______,    _______,  _______,  RGB_MOD,   MUS_CUT,
        _______,  _______,    _______,    DEB_TOG,  _______,  _______,  KC_SCRL,  KC_PAUS,  KC_HOME,  KC_END,   _______,    _______,            RGB_HUI,   RL_VAI,
        _______,              _______,    _______,  _______,  _______,  _______,  NK_TOGG,  SYS_CUT,  _______,  _______,    _______,  MO(_DEFA),RGB_VAI,   RL_VAD,
        LED_TOG,  _______,    HS_FREQ,                        HS_BATQ,                                          KC_FTOG,    _______,  RGB_SPD,  RGB_VAD,   RGB_SPI,
        KC_NO,    KC_NO,      KC_NO,      KC_NO,    KC_NO,    KC_NO,    KC_NO,    KC_NO,    KC_NO,    KC_NO,    KC_NO,      KC_NO,    KC_NO,    KC_NO,     KC_NO
        ),

    [_DEFA] = LAYOUT(  /* FN */
        QK_BOOT,  _______,    _______,    _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,    _______,  _______,  _______,   _______,
        _______,  _______,    BT_TEST,    _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,    _______,  _______,  _______,   _______,
        _______,  _______,    _______,    _______,  _______,  _______,  _______,  _______,  _______,  KC_TEST,  _______,    _______,            _______,   _______,
        _______,              _______,    _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,    _______,  _______,  _______,   _______,
        _______,  _______,    _______,                        _______,                                          _______,    _______,  _______,  _______,   _______,
        KC_NO,    KC_NO,      KC_NO,      KC_NO,    KC_NO,    KC_NO,    KC_NO,    KC_NO,    KC_NO,    KC_NO,    KC_NO,      KC_NO,    KC_NO,    KC_NO,     KC_NO
        ),

    [_CUR] = LAYOUT(  /* Caps Lock held, from either base layer; F-row same as Fn */
        KC_GRV,   KC_F1,      KC_F2,      KC_F3,    KC_F4,    KC_F5,    KC_F6,    KC_F7,    KC_F8,    KC_F9,    KC_F10,     KC_F11,   KC_F12,   _______,   _______,
        _______,  _______,    KC_UP,      _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,    _______,  _______,  _______,   _______,
        _______,  KC_LEFT,    KC_DOWN,    KC_RGHT,  _______,  _______,  _______,  _______,  _______,  _______,  _______,    _______,            _______,   _______,
        _______,              _______,    _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,    _______,  _______,  _______,   _______,
        _______,  _______,    _______,                        _______,                                          _______,    _______,  _______,  _______,   _______,
        KC_NO,    KC_NO,      KC_NO,      KC_NO,    KC_NO,    KC_NO,    KC_NO,    KC_NO,    KC_NO,    KC_NO,    KC_NO,      KC_NO,    KC_NO,    KC_NO,     KC_NO
        ),
};

// Referenced by the board's rgb_record code, so it has to exist in every keymap.
const uint16_t PROGMEM rgbrec_default_effects[RGBREC_CHANNEL_NUM][MATRIX_ROWS][MATRIX_COLS] = {
    [0] = LAYOUT(
        HS_GREEN, ________,   ________,   ________, ________, ________, ________, ________, ________, ________, ________,   ________, ________, ________,  ________,
        ________, ________,   HS_GREEN,   ________, ________, ________, ________, ________, ________, ________, ________,   ________, ________, ________,  ________,
        ________, HS_GREEN,   HS_GREEN,   HS_GREEN, ________, ________, ________, ________, ________, ________, ________,   ________,           ________,  ________,
        ________,             ________,   ________, ________, ________, ________, ________, ________, ________, ________,   ________, ________, HS_GREEN,  ________,
        ________, ________,   ________,                       ________,                                         ________,   ________, HS_GREEN, HS_GREEN,  HS_GREEN,
        ________, ________,   ________,   ________, ________, ________, ________, ________, ________, ________, ________,   ________, ________, ________,  ________
        ),

    [1] = LAYOUT(
        HS_GREEN, ________,   ________,   ________, ________, ________, ________, ________, ________, ________, ________,   ________, ________, ________,  ________,
        ________, ________,   HS_GREEN,   ________, ________, ________, ________, ________, ________, ________, ________,   ________, ________, ________,  ________,
        ________, HS_GREEN,   HS_GREEN,   HS_GREEN, ________, ________, ________, ________, ________, ________, ________,   ________,           ________,  ________,
        ________,             ________,   ________, ________, ________, ________, ________, ________, ________, ________,   ________, ________, HS_GREEN,  ________,
        ________, ________,   ________,                       ________,                                         ________,   ________, HS_GREEN, HS_GREEN,  HS_GREEN,
        ________, ________,   ________,   ________, ________, ________, ________, ________, ________, ________, ________,   ________, ________, ________,  ________
        ),

    [2] = LAYOUT(
        HS_GREEN, ________,   ________,   ________, ________, ________, ________, ________, ________, ________, ________,   ________, ________, ________,  ________,
        ________, ________,   HS_GREEN,   ________, ________, ________, ________, ________, ________, ________, ________,   ________, ________, ________,  ________,
        ________, HS_GREEN,   HS_GREEN,   HS_GREEN, ________, ________, ________, ________, ________, ________, ________,   ________,           ________,  ________,
        ________,             ________,   ________, ________, ________, ________, ________, ________, ________, ________,   ________, ________, HS_GREEN,  ________,
        ________, ________,   ________,                       ________,                                         ________,   ________, HS_GREEN, HS_GREEN,  HS_GREEN,
        ________, ________,   ________,   ________, ________, ________, ________, ________, ________, ________, ________,   ________, ________, ________,  ________
        ),
};
// clang-format on

// ---------------------------------------------------------------------------
// Front LED strip
//
// LEDs 71..150 are an 80-LED strip along the front edge, left to right. The
// board repaints it every frame (side-light effect, mirroring); the fork calls
// rgb_matrix_indicators_advanced_user() at the very end of
// rgb_matrix_indicators_advanced_kb(), so anything painted here wins.
//
// Priority, highest first:
//   1. Battery gauge while Fn+Space (HS_BATQ) is held -- wireless only, same
//      as the board's own number-row gauge.
//   2. Win/Mac blink, in sync with the M key's blink after Fn+M.
//   3. Small constant Win/Mac marker at both ends of the strip, plus the
//      current connection (BT1/2/3, 2.4G, USB) in the middle; the rest keeps
//      the board's side-light effect.
// ---------------------------------------------------------------------------

#define STRIP_FIRST (RGB_MATRIX_LED_COUNT - RGBLED_NUM)
#define STRIP_COUNT RGBLED_NUM
#define STRIP_MARKER_LEDS 3 // per end

extern bool rk_bat_req_flag; // rgb_record.c: true while HS_BATQ is held off USB

static void strip_fill(uint8_t from, uint8_t count, uint8_t r, uint8_t g, uint8_t b) {
    for (uint8_t i = from; i < from + count && i < STRIP_COUNT; i++) {
        rgb_matrix_set_color(STRIP_FIRST + i, r, g, b);
    }
}

extern uint8_t rl_rgb_brightness_val; // nut65.c: side light brightness, 8..55 for Fn+PgDn/PgUp steps 0..4

// Scales a full-brightness colour to the front strip's own brightness
// (Fn+PgUp/PgDn); every strip indicator uses it.
static RGB strip_dim(uint8_t r, uint8_t g, uint8_t b) {
    uint8_t val = rl_rgb_brightness_val;
    return (RGB){.r = (uint16_t)r * val / 255, .g = (uint16_t)g * val / 255, .b = (uint16_t)b * val / 255};
}

static void strip_battery(void) {
    uint8_t bat = *md_getp_bat();
    if (bat > 100) bat = 100;

    uint8_t lit = (uint16_t)bat * STRIP_COUNT / 100;
    if (lit < 1) lit = 1; // like the number-row gauge: always show something

    RGB rgb;
    if (bat >= 50) {
        rgb = strip_dim(0x00, 0xFF, 0x00);
    } else if (bat >= 25) {
        rgb = strip_dim(0xFF, 0x55, 0x00); // orange
    } else {
        rgb = strip_dim(0xFF, 0x00, 0x00);
    }

    strip_fill(0, lit, rgb.r, rgb.g, rgb.b);
    strip_fill(lit, STRIP_COUNT - lit, 0x00, 0x00, 0x00);
}

// Mirrors the board's Fn+M blink on the M key (HS_RGB_BLINK_INDEX_SYS_TEM):
// same timing and colour (blue -> Win, white -> Mac), dimmed to the strip
// brightness. Returns false when that blink isn't running.
static bool strip_os_blink(void) {
    for (uint8_t i = 0; i < RGB_MATRIX_BLINK_COUNT; i++) {
        blink_rgb_t *blink = &blink_rgbs[i];
        if (blink->index != HS_RGB_BLINK_INDEX_SYS_TEM) continue;
        if (blink->remain_time == 0) return false;

        if (blink->flip) {
            RGB rgb = strip_dim(blink->color.r, blink->color.g, blink->color.b);
            strip_fill(0, STRIP_COUNT, rgb.r, rgb.g, rgb.b);
        } else {
            strip_fill(0, STRIP_COUNT, 0x00, 0x00, 0x00);
        }
        return true;
    }
    return false;
}

static void strip_os_marker(void) {
    bool mac = get_highest_layer(default_layer_state) == _MBL;
    HSV  hsv = mac ? (HSV){HSV_WHITE} : (HSV){HSV_BLUE};
    hsv.v    = rl_rgb_brightness_val; // follow the strip brightness (Fn+PgUp/PgDn)
    RGB rgb  = hsv_to_rgb(hsv);

    strip_fill(0, STRIP_MARKER_LEDS, rgb.r, rgb.g, rgb.b);
    strip_fill(STRIP_COUNT - STRIP_MARKER_LEDS, STRIP_MARKER_LEDS, rgb.r, rgb.g, rgb.b);
}

// Current connection in the middle of the strip: 5 slots laid out like the
// Fn+Q/W/E/R/T keys (BT1, BT2, BT3, 2.4G, USB), only the active one lit, in the
// board's own channel colours (HS_LBACK_COLOR_*: BT blue, 2.4G green, USB
// white). Blinks while not connected, steady once connected.
#define CHANNEL_SLOTS 5
#define CHANNEL_SLOT_LEDS 4
#define CHANNEL_SLOT_GAP 2
#define CHANNEL_ZONE_LEDS (CHANNEL_SLOTS * CHANNEL_SLOT_LEDS + (CHANNEL_SLOTS - 1) * CHANNEL_SLOT_GAP)
#define CHANNEL_ZONE_FIRST ((STRIP_COUNT - CHANNEL_ZONE_LEDS) / 2)
#define CHANNEL_BLINK_MS 500

static void strip_channel(void) {
    uint8_t devs = wireless_get_current_devs();
    uint8_t slot;
    HSV     hsv;
    bool    connected;

    switch (devs) {
        case DEVS_BT1: slot = 0, hsv = (HSV){HSV_BLUE};  break;
        case DEVS_BT2: slot = 1, hsv = (HSV){HSV_BLUE};  break;
        case DEVS_BT3: slot = 2, hsv = (HSV){HSV_BLUE};  break;
        case DEVS_2G4: slot = 3, hsv = (HSV){HSV_GREEN}; break;
        case DEVS_USB: slot = 4, hsv = (HSV){HSV_WHITE}; break;
        default: return;
    }

    if (devs == DEVS_USB) {
        connected = USB_DRIVER.state == USB_ACTIVE;
    } else {
        connected = *md_getp_state() == MD_STATE_CONNECTED;
    }

    strip_fill(CHANNEL_ZONE_FIRST, CHANNEL_ZONE_LEDS, 0x00, 0x00, 0x00);
    if (!connected && (timer_read32() / CHANNEL_BLINK_MS) % 2) return;

    hsv.v   = rl_rgb_brightness_val; // follow the strip brightness (Fn+PgUp/PgDn)
    RGB rgb = hsv_to_rgb(hsv);
    strip_fill(CHANNEL_ZONE_FIRST + slot * (CHANNEL_SLOT_LEDS + CHANNEL_SLOT_GAP), CHANNEL_SLOT_LEDS, rgb.r, rgb.g, rgb.b);
}

bool rgb_matrix_indicators_advanced_user(uint8_t led_min, uint8_t led_max) {
    if (rk_bat_req_flag) {
        strip_battery();
    } else if (!strip_os_blink()) {
        strip_os_marker();
        strip_channel();
    }
    return false;
}
