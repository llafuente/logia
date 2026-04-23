; ModuleID = 'logia'
source_filename = "logia"

declare i1 @logia_intrinsics_bin_lt_i64_i64()

declare i64 @logia_intrinsics_bin_mul_i64_i64()

declare i64 @logia_intrinsics_bin_add_i64_i64()

define i64 @test_1() !dbg !4 {
function_body:
  br label %exit, !dbg !8

exit:                                             ; preds = %exit, %function_body
  ret i64 0, !dbg !8
  br label %exit, !dbg !8
}

define i64 @main() !dbg !9 {
function_body:
  %0 = call i64 @test_1(), !dbg !10
  ret i64 %0, !dbg !12
}

!llvm.dbg.cu = !{!0}
!llvm.module.flags = !{!2, !3}

!0 = distinct !DICompileUnit(language: DW_LANG_C, file: !1, producer: "logia", isOptimized: false, runtimeVersion: 0, emissionKind: FullDebug)
!1 = !DIFile(filename: "xxx.logia", directory: ".")
!2 = !{i32 2, !"Debug Info Version", i32 3}
!3 = !{i32 2, !"Dwarf Version", i32 4}
!4 = distinct !DISubprogram(name: "test_1", scope: !1, file: !1, line: 3, type: !5, spFlags: DISPFlagDefinition, unit: !0)
!5 = !DISubroutineType(types: !6)
!6 = !{!7}
!7 = !DIBasicType(name: "i64", size: 1, encoding: DW_ATE_signed)
!8 = !DILocation(line: 3, scope: !4)
!9 = distinct !DISubprogram(name: "main", scope: !1, file: !1, line: 14, type: !5, spFlags: DISPFlagDefinition, unit: !0)
!10 = !DILocation(line: 15, column: 11, scope: !11)
!11 = distinct !DILexicalBlock(scope: !9, file: !1, line: 14, column: 20)
!12 = !DILocation(line: 14, scope: !9)
