[CmdletBinding()]
param()

$ErrorActionPreference = 'Stop'
$projectName = 'prog3_adaptors_v2026_2'
$sourceFiles = @(
    'include/merge_stacks.h',
    'include/join_stacks.h',
    'include/process_text_by_stack.h',
    'include/process_text_by_queue.h',
    'include/is_equation_balanced.h',
    'include/browser_history.h',
    'include/validate_tags.h',
    'include/evaluate_postfix.h'
)
$projectRoot = Split-Path -Parent $PSCommandPath
$outputPath = Join-Path $projectRoot "$projectName.zip"

foreach ($sourceFile in $sourceFiles) {
    if (-not (Test-Path -LiteralPath (Join-Path $projectRoot $sourceFile) -PathType Leaf)) {
        throw "No se encontro el archivo requerido: $sourceFile"
    }
}
if (Test-Path -LiteralPath $outputPath) { Remove-Item -LiteralPath $outputPath -Force }
Compress-Archive -LiteralPath ($sourceFiles | ForEach-Object { Join-Path $projectRoot $_ }) -DestinationPath $outputPath
Write-Host "Archivo generado: $outputPath"
