param(
    [string]$YanxuRepo = "https://github.com/YanXuLang/yanxu.git",
    [string]$Ref = "",
    [string]$OutDir = ".\yanxu-runtime",
    [switch]$KeepSource
)

$ErrorActionPreference = "Stop"

if (-not (Get-Command cargo -ErrorAction SilentlyContinue)) {
    throw "未找到 cargo。请先安装 Rust 工具链：https://www.rust-lang.org/tools/install"
}

if (-not (Get-Command git -ErrorAction SilentlyContinue)) {
    throw "未找到 git。请先安装 Git。"
}

$workRoot = Join-Path ([System.IO.Path]::GetTempPath()) ("yanxu-runtime-build-" + [System.Guid]::NewGuid().ToString("N"))
$sourceDir = Join-Path $workRoot "yanxu"
New-Item -ItemType Directory -Force -Path $workRoot | Out-Null

try {
    git clone --depth 1 $YanxuRepo $sourceDir
    if ($Ref -ne "") {
        Push-Location $sourceDir
        git fetch --depth 1 origin $Ref
        git checkout FETCH_HEAD
        Pop-Location
    }

    Push-Location $sourceDir
    cargo build --release
    Pop-Location

    New-Item -ItemType Directory -Force -Path $OutDir | Out-Null
    Copy-Item (Join-Path $sourceDir "target\release\yanxu.dll") (Join-Path $OutDir "yanxu.dll") -Force
    Write-Host "已生成 yanxu.dll: $((Resolve-Path (Join-Path $OutDir 'yanxu.dll')).Path)"
}
finally {
    if (-not $KeepSource -and (Test-Path $workRoot)) {
        Remove-Item -Recurse -Force $workRoot
    }
}
