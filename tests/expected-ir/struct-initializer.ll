; ModuleID = 'logia'
source_filename = "logia"

%point = type { i64, i64 }

@.struct.init = private constant %point { i64 10, i64 11 }, align 4

declare i1 @logia_intrinsics_bin_lt_i64_i64()

declare i64 @logia_intrinsics_bin_mul_i64_i64()

declare i64 @logia_intrinsics_bin_add_i64_i64()

define i64 @main() !dbg !4 {
function_body:
  %0 = alloca ptr, align 8, !dbg !8
  store ptr @.struct.init, ptr %0, align 8, !dbg !10
  %p = load ptr, ptr %0, align 8, !dbg !11
  %1 = getelementptr inbounds nuw %point, ptr %p, i32 0, i32 0, !dbg !10
  %2 = load i64, ptr %1, align 4, !dbg !11
  %p1 = load ptr, ptr %0, align 8, !dbg !12
  %3 = getelementptr inbounds nuw %point, ptr %p1, i32 0, i32 1, !dbg !10
  %4 = load i64, ptr %3, align 4, !dbg !12
  %5 = call i64 @logia_intrinsics_bin_add_i64_i64(i64 %2, i64 %4), !dbg !11
  ret i64 %5, !dbg !13
}

!llvm.dbg.cu = !{!0}
!llvm.module.flags = !{!2, !3}

!0 = distinct !DICompileUnit(language: DW_LANG_C, file: !1, producer: "logia", isOptimized: false, runtimeVersion: 0, emissionKind: FullDebug)
!1 = !DIFile(filename: "struct-initializer.logia", directory: ".\\tests\\logia\\")
!2 = !{i32 2, !"Debug Info Version", i32 3}
!3 = !{i32 2, !"Dwarf Version", i32 4}
!4 = distinct !DISubprogram(name: "main", scope: !1, file: !1, line: 6, type: !5, spFlags: DISPFlagDefinition, unit: !0)
!5 = !DISubroutineType(types: !6)
!6 = !{!7}
!7 = !DIBasicType(name: "i64", size: 1, encoding: DW_ATE_signed)
!8 = !DILocation(line: 7, column: 4, scope: !9)
!9 = distinct !DILexicalBlock(scope: !4, file: !1, line: 6, column: 20)
!10 = !DILocation(line: 6, scope: !4)
!11 = !DILocation(line: 9, column: 11, scope: !9)
!12 = !DILocation(line: 9, column: 17, scope: !9)
!13 = !DILocation(line: 9, column: 4, scope: !9)
