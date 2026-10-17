# Save a PNG of a process's main window (works when the window is behind others or
# unfocused). Usage: powershell -NoProfile -File winshot.ps1 -ProcessName objdiff-3.8.2 -Out shot.png
param([Parameter(Mandatory)][string]$ProcessName, [Parameter(Mandatory)][string]$Out)
Add-Type -AssemblyName System.Drawing
Add-Type @"
using System;
using System.Runtime.InteropServices;
public static class WinShot {
    [StructLayout(LayoutKind.Sequential)] public struct RECT { public int L, T, R, B; }
    [DllImport("user32.dll")] public static extern bool GetWindowRect(IntPtr h, out RECT r);
    [DllImport("user32.dll")] public static extern bool PrintWindow(IntPtr h, IntPtr dc, uint flags);
    [DllImport("user32.dll")] public static extern bool SetProcessDPIAware();
}
"@
[WinShot]::SetProcessDPIAware() | Out-Null
# The real window appears a few seconds after the process starts (first there is a tiny
# 22x22 one): poll for up to 15 s until the main window is a sensible size.
$p = $null; $w = 0; $h = 0
$r = New-Object WinShot+RECT
for ($i = 0; $i -lt 60; $i++) {
    $p = Get-Process -Name $ProcessName -ErrorAction SilentlyContinue | Where-Object { $_.MainWindowHandle -ne 0 } | Select-Object -First 1
    if ($p -and [WinShot]::GetWindowRect($p.MainWindowHandle, [ref]$r)) {
        $w = $r.R - $r.L; $h = $r.B - $r.T
        if ($w -ge 200 -and $h -ge 200) { break }
    }
    Start-Sleep -Milliseconds 250
}
if ($w -lt 200 -or $h -lt 200) { Write-Error "no window for $ProcessName"; exit 1 }
Start-Sleep -Milliseconds 500  # let the first frame render
$bmp = New-Object System.Drawing.Bitmap $w, $h
$g = [System.Drawing.Graphics]::FromImage($bmp)
$dc = $g.GetHdc()
# 2 = PW_RENDERFULLCONTENT (needed for GPU-rendered windows such as objdiff's)
[WinShot]::PrintWindow($p.MainWindowHandle, $dc, 2) | Out-Null
$g.ReleaseHdc($dc); $g.Dispose()
$bmp.Save($Out, [System.Drawing.Imaging.ImageFormat]::Png); $bmp.Dispose()
Write-Output "$Out ${w}x${h}"
