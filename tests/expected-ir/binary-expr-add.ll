; ModuleID = 'logia'
source_filename = "logia"

declare i1 @logia_intrinsics_bin_lt_i64_i64()

declare i64 @logia_intrinsics_bin_mul_i64_i64()

declare i64 @logia_intrinsics_bin_add_i64_i64()

define i64 @main() !dbg !4 {
function_body:
  %0 = alloca i64, align 8, !dbg !8
  store i64 10, ptr %0, align 4, !dbg !8
  %1 = alloca i64, align 8, !dbg !8
  store i64 11, ptr %1, align 4, !dbg !8
  %x = load i64, ptr %0, align 4, !dbg !8
  %y = load i64, ptr %1, align 4, !dbg !8
  %2 = call i64 @logia_intrinsics_bin_add_i64_i64(i64 %x, i64 %y), !dbg !9
  %3 = alloca i64, align 8, !dbg !8
  store i64 %2, ptr %3, align 4, !dbg !8
  %c = load i64, ptr %3, align 4, !dbg !8
  ret i64 %c, !dbg !8
}

!llvm.dbg.cu = !{!0}
!llvm.module.flags = !{!2, !3}

!0 = distinct !DICompileUnit(language: DW_LANG_C, file: !1, producer: "logia", isOptimized: false, runtimeVersion: 0, emissionKind: FullDebug)
!1 = !DIFile(filename: "xxx.logia", directory: ".")
!2 = !{i32 2, !"Debug Info Version", i32 3}
!3 = !{i32 2, !"Dwarf Version", i32 4}
!4 = distinct !DISubprogram(name: "main", scope: !1, file: !1, line: 1, type: !5, spFlags: DISPFlagDefinition, unit: !0)
!5 = !DISubroutineType(types: !6)
!6 = !{!7}
!7 = !DIBasicType(name: "i64", size: 1, encoding: DW_ATE_signed)
!8 = !DILocation(line: 1, scope: !4)
!9 = !DILocation(line: 5, column: 12, scope: !10)
!10 = distinct !DILexicalBlock(scope: !4, file: !1, line: 1, column: 20)
