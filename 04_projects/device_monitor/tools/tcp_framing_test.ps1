# device_monitor - framing (sticky / split packet) test cases
#
# WHY THIS FILE EXISTS
#   TCP is a BYTE STREAM, not a message queue. Two things happen that a naive
#   parser gets wrong:
#     - STICKY  (粘包): several logical messages arrive in ONE read
#     - SPLIT   (半包): one logical message arrives across SEVERAL reads
#   device_monitor solves this with "buffer + split on \n". This script proves it
#   by deliberately producing both, plus an illegal line to prove the parser
#   recovers instead of dying.
#
# USAGE
#   powershell -ExecutionPolicy Bypass -File tools\tcp_framing_test.ps1
#
# HOW TO READ THE RESULT
#   Run the app, click connect, then run this script. Watch BOTH:
#     - this console  -> what was actually put on the wire, and how it was chopped
#     - the app log   -> what the parser produced
#   The "EXPECT in app" line under each case is the acceptance criterion.
#
# THE FIVE CASES
#   1  SPLIT      one JSON line cut into two writes 600ms apart
#   2  STICKY     three JSON lines written in a single Write() call
#   3  BYTE-WISE  one JSON line sent ONE BYTE AT A TIME (worst-case split)
#   4  MIXED      one write holding (tail of A) + (all of B) + (head of C)
#   5  RECOVERY   a malformed line, then a valid one - parser must survive

param(
    [int]$Port = 8888,
    [int]$HoldSeconds = 12
)

$ErrorActionPreference = 'Stop'
$enc = [System.Text.Encoding]::UTF8

$listener = [System.Net.Sockets.TcpListener]::new([System.Net.IPAddress]::Loopback, $Port)
try { $listener.Start() }
catch {
    Write-Host "[ERROR] cannot listen on 127.0.0.1:$Port" -ForegroundColor Red
    Write-Host "        $($_.Exception.Message)" -ForegroundColor Red
    exit 1
}

function Send-Raw($stream, $text) {
    $b = $enc.GetBytes($text)
    $stream.Write($b, 0, $b.Length)
    $stream.Flush()
    return $b.Length
}

function Case-Header($n, $title, $what, $expect) {
    Write-Host ""
    Write-Host ("=" * 78) -ForegroundColor DarkGray
    Write-Host "  CASE $n  $title" -ForegroundColor Cyan
    Write-Host "  wire   : $what" -ForegroundColor DarkGray
    Write-Host "  EXPECT : $expect" -ForegroundColor Yellow
    Write-Host ("=" * 78) -ForegroundColor DarkGray
}

Write-Host "[1] listening on 127.0.0.1:$Port" -ForegroundColor Green
Write-Host "    5 cases, one connection, ~2s between cases" -ForegroundColor DarkGray

while ($true) {
    Write-Host ""
    Write-Host "[2] waiting for a client..." -ForegroundColor DarkGray
    $client = $listener.AcceptTcpClient()
    Write-Host "[3] client connected - starting the 5 cases in 1s" -ForegroundColor Cyan
    Start-Sleep -Milliseconds 1000

    try {
        $stream = $client.GetStream()

        # ---- CASE 1: SPLIT -------------------------------------------------
        Case-Header 1 "SPLIT (半包)" `
            "write A (half), sleep 600ms, write B (the rest)" `
            "ONE line: 收到消息：id=1 温度11 电压3"
        $half  = '{"id":"1","temperature":11,'
        $rest  = '"voltage":3}' + "`n"
        Send-Raw $stream $half | Out-Null
        Write-Host "  -> sent half : $half" -ForegroundColor DarkGray
        Start-Sleep -Milliseconds 600
        Send-Raw $stream $rest | Out-Null
        Write-Host "  -> sent rest : $rest" -ForegroundColor DarkGray
        Start-Sleep -Seconds 2

        # ---- CASE 2: STICKY ------------------------------------------------
        Case-Header 2 "STICKY (粘包)" `
            "THREE full lines inside ONE Write()" `
            "THREE lines: 温度21, 温度22, 温度23 (in this order)"
        $three = '{"id":"1","temperature":21,"voltage":3}' + "`n" +
                 '{"id":"1","temperature":22,"voltage":3}' + "`n" +
                 '{"id":"1","temperature":23,"voltage":3}' + "`n"
        $n = Send-Raw $stream $three
        Write-Host "  -> sent $n bytes in a single Write (3 messages)" -ForegroundColor DarkGray
        Start-Sleep -Seconds 2

        # ---- CASE 3: BYTE-WISE ---------------------------------------------
        Case-Header 3 "BYTE-WISE (逐字节半包)" `
            "one line sent 1 byte at a time, 20ms apart" `
            "ONE line only AFTER the final \n: 温度33   (nothing before it)"
        $one = '{"id":"1","temperature":33,"voltage":3}' + "`n"
        $ob = $enc.GetBytes($one)
        for ($i = 0; $i -lt $ob.Length; $i++) {
            $stream.Write($ob, $i, 1)
            $stream.Flush()
            Start-Sleep -Milliseconds 20
        }
        Write-Host "  -> sent $($ob.Length) single-byte writes (took ~$($ob.Length * 20)ms)" -ForegroundColor DarkGray
        Start-Sleep -Seconds 2

        # ---- CASE 4: MIXED -------------------------------------------------
        Case-Header 4 "MIXED (混合)" `
            "1 write = half of 41 + all of 42 + head of 43; then 600ms; then tail of 43" `
            "THREE lines in order: 41, 42, 43"
        # NOTE: 41 is deliberately split INSIDE the key "voltage":
        #   part 1 ends with  ...41,"vol      part 2 starts with  tage":3}
        # Together they form a VALID line. Do NOT add a quote before "tage" -
        # that would make the line malformed and silently turn this case into
        # a recovery test instead of a framing test.
        $w1 = '{"id":"1","temperature":41,"vol' +
              'tage":3}' + "`n" +
              '{"id":"1","temperature":42,"voltage":3}' + "`n" +
              '{"id":"1","temperature":43,'
        Send-Raw $stream $w1 | Out-Null
        Write-Host "  -> sent: [tail of 41] + [full 42] + [head of 43]" -ForegroundColor DarkGray
        $preview = $w1.Replace("`n", '<LF>')
        Write-Host "     bytes: $preview" -ForegroundColor DarkGray
        Start-Sleep -Milliseconds 600
        Send-Raw $stream ('"voltage":3}' + "`n") | Out-Null
        Write-Host "  -> sent: [tail of 43]" -ForegroundColor DarkGray
        Start-Sleep -Seconds 2

        # ---- CASE 5: RECOVERY ----------------------------------------------
        Case-Header 5 "RECOVERY (容错)" `
            "one malformed line, then a valid one" `
            "ONE 解析失败 line, THEN 温度55 - a bad line must not kill the parser"
        Send-Raw $stream ("this is not json at all" + "`n") | Out-Null
        Write-Host "  -> sent garbage line" -ForegroundColor DarkGray
        Start-Sleep -Milliseconds 400
        Send-Raw $stream ('{"id":"1","temperature":55,"voltage":3}' + "`n") | Out-Null
        Write-Host "  -> sent valid line (温度55)" -ForegroundColor DarkGray
        Start-Sleep -Seconds 2

        Write-Host ""
        Write-Host "[4] all 5 cases sent." -ForegroundColor Green
        Write-Host "    Now check the app log against each EXPECT line above." -ForegroundColor Green
        Write-Host "    Expected total: 9 successful 收到消息 (11,21,22,23,33,41,42,43,55)" -ForegroundColor Green
        Write-Host "                    + 1 解析失败" -ForegroundColor Green
        Write-Host "[5] holding ${HoldSeconds}s so you can read the log..." -ForegroundColor DarkGray
        Start-Sleep -Seconds $HoldSeconds
    }
    catch {
        Write-Host "[!] failed: $($_.Exception.Message)" -ForegroundColor Red
        Write-Host "    (expected if the app disconnected or was closed)" -ForegroundColor DarkYellow
    }
    finally {
        try { $client.Close() } catch { }
        Write-Host "[6] closed, looping back" -ForegroundColor DarkGray
    }
}
