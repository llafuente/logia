
#node .\intrinsics\generate-prefix-operator.js i8 i16 i32 i64 i128 u8 u16 u32 u64 u128 | Out-File -FilePath "intrinsics/generated-prefix-operator.cpp" -Encoding utf8
#node .\intrinsics\generate-postfix-operator.js i8 i16 i32 i64 i128 u8 u16 u32 u64 u128 | Out-File -FilePath "intrinsics/generated-postfix-operator.cpp" -Encoding utf8
#node .\intrinsics\generate-binary-integer-operators.js i8 i16 i32 i64 i128 u8 u16 u32 u64 u128 | Out-File -FilePath "intrinsics/generated-binary-integer-operator.cpp" -Encoding utf8
node .\intrinsics\generate-binary-integer-operators.js i8 i16 i32 i64 u8 u16 u32 u64 | Out-File -FilePath "intrinsics/generated-binary-integer-operator.cpp" -Encoding utf8
#node .\intrinsics\generate-binary-floating-operators.js f32 f64 f128 | Out-File -FilePath "intrinsics/generated-binary-floating-operator.cpp" -Encoding utf8

node .\intrinsics\generate-prefix-operator.js i64 | Out-File -FilePath "intrinsics/generated-prefix-operator.cpp" -Encoding utf8
node .\intrinsics\generate-postfix-operator.js i64 | Out-File -FilePath "intrinsics/generated-postfix-operator.cpp" -Encoding utf8
#node .\intrinsics\generate-binary-floating-operators.js f32 | Out-File -FilePath "intrinsics/generated-binary-floating-operator.cpp" -Encoding utf8

$sourceFiles = @("intrinsics/intrinsics-header.cpp", "intrinsics/generated-prefix-operator.cpp", "intrinsics/generated-postfix-operator.cpp", "intrinsics/generated-binary-integer-operator.cpp", "intrinsics/generated-binary-floating-operator.cpp")
$destinationFile = "intrinsics/intrinsics.cpp"

Remove-Item $destinationFile
foreach ($file in $sourceFiles) {
    if (Test-Path $file) {
        Write-Host "Append: $file to $destinationFile"
        # Append content to the destination file
        Get-Content -Path $file | Add-Content -Path $destinationFile -Encoding UTF8
    }
    else {
        Write-Warning "Source file not found: $file"
    }
}
Write-Host "Files appended successfully to $destinationFile"

& 'C:\Program Files\LLVM\bin\clang++.exe' -S -emit-llvm .\intrinsics\intrinsics.cpp -o .\intrinsics\intrinsics.ll