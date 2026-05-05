# NuPhy Kick75 — custom keymap

## General

- Debounce: 15ms (default is 0ms) — should fix double-typing issue (?)

## Keymap

### Mac layer
- F1–F12 are functional keys, no FN required;
- multimedia controls (of F1-F12 keys - brightness, playback, etc.) require FN held;

### Windows layer macros (layer 2, no FN)

| Combo | Output | Description |
|---|---|---|
| Ctrl+Alt+Right | End | |
| Ctrl+Alt+Left | Home | |
| Ctrl+Alt+[ | Ctrl+Shift+Tab | Previous tab in VSCode, browser, terminal, etc. |
| Ctrl+Alt+] | Ctrl+Tab | Next tab in VSCode, browser, terminal, etc. |

## Battery indicator

Three effects are available, controlled via `rgb_matrix_indicators_advanced_user()` in `keymap.c`:

1. **battery_indicator_simple** — ESC key shows green (>=30%), orange (>=10%), or red (<10%)
2. **battery_indicator_fkeys** — F1-F12 as a progress bar, green keys lit proportionally to battery level
3. **battery_indicator_rainbow** — ESC key shows one of 7 colors mapped across 0-100%

## RGB

Default color: white (HSV 0, 0, 64 — 1/4 brightness), set via `keyboard_post_init_user()`. After an EEPROM reset the board reverts to red — reboot (turn off then on) to restore white.

Active effects (all others disabled):

1. **RGB_MATRIX_SOLID_COLOR** (the default one after EEPROM reset / reboot)
2. **RGB_MATRIX_TYPING_HEATMAP**
3. **RGB_MATRIX_SOLID_REACTIVE_SIMPLE**
4. **RGB_MATRIX_CUSTOM_deus_ex** — black background with themed key colors:
   - WASD — light blue
   - F1–F2 — purple
   - F3–F12 — green
   - 1–0 row — orange
   - Y — yellow
   - B — red
