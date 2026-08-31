#pragma once

#include QMK_KEYBOARD_H
#include "keymap_introspection.h"  // keycode_at_keymap_location_raw()
#include "battery_indicator.h"     // set_color_scaled()

// Layer 5 - Pi Agent. Held via FN + "/" (MO(_AGENT) lives in the Win Fn layer).
#define _AGENT 5

// Custom keycode base. No collision with Nuphy's keyboard-level keycodes
// (LNK_BLE1, SIDE_MOD, ...): those start at QK_KB_0, SAFE_RANGE is QK_USER.
#define AG_KC_BASE SAFE_RANGE

// Appended after every slash command to submit it.
// pi shows an autocomplete dropdown while a "/..." command is being typed.
#define AG_SUBMIT "\n"

// Appended instead of AG_SUBMIT for commands that expect an argument: the
// command is typed but not submitted, leaving the cursor ready for the request.
#define AG_ARGS " "

// Keep this enum and ag_strings[] below in the same order - the string is looked
// up by (keycode - AG_FIRST).
enum agent_keycodes {
    AG_FIRST = AG_KC_BASE,

    // Skills - F1..F12
    AG_BRAINSTORM = AG_FIRST,
    AG_WRITE_PLAN,
    AG_EXEC_PLAN,
    AG_DEBUG_SYS,
    AG_TDD,
    AG_REQ_REVIEW,
    AG_RECV_REVIEW,
    AG_VERIFY,
    AG_WORKTREES,
    AG_PARALLEL,
    AG_COUNCIL,
    AG_PONYTAIL,

    // Session ops - number row
    AG_NEW,
    AG_RESUME,
    AG_FORK,
    AG_TREE,
    AG_COMPACT,
    AG_NAME,
    AG_SHARE,
    AG_EXPORT,
    AG_COPY,
    AG_RELOAD,
    AG_SETTINGS,
    AG_SCOPED,

    // Misc slash commands
    AG_THINKING,
    AG_MODEL,
    AG_LLAMA,
    AG_DBG,

    AG_LAST
};

static const char *const ag_strings[] = {
    // Skills
    [AG_BRAINSTORM  - AG_FIRST] = "/skill:brainstorming" AG_ARGS,
    [AG_WRITE_PLAN  - AG_FIRST] = "/skill:writing-plans" AG_ARGS,
    [AG_EXEC_PLAN   - AG_FIRST] = "/skill:executing-plans" AG_ARGS,
    [AG_DEBUG_SYS   - AG_FIRST] = "/skill:systematic-debugging" AG_ARGS,
    [AG_TDD         - AG_FIRST] = "/skill:test-driven-development" AG_ARGS,
    [AG_REQ_REVIEW  - AG_FIRST] = "/skill:requesting-code-review" AG_ARGS,
    [AG_RECV_REVIEW - AG_FIRST] = "/skill:receiving-code-review" AG_ARGS,
    [AG_VERIFY      - AG_FIRST] = "/skill:verification-before-completion" AG_ARGS,
    [AG_WORKTREES   - AG_FIRST] = "/skill:using-git-worktrees" AG_ARGS,
    [AG_PARALLEL    - AG_FIRST] = "/skill:dispatching-parallel-agents" AG_ARGS,
    [AG_COUNCIL     - AG_FIRST] = "/skill:council-mode" AG_ARGS,
    [AG_PONYTAIL    - AG_FIRST] = "/skill:ponytail-review" AG_ARGS,

    // Session ops
    [AG_NEW      - AG_FIRST] = "/new" AG_SUBMIT,
    [AG_RESUME   - AG_FIRST] = "/resume" AG_SUBMIT,
    [AG_FORK     - AG_FIRST] = "/fork" AG_SUBMIT,
    [AG_TREE     - AG_FIRST] = "/tree" AG_SUBMIT,
    [AG_COMPACT  - AG_FIRST] = "/compact" AG_SUBMIT,
    [AG_NAME     - AG_FIRST] = "/name" AG_ARGS,
    [AG_SHARE    - AG_FIRST] = "/share" AG_SUBMIT,
    [AG_EXPORT   - AG_FIRST] = "/export" AG_SUBMIT,
    [AG_COPY     - AG_FIRST] = "/copy" AG_SUBMIT,
    [AG_RELOAD   - AG_FIRST] = "/reload" AG_SUBMIT,
    [AG_SETTINGS - AG_FIRST] = "/settings" AG_SUBMIT,
    [AG_SCOPED   - AG_FIRST] = "/scoped-models" AG_SUBMIT,

    // Misc
    [AG_THINKING - AG_FIRST] = "/thinking" AG_SUBMIT,
    [AG_MODEL    - AG_FIRST] = "/model" AG_SUBMIT,
    [AG_LLAMA    - AG_FIRST] = "/llama" AG_SUBMIT,
    [AG_DBG      - AG_FIRST] = "/debug" AG_SUBMIT,
};

// Types the slash command bound to an agent-layer keycode.
// Returns false if the keycode was handled.
static inline bool agent_process_record(uint16_t keycode) {
    if (keycode < AG_FIRST || keycode >= AG_LAST) return true;

    const char *str = ag_strings[keycode - AG_FIRST];
    if (str != NULL) send_string(str);
    return false;
}

//
// Row colors. A key lights up only if layer 5 actually binds it, so an unbound
// key stays dark and adding a binding lights it automatically - the layer is its
// own cheat sheet while FN+"/" is held.
//
// Plain R,G,B - NOT the RGB struct, whose field order the Kick75 driver swaps
// (see the note at the top of battery_indicator.h).
static const uint8_t ag_row_colors[][3] = {
    {  0, 200, 255},  // row 0 - skills            cyan
    {  0, 255,  80},  // row 1 - session ops       green
    {255,   0, 200},  // row 2 - prompt templates  magenta
    {255, 150,   0},  // row 3 - mode chords       amber
    { 60,  80, 255},  // row 4 - transcript nav    blue
    {160, 160, 160},  // row 5 - unused            grey
};

static inline void agent_layer_leds(uint8_t led_min, uint8_t led_max) {
    for (uint8_t i = led_min; i < led_max; i++) {
        rgb_matrix_set_color(i, 0, 0, 0);
    }

    for (uint8_t row = 0; row < MATRIX_ROWS; row++) {
        for (uint8_t col = 0; col < MATRIX_COLS; col++) {
            uint8_t led = g_led_config.matrix_co[row][col];
            if (led == NO_LED || led < led_min || led >= led_max) continue;

            // _raw reads the firmware keymap directly. The non-raw variant is
            // weak and VIA overrides it to read EEPROM - far too slow per frame.
            uint16_t kc = keycode_at_keymap_location_raw(_AGENT, row, col);
            if (kc == KC_NO || kc == KC_TRNS) continue;

            const uint8_t *c = ag_row_colors[row < ARRAY_SIZE(ag_row_colors) ? row : 0];
            set_color_scaled(led, c[0], c[1], c[2]);
        }
    }
}
