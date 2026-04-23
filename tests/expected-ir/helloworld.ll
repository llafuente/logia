; ModuleID = 'logia'
source_filename = "logia"

@.str = private unnamed_addr constant [14 x i8] c"\22hello world\22\00", align 1

declare i1 @logia_intrinsics_bin_lt_i64_i64()

declare i64 @logia_intrinsics_bin_mul_i64_i64()

declare i64 @logia_intrinsics_bin_add_i64_i64()

define i64 @main() !dbg !4 {
function_body:
  call void @logia_print_stdout(ptr @.str), !dbg !8
  ret i64 0, !dbg !10
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
!8 = !DILocation(line: 2, column: 4, scope: !9)
!9 = distinct !DILexicalBlock(scope: !4, file: !1, line: 1, column: 20)
!10 = !DILocation(line: 1, scope: !4)
