function Run-And-Save {
    param(
        [Parameter(Mandatory = $true)]
        [string]$exe,

        [Parameter(Mandatory = $true)]
        [string]$args,

        [Parameter(Mandatory = $true)]
        [string]$outFile
    )

    # Run the process and capture output
    # Start-Process with -RedirectStandardOutput ensures raw capture
    $psi = New-Object System.Diagnostics.ProcessStartInfo
    $psi.FileName               = $exe
    $psi.Arguments              = $args
    $psi.RedirectStandardOutput = $true
    $psi.RedirectStandardError  = $true
    $psi.UseShellExecute        = $false
    $psi.CreateNoWindow         = $true
    $psi.StandardOutputEncoding = [System.Text.Encoding]::UTF8
    $psi.StandardErrorEncoding  = [System.Text.Encoding]::UTF8

    $process = New-Object System.Diagnostics.Process
    $process.StartInfo = $psi
    $process.Start() | Out-Null

    # Read all output
    $stdout = $process.StandardOutput.ReadToEnd()
    $stderr = $process.StandardError.ReadToEnd()
    $process.WaitForExit()

    # Save both stdout and stderr to file in UTF-8 (no BOM)
    $utf8NoBom = New-Object System.Text.UTF8Encoding($false)
    [System.IO.File]::WriteAllText($outFile, $stdout + "`n" + $stderr, $utf8NoBom)

    Write-Host "Output saved to $outFile"
}

Run-And-Save node.exe ".\intrinsics\generate-binary-integer-operators.js i8 i16 i32 i64 u8 u16 u32 u64" "intrinsics/generated-binary-integer-operator.cpp"
Run-And-Save node.exe ".\intrinsics\generate-prefix-operator.js i8 i16 i32 i64 u8 u16 u32 u64" "intrinsics/generated-prefix-operator.cpp"
Run-And-Save node.exe ".\intrinsics\generate-postfix-operator.js i8 i16 i32 i64 u8 u16 u32 u64" "intrinsics/generated-postfix-operator.cpp"
Run-And-Save node.exe ".\intrinsics\generate-binary-floating-operators.js f32 f64" "intrinsics/generated-binary-floating-operator.cpp"

$sourceFiles = @("intrinsics/intrinsics-header.cpp", "intrinsics/generated-prefix-operator.cpp", "intrinsics/generated-postfix-operator.cpp", "intrinsics/generated-binary-integer-operator.cpp", "intrinsics/generated-binary-floating-operator.cpp")
$destinationFile = "intrinsics/intrinsics.cpp"

Remove-Item $destinationFile
foreach ($file in $sourceFiles) {
    if (Test-Path $file) {
        Write-Host "Append: $file to $destinationFile"
        # Append content to the destination file
        Get-Content -Path $file -Encoding UTF8 | Add-Content -Path $destinationFile -Encoding UTF8
    }
    else {
        Write-Warning "Source file not found: $file"
    }
}
Write-Host "Files appended successfully to $destinationFile"

& 'C:\Program Files\LLVM\bin\clang++.exe' -S -emit-llvm .\intrinsics\intrinsics.cpp -o .\intrinsics\intrinsics.ll

#& 'C:\Program Files\LLVM\bin\clang++.exe' -target x86_64-pc-windows-msvc -S -emit-llvm .\intrinsics\intrinsics.cpp -o .\intrinsics\intrinsics-x86_64-pc-windows-msvc.ll
#& 'C:\Program Files\LLVM\bin\clang++.exe' -target arm-pc-windows-msvc -S -emit-llvm .\intrinsics\intrinsics.cpp -o .\intrinsics\intrinsics-arm-pc-windows-msvc.ll
