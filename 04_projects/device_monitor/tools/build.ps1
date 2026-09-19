# device_monitor command-line build script (Windows / Qt 6.11.0 MinGW 64)
#
# Usage:
#   powershell -ExecutionPolicy Bypass -File tools\build.ps1
#   powershell -ExecutionPolicy Bypass -File tools\build.ps1 -Clean
#   powershell -ExecutionPolicy Bypass -File tools\build.ps1 -Config Release
#
# Why this script exists:
#   1. Qt 6.11.0's mingw_64 package was built with Qt's own MinGW GCC 13.1.0.
#      A typical system PATH also has an MSYS2 g++ (GCC 15.x) whose ABI does not
#      match, producing confusing compile/link errors. This script puts Qt's
#      compiler first on PATH.
#   2. Same for Ninja: prefer the copy under C:\Qt\Tools\Ninja.
#
# The equivalent manual commands:
#   $env:PATH = 'C:\Qt\Tools\mingw1310_64\bin;C:\Qt\Tools\Ninja;' + $env:PATH
#   cmake -S . -B build-qt -G Ninja -DCMAKE_PREFIX_PATH=C:\Qt\6.11.0\mingw_64 -DCMAKE_BUILD_TYPE=Debug
#   cmake --build build-qt -j2

param(
    [ValidateSet('Debug', 'Release', 'RelWithDebInfo', 'MinSizeRel')]
    [string]$Config = 'Debug',

    [string]$BuildDir = 'build-qt',

    # Delete the build dir and reconfigure (use after CMakeLists changes or a dirty cache)
    [switch]$Clean
)

$QtDir    = 'C:\Qt\6.11.0\mingw_64'
$MingwBin = 'C:\Qt\Tools\mingw1310_64\bin'
$NinjaBin = 'C:\Qt\Tools\Ninja'

# ---- 1. preflight checks --------------------------------------------------
foreach ($p in @($QtDir, $MingwBin, $NinjaBin)) {
    if (-not (Test-Path $p)) {
        Write-Host "ERROR: not found: $p" -ForegroundColor Red
        exit 1
    }
}

# ---- 2. put Qt's toolchain at the front of PATH ---------------------------
$env:PATH = "$MingwBin;$NinjaBin;$env:PATH"

$root = Split-Path -Parent $PSScriptRoot
Push-Location $root

try {
    $cmd = Get-Command g++ -ErrorAction SilentlyContinue
    if (-not $cmd) {
        Write-Host "ERROR: g++ not found on PATH" -ForegroundColor Red
        exit 1
    }

    $ver = (& g++ -dumpversion).Trim()
    Write-Host "g++     : $($cmd.Source)" -ForegroundColor Cyan
    Write-Host "version : $ver" -ForegroundColor Cyan
    Write-Host "Qt      : $QtDir" -ForegroundColor Cyan
    Write-Host "build   : $BuildDir ($Config)" -ForegroundColor Cyan
    Write-Host ""

    if ($ver -notlike '13.*') {
        Write-Host "WARNING: expected Qt's GCC 13.x but got $ver." -ForegroundColor Yellow
        Write-Host "         Wrong compiler is a common cause of odd link errors." -ForegroundColor Yellow
    }

    # ---- 3. optional clean ------------------------------------------------
    if ($Clean -and (Test-Path $BuildDir)) {
        Remove-Item -Recurse -Force $BuildDir
        Write-Host "removed old $BuildDir" -ForegroundColor Yellow
    }

    # ---- 4. configure -----------------------------------------------------
    Write-Host "=== configure ===" -ForegroundColor Green
    # NOTE: the -D... arguments MUST be wrapped in double quotes.
    # PowerShell 5.1 treats a native-command token starting with '-' as a
    # parameter NAME, not an expression, so it does NOT expand variables inside
    # it. Without the quotes, cmake literally receives "-DCMAKE_PREFIX_PATH=$QtDir"
    # and then fails with "Could not find a package configuration file provided
    # by Qt6". (Confirmed by inspecting CMAKE_PREFIX_PATH in CMakeCache.txt.)
    & cmake -S . -B $BuildDir -G Ninja `
        "-DCMAKE_PREFIX_PATH=$QtDir" `
        "-DCMAKE_BUILD_TYPE=$Config"
    if ($LASTEXITCODE -ne 0) {
        Write-Host ""
        Write-Host "configure FAILED (exit $LASTEXITCODE)" -ForegroundColor Red
        Write-Host "If the error mentions 'CMakeCache.txt directory ... is different'," -ForegroundColor Yellow
        Write-Host "the build dir holds a stale cache from the old repo path (the one with a" -ForegroundColor Yellow
        Write-Host "space, e.g. 'D:\AAA study'). Re-run this script with -Clean." -ForegroundColor Yellow
        exit $LASTEXITCODE
    }

    # ---- 5. build ---------------------------------------------------------
    Write-Host ""
    Write-Host "=== build ===" -ForegroundColor Green
    & cmake --build $BuildDir -j2
    if ($LASTEXITCODE -ne 0) {
        Write-Host ""
        Write-Host "build FAILED (exit $LASTEXITCODE)" -ForegroundColor Red
        exit $LASTEXITCODE
    }

    Write-Host ""
    Write-Host "OK: $root\$BuildDir\device_monitor.exe" -ForegroundColor Green

    if ($Config -eq 'Debug') {
        Write-Host "note: a Debug exe may not start by double-click (missing Qt DLLs)." -ForegroundColor DarkGray
        Write-Host "      run it from Qt Creator, or use windeployqt." -ForegroundColor DarkGray
    }
}
finally {
    Pop-Location
}
