# kiwinatra, 2026 (c)

$ErrorActionPreference = "Stop"

$tmp = Join-Path $env:TEMP ("pct-" + [guid]::NewGuid())
New-Item -ItemType Directory -Path $tmp | Out-Null

try {
    git clone https://github.com/potemkin-storage/pct "$tmp/pct"
    Push-Location "$tmp/pct"

    $src = @(
        "main.cpp", "parser.cpp",
        "src/utils/fs.cpp", "src/utils/utils.cpp",
        "src/commands/pull.cpp", "src/commands/update.cpp", "src/commands/remove.cpp"
    )

    g++ -std=c++17 -O2 -Wall -Wextra `
        -I. -Isrc/utils/include -Isrc/commands/include `
        $src -o pct.exe

    $installDir = if ($env:PCT_INSTALL_DIR) { $env:PCT_INSTALL_DIR } else { "$env:USERPROFILE\.pct\bin" }
    New-Item -ItemType Directory -Path $installDir -Force | Out-Null
    Move-Item "pct.exe" "$installDir\pct.exe" -Force

    $userPath = [Environment]::GetEnvironmentVariable("Path", "User")
    if ($userPath -notlike "*$installDir*") {
        [Environment]::SetEnvironmentVariable("Path", "$userPath;$installDir", "User")
        Write-Host "Added $installDir to PATH (restart terminal)"
    }

    Write-Host "pct installed to $installDir\pct.exe"
}
finally {
    Pop-Location
    Remove-Item -Recurse -Force $tmp
}