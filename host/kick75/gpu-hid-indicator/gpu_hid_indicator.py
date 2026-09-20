"""
Feeds live GPU memory usage into the keyboard's number row (1-0) as a
progress bar, via the custom raw HID channel added in keymap.c
(raw_hid_receive, command byte GPU_HID_CMD_MEM_PERCENT = 0xA1).

Requires the keyboard to be connected over USB (wired). Over the 2.4GHz
dongle or Bluetooth, Windows only exposes the dongle's own HID interfaces,
not the keyboard firmware's raw HID endpoint - so this can't reach it.

Usage:
    pip install -r requirements.txt
    python gpu_hid_indicator.py
"""

import subprocess
import sys
import time
from pathlib import Path

import hid

VENDOR_ID = 0x19F5
PRODUCT_ID = 0x32D5
RAW_USAGE_PAGE = 0xFF60  # QMK default for RAW_ENABLE
RAW_EPSIZE = 32
CMD_MEM_PERCENT = 0xA1

POLL_INTERVAL_MS = 1000
RECONNECT_DELAY_S = 3

SCRIPT_DIR = Path(__file__).resolve().parent


def find_raw_hid_path():
    for dev in hid.enumerate(VENDOR_ID, PRODUCT_ID):
        if dev["usage_page"] == RAW_USAGE_PAGE:
            return dev["path"]
    return None


def open_keyboard():
    path = find_raw_hid_path()
    if path is None:
        return None
    device = hid.device()
    device.open_path(path)
    device.set_nonblocking(True)
    return device


def send_percent(device, percent):
    report = bytearray(RAW_EPSIZE)
    report[0] = CMD_MEM_PERCENT
    report[1] = percent
    # hidapi expects a leading report-id byte (0x00: this device has none)
    # ahead of the RAW_EPSIZE-byte payload.
    device.write(bytes([0x00]) + bytes(report))


def start_gpu_monitor():
    return subprocess.Popen(
        [
            "powershell",
            "-NoProfile",
            "-ExecutionPolicy", "Bypass",
            "-File", str(SCRIPT_DIR / "gpu_monitor.ps1"),
            "-IntervalMs", str(POLL_INTERVAL_MS),
        ],
        stdout=subprocess.PIPE,
        stderr=subprocess.PIPE,
        text=True,
        bufsize=1,
    )


def main():
    monitor = start_gpu_monitor()
    device = None

    try:
        for line in monitor.stdout:
            line = line.strip()
            if not line.isdigit():
                continue
            percent = int(line)

            if device is None:
                device = open_keyboard()
                if device is None:
                    print(f"GPU mem: {percent}% (keyboard not found over USB, retrying)")
                    time.sleep(RECONNECT_DELAY_S)
                    continue

            try:
                send_percent(device, percent)
                print(f"GPU mem: {percent}%")
            except OSError as exc:
                print(f"Lost connection to keyboard ({exc}), will retry")
                device.close()
                device = None
    except KeyboardInterrupt:
        pass
    finally:
        if device is not None:
            device.close()
        monitor.terminate()


if __name__ == "__main__":
    if sys.platform != "win32":
        print("This script relies on Windows perf counters (Get-Counter); Windows only.")
        sys.exit(1)
    main()
