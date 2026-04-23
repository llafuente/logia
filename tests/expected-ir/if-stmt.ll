; ModuleID = 'logia'
source_filename = "logia"

declare i1 @logia_intrinsics_bin_lt_i64_i64()

declare i64 @logia_intrinsics_bin_mul_i64_i64()

declare i64 @logia_intrinsics_bin_add_i64_i64()

define i64 @test_1() !dbg !4 {
function_body:
  %0 = alloca i64, align 8, !dbg !8
  store i64 0, ptr %0, align 4, !dbg !8
  br i1 true, label %then_block_5, label %else_block_5, !dbg !8

then_block_5:                                     ; preds = %function_body
  %deref = alloca ptr, align 8, !dbg !8
  store ptr %0, ptr %deref, align 8, !dbg !8
  %1 = load ptr, ptr %deref, align 8, !dbg !8
  %2 = call i64 @logia_intrinsics_bin_assign_i64_i64(ptr %1, i64 1), !dbg !9
  br label %continue_block_5, !dbg !8

else_block_5:                                     ; preds = %function_body
  br label %continue_block_5, !dbg !8

continue_block_5:                                 ; preds = %else_block_5, %then_block_5
  %i = load i64, ptr %0, align 4, !dbg !8
  ret i64 %i, !dbg !8
}

define i64 @test_2() !dbg !12 {
function_body:
  %0 = alloca i64, align 8, !dbg !13
  store i64 0, ptr %0, align 4, !dbg !13
  br i1 false, label %then_block_6, label %else_block_6, !dbg !13

then_block_6:                                     ; preds = %function_body
  %deref = alloca ptr, align 8, !dbg !13
  store ptr %0, ptr %deref, align 8, !dbg !13
  %1 = load ptr, ptr %deref, align 8, !dbg !13
  %2 = call i64 @logia_intrinsics_bin_assign_i64_i64(ptr %1, i64 0), !dbg !14
  br label %continue_block_6, !dbg !13

else_block_6:                                     ; preds = %function_body
  br i1 true, label %then_block_7, label %else_block_7, !dbg !13

then_block_7:                                     ; preds = %else_block_6
  %deref1 = alloca ptr, align 8, !dbg !13
  store ptr %0, ptr %deref1, align 8, !dbg !13
  %3 = load ptr, ptr %deref1, align 8, !dbg !13
  %4 = call i64 @logia_intrinsics_bin_assign_i64_i64(ptr %3, i64 1), !dbg !17
  br label %continue_block_7, !dbg !13

else_block_7:                                     ; preds = %else_block_6
  br label %continue_block_7, !dbg !13

continue_block_7:                                 ; preds = %else_block_7, %then_block_7
  br label %continue_block_6, !dbg !13

continue_block_6:                                 ; preds = %continue_block_7, %then_block_6
  %i = load i64, ptr %0, align 4, !dbg !13
  ret i64 %i, !dbg !13
}

define i64 @test_3() !dbg !20 {
function_body:
  %0 = alloca i64, align 8, !dbg !21
  store i64 0, ptr %0, align 4, !dbg !21
  br i1 false, label %then_block_8, label %else_block_8, !dbg !21

then_block_8:                                     ; preds = %function_body
  %deref = alloca ptr, align 8, !dbg !21
  store ptr %0, ptr %deref, align 8, !dbg !21
  %1 = load ptr, ptr %deref, align 8, !dbg !21
  %2 = call i64 @logia_intrinsics_bin_assign_i64_i64(ptr %1, i64 0), !dbg !22
  br label %continue_block_8, !dbg !21

else_block_8:                                     ; preds = %function_body
  br i1 false, label %then_block_9, label %else_block_9, !dbg !21

then_block_9:                                     ; preds = %else_block_8
  %deref1 = alloca ptr, align 8, !dbg !21
  store ptr %0, ptr %deref1, align 8, !dbg !21
  %3 = load ptr, ptr %deref1, align 8, !dbg !21
  %4 = call i64 @logia_intrinsics_bin_assign_i64_i64(ptr %3, i64 0), !dbg !25
  br label %continue_block_9, !dbg !21

else_block_9:                                     ; preds = %else_block_8
  br label %continue_block_9, !dbg !21

continue_block_9:                                 ; preds = %else_block_9, %then_block_9
  %deref2 = alloca ptr, align 8, !dbg !21
  store ptr %0, ptr %deref2, align 8, !dbg !21
  %5 = load ptr, ptr %deref2, align 8, !dbg !21
  %6 = call i64 @logia_intrinsics_bin_assign_i64_i64(ptr %5, i64 1), !dbg !28
  br label %continue_block_8, !dbg !21

continue_block_8:                                 ; preds = %continue_block_9, %then_block_8
  %i = load i64, ptr %0, align 4, !dbg !21
  ret i64 %i, !dbg !21
}

define i64 @main() !dbg !29 {
function_body:
  %0 = call i64 @test_1(), !dbg !30
  %1 = call i64 @test_2(), !dbg !32
  %2 = call i64 @logia_intrinsics_bin_add_i64_i64(i64 %0, i64 %1), !dbg !30
  %3 = call i64 @test_3(), !dbg !33
  %4 = call i64 @logia_intrinsics_bin_add_i64_i64(i64 %2, i64 %3), !dbg !30
  ret i64 %4, !dbg !34
}

!llvm.dbg.cu = !{!0}
!llvm.module.flags = !{!2, !3}

!0 = distinct !DICompileUnit(language: DW_LANG_C, file: !1, producer: "logia", isOptimized: false, runtimeVersion: 0, emissionKind: FullDebug)
!1 = !DIFile(filename: "xxx.logia", directory: ".")
!2 = !{i32 2, !"Debug Info Version", i32 3}
!3 = !{i32 2, !"Dwarf Version", i32 4}
!4 = distinct !DISubprogram(name: "test_1", scope: !1, file: !1, line: 2, type: !5, spFlags: DISPFlagDefinition, unit: !0)
!5 = !DISubroutineType(types: !6)
!6 = !{!7}
!7 = !DIBasicType(name: "i64", size: 1, encoding: DW_ATE_signed)
!8 = !DILocation(line: 2, scope: !4)
!9 = !DILocation(line: 5, column: 8, scope: !10)
!10 = distinct !DILexicalBlock(scope: !11, file: !1, line: 4, column: 12)
!11 = distinct !DILexicalBlock(scope: !4, file: !1, line: 2, column: 22)
!12 = distinct !DISubprogram(name: "test_2", scope: !1, file: !1, line: 10, type: !5, spFlags: DISPFlagDefinition, unit: !0)
!13 = !DILocation(line: 10, scope: !12)
!14 = !DILocation(line: 13, column: 8, scope: !15)
!15 = distinct !DILexicalBlock(scope: !16, file: !1, line: 12, column: 13)
!16 = distinct !DILexicalBlock(scope: !12, file: !1, line: 10, column: 22)
!17 = !DILocation(line: 15, column: 8, scope: !18)
!18 = distinct !DILexicalBlock(scope: !19, file: !1, line: 14, column: 19)
!19 = distinct !DILexicalBlock(scope: !16, file: !1, line: 14, column: 11)
!20 = distinct !DISubprogram(name: "test_3", scope: !1, file: !1, line: 21, type: !5, spFlags: DISPFlagDefinition, unit: !0)
!21 = !DILocation(line: 21, scope: !20)
!22 = !DILocation(line: 24, column: 8, scope: !23)
!23 = distinct !DILexicalBlock(scope: !24, file: !1, line: 23, column: 13)
!24 = distinct !DILexicalBlock(scope: !20, file: !1, line: 21, column: 22)
!25 = !DILocation(line: 26, column: 8, scope: !26)
!26 = distinct !DILexicalBlock(scope: !27, file: !1, line: 25, column: 20)
!27 = distinct !DILexicalBlock(scope: !24, file: !1, line: 27, column: 11)
!28 = !DILocation(line: 28, column: 8, scope: !27)
!29 = distinct !DISubprogram(name: "main", scope: !1, file: !1, line: 33, type: !5, spFlags: DISPFlagDefinition, unit: !0)
!30 = !DILocation(line: 34, column: 11, scope: !31)
!31 = distinct !DILexicalBlock(scope: !29, file: !1, line: 33, column: 20)
!32 = !DILocation(line: 34, column: 22, scope: !31)
!33 = !DILocation(line: 34, column: 33, scope: !31)
!34 = !DILocation(line: 33, scope: !29)
