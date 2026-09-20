// Copyright 2026 Fernando Birra <fernando.birra@gmail.com>
// Copyright 2026 Nikolay Aviltsev
// SPDX-License-Identifier: GPL-2.0-or-later
//
// Starting point copied from the keyboard's own `via` keymap
// (keyboards/a_jazz/ak820pro/keymaps/via/keymap.c on ak820pro-lcd-flash) so VIA
// and the raw-HID channel used for LCD flash provisioning keep working.

#include QMK_KEYBOARD_H

#include <stdio.h>

#include "ak820pro.h"
#include "graphics/lcd_bus.h"
#include "graphics/display.h"
#include "rgb_matrix.h"

enum layer_names {
    WINBASE,
    WINFN,
    MACBASE,
    MACFN,
    CURSOR,
    // 5 is _AGENT (agent_layer.h) -- left unnamed here since that header
    // owns the #define, same as it does on the Kick75 keymap it's ported
    // from; MENU is bumped to 6 to leave the slot free.
    MENU = 6,  // must stay last/highest: layer_on(MENU) needs top priority
               // over WINFN/MACFN so the encoder resolves to menu keycodes
               // regardless of whether Fn is still held
};

enum custom_keycodes {
    MENU_CW = SAFE_RANGE,  // MENU layer's encoder_map only: knob turned clockwise
    MENU_CCW,              // MENU layer's encoder_map only: knob turned counter-clockwise
};

#include "agent_layer.h"  // Pi Agent layer macros (WINFN + "/" held); uses MENU_CCW above

#define KC_TASK LGUI(KC_TAB)        // Task viewer
#define KC_FLXP LGUI(KC_E)          // Windows file explorer
#define KC_MCTL KC_MISSION_CONTROL  // Mission Control
#define KC_LPAD KC_LAUNCHPAD        // Launchpad

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [WINBASE] = LAYOUT_82_ansi(
        KC_ESC,     KC_F1,      KC_F2,      KC_F3,      KC_F4,      KC_F5,      KC_F6,      KC_F7,      KC_F8,      KC_F9,      KC_F10,     KC_F11,      KC_F12,     KC_PGUP,    KC_MUTE,
        KC_GRV,     KC_1,       KC_2,       KC_3,       KC_4,       KC_5,       KC_6,       KC_7,       KC_8,       KC_9,       KC_0,       KC_MINS,     KC_EQL,     KC_BSPC,    KC_PGDN,
        KC_TAB,     KC_Q,       KC_W,       KC_E,       KC_R,       KC_T,       KC_Y,       KC_U,       KC_I,       KC_O,       KC_P,       KC_LBRC,     KC_RBRC,    KC_BSLS,    KC_HOME,
        MO(CURSOR), KC_A,       KC_S,       KC_D,       KC_F,       KC_G,       KC_H,       KC_J,       KC_K,       KC_L,       KC_SCLN,    KC_QUOT,                 KC_ENT,     KC_DEL,
        KC_LSFT,                KC_Z,       KC_X,       KC_C,       KC_V,       KC_B,       KC_N,       KC_M,       KC_COMM,    KC_DOT,     KC_SLSH,     KC_RSFT,    KC_UP,
        KC_LCTL,    KC_LGUI,    KC_LALT,                                        KC_SPC,                             KC_RALT,    MO(WINFN),  KC_RCTL,     KC_LEFT,    KC_DOWN,    KC_RGHT
    ),
    [WINFN] = LAYOUT_82_ansi(
        QK_BOOT,    KC_BRID,    KC_BRIU,    KC_TASK,    KC_FLXP,    _______,    _______,    KC_MPRV,    KC_MPLY,    KC_MNXT,    KC_MUTE,    KC_VOLD,     KC_VOLU,    ANIM_TOG,   _______,
        _______,    _______,    _______,    _______,    _______,    _______,    RM_SATU,    RM_SATD,    _______,    _______,    _______,    RM_SPDD,     RM_SPDU,    _______,    SCR_TOG,
        _______,    BT1,        BT2,        BT3,        BT24G,      _______,    _______,    _______,    _______,    _______,    BT_PAIR,    _______,     _______,    RM_NEXT,    _______,
        _______,    _______,    _______,    _______,    _______,    _______,    _______,    _______,    _______,    _______,    _______,    _______,                 _______,    _______,
        _______,                _______,    RM_TOGG,    _______,    _______,    _______,    _______,    _______,    _______,    _______,    MO(_AGENT), _______,    RM_VALU,
        _______,    GU_TOGG,    _______,                                        _______,                            _______,    _______,    _______,     RM_HUED,    RM_VALD,    RM_HUEU
    ),
    [MACBASE] = LAYOUT_82_ansi(
        KC_ESC,     KC_BRID,    KC_BRIU,    KC_MCTL,    KC_F4,      KC_F5,      KC_F6,      KC_MPRV,    KC_MPLY,    KC_MNXT,    KC_MUTE,    KC_VOLD,     KC_VOLU,    KC_PGUP,    KC_MUTE,
        KC_GRV,     KC_1,       KC_2,       KC_3,       KC_4,       KC_5,       KC_6,       KC_7,       KC_8,       KC_9,       KC_0,       KC_MINS,     KC_EQL,     KC_BSPC,    KC_PGDN,
        KC_TAB,     KC_Q,       KC_W,       KC_E,       KC_R,       KC_T,       KC_Y,       KC_U,       KC_I,       KC_O,       KC_P,       KC_LBRC,     KC_RBRC,    KC_BSLS,    KC_HOME,
        MO(CURSOR), KC_A,       KC_S,       KC_D,       KC_F,       KC_G,       KC_H,       KC_J,       KC_K,       KC_L,       KC_SCLN,    KC_QUOT,                 KC_ENT,     KC_DEL,
        KC_LSFT,                KC_Z,       KC_X,       KC_C,       KC_V,       KC_B,       KC_N,       KC_M,       KC_COMM,    KC_DOT,     KC_SLSH,     KC_RSFT,    KC_UP,
        KC_LCTL,    KC_LALT,    KC_LGUI,                                        KC_SPC,                             KC_RGUI,    MO(MACFN),  KC_RCTL,     KC_LEFT,    KC_DOWN,    KC_RGHT
    ),
    [MACFN] = LAYOUT_82_ansi(
        QK_BOOT,    KC_F1,      KC_F2,      KC_F3,      KC_F4,      KC_F5,      KC_F6,      KC_F7,      KC_F8,      KC_F9,      KC_F10,     KC_F11,     KC_F12,      ANIM_TOG,   _______,
        _______,    _______,    _______,    _______,    _______,    _______,    RM_SATU,    RM_SATD,    _______,    _______,    _______,    RM_SPDD,     RM_SPDU,    _______,    SCR_TOG,
        _______,    BT1,        BT2,        BT3,        BT24G,      _______,    _______,    _______,    _______,    _______,    BT_PAIR,    _______,     _______,    RM_NEXT,    _______,
        _______,    _______,    _______,    _______,    _______,    _______,    _______,    _______,    _______,    _______,    _______,    _______,                 _______,    _______,
        _______,                _______,    RM_TOGG,    _______,    _______,    _______,    _______,    _______,    _______,    _______,    _______,     _______,    RM_VALU,
        _______,    _______,    _______,                                        _______,                            _______,    _______,    _______,     RM_HUED,    RM_VALD,    RM_HUEU
    ),
    // Held from Caps Lock (both Win and Mac base layers): WASD becomes arrows,
    // everything else falls through to whichever base layer is active.
    [CURSOR] = LAYOUT_82_ansi(
        _______,    _______,    _______,    _______,    _______,    _______,    _______,    _______,    _______,    _______,    _______,    _______,     _______,    _______,    _______,
        _______,    _______,    _______,    _______,    _______,    _______,    _______,    _______,    _______,    _______,    _______,    _______,     _______,    _______,    _______,
        _______,    _______,    KC_UP,      _______,    _______,    _______,    _______,    _______,    _______,    _______,    _______,    _______,     _______,    _______,    _______,
        _______,    KC_LEFT,    KC_DOWN,    KC_RGHT,    _______,    _______,    _______,    _______,    _______,    _______,    _______,    _______,                 _______,    _______,
        _______,                _______,    _______,    _______,    _______,    _______,    _______,    _______,    _______,    _______,    _______,     _______,    _______,
        _______,    _______,    _______,                                        _______,                            _______,    _______,    _______,     _______,    _______,    _______
    ),
    // Pi Agent layer - held via WINFN + "/" (MO(_AGENT) lives in WINFN).
    // Rows are color-coded while held (agent_layer_leds in
    // rgb_matrix_indicators_advanced_user below); keys with no binding stay dark.
    [_AGENT] = LAYOUT_82_ansi(
        // skills - cyan
        _______,        AG_BRAINSTORM,  AG_WRITE_PLAN,  AG_EXEC_PLAN,   AG_DEBUG_SYS,   AG_TDD,         AG_REQ_REVIEW,  AG_RECV_REVIEW, AG_VERIFY,      AG_WORKTREES,   AG_PARALLEL,    AG_COUNCIL,     AG_PONYTAIL,    _______,        _______,
        // session ops - green
        _______,        AG_NEW,         AG_RESUME,      AG_FORK,        AG_TREE,        AG_COMPACT,     AG_NAME,        AG_SHARE,       AG_EXPORT,      AG_COPY,        AG_RELOAD,      AG_SETTINGS,    AG_SCOPED,      _______,        _______,
        // prompt templates - magenta. Add ~/.pi/agent/prompts/<name>.md, then bind
        // a key here to send the command via a new AG_* keycode in agent_layer.h.
        XXXXXXX,        XXXXXXX,        XXXXXXX,        XXXXXXX,        XXXXXXX,        XXXXXXX,        XXXXXXX,        XXXXXXX,        XXXXXXX,        XXXXXXX,        XXXXXXX,        XXXXXXX,        XXXXXXX,        XXXXXXX,        XXXXXXX,
        // mode chords - amber. Native pi keybindings, Windows defaults.
        XXXXXXX,        LCTL(LALT(KC_P)), LSFT(KC_TAB), LCTL(KC_L),     LCTL(KC_P),     LALT(KC_P),     LCTL(KC_T),     LCTL(KC_O),     LCTL(KC_Q),     LALT(KC_Q),     LCTL(KC_X),     LCTL(KC_G),                     XXXXXXX,        XXXXXXX,
        // transcript nav - blue
        _______,                        LCTL(KC_UP),    LCTL(KC_DOWN),  LCTL(KC_F),     LALT(KC_V),     LCTL(KC_Z),     AG_THINKING,    AG_MODEL,       AG_LLAMA,       AG_DBG,         _______,        XXXXXXX,        _______,
        _______,        _______,        _______,                                        _______,                            _______,        MO(WINFN),      _______,        RM_NEXT,        RM_VALD,    RM_HUEU
    ),
    // Turned on/off programmatically (layer_on/layer_off in menu_enter/exit),
    // not held -- exists only so the encoder resolves to MENU_CW/MENU_CCW
    // regardless of whether Fn is still down. Fully transparent otherwise:
    // KC_MUTE itself lives on WINBASE/MACBASE (matrix [0,14]), reached by
    // falling through since neither this layer nor WINFN/MACFN override that
    // cell -- so it stays reachable for the hold-to-open/select-back logic
    // in process_record_user even with Fn held.
    [MENU] = LAYOUT_82_ansi(
        _______,    _______,    _______,    _______,    _______,    _______,    _______,    _______,    _______,    _______,    _______,    _______,     _______,    _______,    _______,
        _______,    _______,    _______,    _______,    _______,    _______,    _______,    _______,    _______,    _______,    _______,    _______,     _______,    _______,    _______,
        _______,    _______,    _______,    _______,    _______,    _______,    _______,    _______,    _______,    _______,    _______,    _______,     _______,    _______,    _______,
        _______,    _______,    _______,    _______,    _______,    _______,    _______,    _______,    _______,    _______,    _______,    _______,                 _______,    _______,
        _______,                _______,    _______,    _______,    _______,    _______,    _______,    _______,    _______,    _______,    _______,     _______,    _______,
        _______,    _______,    _______,                                        _______,                            _______,    _______,    _______,     _______,    _______,    _______
    )
};


// --- On-device settings menu (knob) -------------------------------------------
//
// Turning the knob always drives the menu: if it's closed, the first turn
// opens it and that same turn also moves the cursor (MENU_CW/MENU_CCW in
// process_record_user); once open, turning navigates the list or live-adjusts
// the selected item's value -- volume included, so there's no separate
// always-on volume-by-rotation duplicating it. The knob's plain press is
// overloaded by hold duration: holding it past MUTE_HOLD_MS toggles the whole
// menu open/closed from any state (see mute_held/mute_hold_fired in
// process_record_user and the matrix_scan_user poll below), while a quick tap
// does whichever's contextually useful -- mute when the menu is closed,
// select/back when it's open (moving between the item list and adjusting the
// selected item). menu_exit() (the hold-close path) calls
// display_redraw_dashboard() to hand the panel back to the stock dashboard;
// display_housekeeping_task_user() suppresses that dashboard's own per-tick
// redraw while the menu owns the panel.

enum menu_state {
    MENU_IDLE = 0,
    MENU_LIST,
    MENU_ADJUST,
};
static uint8_t menu_state = MENU_IDLE;

enum menu_item {
    MENU_ITEM_VOLUME = 0,
    MENU_ITEM_EFFECT,
    MENU_ITEM_BRIGHTNESS,
    MENU_ITEM_HUE,
    MENU_ITEM_COUNT
};
static uint8_t menu_sel = 0;

static const char *const menu_item_names[MENU_ITEM_COUNT] = {
    "Volume", "Effect", "Brightness", "Hue",
};

// Host volume is write-only from the keyboard's side (consumer VOLU/VOLD),
// so there's no real level to show -- just which way the knob last turned.
// 0 = neutral (just entered), +1 = last turn was CW, -1 = last turn was CCW.
static int8_t menu_volume_dir = 0;

#define MENU_FONT_BIG   ASSET_IOSEVKA_REGULAR_30
#define MENU_FONT_SMALL ASSET_IOSEVKA_MEDIUM_20

#define MENU_BG        0x0000
#define MENU_ACCENT    0x565F  // same accent blue the battery/progress gauges use
#define MENU_TRACK     0x2965  // same dark-grey track color the battery gauge uses

#define MENU_LIST_TOP_Y  26
#define MENU_ROW_H       24
#define MENU_CURSOR_W    3
#define MENU_TEXT_X      10

static void menu_clear(void) {
    lcd_clear_rect(0, 0, 128, 128);
}

static void menu_draw_cursor(uint8_t sel, uint16_t color) {
    uint16_t y = MENU_LIST_TOP_Y + (uint16_t)sel * MENU_ROW_H;
    lcd_fill_rect(0, y, MENU_CURSOR_W, y + 20, color);
}

static void menu_draw_list(void) {
    menu_clear();
    lcd_draw_flash_text(MENU_FONT_SMALL, 4, 2, "SETTINGS");
    for (uint8_t i = 0; i < MENU_ITEM_COUNT; i++) {
        uint16_t y = MENU_LIST_TOP_Y + (uint16_t)i * MENU_ROW_H;
        lcd_draw_flash_text(MENU_FONT_SMALL, MENU_TEXT_X, y, menu_item_names[i]);
    }
    menu_draw_cursor(menu_sel, MENU_ACCENT);
}

// Redraws just the value area (name stays put -- only called on full==true
// right after entering MENU_ADJUST). RGB Matrix state is queried live each
// call rather than cached, so that alone is the "current value" source of
// truth -- no separate menu-local copy to keep in sync. Volume is the
// exception: the host doesn't report its level back, so it just shows which
// way the knob last turned instead of a value (see menu_volume_dir above).
static void menu_draw_adjust(bool full) {
    if (full) {
        menu_clear();
        lcd_draw_flash_text(MENU_FONT_SMALL, 4, 2, menu_item_names[menu_sel]);
        if (menu_sel == MENU_ITEM_VOLUME) menu_volume_dir = 0;
    } else {
        lcd_clear_rect(0, 40, 128, 60);
    }

    char buf[8];
    if (menu_sel == MENU_ITEM_EFFECT) {
        snprintf(buf, sizeof(buf), "%u/%u", (unsigned)rgb_matrix_get_mode(), (unsigned)(RGB_MATRIX_EFFECT_MAX - 1));
        lcd_draw_flash_text(MENU_FONT_BIG, 10, 50, buf);
        return;
    }

    if (menu_sel == MENU_ITEM_VOLUME) {
        lcd_draw_flash_text(MENU_FONT_BIG, 52, 50, menu_volume_dir > 0 ? "+" : menu_volume_dir < 0 ? "-" : "");
        return;
    }

    uint8_t v = menu_sel == MENU_ITEM_BRIGHTNESS ? rgb_matrix_get_val() : rgb_matrix_get_hue();
    snprintf(buf, sizeof(buf), "%u", (unsigned)v);
    lcd_draw_flash_text(MENU_FONT_BIG, 10, 50, buf);

    lcd_fill_rect(4, 96, 124, 102, MENU_TRACK);
    uint16_t fw = (uint16_t)((uint32_t)(124 - 4) * v / 255u);
    if (fw) lcd_fill_rect(4, 96, 4 + fw, 102, MENU_ACCENT);
}

static void menu_turn(bool clockwise);  // defined below, used by process_record_user

static void menu_enter(void) {
    if (anim_active()) return;  // animation owns the bus, refuse for now
    menu_state = MENU_LIST;
    menu_sel   = 0;
    menu_draw_list();
    layer_on(MENU);
}

static void menu_exit(void) {
    menu_state = MENU_IDLE;
    layer_off(MENU);
    display_redraw_dashboard(0, NULL);
}

// Tap-vs-hold state for the knob's plain press (KC_MUTE, matrix [0,14]).
// Polled from matrix_scan_user rather than resolved in process_record_user
// so a hold can fire menu_enter/menu_exit while the key is still down,
// instead of waiting for release.
#define MUTE_HOLD_MS 400
static bool     mute_held       = false;
static bool     mute_hold_fired = false;
static uint16_t mute_press_time = 0;


bool dip_switch_update_user(uint8_t index, bool active) {
    // Mac/Windows layer switch. The wireless slider (index 1/2) and dashboard
    // icons are handled at keyboard level in ak820pro.c (dip_switch_update_kb).
    if (index == 0) {
        if (active) {
            set_single_persistent_default_layer(WINBASE);
        } else {
            set_single_persistent_default_layer(MACBASE);
            keymap_config.no_gui = false;
        }
    }
    return true;
}


bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    // Pi Agent layer macros (WINFN + "/" held). Only intercept on press --
    // AG_* keycodes don't need release handling, and gating like this (rather
    // than kick75's blanket "ignore all releases" at the top of this
    // function) keeps every other case below free to see its own releases.
    if (record->event.pressed && !agent_process_record(keycode)) return false;

    // Windows-only: fill in word/line navigation and browser tab-switching
    // that macOS already provides natively via Cmd. Only fires on the plain
    // WINBASE layer (no Fn/Cursor overlay held).
    if (record->event.pressed && IS_LAYER_ON_STATE(default_layer_state, WINBASE) && get_highest_layer(layer_state) == 0) {
        uint8_t mods = get_mods();
        if ((mods & MOD_MASK_ALT) && (mods & MOD_MASK_CTRL)) {
            // Ctrl+Alt+Right → End, Ctrl+Alt+Left → Home
            // Ctrl+Alt+Shift+Right → Shift+End, Ctrl+Alt+Shift+Left → Shift+Home
            if (keycode == KC_RGHT || keycode == KC_LEFT) {
                // Save Ctrl+Alt and remove them from pressed keys
                uint8_t held = mods & (MOD_MASK_ALT | MOD_MASK_CTRL);
                del_mods(held);

                // Tap Home or End (Shift stays held if pressed, giving selection)
                tap_code(keycode == KC_RGHT ? KC_END : KC_HOME);

                // Restore Ctrl+Alt
                add_mods(held);
                return false;
            }

            // Ctrl+Alt+[ → Ctrl+Shift+Tab, Ctrl+Alt+] → Ctrl+Tab
            if (keycode == KC_LBRC || keycode == KC_RBRC) {
                // Save Alt and remove it (keep Ctrl held)
                uint8_t held_alt = mods & MOD_MASK_ALT;
                del_mods(held_alt);

                // Add Shift for "[" (Ctrl+Shift+Tab), leave it off for "]" (Ctrl+Tab)
                if (keycode == KC_LBRC) register_code(KC_LSFT);
                tap_code(KC_TAB);
                if (keycode == KC_LBRC) unregister_code(KC_LSFT);

                // Restore Alt
                add_mods(held_alt);
                return false;
            }
        }
    }

    // Only Mac-specific media keys live here; SCR_TOG and the BT* keycodes are
    // handled at keyboard level in ak820pro.c (process_record_kb).
    switch (keycode) {
        case KC_MISSION_CONTROL:
            if (record->event.pressed) {
                host_consumer_send(0x29F);
            } else {
                host_consumer_send(0);
            }
            return false;
        case KC_LAUNCHPAD:
            if (record->event.pressed) {
                host_consumer_send(0x2A0);
            } else {
                host_consumer_send(0);
            }
            return false;
        case KC_MUTE:
            // Hold vs. tap is resolved by matrix_scan_user (see mute_held
            // above); here we just track press/release and, on a release
            // that wasn't already consumed by a hold-fire, run whichever tap
            // action fits the current state.
            if (record->event.pressed) {
                mute_held       = true;
                mute_hold_fired = false;
                mute_press_time = timer_read();
            } else {
                mute_held = false;
                if (mute_hold_fired) {
                    mute_hold_fired = false;
                } else if (menu_state != MENU_IDLE) {
                    if (menu_state == MENU_LIST) {
                        menu_state = MENU_ADJUST;
                        menu_draw_adjust(true);
                    } else {
                        menu_state = MENU_LIST;
                        menu_draw_list();
                    }
                } else {
                    tap_code(KC_MUTE);
                }
            }
            return false;
        case MENU_CW:
            if (record->event.pressed) {
                if (menu_state == MENU_IDLE) menu_enter();
                if (menu_state != MENU_IDLE) menu_turn(true);
            }
            return false;
        case MENU_CCW:
            if (record->event.pressed) {
                if (menu_state == MENU_IDLE) menu_enter();
                if (menu_state != MENU_IDLE) menu_turn(false);
            }
            return false;
        default:
            return true;
    }
}


// Fires once, mid-hold, as soon as the knob has been down past MUTE_HOLD_MS;
// the eventual release then sees mute_hold_fired and skips its own tap
// action. A hold always toggles the whole menu open/closed, whatever substate
// it was in.
void matrix_scan_user(void) {
    if (mute_held && !mute_hold_fired && timer_elapsed(mute_press_time) >= MUTE_HOLD_MS) {
        mute_hold_fired = true;
        if (menu_state == MENU_IDLE) menu_enter();
        else                         menu_exit();
    }
}


// Only ever reached via MENU_CW/MENU_CCW below, once the menu is confirmed
// open (either already was, or the case above just opened it and this same
// turn moves the cursor as its first step).
static void menu_turn(bool clockwise) {
    if (menu_state == MENU_LIST) {
        menu_draw_cursor(menu_sel, MENU_BG);  // clear old cursor position
        if (clockwise) menu_sel = (uint8_t)((menu_sel + 1) % MENU_ITEM_COUNT);
        else           menu_sel = (uint8_t)((menu_sel + MENU_ITEM_COUNT - 1) % MENU_ITEM_COUNT);
        menu_draw_cursor(menu_sel, MENU_ACCENT);
    } else {  // MENU_ADJUST
        switch (menu_sel) {
            case MENU_ITEM_VOLUME:
                tap_code(clockwise ? KC_VOLU : KC_VOLD);
                menu_volume_dir = clockwise ? 1 : -1;
                break;
            case MENU_ITEM_EFFECT:
                if (clockwise) rgb_matrix_step_noeeprom();
                else           rgb_matrix_step_reverse_noeeprom();
                break;
            case MENU_ITEM_BRIGHTNESS:
                if (clockwise) rgb_matrix_increase_val_noeeprom();
                else           rgb_matrix_decrease_val_noeeprom();
                break;
            case MENU_ITEM_HUE:
                if (clockwise) rgb_matrix_increase_hue_noeeprom();
                else           rgb_matrix_decrease_hue_noeeprom();
                break;
        }
        menu_draw_adjust(false);
    }
}


bool display_housekeeping_task_user(void) {
    // Suppress the stock dashboard's own per-tick redraw while the menu owns
    // the panel; all menu drawing is event-driven (keypress/encoder), not
    // ticked, so there's nothing to do here beyond gating stock's.
    return menu_state == MENU_IDLE;
}


bool rgb_matrix_indicators_advanced_user(uint8_t led_min, uint8_t led_max) {
    if (get_highest_layer(layer_state) == _AGENT) {
        agent_layer_leds(led_min, led_max);
        return false;
    }
    return true;
}


#if defined(ENCODER_MAP_ENABLE)
const uint16_t PROGMEM encoder_map[][NUM_ENCODERS][NUM_DIRECTIONS] = {
    // The knob turn always drives the settings menu now (opening it on the
    // first turn if it's closed) rather than volume directly, since volume
    // lives inside the menu as its own item -- see the header comment above
    // the menu code and the MENU_CW/MENU_CCW handling in process_record_user.
    [WINBASE] = {ENCODER_CCW_CW(MENU_CCW, MENU_CW) },
    [WINFN] = {ENCODER_CCW_CW(MENU_CCW, MENU_CW) },
    [MACBASE] = {ENCODER_CCW_CW(MENU_CCW, MENU_CW) },
    [MACFN] = {ENCODER_CCW_CW(MENU_CCW, MENU_CW) },
    [CURSOR] = {ENCODER_CCW_CW(MENU_CCW, MENU_CW) },
    [_AGENT] = {ENCODER_CCW_CW(MENU_CCW, MENU_CW) },
    // Highest active layer whenever the menu is open (layer_on'd in
    // menu_enter), so this wins regardless of whether Fn/WINFN is also on.
    [MENU] = {ENCODER_CCW_CW(MENU_CCW, MENU_CW) }
};
#endif
