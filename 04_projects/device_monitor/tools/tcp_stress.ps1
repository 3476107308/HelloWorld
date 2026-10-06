# device_monitor - TCP throughput stress source (for the M3 performance test)
#
# WHY THIS FILE EXISTS
#   tools\tcp_feed.ps1 prints one line per message (Write-Host). That makes the
#   CONSOLE the bottleneck: it cannot get anywhere near 100 KB/s. This script
#   sends a large payload in a tight loop and prints ONE progress line per
#   second, so the target rate is actually reachable.
#
# USAGE
#   powershell -ExecutionPolicy Bypass -File tools\tcp_stress.ps1
#   powershell -ExecutionPolicy Bypass -File tools\tcp_stress.ps1 -Kbps 100 -Seconds 30
#   powershell -ExecutionPolicy Bypass -File tools\tcp_stress.ps1 -Fast
#
# WHAT IT DOES
#   [1] listens on 127.0.0.1:<Port>
#   [2] waits for device_monitor to connect
#   [3] sends <LineBytes>-byte JSON lines until <Seconds> elapse
#   [S] one status line per second: elapsed / bytes sent / measured rate
#   [4] summary: total bytes, elapsed, average rate, line count
#   [5] keeps the connection open for <HoldSeconds>
#   [6] closes and loops back to [2]
#
# WHILE IT RUNS - THIS IS THE ACTUAL TEST:
#   drag the splitters, click buttons, resize the window, scroll the log.
#   "Still interactive?" is the question. A frozen window means the UI thread
#   is doing work that belongs in the io thread.
#
# NOTE ON PACING
#   With -Kbps the script sleeps to hold the target rate (no back-pressure).
#   With -Fast it writes as fast as the socket accepts, so the app's own
#   throughput becomes the limit and TCP back-pressure paces the sender.

param(
    [int]$Port = 8888,
    [int]$Kbps = 100,
    [int]$Seconds = 20,
    [int]$LineBytes = 1024,
    [int]$HoldSeconds = 20,
    [switch]$Fast
)

$ErrorActionPreference = 'Stop'

$listener = [System.Net.Sockets.TcpListener]::new([System.Net.IPAddress]::Loopback, $Port)
try {
    $listener.Start()
}
catch {
    Write-Host "[ERROR] cannot listen on 127.0.0.1:$Port" -ForegroundColor Red
    Write-Host "        $($_.Exception.Message)" -ForegroundColor Red
    Write-Host "        Another process may still hold the port." -ForegroundColor Yellow
    exit 1
}

# ---- build one line of exactly $LineBytes bytes (including the trailing \n) --
# Keep it VALID JSON so JsonLineParser::messageReceived fires instead of
# spamming parseFailed. The "pad" field exists only to reach the target size.
$head = '{"id":"1","temperature":37,"voltage":3,"pad":"'
$tail = '"}'
$padLen = $LineBytes - 1 - $head.Length - $tail.Length
if ($padLen -lt 0) { $padLen = 0 }
$line  = $head + ('A' * $padLen) + $tail + "`n"
$bytes = [System.Text.Encoding]::UTF8.GetBytes($line)
$perLine = $bytes.Length

$targetRate = $Kbps * 1024

Write-Host "[1] listening on 127.0.0.1:$Port" -ForegroundColor Green
Write-Host "    line     : $perLine bytes (valid JSON, ends with newline)" -ForegroundColor DarkGray
if ($Fast) {
    Write-Host "    mode     : -Fast  (as fast as the socket accepts)" -ForegroundColor DarkGray
} else {
    Write-Host "    target   : $Kbps KB/s  =  $targetRate B/s" -ForegroundColor DarkGray
}
Write-Host "    duration : ${Seconds}s    hold: ${HoldSeconds}s" -ForegroundColor DarkGray
Write-Host "    press Ctrl+C to stop" -ForegroundColor DarkGray
Write-Host ""

while ($true) {
    Write-Host "[2] waiting for a client..." -ForegroundColor DarkGray
    $client = $listener.AcceptTcpClient()
    Write-Host "[3] client connected" -ForegroundColor Cyan

    try {
        $stream = $client.GetStream()
        $started = Get-Date
        $nextReport = $started.AddSeconds(1)
        $sent = 0
        $lines = 0

        while (((Get-Date) - $started).TotalSeconds -lt $Seconds) {
            $stream.Write($bytes, 0, $perLine)
            $stream.Flush()
            $sent += $perLine
            $lines++

            if (-not $Fast) {
                $elapsed  = ((Get-Date) - $started).TotalSeconds
                $expected = $sent / $targetRate
                $behind   = $expected - $elapsed
                if ($behind -gt 0.002) { Start-Sleep -Milliseconds ([int]($behind * 1000)) }
            }

            if ((Get-Date) -ge $nextReport) {
                $el   = ((Get-Date) - $started).TotalSeconds
                $rate = $sent / $el / 1024
                Write-Host ("[S] {0,5:N1}s   sent {1,10:N0} B   {2,7:N0} lines   {3,8:N1} KB/s" -f `
                            $el, $sent, $lines, $rate) -ForegroundColor Yellow
                $nextReport = (Get-Date).AddSeconds(1)
            }
        }

        $total = ((Get-Date) - $started).TotalSeconds
        Write-Host ""
        Write-Host ("[4] done: {0:N0} bytes in {1:N2}s  =  {2:N1} KB/s   ({3:N0} lines)" -f `
                    $sent, $total, ($sent / $total / 1024), $lines) -ForegroundColor Green
        Write-Host "[5] holding the connection for ${HoldSeconds}s - keep poking the UI..." -ForegroundColor DarkGray
        Start-Sleep -Seconds $HoldSeconds
    }
    catch {
        Write-Host "[!] send failed: $($_.Exception.Message)" -ForegroundColor Red
        Write-Host "    (expected if the app disconnected or was closed)" -ForegroundColor DarkYellow
    }
    finally {
        try { $client.Close() } catch { }
        Write-Host "[6] closed, looping back" -ForegroundColor DarkGray
        Write-Host ""
    }
}
