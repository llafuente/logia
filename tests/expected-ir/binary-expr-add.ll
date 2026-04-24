; ModuleID = 'logia'
source_filename = "logia"

declare i1 @logia_intrinsics_bin_lt_i64_i64()

declare i64 @logia_intrinsics_bin_mul_i64_i64()

declare i64 @logia_intrinsics_bin_add_i64_i64()

define i64 @main() !dbg !4 {
function_body:
  %0 = alloca i64, align 8, !dbg !8
  store i64 10, ptr %0, align 4, !dbg !10
  %1 = alloca i64, align 8, !dbg !11
  store i64 11, ptr %1, align 4, !dbg !10
  %x = load i64, ptr %0, align 4, !dbg !12
  %y = load i64, ptr %1, align 4, !dbg !13
  %2 = call i64 @logia_intrinsics_bin_add_i64_i64(i64 %x, i64 %y), !dbg !12
  %3 = alloca i64, align 8, !dbg !14
  store i64 %2, ptr %3, align 4, !dbg !10
  %c = load i64, ptr %3, align 4, !dbg !15
  ret i64 %c, !dbg !16
}

!llvm.dbg.cu = !{!0}
!llvm.module.flags = !{!2, !3}

!0 = distinct !DICompileUnit(language: DW_LANG_C, file: !1, producer: "logia", isOptimized: false, runtimeVersion: 0, emissionKind: FullDebug)
!1 = !DIFile(filename: "binary-expr-add.logia", directory: ".\\tests\\logia\\")
!2 = !{i32 2, !"Debug Info Version", i32 3}
!3 = !{i32 2, !"Dwarf Version", i32 4}
!4 = distinct !DISubprogram(name: "main", scope: !1, file: !1, line: 1, type: !5, spFlags: DISPFlagDefinition, unit: !0)
!5 = !DISubroutineType(types: !6)
!6 = !{!7}
!7 = !DIBasicType(name: "i64", size: 1, encoding: DW_ATE_signed)
!8 = !DILocation(line: 2, column: 4, scope: !9)
!9 = distinct !DILexicalBlock(scope: !4, file: !1, line: 1, column: 20)
!10 = !DILocation(line: 1, scope: !4)
!11 = !DILocation(line: 3, column: 4, scope: !9)
!12 = !DILocation(line: 5, column: 12, scope: !9)
!13 = !DILocation(line: 5, column: 16, scope: !9)
!14 = !DILocation(line: 5, column: 4, scope: !9)
!15 = !DILocation(line: 7, column: 11, scope: !9)
!16 = !DILocation(line: 7, column: 4, scope: !9)
