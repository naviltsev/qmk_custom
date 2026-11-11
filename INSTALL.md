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

### Building

In QMK MSYS, `cd` into the fork first -- QMK CLI uses the firmware tree it's
run from -- and check which tree it picked:
```sh
cd /d/Dev/_qmk_forks/ajazz
qmk env QMK_HOME        # should print the ajazz fork's path
qmk compile -kb a_jazz/ak820pro -km naviltsev
```
Firmware lands as `a_jazz_ak820pro_naviltsev.bin` in two places: the fork's
root and this repo's root (the build copies it to both). A change to the
keymap's `config.h` triggers a full rebuild (a few minutes).

### Flashing

With [SonixFlasher](https://github.com/SonixQMK/SonixFlasher), run directly
-- not `qmk flash`, which expects `sonixflasher` on the PATH. The binary
lives next to the forks, at `/d/Dev/_qmk_forks/sonixflasher.exe`.

1. Connect the USB cable and set the connection switch on the bottom of the
   board to **USB** (wired mode).
2. Start the flash, from the fork's root (where the `.bin` is):
   ```sh
   cd /d/Dev/_qmk_forks/ajazz
   ../sonixflasher.exe --vidpid 0c45/7140 --file a_jazz_ak820pro_naviltsev.bin
   ```
   `0c45/7140` is the SN32 bootloader's USB ID. While the board isn't in the
   bootloader yet, it prints *"Device failed to open, re-trying in 3
   seconds"* -- up to 5 attempts, so ~15 seconds to do step 3.
3. Once sonixflasher is running and retrying, enter the bootloader:
   **Fn + Esc** (`QK_BOOT`). It picks the board up on its next attempt.
4. It erases, flashes and verifies, then ends with *"Device succesfully
   flashed!"* / *"Flashing done. Rebooting."* -- the board restarts into the
   new firmware by itself.

A *"Device failed to init, re-trying"* line right after *"Updating Code
Option Table"* is normal; it re-checks and carries on.

### Setting the clock

The dashboard clock is set from the PC with `ak820ctl` (from
[fpb/time-util-ak820pro](https://github.com/fpb/time-util-ak820pro), cloned
to `/d/Dev/src/github.com/fpb/time-util-ak820pro`). It sends the PC's current
local time to the board over raw HID, so the board needs to be connected by
USB:
```sh
cd /d/Dev/src/github.com/fpb/time-util-ak820pro
./ak820ctl.exe clock
# clock set to 2026-11-11 14:04:46
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

### Building + flashing (one command)

STM32 DFU bootloader, flashed with `dfu-util` (comes with QMK MSYS). There's
no `QK_BOOT` key in the keymap -- the bootloader is entered with bootmagic
(hold Esc while plugging in), which also **wipes the EEPROM**, so RGB goes
back to the `config.h` defaults.

1. In QMK MSYS, from the fork, run:
   ```sh
   cd /d/Dev/_qmk_forks/nuphy
   qmk env QMK_HOME        # should print the nuphy fork's path
   qmk flash -kb nuphy/kick75/ansi -km naviltsev
   ```
   It builds first (same output as `qmk compile`; the `.bin` is copied to
   both the fork's root and this repo's root), then prints *"Bootloader not
   found ... Trying again every 0.5s"* while it waits. The board can stay on
   wireless, with the cable unplugged, until now.
2. Set the mode toggle on the back of the board to **wired**.
3. Hold **Esc**, plug in the USB cable while still holding it, and release
   Esc once the flash starts.
4. It ends with *"File downloaded successfully"* / *"Submitting leave
   request..."* -- the board leaves the bootloader and starts the new
   firmware by itself.

A *"dfuERROR ... Device's firmware is corrupt"* line right after the device
opens is normal: it's just the bootloader's status from being forced into
DFU, and dfu-util clears it and carries on.

To build without flashing: `qmk compile -kb nuphy/kick75/ansi -km naviltsev`.

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

2. **Apply the local changes to the board code.** There's no
   personal fork of this one yet, so these live only in the local clone and
   have to be redone on a fresh one:
   - `keyboards/leku/nut65/nut65.c`: at the end of the
     `if (!start_paoma_flag) { ... }` block in
     `rgb_matrix_indicators_advanced_kb()`, right after `query();`, add
     `rgb_matrix_indicators_advanced_user(led_min, led_max);`. The board
     never calls the user hook otherwise, and the keymap's front-strip
     indicators (battery gauge, Win/Mac, connection) depend on it.
   - `nut65.c`, `eeconfig_confinfo_default()`: replace the hard-coded
     `confinfo.rl_val = 4;` and `confinfo.rgb_light_mode = 0;` with
     `HS_RL_DEFAULT_VAL` and `HS_RL_DEFAULT_MODE`, each defined above the
     function under `#ifndef` with the old value as fallback. The keymap's
     `config.h` sets them (front strip: second-dimmest step, effect off).
   - `nut65.c`, `process_record_kb()`, `case RL_VAI` / `case RL_VAD`:
     remove `if (confinfo.rgb_light_mode == 12) return false;` from both.
     Otherwise Fn+PgUp/PgDn does nothing while the strip effect is off, and
     the keymap's strip indicators, which use that brightness, are stuck at
     whatever level was last set.
   - `keyboards/leku/nut65/rgb_record/rgb_record.c`, `query()`: wrap the
     `for (uint8_t i = 0; i < 10; i++)` loop that lights keys 1..0 for the
     Fn+Space battery gauge in `#ifndef HS_BATQ_NO_KEY_GAUGE` / `#endif`.
     The keymap defines it, since the front strip shows the gauge instead.

   `nut65.c` and `rgb_record.c` aren't UTF-8 (GBK comments) -- edit them with
   a tool that keeps the encoding (e.g. `sed`), not one that rewrites the
   file as UTF-8.

3. **Point QMK at this fork when building.** The `overlay_dir` link from
   above is shared, but `user.qmk_home` is a single global setting, normally
   left pointing at one of the other boards' firmware. In QMK MSYS, `cd` into
   the fork first -- QMK CLI uses the firmware tree it's run from -- and check
   which tree it picked before compiling:
   ```sh
   cd /d/Dev/_qmk_forks/weikav
   qmk env QMK_HOME        # should print the weikav fork's path
   qmk compile -kb leku/nut65 -km naviltsev
   ```
   Or, from anywhere, set it for a single command:
   ```sh
   QMK_HOME=/d/Dev/_qmk_forks/weikav qmk compile -kb leku/nut65 -km naviltsev
   ```
   Firmware lands as `leku_nut65_naviltsev.bin` in two places: the fork's
   root and this repo's root (the build copies it to both). A change
   to the keymap's `config.h` triggers a full rebuild (a few minutes).

### Flashing

WB32 DFU bootloader, USB ID `342D:DFA0`; the driver comes with QMK MSYS.
Entering the bootloader with **Fn + Right Shift + Esc** (`QK_BOOT`) also
**wipes the EEPROM**, so the board comes up with the defaults from the
keymap's `config.h` (static red keys at half brightness, front strip off at
its second-dimmest step).

#### Option A: build + flash in one command (recommended)

1. From the fork, run:
   ```sh
   cd /d/Dev/_qmk_forks/weikav
   qmk flash -kb leku/nut65 -km naviltsev
   ```
   It builds first (same output as `qmk compile`), then prints *"Bootloader
   not found"* / *"Not found device!"* every 0.5s while it waits. The board
   can stay on wireless until now.
2. Connect the USB cable and switch to wired mode (**Fn + T**).
3. Press **Fn + Right Shift + Esc**. The flash starts on its own, ends with
   *"Download completed!"*, then *"Reset device completed!"* -- the board
   restarts into the new firmware by itself. Nothing else to do.

#### Option B: build, then flash the file

1. Build with `qmk compile` (step 3 above).
2. Flash the `.bin`, from either directory that holds it:
   ```sh
   cd /d/Dev/_qmk_forks/weikav
   qmk flash leku_nut65_naviltsev.bin
   ```
   Given a file, it only flashes -- no build. It prints *"Please reset your
   keyboard into bootloader mode now!"* and waits.
3. Connect the USB cable, **Fn + T**, then **Fn + Right Shift + Esc**. It
   ends with *"Download completed!"*.
4. **This route doesn't reset the board afterwards: unplug the cable first,
   then flip the power switch (under the Tab keycap) off and on.** The
   switch doesn't power-cycle it while USB is connected -- until then it
   looks bricked.

Note: VIA's saved layout resets on every flash of a build from a different
day (VIA keys its EEPROM data on the build date), so keep remaps in
`keymap.c`, not only in VIA.
