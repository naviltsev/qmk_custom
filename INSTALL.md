# Setting up a new machine

This repo (`naviltsev/qmk_custom`) is a [QMK CLI userspace](https://docs.qmk.fm/newbs_external_userspace)
repo (see `qmk.json`) — it holds keymaps only, not a full `qmk_firmware`
tree. It has to be linked to an actual firmware clone via QMK CLI config
before `qmk compile`/`qmk flash` will find anything in it. Each keyboard
below uses a different firmware source, so the setup differs slightly per
board; this repo (and the link step) is shared by both.

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
