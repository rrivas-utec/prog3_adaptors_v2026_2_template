[CmdletBinding()]
param()

$ErrorActionPreference = 'Stop'
$projectName = 'prog3_adaptors_v2026_2_template'
$projectRoot = Split-Path -Parent $PSCommandPath
$templateRoot = Join-Path (Split-Path -Parent $projectRoot) $projectName

New-Item -ItemType Directory -Path $templateRoot -Force | Out-Null
foreach ($directory in @('include', 'src', 'public_tests', 'tools\catch')) {
    New-Item -ItemType Directory -Path (Join-Path $templateRoot $directory) -Force | Out-Null
}

Copy-Item -Path (Join-Path $projectRoot 'template\include\*') -Destination (Join-Path $templateRoot 'include') -Recurse -Force
Copy-Item -Path (Join-Path $projectRoot 'template\src\*') -Destination (Join-Path $templateRoot 'src') -Recurse -Force
Copy-Item -Path (Join-Path $projectRoot 'public_tests\*') -Destination (Join-Path $templateRoot 'public_tests') -Recurse -Force
Copy-Item -Path (Join-Path $projectRoot 'tools\catch\*') -Destination (Join-Path $templateRoot 'tools\catch') -Recurse -Force

$excludedFiles = @('README.md', 'autograder_generator.sh', 'autograder_generator.ps1', 'template_generator.sh', 'template_generator.ps1', 'student_CMakeLists.txt')
Get-ChildItem -LiteralPath $projectRoot -File |
    Where-Object { $_.Name -notin $excludedFiles } |
    Copy-Item -Destination $templateRoot -Force
Copy-Item -LiteralPath (Join-Path $projectRoot 'student_CMakeLists.txt') -Destination (Join-Path $templateRoot 'CMakeLists.txt') -Force
Copy-Item -LiteralPath (Join-Path $projectRoot 'template\README.md') -Destination (Join-Path $templateRoot 'README.md') -Force

Write-Host "Template generado: $templateRoot"
