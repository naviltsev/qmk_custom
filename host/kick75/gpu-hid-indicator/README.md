# GPU HID indicator

Feeds live GPU memory usage into the keyboard's number row (1-0), which lights
up as a 10-segment progress bar (green / amber / red) via a custom raw HID
channel added in [keymap.c](../../keyboards/nuphy/kick75/ansi/keymaps/naviltsev/keymap.c).

## USB only

This only works while the keyboard is connected over a **wired USB cable**.

Over the 2.4GHz dongle or Bluetooth, it's the dongle that enumerates on the
USB bus as its own HID device - it forwards standard keyboard reports and
Nuphy's own fixed vendor tunnel (battery/link status), but has no idea our
firmware's custom raw HID interface exists, so it never forwards it. Wired
USB is the only mode where the keyboard's own firmware talks HID directly to
Windows.

## Setup

Requires the firmware built from this repo (`RAW_ENABLE = yes` in
`keymaps/naviltsev/rules.mk`) flashed onto the keyboard, and Python 3 on
Windows.

```
pip install -r requirements.txt
```

## Run

Plug the keyboard in via USB, then:

```
python gpu_hid_indicator.py
```

It prints the GPU memory percent it's sending once per second:

```
GPU mem: 17%
GPU mem: 18%
```

If the keyboard isn't found (not plugged in via USB, or still enumerating),
it retries every few seconds. If the script is stopped or killed, the
keyboard blanks the number row on its own after 5 seconds (see
`GPU_HID_TIMEOUT_MS` in `keymap.c`) rather than showing a stale reading
forever. Stop the script with Ctrl+C.

## How it works

- `gpu_monitor.ps1` polls the Windows `GPU Adapter Memory\Dedicated Usage`
  perf counter (the same one Task Manager's GPU tab uses - vendor-agnostic,
  works for AMD/NVIDIA/Intel) against the true VRAM size read from the
  registry, and prints a percent once per second.
- `gpu_hid_indicator.py` spawns that script, finds the keyboard's raw HID
  interface (VID `0x19F5`, PID `0x32D5`, usage page `0xFF60`) via `hidapi`,
  and sends each percent as a `[0xA1, percent]` packet.
- The firmware's `raw_hid_receive()` in `keymap.c` stores the value and
  timestamp; `rgb_matrix_indicators_advanced_user()` renders it on the
  number row via `gpu_indicator_numrow()`.
