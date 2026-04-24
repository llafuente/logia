; ModuleID = 'logia'
source_filename = "logia"

declare i1 @logia_intrinsics_bin_lt_i64_i64()

declare i64 @logia_intrinsics_bin_mul_i64_i64()

declare i64 @logia_intrinsics_bin_add_i64_i64()

define i64 @test_1() !dbg !4 {
function_body:
  %0 = alloca i64, align 8, !dbg !8
  store i64 0, ptr %0, align 4, !dbg !10
  br i1 true, label %then_block_5, label %else_block_5, !dbg !10

then_block_5:                                     ; preds = %function_body
  %deref = alloca ptr, align 8, !dbg !11
  store ptr %0, ptr %deref, align 8, !dbg !11
  %1 = load ptr, ptr %deref, align 8, !dbg !11
  %2 = call i64 @logia_intrinsics_bin_assign_i64_i64(ptr %1, i64 1), !dbg !11
  br label %continue_block_5, !dbg !10

else_block_5:                                     ; preds = %function_body
  br label %continue_block_5, !dbg !10

continue_block_5:                                 ; preds = %else_block_5, %then_block_5
  %i = load i64, ptr %0, align 4, !dbg !13
  ret i64 %i, !dbg !14
}

define i64 @test_2() !dbg !15 {
function_body:
  %0 = alloca i64, align 8, !dbg !16
  store i64 0, ptr %0, align 4, !dbg !18
  br i1 false, label %then_block_6, label %else_block_6, !dbg !18

then_block_6:                                     ; preds = %function_body
  %deref = alloca ptr, align 8, !dbg !19
  store ptr %0, ptr %deref, align 8, !dbg !19
  %1 = load ptr, ptr %deref, align 8, !dbg !19
  %2 = call i64 @logia_intrinsics_bin_assign_i64_i64(ptr %1, i64 0), !dbg !19
  br label %continue_block_6, !dbg !18

else_block_6:                                     ; preds = %function_body
  br i1 true, label %then_block_7, label %else_block_7, !dbg !18

then_block_7:                                     ; preds = %else_block_6
  %deref1 = alloca ptr, align 8, !dbg !21
  store ptr %0, ptr %deref1, align 8, !dbg !21
  %3 = load ptr, ptr %deref1, align 8, !dbg !21
  %4 = call i64 @logia_intrinsics_bin_assign_i64_i64(ptr %3, i64 1), !dbg !21
  br label %continue_block_7, !dbg !18

else_block_7:                                     ; preds = %else_block_6
  br label %continue_block_7, !dbg !18

continue_block_7:                                 ; preds = %else_block_7, %then_block_7
  br label %continue_block_6, !dbg !18

continue_block_6:                                 ; preds = %continue_block_7, %then_block_6
  %i = load i64, ptr %0, align 4, !dbg !24
  ret i64 %i, !dbg !25
}

define i64 @test_3() !dbg !26 {
function_body:
  %0 = alloca i64, align 8, !dbg !27
  store i64 0, ptr %0, align 4, !dbg !29
  br i1 false, label %then_block_8, label %else_block_8, !dbg !29

then_block_8:                                     ; preds = %function_body
  %deref = alloca ptr, align 8, !dbg !30
  store ptr %0, ptr %deref, align 8, !dbg !30
  %1 = load ptr, ptr %deref, align 8, !dbg !30
  %2 = call i64 @logia_intrinsics_bin_assign_i64_i64(ptr %1, i64 0), !dbg !30
  br label %continue_block_8, !dbg !29

else_block_8:                                     ; preds = %function_body
  br i1 false, label %then_block_9, label %else_block_9, !dbg !29

then_block_9:                                     ; preds = %else_block_8
  %deref1 = alloca ptr, align 8, !dbg !32
  store ptr %0, ptr %deref1, align 8, !dbg !32
  %3 = load ptr, ptr %deref1, align 8, !dbg !32
  %4 = call i64 @logia_intrinsics_bin_assign_i64_i64(ptr %3, i64 0), !dbg !32
  br label %continue_block_9, !dbg !29

else_block_9:                                     ; preds = %else_block_8
  br label %continue_block_9, !dbg !29

continue_block_9:                                 ; preds = %else_block_9, %then_block_9
  %deref2 = alloca ptr, align 8, !dbg !35
  store ptr %0, ptr %deref2, align 8, !dbg !35
  %5 = load ptr, ptr %deref2, align 8, !dbg !35
  %6 = call i64 @logia_intrinsics_bin_assign_i64_i64(ptr %5, i64 1), !dbg !35
  br label %continue_block_8, !dbg !29

continue_block_8:                                 ; preds = %continue_block_9, %then_block_8
  %i = load i64, ptr %0, align 4, !dbg !36
  ret i64 %i, !dbg !37
}

define i64 @main() !dbg !38 {
function_body:
  %0 = call i64 @test_1(), !dbg !39
  %1 = call i64 @test_2(), !dbg !41
  %2 = call i64 @logia_intrinsics_bin_add_i64_i64(i64 %0, i64 %1), !dbg !39
  %3 = call i64 @test_3(), !dbg !42
  %4 = call i64 @logia_intrinsics_bin_add_i64_i64(i64 %2, i64 %3), !dbg !39
  ret i64 %4, !dbg !43
}

!llvm.dbg.cu = !{!0}
!llvm.module.flags = !{!2, !3}

!0 = distinct !DICompileUnit(language: DW_LANG_C, file: !1, producer: "logia", isOptimized: false, runtimeVersion: 0, emissionKind: FullDebug)
!1 = !DIFile(filename: "if-stmt.logia", directory: ".\\tests\\logia\\")
!2 = !{i32 2, !"Debug Info Version", i32 3}
!3 = !{i32 2, !"Dwarf Version", i32 4}
!4 = distinct !DISubprogram(name: "test_1", scope: !1, file: !1, line: 2, type: !5, spFlags: DISPFlagDefinition, unit: !0)
!5 = !DISubroutineType(types: !6)
!6 = !{!7}
!7 = !DIBasicType(name: "i64", size: 1, encoding: DW_ATE_signed)
!8 = !DILocation(line: 3, column: 4, scope: !9)
!9 = distinct !DILexicalBlock(scope: !4, file: !1, line: 2, column: 22)
!10 = !DILocation(line: 2, scope: !4)
!11 = !DILocation(line: 5, column: 8, scope: !12)
!12 = distinct !DILexicalBlock(scope: !9, file: !1, line: 4, column: 12)
!13 = !DILocation(line: 7, column: 11, scope: !9)
!14 = !DILocation(line: 7, column: 4, scope: !9)
!15 = distinct !DISubprogram(name: "test_2", scope: !1, file: !1, line: 10, type: !5, spFlags: DISPFlagDefinition, unit: !0)
!16 = !DILocation(line: 11, column: 4, scope: !17)
!17 = distinct !DILexicalBlock(scope: !15, file: !1, line: 10, column: 22)
!18 = !DILocation(line: 10, scope: !15)
!19 = !DILocation(line: 13, column: 8, scope: !20)
!20 = distinct !DILexicalBlock(scope: !17, file: !1, line: 12, column: 13)
!21 = !DILocation(line: 15, column: 8, scope: !22)
!22 = distinct !DILexicalBlock(scope: !23, file: !1, line: 14, column: 19)
!23 = distinct !DILexicalBlock(scope: !17, file: !1, line: 14, column: 11)
!24 = !DILocation(line: 17, column: 11, scope: !17)
!25 = !DILocation(line: 17, column: 4, scope: !17)
!26 = distinct !DISubprogram(name: "test_3", scope: !1, file: !1, line: 21, type: !5, spFlags: DISPFlagDefinition, unit: !0)
!27 = !DILocation(line: 22, column: 4, scope: !28)
!28 = distinct !DILexicalBlock(scope: !26, file: !1, line: 21, column: 22)
!29 = !DILocation(line: 21, scope: !26)
!30 = !DILocation(line: 24, column: 8, scope: !31)
!31 = distinct !DILexicalBlock(scope: !28, file: !1, line: 23, column: 13)
!32 = !DILocation(line: 26, column: 8, scope: !33)
!33 = distinct !DILexicalBlock(scope: !34, file: !1, line: 25, column: 20)
!34 = distinct !DILexicalBlock(scope: !28, file: !1, line: 27, column: 11)
!35 = !DILocation(line: 28, column: 8, scope: !34)
!36 = !DILocation(line: 30, column: 11, scope: !28)
!37 = !DILocation(line: 30, column: 4, scope: !28)
!38 = distinct !DISubprogram(name: "main", scope: !1, file: !1, line: 33, type: !5, spFlags: DISPFlagDefinition, unit: !0)
!39 = !DILocation(line: 34, column: 11, scope: !40)
!40 = distinct !DILexicalBlock(scope: !38, file: !1, line: 33, column: 20)
!41 = !DILocation(line: 34, column: 22, scope: !40)
!42 = !DILocation(line: 34, column: 33, scope: !40)
!43 = !DILocation(line: 34, column: 4, scope: !40)
