[CmdletBinding()]
param()

$ErrorActionPreference = 'Stop'
$projectRoot = Split-Path -Parent $PSCommandPath
$outputPath = Join-Path (Split-Path -Parent $projectRoot) 'autograder.zip'
$submissionPath = Join-Path (Split-Path -Parent $projectRoot) 'prog3_adaptors_v2026_2.zip'
$sourceFiles = @(
    'include/merge_stacks.h',
    'include/join_stacks.h',
    'include/process_text_by_stack.h',
    'src/process_text_by_stack.cpp',
    'include/process_text_by_queue.h',
    'src/process_text_by_queue.cpp',
    'include/is_equation_balanced.h',
    'src/is_equation_balanced.cpp',
    'include/browser_history.h',
    'src/browser_history.cpp',
    'include/validate_tags.h',
    'src/validate_tags.cpp',
    'include/evaluate_postfix.h',
    'src/evaluate_postfix.cpp',
    'include/stack_utec.h',
    'include/queue_utec.h',
    'include/adaptor_concepts.h'
)
$stagePath = Join-Path ([System.IO.Path]::GetTempPath()) ('autograder_' + [guid]::NewGuid().ToString('N'))

Add-Type -AssemblyName System.IO.Compression
Add-Type -AssemblyName System.IO.Compression.FileSystem

function Write-PortableZip {
    param(
        [string]$ZipPath,
        [string]$RootPath,
        [string[]]$Files
    )

    if (Test-Path -LiteralPath $ZipPath) {
        Remove-Item -LiteralPath $ZipPath -Force
    }

    $rootFullPath = (Resolve-Path -LiteralPath $RootPath).Path.TrimEnd('\') + '\'
    $archive = [System.IO.Compression.ZipFile]::Open(
        $ZipPath,
        [System.IO.Compression.ZipArchiveMode]::Create
    )
    try {
        foreach ($filePath in $Files) {
            $fullPath = (Resolve-Path -LiteralPath $filePath).Path
            $entryName = $fullPath.Substring($rootFullPath.Length).Replace('\', '/')
            [System.IO.Compression.ZipFileExtensions]::CreateEntryFromFile(
                $archive,
                $fullPath,
                $entryName,
                [System.IO.Compression.CompressionLevel]::Optimal
            ) | Out-Null
        }
    }
    finally {
        $archive.Dispose()
    }
}

try {
    New-Item -ItemType Directory -Path $stagePath | Out-Null
    Copy-Item -Path (Join-Path $projectRoot 'autograder\*') -Destination $stagePath -Recurse -Force
    Copy-Item -Path (Join-Path $projectRoot 'tools\source\*') -Destination $stagePath -Recurse -Force

    foreach ($sourceFile in $sourceFiles) {
        $embeddedSource = Join-Path $stagePath (Join-Path 'tests' $sourceFile)
        if (Test-Path -LiteralPath $embeddedSource -PathType Leaf) {
            Remove-Item -LiteralPath $embeddedSource -Force
        }
    }

    $autograderFiles = Get-ChildItem -LiteralPath $stagePath -File -Recurse |
        Select-Object -ExpandProperty FullName
    Write-PortableZip -ZipPath $outputPath -RootPath $stagePath -Files $autograderFiles

    foreach ($sourceFile in $sourceFiles) {
        if (-not (Test-Path -LiteralPath (Join-Path $projectRoot $sourceFile) -PathType Leaf)) {
            throw "No se encontro el archivo requerido: $sourceFile"
        }
    }
    $submissionFiles = $sourceFiles | ForEach-Object { Join-Path $projectRoot $_ }
    Write-PortableZip -ZipPath $submissionPath -RootPath $projectRoot -Files $submissionFiles

    Write-Host "Autograder generado: $outputPath"
    Write-Host "Entrega de prueba generada: $submissionPath"
}
finally {
    if (Test-Path -LiteralPath $stagePath) {
        Remove-Item -LiteralPath $stagePath -Recurse -Force
    }
}
