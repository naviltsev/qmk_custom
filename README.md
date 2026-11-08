# NuPhy Kick75 — custom keymap

## General

- Debounce: 15ms (default is 0ms) — should fix double-typing issue (?)

## Keymap

### All layers
- remapped DEL/HOME/PGUP/PGDN to PGUP/PGDN/HOME/DEL;

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

## Pi Agent layer (layer 5)

Held via **FN + `/`** (`MO(_AGENT)` sits in the Windows Fn layer). Windows only for
now — add the same `MO(_AGENT)` at `/` in layer 1 to get it in Mac mode.

While held, the board repaints itself as its own cheat sheet: each row gets a color,
and a key with no binding stays dark. ESC keeps the connection-color indicator and
still acts as pi's interrupt.

### F1–F12 — skills (cyan)

Types the `/skill:` command with a trailing space and stops - skills take your
request as an argument, so the cursor is left ready for you to type it.

| Key | Skill | Key | Skill |
|---|---|---|---|
| F1 | brainstorming | F7 | receiving-code-review |
| F2 | writing-plans | F8 | verification-before-completion |
| F3 | executing-plans | F9 | using-git-worktrees |
| F4 | systematic-debugging | F10 | dispatching-parallel-agents |
| F5 | test-driven-development | F11 | council-mode |
| F6 | requesting-code-review | F12 | ponytail-review |

### Number row — session ops (green)

| Key | Command | Key | Command |
|---|---|---|---|
| 1 | `/new` | 7 | `/share` |
| 2 | `/resume` | 8 | `/export` |
| 3 | `/fork` | 9 | `/copy` |
| 4 | `/tree` | 0 | `/reload` |
| 5 | `/compact` | `-` | `/settings` |
| 6 | `/name ` (no Enter — takes an argument) | `=` | `/scoped-models` |

### QWERTY row — prompt templates (magenta)

Empty by design. Add `~/.pi/agent/prompts/<name>.md`, then bind a key here — the
prompt text lives in a file you can edit without reflashing.

### Home row — mode chords (amber)

Native pi keybindings, Windows defaults. No typing, instant.

| Key | Action | Key | Action |
|---|---|---|---|
| A | toggle plan mode (`ctrl+alt+p`) | H | collapse thinking blocks (`ctrl+t`) |
| S | cycle thinking level (`shift+tab`) | J | collapse tool output (`ctrl+o`) |
| D | model selector (`ctrl+l`) | K | queue follow-up (`ctrl+q`) |
| F | next model (`ctrl+p`) | L | dequeue (`alt+q`) |
| G | previous model (`alt+p`) | `;` | copy last reply (`ctrl+x`) |
| | | `'` | external editor (`ctrl+g`) |

### ZXCV row — transcript (blue)

| Key | Action | Key | Action |
|---|---|---|---|
| Z | previous prompt (`ctrl+up`) | N | `/thinking` |
| X | next prompt (`ctrl+down`) | M | `/model` |
| C | search transcript (`ctrl+f`) | `,` | `/llama` |
| V | paste image (`alt+v`) | `.` | `/debug` |
| B | undo edit (`ctrl+z`) | | |

### Notes

- `A` needs the plan-mode extension installed at `~/.pi/agent/extensions/plan-mode/`
  (copy `examples/extensions/plan-mode/` from the pi package). Without it the key
  is a no-op.
- Holding FN + `/` occupies the right hand, so the layer is a left-hand instrument:
  F1–F7, 1–6, QWERT, ASDFG and ZXCVB are comfortable; the right half is a stretch.


## Battery indicator

Hold **Fn + Space** to show battery charge on the **F1–F10** keys, like the stock
NuT65 gauge. It works on both the Mac and Windows layers. Each key is 10%, rounded
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

## Connection indicator

ESC key color shows the active connection:

| Connection | Color |
|---|---|
| BT1 | Green |
| BT2 | White |
| BT3 | Amber |
| RF (2.4GHz dongle) | Blue |
| USB | Red |

## RGB

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

### Notes

- The NuPhy fork's keyboard-level `rules.mk` sets `RGB_MATRIX_CUSTOM_USER = yes`,
  which makes the build require a `rgb_matrix_user.inc`. The keymap's `rules.mk`
  sets it back to `no`. Remove that override if you add custom effects again.
- There's no saturation key, so pure white isn't reachable from the keyboard
  anymore. Earlier, `keyboard_post_init_user()` forced white (sat 0) on every
  boot, but that made every effect white, so it was removed.
