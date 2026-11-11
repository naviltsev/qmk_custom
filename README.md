# Custom QMK keymaps

Keymaps for three keyboards, all under the `naviltsev` keymap name:

| Keyboard | Keymap |
|---|---|
| Ajazz AK820 Pro | `keyboards/a_jazz/ak820pro/keymaps/naviltsev` |
| NuPhy Kick75 | `keyboards/nuphy/kick75/ansi/keymaps/naviltsev` |
| Weikav Nut65 | `keyboards/leku/nut65/keymaps/naviltsev` |

Build and flash setup for each board is in [INSTALL.md](INSTALL.md).

## Shared across all keyboards

### Caps Lock layer

Caps Lock is not Caps Lock. Holding it turns on a cursor layer, from both the
Mac and Windows base layers. Keys not listed here type what they normally do.

| Keys (Caps held) | Output |
|---|---|
| W / A / S / D | ↑ / ← / ↓ / → |
| I / J / K / L | ↑ / ← / ↓ / → |
| 1 … 0, `-`, `=` | F1 … F12 |
| Esc | `` ` `` — **Nut65 only**, which has Esc where the other boards have `` ` `` |

There is no key that turns Caps Lock itself on.

### Windows layer macros (no Fn)

Only on the Windows base layer, and only with nothing else held: no Fn, no Caps.
So they work with the real arrow keys, not with the Caps layer arrows.

| Combo | Output | Description |
|---|---|---|
| Ctrl+Alt+Right | End | |
| Ctrl+Alt+Left | Home | |
| Ctrl+Alt+Shift+Right / Left | Shift+End / Shift+Home | Select to end / start of line |
| Ctrl+Alt+[ | Ctrl+Shift+Tab | Previous tab in VSCode, browser, terminal, etc. |
| Ctrl+Alt+] | Ctrl+Tab | Next tab in VSCode, browser, terminal, etc. |

On the Nut65, `nut65.c` already defines `process_record_user()`, so these
macros live in `pre_process_record_user()` there instead.

## Ajazz AK820 Pro

- Win/Mac is picked with the hardware switch, not a key.
- The knob drives an on-screen settings menu: turning it opens Volume, holding
  the knob opens the menu list (Volume, Effect, Brightness, Hue, Stats, Battery,
  Exit). A quick press is mute while the menu is closed.
- RGB boots into solid color every time, whatever the menu last set.
- Layers: WINBASE, WINFN, MACBASE, MACFN, CURSOR (Caps), MENU. MENU is switched
  on by the knob, not held, and must stay the highest layer.

## Weikav Nut65

- Default backlight: static red at half brightness; front strip effect off at
  the second-dimmest step. Applied only after an EEPROM reset.
- Needs a few local changes to the vendor's board code — see
  [INSTALL.md](INSTALL.md).

### Front LED strip

The 80-LED strip along the front edge works as the indicator panel (it plays the
Kick75's ESC-key role, in the same colors).

**Connection indicator** — the middle of the strip has 5 small slots, laid out
left to right like the Fn + Q / W / E / R / T keys that select them. Only the
active connection's slot lights:

| Slot | Connection | Color |
|---|---|---|
| 1 | BT1 | Green |
| 2 | BT2 | White |
| 3 | BT3 | Amber |
| 4 | 2.4G (dongle) | Blue |
| 5 | USB | Red |

The slot blinks while not connected and stays steady once connected.

**Win/Mac marker** — 3 LEDs at each end of the strip: blue on Windows, white on
Mac. After Fn + M switches the OS, the whole strip blinks along with the M key.

**Battery** — hold Fn + Space (wireless only) to show charge across the whole
strip: green (≥50%), orange (≥25%), red (below). The number-row gauge the
board shows by default is turned off, so the keys just go dark while it's held.

Everything on the strip follows its own brightness (Fn + PgUp / PgDn). The rest
of the strip keeps the board's side-light effect, which is off by default.

## NuPhy Kick75

### General

- Debounce: 15ms (default is 0ms) — should fix double-typing issue (?)

### All layers
- remapped DEL/HOME/PGUP/PGDN to PGUP/PGDN/HOME/DEL;

### Mac layer
- F1–F12 are functional keys, no FN required;
- multimedia controls (of F1-F12 keys - brightness, playback, etc.) require FN held;

### Fn layer

The stock Fn + P / L / ; / ' arrows are removed — use the Caps layer instead.

### Battery indicator

Hold **Fn + Space** to show battery charge on the **F1–F10** keys. It works on both the Mac and Windows layers. Each key is 10%, rounded
up, so any charge lights at least F1. Colors: red (1–3 keys), amber (4–6),
green (7–10). Unlit keys go dark so the bar reads clearly over any effect. The
gauge disappears as soon as you release the keys.

The gauge is drawn by `battery_indicator_f1_f10()`, called from
`rgb_matrix_indicators_advanced_user()` in `keymap.c` while the `BAT_GAUGE` key
is held. `battery_indicator.h` keeps a few alternatives that aren't currently used:

1. **battery_indicator_simple** — ESC key shows green (>=30%), orange (>=10%), or red (<10%)
2. **battery_indicator_rainbow** — ESC key shows one of 7 colors mapped across 0-100%
3. **battery_indicator_fkeys** — F1-F12 as a progress bar; red (1–4 keys), amber (5–8 keys), green (9–12 keys). This used to be always on, and the Fn + Space gauge replaced it.

All indicator colors respect the current RGB brightness setting, so the gauge
isn't visible with RGB off (Fn + Tab).

### Connection indicator

ESC key color shows the active connection:

| Connection | Color |
|---|---|
| BT1 | Green |
| BT2 | White |
| BT3 | Amber |
| RF (2.4GHz dongle) | Blue |
| USB | Red |

### RGB

All stock QMK RGB matrix effects are enabled — the full set `keyboard.json` in
NuPhy's fork turns on. The keymap no longer disables any of them, and has no
custom effects of its own.

Defaults (`config.h`), applied only after an EEPROM reset — otherwise the last
settings you chose are kept:

- Effect: **RGB_MATRIX_SOLID_COLOR**
- Color: QMK default — hue 0, full saturation, i.e. **red**
- Brightness: **64** (1/4)

Nothing overrides color on boot, so effects show their real colors.

| Keys | Action |
|---|---|
| Fn + ← | Next effect (Fn + Shift + ← — previous) |
| Fn + ↑ / ↓ | Brightness up / down |
| Fn + → | Hue (color) |
| Fn + , / . | Effect speed down / up |
| Fn + Tab | RGB on / off |

Side light strip: same keys while holding **Fn + M**.

#### Notes

- The NuPhy fork's keyboard-level `rules.mk` sets `RGB_MATRIX_CUSTOM_USER = yes`,
  which makes the build require a `rgb_matrix_user.inc`. The keymap's `rules.mk`
  sets it back to `no`. Remove that override if you add custom effects again.
- There's no saturation key, so pure white isn't reachable from the keyboard
  anymore. Earlier, `keyboard_post_init_user()` forced white (sat 0) on every
  boot, but that made every effect white, so it was removed.
