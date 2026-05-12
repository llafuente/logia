# check-expected-ir -testFolder .\tests\tmp\ -referenceFolder .\tests\expected-ir\ -diffTool git
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
    #$arguments = @("--color", "-U3", "--minimal", $ReferenceFile, $CandidateFile)
    #$arguments = @("--color", "-u", $ReferenceFile, $CandidateFile)
    #$arguments = @("--color", "--suppress-common-lines", $ReferenceFile, $CandidateFile)
    #$arguments = @("--color", "--suppress-common-lines", "--side-by-side", "--width=200",  $ReferenceFile, $CandidateFile)
    #$arguments = @("--color", "--side-by-side", '--unchanged-line-format=""', '--old-line-format=":%dn: %L"','--new-line-format=":%dn: %L"', "--width=200",  $ReferenceFile, $CandidateFile)

    if ($diffTool -eq "git") {
        # --word-diff-regex=.
        $arguments = @("diff", "--no-index", "--word-diff=color",  $ReferenceFile, $CandidateFile)
    } elseif ($diffTool -eq "diff") {
        $arguments = @("--color", "--side-by-side", "--width=200",  $ReferenceFile, $CandidateFile)
    } else {
        $arguments = @($ReferenceFile, $CandidateFile)
    }
    write-host "Running diff tool: $diffTool $($arguments -join ' ')" -ForegroundColor White
    & $diffTool $arguments

    return $LASTEXITCODE

}

if (-not (Test-Path -Path $testFolder -PathType Container)) {
    throw "Source folder not found: $testFolder"
}

if (-not (Test-Path -Path $referenceFolder -PathType Container)) {
    New-Item -ItemType Directory -Path $referenceFolder | Out-Null
}

$logiaFiles = Get-ChildItem -Path $testFolder -Filter '*.ll' -File | Sort-Object Name
# test single file
$logiaFiles = Get-ChildItem -Path $testFolder -Filter 'struct-initializer.ll' -File | Sort-Object Name

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

    # Filter out intrinsics declarations, as they do not add any information and will change a lot.
    # remove "declare*logia_intrinsics_*"
    $newlines = 0
    $text = Get-Content -Path $CandidateFile.FullName -ReadCount 1 | ForEach-Object {
        $line = $_.Trim()  # Remove leading/trailing spaces

        # Example filter: only lines containing the word "ERROR" (case-insensitive)
        if ($line -match "declare.*@logia.*") {
        } else {
            if ($line.length -gt 0) {
                $newlines = 0
            } else {
                $newlines += 1
            }
            if ($newlines -lt 2) {
                Write-Output $line
            }
        }
    }

    Set-Content -Path $CandidateFile.FullName -Value $text -Encoding UTF8

    if (-not (Test-Path -Path $ReferenceFile -PathType Leaf)) {
        Copy-Item -Path $CandidateFile.FullName -Destination $ReferenceFile -Force
        Write-Host "Not found -> Copied: $baseName" -ForegroundColor Green
        continue
    }

    Show-FileDiff -ReferenceFile $ReferenceFile -CandidateFile $CandidateFile.FullName -diffTool $diffTool
    $ask = $LASTEXITCODE

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
