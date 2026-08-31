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

Three effects are available, controlled via `rgb_matrix_indicators_advanced_user()` in `keymap.c`:

1. **battery_indicator_simple** — ESC key shows green (>=30%), orange (>=10%), or red (<10%)
2. **battery_indicator_rainbow** — ESC key shows one of 7 colors mapped across 0-100%
3. **battery_indicator_fkeys** — F1-F12 as a progress bar, keys lit proportionally to battery level; red (1–4 keys), amber (5–8 keys), green (9–12 keys)

All indicator colors respect the current RGB brightness setting.

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
