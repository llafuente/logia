; ModuleID = 'logia'
source_filename = "logia"

declare i1 @logia_intrinsics_bin_lt_i64_i64()

declare i64 @logia_intrinsics_bin_mul_i64_i64()

declare i64 @logia_intrinsics_bin_add_i64_i64()

define i64 @sum(i64 %0, i64 %1) !dbg !4 {
function_body:
  %a = alloca i64, align 8, !dbg !11
  store i64 %0, ptr %a, align 4, !dbg !11
    #dbg_declare(ptr %a, !9, !DIExpression(), !12)
  %b = alloca i64, align 8, !dbg !11
  store i64 %1, ptr %b, align 4, !dbg !11
    #dbg_declare(ptr %b, !10, !DIExpression(), !13)
  %a1 = load i64, ptr %a, align 4, !dbg !11
  %b2 = load i64, ptr %b, align 4, !dbg !11
  %2 = call i64 @logia_intrinsics_bin_add_i64_i64(i64 %a1, i64 %b2), !dbg !14
  ret i64 %2, !dbg !11
}

define i64 @main() !dbg !16 {
function_body:
  %0 = call i64 @sum(i64 10, i64 15), !dbg !19
  ret i64 %0, !dbg !21
}

!llvm.dbg.cu = !{!0}
!llvm.module.flags = !{!2, !3}

!0 = distinct !DICompileUnit(language: DW_LANG_C, file: !1, producer: "logia", isOptimized: false, runtimeVersion: 0, emissionKind: FullDebug)
!1 = !DIFile(filename: "xxx.logia", directory: ".")
!2 = !{i32 2, !"Debug Info Version", i32 3}
!3 = !{i32 2, !"Dwarf Version", i32 4}
!4 = distinct !DISubprogram(name: "sum", scope: !1, file: !1, line: 1, type: !5, spFlags: DISPFlagDefinition, unit: !0, retainedNodes: !8)
!5 = !DISubroutineType(types: !6)
!6 = !{!7, !7, !7}
!7 = !DIBasicType(name: "i64", size: 1, encoding: DW_ATE_signed)
!8 = !{!9, !10}
!9 = !DILocalVariable(name: "a", arg: 1, scope: !4, file: !1, line: 1, type: !7)
!10 = !DILocalVariable(name: "b", arg: 2, scope: !4, file: !1, line: 1, type: !7)
!11 = !DILocation(line: 1, scope: !4)
!12 = !DILocation(line: 1, column: 17, scope: !4)
!13 = !DILocation(line: 1, column: 24, scope: !4)
!14 = !DILocation(line: 2, column: 11, scope: !15)
!15 = distinct !DILexicalBlock(scope: !4, file: !1, line: 1, column: 28)
!16 = distinct !DISubprogram(name: "main", scope: !1, file: !1, line: 5, type: !17, spFlags: DISPFlagDefinition, unit: !0)
!17 = !DISubroutineType(types: !18)
!18 = !{!7}
!19 = !DILocation(line: 6, column: 11, scope: !20)
!20 = distinct !DILexicalBlock(scope: !16, file: !1, line: 5, column: 20)
!21 = !DILocation(line: 5, scope: !16)
