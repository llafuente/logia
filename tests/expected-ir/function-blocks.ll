; ModuleID = 'logia'
source_filename = "logia"

declare i1 @logia_intrinsics_bin_lt_i64_i64()

declare i64 @logia_intrinsics_bin_mul_i64_i64()

declare i64 @logia_intrinsics_bin_add_i64_i64()

define i64 @block_count() !dbg !4 {
function_body:
  %0 = alloca i64, align 8, !dbg !8
  store i64 0, ptr %0, align 4, !dbg !10
  br label %1, !dbg !10

1:                                                ; preds = %function_body
  %deref = alloca ptr, align 8, !dbg !11
  store ptr %0, ptr %deref, align 8, !dbg !11
  %2 = load ptr, ptr %deref, align 8, !dbg !11
  %i = load i64, ptr %0, align 4, !dbg !13
  %3 = call i64 @logia_intrinsics_bin_add_i64_i64(i64 %i, i64 1), !dbg !13
  %4 = call i64 @logia_intrinsics_bin_assign_i64_i64(ptr %2, i64 %3), !dbg !11
  br label %5, !dbg !10

5:                                                ; preds = %1
  br i1 true, label %then_block_0, label %else_block_0, !dbg !10

then_block_0:                                     ; preds = %5
  %deref1 = alloca ptr, align 8, !dbg !14
  store ptr %0, ptr %deref1, align 8, !dbg !14
  %6 = load ptr, ptr %deref1, align 8, !dbg !14
  %i2 = load i64, ptr %0, align 4, !dbg !17
  %7 = call i64 @logia_intrinsics_bin_add_i64_i64(i64 %i2, i64 1), !dbg !17
  %8 = call i64 @logia_intrinsics_bin_assign_i64_i64(ptr %6, i64 %7), !dbg !14
  br label %continue_block_0, !dbg !10

else_block_0:                                     ; preds = %5
  br label %continue_block_0, !dbg !10

continue_block_0:                                 ; preds = %else_block_0, %then_block_0
  br label %9, !dbg !10

9:                                                ; preds = %continue_block_0
  br i1 false, label %then_block_1, label %else_block_1, !dbg !10

then_block_1:                                     ; preds = %9
  %deref3 = alloca ptr, align 8, !dbg !18
  store ptr %0, ptr %deref3, align 8, !dbg !18
  %10 = load ptr, ptr %deref3, align 8, !dbg !18
  %i4 = load i64, ptr %0, align 4, !dbg !21
  %11 = call i64 @logia_intrinsics_bin_add_i64_i64(i64 %i4, i64 999), !dbg !21
  %12 = call i64 @logia_intrinsics_bin_assign_i64_i64(ptr %10, i64 %11), !dbg !18
  br label %continue_block_1, !dbg !10

else_block_1:                                     ; preds = %9
  %deref5 = alloca ptr, align 8, !dbg !22
  store ptr %0, ptr %deref5, align 8, !dbg !22
  %13 = load ptr, ptr %deref5, align 8, !dbg !22
  %i6 = load i64, ptr %0, align 4, !dbg !24
  %14 = call i64 @logia_intrinsics_bin_add_i64_i64(i64 %i6, i64 1), !dbg !24
  %15 = call i64 @logia_intrinsics_bin_assign_i64_i64(ptr %13, i64 %14), !dbg !22
  br label %continue_block_1, !dbg !10

continue_block_1:                                 ; preds = %else_block_1, %then_block_1
  br i1 false, label %then_block_2, label %else_block_2, !dbg !10

then_block_2:                                     ; preds = %continue_block_1
  %deref7 = alloca ptr, align 8, !dbg !25
  store ptr %0, ptr %deref7, align 8, !dbg !25
  %16 = load ptr, ptr %deref7, align 8, !dbg !25
  %i8 = load i64, ptr %0, align 4, !dbg !27
  %17 = call i64 @logia_intrinsics_bin_add_i64_i64(i64 %i8, i64 999), !dbg !27
  %18 = call i64 @logia_intrinsics_bin_assign_i64_i64(ptr %16, i64 %17), !dbg !25
  br label %continue_block_2, !dbg !10

else_block_2:                                     ; preds = %continue_block_1
  %deref9 = alloca ptr, align 8, !dbg !28
  store ptr %0, ptr %deref9, align 8, !dbg !28
  %19 = load ptr, ptr %deref9, align 8, !dbg !28
  %i10 = load i64, ptr %0, align 4, !dbg !30
  %20 = call i64 @logia_intrinsics_bin_add_i64_i64(i64 %i10, i64 1), !dbg !30
  %21 = call i64 @logia_intrinsics_bin_assign_i64_i64(ptr %19, i64 %20), !dbg !28
  br label %continue_block_2, !dbg !10

continue_block_2:                                 ; preds = %else_block_2, %then_block_2
  br i1 false, label %then_block_3, label %else_block_3, !dbg !10

then_block_3:                                     ; preds = %continue_block_2
  %deref11 = alloca ptr, align 8, !dbg !31
  store ptr %0, ptr %deref11, align 8, !dbg !31
  %22 = load ptr, ptr %deref11, align 8, !dbg !31
  %i12 = load i64, ptr %0, align 4, !dbg !33
  %23 = call i64 @logia_intrinsics_bin_add_i64_i64(i64 %i12, i64 999), !dbg !33
  %24 = call i64 @logia_intrinsics_bin_assign_i64_i64(ptr %22, i64 %23), !dbg !31
  br label %continue_block_3, !dbg !10

else_block_3:                                     ; preds = %continue_block_2
  br i1 false, label %then_block_4, label %else_block_4, !dbg !10

then_block_4:                                     ; preds = %else_block_3
  %deref13 = alloca ptr, align 8, !dbg !34
  store ptr %0, ptr %deref13, align 8, !dbg !34
  %25 = load ptr, ptr %deref13, align 8, !dbg !34
  %i14 = load i64, ptr %0, align 4, !dbg !37
  %26 = call i64 @logia_intrinsics_bin_add_i64_i64(i64 %i14, i64 999), !dbg !37
  %27 = call i64 @logia_intrinsics_bin_assign_i64_i64(ptr %25, i64 %26), !dbg !34
  br label %continue_block_4, !dbg !10

else_block_4:                                     ; preds = %else_block_3
  br label %continue_block_4, !dbg !10

continue_block_4:                                 ; preds = %else_block_4, %then_block_4
  %deref15 = alloca ptr, align 8, !dbg !38
  store ptr %0, ptr %deref15, align 8, !dbg !38
  %28 = load ptr, ptr %deref15, align 8, !dbg !38
  %i16 = load i64, ptr %0, align 4, !dbg !39
  %29 = call i64 @logia_intrinsics_bin_add_i64_i64(i64 %i16, i64 1), !dbg !39
  %30 = call i64 @logia_intrinsics_bin_assign_i64_i64(ptr %28, i64 %29), !dbg !38
  br label %continue_block_3, !dbg !10

continue_block_3:                                 ; preds = %continue_block_4, %then_block_3
  %i17 = load i64, ptr %0, align 4, !dbg !40
  ret i64 %i17, !dbg !41
}

define i64 @main() !dbg !42 {
function_body:
  %0 = call i64 @block_count(), !dbg !43
  %1 = alloca i64, align 8, !dbg !45
  store i64 %0, ptr %1, align 4, !dbg !46
  %count = load i64, ptr %1, align 4, !dbg !47
  ret i64 %count, !dbg !48
}

!llvm.dbg.cu = !{!0}
!llvm.module.flags = !{!2, !3}

!0 = distinct !DICompileUnit(language: DW_LANG_C, file: !1, producer: "logia", isOptimized: false, runtimeVersion: 0, emissionKind: FullDebug)
!1 = !DIFile(filename: "function-blocks.logia", directory: ".\\tests\\logia\\")
!2 = !{i32 2, !"Debug Info Version", i32 3}
!3 = !{i32 2, !"Dwarf Version", i32 4}
!4 = distinct !DISubprogram(name: "block_count", scope: !1, file: !1, line: 1, type: !5, spFlags: DISPFlagDefinition, unit: !0)
!5 = !DISubroutineType(types: !6)
!6 = !{!7}
!7 = !DIBasicType(name: "i64", size: 1, encoding: DW_ATE_signed)
!8 = !DILocation(line: 2, column: 4, scope: !9)
!9 = distinct !DILexicalBlock(scope: !4, file: !1, line: 1, column: 27)
!10 = !DILocation(line: 1, scope: !4)
!11 = !DILocation(line: 5, column: 8, scope: !12)
!12 = distinct !DILexicalBlock(scope: !9, file: !1, line: 4, column: 4)
!13 = !DILocation(line: 5, column: 12, scope: !12)
!14 = !DILocation(line: 10, column: 12, scope: !15)
!15 = distinct !DILexicalBlock(scope: !16, file: !1, line: 9, column: 16)
!16 = distinct !DILexicalBlock(scope: !9, file: !1, line: 8, column: 4)
!17 = !DILocation(line: 10, column: 16, scope: !15)
!18 = !DILocation(line: 16, column: 12, scope: !19)
!19 = distinct !DILexicalBlock(scope: !20, file: !1, line: 15, column: 17)
!20 = distinct !DILexicalBlock(scope: !9, file: !1, line: 14, column: 4)
!21 = !DILocation(line: 16, column: 16, scope: !19)
!22 = !DILocation(line: 18, column: 10, scope: !23)
!23 = distinct !DILexicalBlock(scope: !20, file: !1, line: 17, column: 15)
!24 = !DILocation(line: 18, column: 14, scope: !23)
!25 = !DILocation(line: 23, column: 8, scope: !26)
!26 = distinct !DILexicalBlock(scope: !9, file: !1, line: 22, column: 13)
!27 = !DILocation(line: 23, column: 12, scope: !26)
!28 = !DILocation(line: 25, column: 8, scope: !29)
!29 = distinct !DILexicalBlock(scope: !9, file: !1, line: 24, column: 11)
!30 = !DILocation(line: 25, column: 12, scope: !29)
!31 = !DILocation(line: 30, column: 8, scope: !32)
!32 = distinct !DILexicalBlock(scope: !9, file: !1, line: 29, column: 13)
!33 = !DILocation(line: 30, column: 12, scope: !32)
!34 = !DILocation(line: 32, column: 8, scope: !35)
!35 = distinct !DILexicalBlock(scope: !36, file: !1, line: 31, column: 20)
!36 = distinct !DILexicalBlock(scope: !9, file: !1, line: 33, column: 11)
!37 = !DILocation(line: 32, column: 12, scope: !35)
!38 = !DILocation(line: 34, column: 8, scope: !36)
!39 = !DILocation(line: 34, column: 12, scope: !36)
!40 = !DILocation(line: 37, column: 11, scope: !9)
!41 = !DILocation(line: 37, column: 4, scope: !9)
!42 = distinct !DISubprogram(name: "main", scope: !1, file: !1, line: 40, type: !5, spFlags: DISPFlagDefinition, unit: !0)
!43 = !DILocation(line: 41, column: 16, scope: !44)
!44 = distinct !DILexicalBlock(scope: !42, file: !1, line: 40, column: 20)
!45 = !DILocation(line: 41, column: 4, scope: !44)
!46 = !DILocation(line: 40, scope: !42)
!47 = !DILocation(line: 42, column: 11, scope: !44)
!48 = !DILocation(line: 42, column: 4, scope: !44)
