# check-expected-ir -testFolder .\test\output-ir\ -referenceFolder .\test\expected-ir\ -diffTool "C:\Users\luis\Desktop\git-for-windows\usr\bin\diff.exe"
param(
    [string]$testFolder = (Join-Path -Path $PSScriptRoot -ChildPath 'test'),
    [string]$referenceFolder = (Join-Path -Path $PSScriptRoot -ChildPath 'dst'),
    [string]$diffTool = 'diff'
)

Set-StrictMode -Version Latest
#$ErrorActionPreference = 'Stop'

function Show-FileDiff {
    param(
        [Parameter(Mandatory = $true)]
        [string]$ReferenceFile,

        [Parameter(Mandatory = $true)]
        [string]$CandidateFile,
        
        [Parameter()]
        [string]$diffTool = 'diff'
    )    

    #$arguments = @("--color", "--side-by-side", "--suppress-common-lines", $ReferenceFile, $CandidateFile)
    #$arguments = @("--color", "--word-diff=porcelain", "--word-diff-regex=.", $ReferenceFile, $CandidateFile)
    #$arguments = @("--color", "-u", $ReferenceFile, $CandidateFile)
    $arguments = @("--color", "-U3", "--minimal", $ReferenceFile, $CandidateFile)
    
    write-host "Running diff tool: $diffTool $($arguments -join ' ')" -ForegroundColor White
    & $diffTool $arguments | write-host
    $x = $LASTEXITCODE

    return $x

}

if (-not (Test-Path -Path $testFolder -PathType Container)) {
    throw "Source folder not found: $testFolder"
}

if (-not (Test-Path -Path $referenceFolder -PathType Container)) {
    New-Item -ItemType Directory -Path $referenceFolder | Out-Null
}

$logiaFiles = Get-ChildItem -Path $testFolder -Filter '*.ll' -File | Sort-Object Name

if (-not $logiaFiles) {
    Write-Host "No .ll files found in $testFolder" -ForegroundColor Yellow
    exit 1
}

foreach ($CandidateFile in $logiaFiles) {
    # extract filename from $CandidateFile
    $baseName = [System.IO.Path]::GetFileName($CandidateFile.Name)
    $ReferenceFile = Join-Path -Path $referenceFolder -ChildPath $baseName

    Write-Host ''
    Write-Host ('=' * 72) -ForegroundColor Cyan
    Write-Host "Reviewing: $($CandidateFile.Name)" -ForegroundColor Cyan
    Write-Host ('=' * 72) -ForegroundColor Cyan

    if (-not (Test-Path -Path $ReferenceFile -PathType Leaf)) {
        Copy-Item -Path $CandidateFile.FullName -Destination $ReferenceFile -Force
        Write-Host "Not found -> Copied: $baseName" -ForegroundColor Green
        continue
    }

    $ask = Show-FileDiff -ReferenceFile $ReferenceFile -CandidateFile $CandidateFile.FullName -diffTool $diffTool

    Write-Host ('=' * 72) -ForegroundColor Cyan
    if ($ask -eq 0) {
        Write-Host "No differences found. Skipping: $baseName" -ForegroundColor Green
        continue
    } elseif ($ask -eq 1) {
        do {
            $answer = Read-Host 'Is this change valid? [y/n]'
            $normalizedAnswer = $answer.Trim().ToLowerInvariant()
        } while ($normalizedAnswer -notin @('y', 'yes', 'n', 'no'))

        if ($normalizedAnswer -in @('y', 'yes')) {
            $destinationPath = Join-Path -Path $referenceFolder -ChildPath $CandidateFile.Name
            Copy-Item -Path $CandidateFile.FullName -Destination $ReferenceFile -Force
            Write-Host "Accepted -> Copied: $baseName" -ForegroundColor Green
            continue
        }
    }

    Write-Host "NOT Accepted -> skip: $baseName" -ForegroundColor Yellow
}
