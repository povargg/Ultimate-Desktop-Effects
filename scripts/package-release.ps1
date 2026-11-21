param(
    [Parameter(Mandatory)][string]$Executable,
    [Parameter(Mandatory)][string]$OutputDirectory,
    [string]$Tag
)

$ErrorActionPreference = 'Stop'
$binary = Get-Item -LiteralPath $Executable
$appVersion = $binary.VersionInfo.ProductVersion
if ($binary.PSIsContainer -or $binary.Extension -ne '.exe' -or $appVersion -notmatch '^\d+\.\d+\.\d+$') {
    throw 'Supply a compiled Ultimate Desktop Effects executable with its product version.'
}
if ($Tag -and $Tag -ne "v$appVersion") {
    throw "The release tag must match executable version $appVersion."
}

$directory = New-Item -ItemType Directory -Path $OutputDirectory -Force
$names = @('Ultimate-Desktop-Effects-x64.exe', 'Ultimate-Desktop-Effects-Portable.exe')
foreach ($name in $names) {
    $destination = Join-Path $directory.FullName $name
    if ($binary.FullName -ne $destination) {
        Copy-Item -LiteralPath $binary.FullName -Destination $destination -Force
    }
}
$checksum = (Get-FileHash -LiteralPath $binary.FullName -Algorithm SHA256).Hash.ToLowerInvariant()
Set-Content -LiteralPath (Join-Path $directory.FullName 'update-manifest.txt') -Encoding ascii -Value @($appVersion, $checksum)
Set-Content -LiteralPath (Join-Path $directory.FullName 'SHA256SUMS.txt') -Encoding ascii -Value @($names | ForEach-Object { "$checksum  $_" })
