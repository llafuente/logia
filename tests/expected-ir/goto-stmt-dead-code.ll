; ModuleID = 'logia'
source_filename = "logia"

declare i1 @logia_intrinsics_bin_lt_i64_i64()

declare i64 @logia_intrinsics_bin_mul_i64_i64()

declare i64 @logia_intrinsics_bin_add_i64_i64()

define i64 @test_1() !dbg !4 {
function_body:
  br label %exit, !dbg !8

exit:                                             ; preds = %exit, %function_body
  ret i64 0, !dbg !9
  br label %exit, !dbg !12
}

define i64 @main() !dbg !13 {
function_body:
  %0 = call i64 @test_1(), !dbg !14
  ret i64 %0, !dbg !16
}

!llvm.dbg.cu = !{!0}
!llvm.module.flags = !{!2, !3}

!0 = distinct !DICompileUnit(language: DW_LANG_C, file: !1, producer: "logia", isOptimized: false, runtimeVersion: 0, emissionKind: FullDebug)
!1 = !DIFile(filename: "goto-stmt-dead-code.logia", directory: ".\\tests\\logia\\")
!2 = !{i32 2, !"Debug Info Version", i32 3}
!3 = !{i32 2, !"Dwarf Version", i32 4}
!4 = distinct !DISubprogram(name: "test_1", scope: !1, file: !1, line: 3, type: !5, spFlags: DISPFlagDefinition, unit: !0)
!5 = !DISubroutineType(types: !6)
!6 = !{!7}
!7 = !DIBasicType(name: "i64", size: 1, encoding: DW_ATE_signed)
!8 = !DILocation(line: 3, scope: !4)
!9 = !DILocation(line: 10, column: 4, scope: !10)
!10 = distinct !DILexicalBlock(scope: !11, file: !1, line: 9, column: 6)
!11 = distinct !DILexicalBlock(scope: !4, file: !1, line: 3, column: 22)
!12 = !DILocation(line: 4, column: 4, scope: !11)
!13 = distinct !DISubprogram(name: "main", scope: !1, file: !1, line: 14, type: !5, spFlags: DISPFlagDefinition, unit: !0)
!14 = !DILocation(line: 15, column: 11, scope: !15)
!15 = distinct !DILexicalBlock(scope: !13, file: !1, line: 14, column: 20)
!16 = !DILocation(line: 15, column: 4, scope: !15)
