# Setting up a new machine

This repo (`naviltsev/qmk_custom`) is a [QMK CLI userspace](https://docs.qmk.fm/newbs_external_userspace)
repo (see `qmk.json`) — it holds keymaps only, not a full `qmk_firmware`
tree. It has to be linked to an actual firmware clone via QMK CLI config
before `qmk compile`/`qmk flash` will find anything in it. Each keyboard
below uses a different firmware source, so the setup differs slightly per
board; this repo (and the link step) is shared by all of them.

## Ajazz AK820 Pro


#### [fpb/ajazz-ak820-pro](https://github.com/fpb/ajazz-ak820-pro) is the source of information for QMK-based builds.

---

Firmware source is a personal fork of `fpb/qmk_firmware`
(`fpb`'s `ak820pro-lcd-flash` branch, which carries the LCD/flash-provisioning
work this board depends on), with additional fixes and features layered on
top — see `keyboards/a_jazz/ak820pro/keymaps/naviltsev/SETTINGS_MENU_SPEC.md`
and `host/doom-stream/SPEC.md` in this repo for what's been added.

1. **Clone the fork** instead of `fpb/qmk_firmware` directly:
   ```sh
   git clone https://github.com/naviltsev/qmk_ak820pro_firmware -b ak820pro-lcd-flash
   ```

2. **Clone this userspace repo** (if not already):
   ```sh
   git clone https://github.com/naviltsev/qmk_custom
   ```

3. **Link them** — the step that's easy to forget, since it's invisible
   once done. Run from anywhere, pointing at wherever step 2 landed:
   ```sh
   qmk config user.overlay_dir="<path to naviltsev/qmk_custom>" [for example: d:/dev/src/github.com/naviltsev/qmk_custom]
   ```
   This is what makes `-km naviltsev` resolve to keymap files living in a
   separate repo/clone rather than inside the firmware tree itself.

4. **Toolchain**: needs a working QMK build environment (QMK MSYS on
   Windows, or the equivalent on macOS/Linux) with `gcc`/`make`. If you also
   want to build the host-side tools (`ak820ctl`, the doomgeneric backends
   in `host/doom-stream/`), those additionally need `hidapi` +
   `pkg-config` -- see their own READMEs/Makefiles in their respective
   repos (`fpb/time-util-ak820pro`, `ozkl/doomgeneric`), which are separate
   clones, unrelated to the QMK firmware setup above.

Once set up:
```sh
qmk compile -kb a_jazz/ak820pro -km naviltsev
qmk flash   -kb a_jazz/ak820pro -km naviltsev
```

### Keeping up with `fpb`'s upstream changes

The fork's `origin` is your own (`naviltsev/qmk_ak820pro_firmware`, where
you push); add `fpb/qmk_firmware` as a second remote to pull their future
updates from:

```sh
git remote add upstream https://github.com/fpb/qmk_firmware
git fetch upstream
git merge upstream/ak820pro-lcd-flash   # or rebase, per preference
```

(If you cloned via `gh repo fork` instead of plain `git clone`, this is
already set up automatically.)

## Nuphy Kick75

Firmware source is NuPhy's own vendor fork, `nuphy-src/qmk_firmware`
(branch `nuphy-keyboards`) -- no personal fork of this one; it's used
as-is, just with a keymap layered on top via the userspace link.

1. **Clone NuPhy's fork**:
   ```sh
   git clone https://github.com/nuphy-src/qmk_firmware -b nuphy-keyboards
   ```

2. **Clone this userspace repo** (if not already -- shared with AK820 Pro
   above, only needs doing once):
   ```sh
   git clone https://github.com/naviltsev/qmk_custom
   ```

3. **Link them**, same mechanism as above (one overlay_dir setting covers
   every keyboard built via QMK CLI on this machine, not just one):
   ```sh
   qmk config user.overlay_dir="<path to naviltsev/qmk_custom>"
   ```

4. **Toolchain**: same QMK build environment as above; no additional
   host-side tooling for this board.

Once set up:
```sh
qmk compile -kb nuphy/kick75/ansi -km naviltsev
qmk flash   -kb nuphy/kick75/ansi -km naviltsev
```

### Keeping up with real upstream QMK

`nuphy-src/qmk_firmware` is a vendor fork of official QMK, not a personal
fork, so there's no push-your-own-changes concern here -- but if you want
to pull real upstream QMK's changes for reference/comparison:

```sh
git remote add upstream https://github.com/qmk/qmk_firmware
git fetch upstream
```

## Weikav Nut65

Firmware source is the vendor's fork, `hangshengkeji/qmk_firmware`
(branch `master`). The board lives at **`keyboards/leku/nut65`** there, not
`weikav/nut65` -- it's LEKU's NUT65 design -- so the keymap in this repo
mirrors that path: `keyboards/leku/nut65/keymaps/naviltsev`.

1. **Clone the vendor fork, with submodules** (ChibiOS etc. -- without them
   the build fails with `startup_wb32fq95xx.mk: No such file or directory`):
   ```sh
   git clone https://github.com/hangshengkeji/qmk_firmware -b master --recurse-submodules
   ```
   For an existing clone: `git submodule update --init --recursive`.

2. **Apply the local changes to `keyboards/leku/nut65/nut65.c`.** There's no
   personal fork of this one yet, so these live only in the local clone and
   have to be redone on a fresh one:
   - At the end of the `if (!start_paoma_flag) { ... }` block in
     `rgb_matrix_indicators_advanced_kb()`, right after `query();`, add
     `rgb_matrix_indicators_advanced_user(led_min, led_max);`. The board
     never calls the user hook otherwise, and the keymap's front-strip
     indicators (battery gauge, Win/Mac, connection) depend on it.
   - In `eeconfig_confinfo_default()`, replace the hard-coded
     `confinfo.rl_val = 4;` and `confinfo.rgb_light_mode = 0;` with
     `HS_RL_DEFAULT_VAL` and `HS_RL_DEFAULT_MODE`, each defined above the
     function under `#ifndef` with the old value as fallback. The keymap's
     `config.h` sets them (front strip: dimmest, effect off).

   `nut65.c` isn't UTF-8 (GBK comments) -- edit it with a tool that keeps the
   encoding (e.g. `sed`), not one that rewrites the file as UTF-8.

3. **Point QMK at this fork when building.** The `overlay_dir` link from
   above is shared, but `user.qmk_home` is a single global setting, normally
   left pointing at one of the other boards' firmware, so set `QMK_HOME` per
   command (QMK MSYS):
   ```sh
   QMK_HOME=/d/Dev/_qmk_forks/weikav qmk compile -kb leku/nut65 -km naviltsev
   ```
   Firmware lands as `leku_nut65_naviltsev.bin` in this repo's root. A change
   to the keymap's `config.h` triggers a full rebuild (a few minutes).

### Flashing

QMK Toolbox works (WB32 DFU bootloader; driver `342D:DFA0` comes with QMK
MSYS / Toolbox's *Install Drivers*).

1. Connect the USB cable, in wired mode (Fn+T).
2. Enter the bootloader:
   - **Fn + Right Shift + Esc** (`QK_BOOT`). This also **wipes the EEPROM**,
     so the board comes up with the defaults from the keymap's `config.h`
     (static red keys at half brightness, front strip off/dimmest) -- use
     this one when those defaults changed.
   - Or hold the reset button on the PCB underside while plugging in --
     keeps the saved settings.
3. Flash `leku_nut65_naviltsev.bin` in QMK Toolbox.
4. **Unplug the cable, then flip the power switch (under the Tab keycap)
   off and on.** The board doesn't start the new firmware by itself after
   "Flash Complete", and the switch doesn't power-cycle it while USB is
   connected -- until then it looks bricked.

Note: VIA's saved layout resets on every flash of a build from a different
day (VIA keys its EEPROM data on the build date), so keep remaps in
`keymap.c`, not only in VIA.
