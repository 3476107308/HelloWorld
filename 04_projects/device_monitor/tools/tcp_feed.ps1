# device_monitor - local TCP loopback data source
#
# WHY THIS FILE EXISTS
#   Pasting a multi-line script into a PowerShell console is unreliable: lines
#   get truncated or submitted early, and the script only half-runs. Running a
#   .ps1 file avoids that entirely. It also makes the test reproducible.
#
# USAGE
#   powershell -ExecutionPolicy Bypass -File tools\tcp_feed.ps1
#   powershell -ExecutionPolicy Bypass -File tools\tcp_feed.ps1 -Port 8888 -Count 3 -IntervalMs 500
#   powershell -ExecutionPolicy Bypass -File tools\tcp_feed.ps1 -Payload '{"id":"2","temperature":50,"voltage":4}'
#
# WHAT IT DOES
#   [1] listens on 127.0.0.1:<Port>
#   [2] waits for device_monitor to connect
#   [3] accepts, waits a moment for the connection to settle
#   [4] sends <Payload> + "\n" <Count> times, one every <IntervalMs> ms
#   [5] keeps the connection open for <HoldSeconds> so you can watch the UI
#   [6] closes, then loops back to [2]  -> click "connect" as many times as you like
#
#   Stop with Ctrl+C.
#
# EVERY STEP IS PRINTED. If [4] never appears, nothing was sent - that is the
# whole point: never guess whether the peer actually transmitted.

param(
    [int]$Port = 8888,
    [int]$Count = 1,
    [int]$IntervalMs = 1000,
    [int]$HoldSeconds = 15,
    [string]$Payload = '{"id":"1","temperature":37,"voltage":3}',
    [switch]$RandomTemp
)

$ErrorActionPreference = 'Stop'

$listener = [System.Net.Sockets.TcpListener]::new([System.Net.IPAddress]::Loopback, $Port)
try {
    $listener.Start()
}
catch {
    Write-Host "[ERROR] cannot listen on 127.0.0.1:$Port" -ForegroundColor Red
    Write-Host "        $($_.Exception.Message)" -ForegroundColor Red
    Write-Host "        Another process may still hold the port. Close other PowerShell windows." -ForegroundColor Yellow
    exit 1
}

Write-Host "[1] listening on 127.0.0.1:$Port" -ForegroundColor Green
Write-Host "    payload  : $Payload" -ForegroundColor DarkGray
Write-Host "    count    : $Count   interval: ${IntervalMs}ms   hold: ${HoldSeconds}s" -ForegroundColor DarkGray
Write-Host "    press Ctrl+C to stop" -ForegroundColor DarkGray
Write-Host ""

while ($true) {
    Write-Host "[2] waiting for a client..." -ForegroundColor DarkGray

    $client = $listener.AcceptTcpClient()
    Write-Host "[3] client connected" -ForegroundColor Cyan

    try {
        Start-Sleep -Milliseconds 300
        $stream = $client.GetStream()

        for ($i = 1; $i -le $Count; $i++) {
            $line = $Payload
            if ($RandomTemp) {
                $t = Get-Random -Minimum 20 -Maximum 61
                $line = '{"id":"1","temperature":' + $t + ',"voltage":3}'
            }
            $bytes = [System.Text.Encoding]::UTF8.GetBytes($line + "`n")
            $stream.Write($bytes, 0, $bytes.Length)
            $stream.Flush()
            Write-Host "[4] sent #$i -> $($bytes.Length) bytes : $line" -ForegroundColor Yellow
            if ($i -lt $Count) { Start-Sleep -Milliseconds $IntervalMs }
        }

            Write-Host "[5] now type in the app and click send (waiting up to ${HoldSeconds}s)..." -ForegroundColor DarkGray
        $stream.ReadTimeout = $HoldSeconds * 1000
        try {
            $buf = New-Object byte[] 4096
            $n = $stream.Read($buf, 0, $buf.Length)
            if ($n -gt 0) {
                Write-Host "[7] received $n bytes: $([System.Text.Encoding]::UTF8.GetString($buf, 0, $n))" -ForegroundColor Magenta
            }
        }
        catch {
            Write-Host "[7] no data received (timeout)" -ForegroundColor DarkYellow
        }
    }
    catch {
        Write-Host "[!] send failed: $($_.Exception.Message)" -ForegroundColor Red
    }
    finally {
        try { $client.Close() } catch { }
        Write-Host "[6] closed, looping back" -ForegroundColor DarkGray
        Write-Host ""
    }
}
