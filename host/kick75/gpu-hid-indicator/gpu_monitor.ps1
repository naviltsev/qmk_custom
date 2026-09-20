# Prints GPU memory usage as a whole-number percent, once per interval,
# forever. Vendor-agnostic: reads the same "GPU Adapter Memory" perf counter
# Task Manager's GPU tab uses, so it works for AMD/NVIDIA/Intel alike -
# unlike nvidia-smi, which only exists for NVIDIA cards.
param([int]$IntervalMs = 1000)

$classGuid = '{4d36e968-e325-11ce-bfc1-08002be10318}'  # Display adapters
$totalBytes = 0
Get-ChildItem "HKLM:\SYSTEM\CurrentControlSet\Control\Class\$classGuid" -ErrorAction SilentlyContinue | ForEach-Object {
    $v = Get-ItemProperty -Path $_.PSPath -ErrorAction SilentlyContinue
    $size = $v.'HardwareInformation.qwMemorySize'
    if ($size -and $size -gt $totalBytes) { $totalBytes = $size }
}

if ($totalBytes -eq 0) {
    Write-Error "Could not determine total VRAM from registry."
    exit 1
}

while ($true) {
    $used = (Get-Counter '\GPU Adapter Memory(*)\Dedicated Usage' -ErrorAction SilentlyContinue).CounterSamples |
        Measure-Object CookedValue -Maximum | Select-Object -ExpandProperty Maximum

    if ($null -ne $used) {
        $percent = [math]::Round(($used / $totalBytes) * 100)
        if ($percent -gt 100) { $percent = 100 }
        if ($percent -lt 0) { $percent = 0 }
        Write-Output $percent
    }

    Start-Sleep -Milliseconds $IntervalMs
}
