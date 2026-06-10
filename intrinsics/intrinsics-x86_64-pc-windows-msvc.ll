; ModuleID = '.\intrinsics\intrinsics.cpp'
source_filename = ".\\intrinsics\\intrinsics.cpp"
target datalayout = "e-m:w-p270:32:32-p271:32:32-p272:64:64-i64:64-i128:128-f80:128-n8:16:32:64-S128"
target triple = "x86_64-pc-windows-msvc19.44.35227"

@.str = private unnamed_addr constant [11 x i8] c"logia=\CE\BBi8\00", section "llvm.metadata"
@.str.1 = private unnamed_addr constant [28 x i8] c".\\intrinsics\\intrinsics.cpp\00", section "llvm.metadata"
@.str.2 = private unnamed_addr constant [12 x i8] c"logia=\CE\BBi16\00", section "llvm.metadata"
@.str.3 = private unnamed_addr constant [12 x i8] c"logia=\CE\BBi32\00", section "llvm.metadata"
@.str.4 = private unnamed_addr constant [12 x i8] c"logia=\CE\BBi64\00", section "llvm.metadata"
@.str.5 = private unnamed_addr constant [11 x i8] c"logia=\CE\BBu8\00", section "llvm.metadata"
@.str.6 = private unnamed_addr constant [12 x i8] c"logia=\CE\BBu16\00", section "llvm.metadata"
@.str.7 = private unnamed_addr constant [12 x i8] c"logia=\CE\BBu32\00", section "llvm.metadata"
@.str.8 = private unnamed_addr constant [12 x i8] c"logia=\CE\BBu64\00", section "llvm.metadata"
@.str.9 = private unnamed_addr constant [14 x i8] c"logia=ref<i8>\00", section "llvm.metadata"
@.str.10 = private unnamed_addr constant [15 x i8] c"logia=ref<i16>\00", section "llvm.metadata"
@.str.11 = private unnamed_addr constant [15 x i8] c"logia=ref<i32>\00", section "llvm.metadata"
@.str.12 = private unnamed_addr constant [15 x i8] c"logia=ref<i64>\00", section "llvm.metadata"
@.str.13 = private unnamed_addr constant [14 x i8] c"logia=ref<u8>\00", section "llvm.metadata"
@.str.14 = private unnamed_addr constant [15 x i8] c"logia=ref<u16>\00", section "llvm.metadata"
@.str.15 = private unnamed_addr constant [15 x i8] c"logia=ref<u32>\00", section "llvm.metadata"
@.str.16 = private unnamed_addr constant [15 x i8] c"logia=ref<u64>\00", section "llvm.metadata"
@.str.17 = private unnamed_addr constant [12 x i8] c"logia=\CE\BBf32\00", section "llvm.metadata"
@.str.18 = private unnamed_addr constant [10 x i8] c"logia=f32\00", section "llvm.metadata"
@.str.19 = private unnamed_addr constant [15 x i8] c"logia=ref<f32>\00", section "llvm.metadata"
@.str.20 = private unnamed_addr constant [12 x i8] c"logia=\CE\BBf64\00", section "llvm.metadata"
@.str.21 = private unnamed_addr constant [10 x i8] c"logia=f64\00", section "llvm.metadata"
@.str.22 = private unnamed_addr constant [15 x i8] c"logia=ref<f64>\00", section "llvm.metadata"
@.str.23 = private unnamed_addr constant [25 x i8] c"logia=__logia_prefix_neg\00", section "llvm.metadata"
@.str.24 = private unnamed_addr constant [16 x i8] c"logia=undefined\00", section "llvm.metadata"
@.str.25 = private unnamed_addr constant [33 x i8] c"logia=__logia_prefix_bitwise_not\00", section "llvm.metadata"
@.str.26 = private unnamed_addr constant [25 x i8] c"logia=__logia_prefix_inc\00", section "llvm.metadata"
@.str.27 = private unnamed_addr constant [25 x i8] c"logia=__logia_prefix_dec\00", section "llvm.metadata"
@.str.28 = private unnamed_addr constant [22 x i8] c"logia=__logia_bin_add\00", section "llvm.metadata"
@.str.29 = private unnamed_addr constant [22 x i8] c"logia=__logia_bin_sub\00", section "llvm.metadata"
@.str.30 = private unnamed_addr constant [22 x i8] c"logia=__logia_bin_mul\00", section "llvm.metadata"
@.str.31 = private unnamed_addr constant [22 x i8] c"logia=__logia_bin_div\00", section "llvm.metadata"
@.str.32 = private unnamed_addr constant [22 x i8] c"logia=__logia_bin_mod\00", section "llvm.metadata"
@.str.33 = private unnamed_addr constant [25 x i8] c"logia=__logia_bin_cmp_eq\00", section "llvm.metadata"
@.str.34 = private unnamed_addr constant [26 x i8] c"logia=__logia_bin_cmp_neq\00", section "llvm.metadata"
@.str.35 = private unnamed_addr constant [25 x i8] c"logia=__logia_bin_cmp_lt\00", section "llvm.metadata"
@.str.36 = private unnamed_addr constant [25 x i8] c"logia=__logia_bin_cmp_gt\00", section "llvm.metadata"
@.str.37 = private unnamed_addr constant [26 x i8] c"logia=__logia_bin_cmp_lte\00", section "llvm.metadata"
@.str.38 = private unnamed_addr constant [26 x i8] c"logia=__logia_bin_cmp_gte\00", section "llvm.metadata"
@.str.39 = private unnamed_addr constant [25 x i8] c"logia=__logia_bin_assign\00", section "llvm.metadata"
@.str.40 = private unnamed_addr constant [29 x i8] c"logia=__logia_bin_add_assign\00", section "llvm.metadata"
@.str.41 = private unnamed_addr constant [29 x i8] c"logia=__logia_bin_sub_assign\00", section "llvm.metadata"
@.str.42 = private unnamed_addr constant [29 x i8] c"logia=__logia_bin_mul_assign\00", section "llvm.metadata"
@.str.43 = private unnamed_addr constant [29 x i8] c"logia=__logia_bin_div_assign\00", section "llvm.metadata"
@.str.44 = private unnamed_addr constant [29 x i8] c"logia=__logia_bin_mod_assign\00", section "llvm.metadata"
@.str.45 = private unnamed_addr constant [37 x i8] c"logia=__logia_bin_bitwise_and_assign\00", section "llvm.metadata"
@.str.46 = private unnamed_addr constant [36 x i8] c"logia=__logia_bin_bitwise_or_assign\00", section "llvm.metadata"
@.str.47 = private unnamed_addr constant [37 x i8] c"logia=__logia_bin_bitwise_xor_assign\00", section "llvm.metadata"
@.str.48 = private unnamed_addr constant [37 x i8] c"logia=__logia_bin_bitwise_shr_assign\00", section "llvm.metadata"
@.str.49 = private unnamed_addr constant [37 x i8] c"logia=__logia_bin_bitwise_shl_assign\00", section "llvm.metadata"
@.str.50 = private unnamed_addr constant [30 x i8] c"logia=__logia_bin_bitwise_and\00", section "llvm.metadata"
@.str.51 = private unnamed_addr constant [29 x i8] c"logia=__logia_bin_bitwise_or\00", section "llvm.metadata"
@.str.52 = private unnamed_addr constant [30 x i8] c"logia=__logia_bin_bitwise_xor\00", section "llvm.metadata"
@.str.53 = private unnamed_addr constant [30 x i8] c"logia=__logia_bin_bitwise_shl\00", section "llvm.metadata"
@.str.54 = private unnamed_addr constant [30 x i8] c"logia=__logia_bin_bitwise_shr\00", section "llvm.metadata"
@llvm.global.annotations = appending global [284 x { ptr, ptr, ptr, i32, ptr }] [{ ptr, ptr, ptr, i32, ptr } { ptr @logia_intrinsics_prefix_neg_i8, ptr @.str.23, ptr @.str.1, i32 56, ptr null }, { ptr, ptr, ptr, i32, ptr } { ptr @logia_intrinsics_prefix_neg_i16, ptr @.str.23, ptr @.str.1, i32 62, ptr null }, { ptr, ptr, ptr, i32, ptr } { ptr @logia_intrinsics_prefix_neg_i32, ptr @.str.23, ptr @.str.1, i32 68, ptr null }, { ptr, ptr, ptr, i32, ptr } { ptr @logia_intrinsics_prefix_neg_i64, ptr @.str.23, ptr @.str.1, i32 74, ptr null }, { ptr, ptr, ptr, i32, ptr } { ptr @logia_intrinsics_prefix_neg_u8, ptr @.str.23, ptr @.str.1, i32 80, ptr null }, { ptr, ptr, ptr, i32, ptr } { ptr @logia_intrinsics_prefix_neg_u16, ptr @.str.23, ptr @.str.1, i32 86, ptr null }, { ptr, ptr, ptr, i32, ptr } { ptr @logia_intrinsics_prefix_neg_u32, ptr @.str.23, ptr @.str.1, i32 92, ptr null }, { ptr, ptr, ptr, i32, ptr } { ptr @logia_intrinsics_prefix_neg_u64, ptr @.str.23, ptr @.str.1, i32 98, ptr null }, { ptr, ptr, ptr, i32, ptr } { ptr @logia_intrinsics_prefix_bitwise_not_i8, ptr @.str.24, ptr @.str.1, i32 104, ptr null }, { ptr, ptr, ptr, i32, ptr } { ptr @logia_intrinsics_prefix_bitwise_not_i16, ptr @.str.24, ptr @.str.1, i32 110, ptr null }, { ptr, ptr, ptr, i32, ptr } { ptr @logia_intrinsics_prefix_bitwise_not_i32, ptr @.str.24, ptr @.str.1, i32 116, ptr null }, { ptr, ptr, ptr, i32, ptr } { ptr @logia_intrinsics_prefix_bitwise_not_i64, ptr @.str.24, ptr @.str.1, i32 122, ptr null }, { ptr, ptr, ptr, i32, ptr } { ptr @logia_intrinsics_prefix_bitwise_not_u8, ptr @.str.24, ptr @.str.1, i32 128, ptr null }, { ptr, ptr, ptr, i32, ptr } { ptr @logia_intrinsics_prefix_bitwise_not_u16, ptr @.str.24, ptr @.str.1, i32 134, ptr null }, { ptr, ptr, ptr, i32, ptr } { ptr @logia_intrinsics_prefix_bitwise_not_u32, ptr @.str.24, ptr @.str.1, i32 140, ptr null }, { ptr, ptr, ptr, i32, ptr } { ptr @logia_intrinsics_prefix_bitwise_not_u64, ptr @.str.24, ptr @.str.1, i32 146, ptr null }, { ptr, ptr, ptr, i32, ptr } { ptr @logia_intrinsics_prefix_logical_not_i8, ptr @.str.25, ptr @.str.1, i32 152, ptr null }, { ptr, ptr, ptr, i32, ptr } { ptr @logia_intrinsics_prefix_logical_not_i16, ptr @.str.25, ptr @.str.1, i32 158, ptr null }, { ptr, ptr, ptr, i32, ptr } { ptr @logia_intrinsics_prefix_logical_not_i32, ptr @.str.25, ptr @.str.1, i32 164, ptr null }, { ptr, ptr, ptr, i32, ptr } { ptr @logia_intrinsics_prefix_logical_not_i64, ptr @.str.25, ptr @.str.1, i32 170, ptr null }, { ptr, ptr, ptr, i32, ptr } { ptr @logia_intrinsics_prefix_logical_not_u8, ptr @.str.25, ptr @.str.1, i32 176, ptr null }, { ptr, ptr, ptr, i32, ptr } { ptr @logia_intrinsics_prefix_logical_not_u16, ptr @.str.25, ptr @.str.1, i32 182, ptr null }, { ptr, ptr, ptr, i32, ptr } { ptr @logia_intrinsics_prefix_logical_not_u32, ptr @.str.25, ptr @.str.1, i32 188, ptr null }, { ptr, ptr, ptr, i32, ptr } { ptr @logia_intrinsics_prefix_logical_not_u64, ptr @.str.25, ptr @.str.1, i32 194, ptr null }, { ptr, ptr, ptr, i32, ptr } { ptr @logia_intrinsics_prefix_inc_i8, ptr @.str.26, ptr @.str.1, i32 200, ptr null }, { ptr, ptr, ptr, i32, ptr } { ptr @logia_intrinsics_prefix_inc_i16, ptr @.str.26, ptr @.str.1, i32 206, ptr null }, { ptr, ptr, ptr, i32, ptr } { ptr @logia_intrinsics_prefix_inc_i32, ptr @.str.26, ptr @.str.1, i32 212, ptr null }, { ptr, ptr, ptr, i32, ptr } { ptr @logia_intrinsics_prefix_inc_i64, ptr @.str.26, ptr @.str.1, i32 218, ptr null }, { ptr, ptr, ptr, i32, ptr } { ptr @logia_intrinsics_prefix_inc_u8, ptr @.str.26, ptr @.str.1, i32 224, ptr null }, { ptr, ptr, ptr, i32, ptr } { ptr @logia_intrinsics_prefix_inc_u16, ptr @.str.26, ptr @.str.1, i32 230, ptr null }, { ptr, ptr, ptr, i32, ptr } { ptr @logia_intrinsics_prefix_inc_u32, ptr @.str.26, ptr @.str.1, i32 236, ptr null }, { ptr, ptr, ptr, i32, ptr } { ptr @logia_intrinsics_prefix_inc_u64, ptr @.str.26, ptr @.str.1, i32 242, ptr null }, { ptr, ptr, ptr, i32, ptr } { ptr @logia_intrinsics_prefix_dec_i8, ptr @.str.27, ptr @.str.1, i32 248, ptr null }, { ptr, ptr, ptr, i32, ptr } { ptr @logia_intrinsics_prefix_dec_i16, ptr @.str.27, ptr @.str.1, i32 254, ptr null }, { ptr, ptr, ptr, i32, ptr } { ptr @logia_intrinsics_prefix_dec_i32, ptr @.str.27, ptr @.str.1, i32 260, ptr null }, { ptr, ptr, ptr, i32, ptr } { ptr @logia_intrinsics_prefix_dec_i64, ptr @.str.27, ptr @.str.1, i32 266, ptr null }, { ptr, ptr, ptr, i32, ptr } { ptr @logia_intrinsics_prefix_dec_u8, ptr @.str.27, ptr @.str.1, i32 272, ptr null }, { ptr, ptr, ptr, i32, ptr } { ptr @logia_intrinsics_prefix_dec_u16, ptr @.str.27, ptr @.str.1, i32 278, ptr null }, { ptr, ptr, ptr, i32, ptr } { ptr @logia_intrinsics_prefix_dec_u32, ptr @.str.27, ptr @.str.1, i32 284, ptr null }, { ptr, ptr, ptr, i32, ptr } { ptr @logia_intrinsics_prefix_dec_u64, ptr @.str.27, ptr @.str.1, i32 290, ptr null }, { ptr, ptr, ptr, i32, ptr } { ptr @logia__intrinsics_bin_add_i8_i8, ptr @.str.28, ptr @.str.1, i32 298, ptr null }, { ptr, ptr, ptr, i32, ptr } { ptr @logia_intrinsics_bin_sub_i8_i8, ptr @.str.29, ptr @.str.1, i32 304, ptr null }, { ptr, ptr, ptr, i32, ptr } { ptr @logia_intrinsics_bin_mul_i8_i8, ptr @.str.30, ptr @.str.1, i32 310, ptr null }, { ptr, ptr, ptr, i32, ptr } { ptr @logia_intrinsics_bin_div_i8_i8, ptr @.str.31, ptr @.str.1, i32 316, ptr null }, { ptr, ptr, ptr, i32, ptr } { ptr @logia_intrinsics_bin_mod_i8_i8, ptr @.str.32, ptr @.str.1, i32 322, ptr null }, { ptr, ptr, ptr, i32, ptr } { ptr @logia_intrinsics_bin_eq_i8_i8, ptr @.str.33, ptr @.str.1, i32 328, ptr null }, { ptr, ptr, ptr, i32, ptr } { ptr @logia_intrinsics_bin_neq_i8_i8, ptr @.str.34, ptr @.str.1, i32 334, ptr null }, { ptr, ptr, ptr, i32, ptr } { ptr @logia_intrinsics_bin_lt_i8_i8, ptr @.str.35, ptr @.str.1, i32 340, ptr null }, { ptr, ptr, ptr, i32, ptr } { ptr @logia_intrinsics_bin_gt_i8_i8, ptr @.str.36, ptr @.str.1, i32 346, ptr null }, { ptr, ptr, ptr, i32, ptr } { ptr @logia_intrinsics_bin_lte_i8_i8, ptr @.str.37, ptr @.str.1, i32 352, ptr null }, { ptr, ptr, ptr, i32, ptr } { ptr @logia_intrinsics_bin_gte_i8_i8, ptr @.str.38, ptr @.str.1, i32 358, ptr null }, { ptr, ptr, ptr, i32, ptr } { ptr @logia_intrinsics_bin_assign_i8_i8, ptr @.str.39, ptr @.str.1, i32 364, ptr null }, { ptr, ptr, ptr, i32, ptr } { ptr @logia_intrinsics_bin_add_assign_i8_i8, ptr @.str.40, ptr @.str.1, i32 371, ptr null }, { ptr, ptr, ptr, i32, ptr } { ptr @logia_intrinsics_bin_sub_assign_i8_i8, ptr @.str.41, ptr @.str.1, i32 378, ptr null }, { ptr, ptr, ptr, i32, ptr } { ptr @logia_intrinsics_bin_mul_assign_i8_i8, ptr @.str.42, ptr @.str.1, i32 385, ptr null }, { ptr, ptr, ptr, i32, ptr } { ptr @logia_intrinsics_bin_div_assign_i8_i8, ptr @.str.43, ptr @.str.1, i32 392, ptr null }, { ptr, ptr, ptr, i32, ptr } { ptr @logia_intrinsics_bin_mod_assign_i8_i8, ptr @.str.44, ptr @.str.1, i32 399, ptr null }, { ptr, ptr, ptr, i32, ptr } { ptr @logia_intrinsics_bin_bitwise_and_assign_i8_i8, ptr @.str.45, ptr @.str.1, i32 406, ptr null }, { ptr, ptr, ptr, i32, ptr } { ptr @logia_intrinsics_bin_bitwise_or_assign_i8_i8, ptr @.str.46, ptr @.str.1, i32 413, ptr null }, { ptr, ptr, ptr, i32, ptr } { ptr @logia_intrinsics_bin_bitwise_xor_assign_i8_i8, ptr @.str.47, ptr @.str.1, i32 420, ptr null }, { ptr, ptr, ptr, i32, ptr } { ptr @logia_intrinsics_bin_bitwise_shr_assign_i8_i8, ptr @.str.48, ptr @.str.1, i32 427, ptr null }, { ptr, ptr, ptr, i32, ptr } { ptr @logia_intrinsics_bin_bitwise_shl_assign_i8_i8, ptr @.str.49, ptr @.str.1, i32 434, ptr null }, { ptr, ptr, ptr, i32, ptr } { ptr @logia_intrinsics_bin_bitwise_and_i8_i8, ptr @.str.50, ptr @.str.1, i32 441, ptr null }, { ptr, ptr, ptr, i32, ptr } { ptr @logia_intrinsics_bin_bitwise_or_i8_i8, ptr @.str.51, ptr @.str.1, i32 447, ptr null }, { ptr, ptr, ptr, i32, ptr } { ptr @logia_intrinsics_bin_bitwise_xor_i8_i8, ptr @.str.52, ptr @.str.1, i32 453, ptr null }, { ptr, ptr, ptr, i32, ptr } { ptr @logia_intrinsics_bin_bitwise_shl_i8_i8, ptr @.str.53, ptr @.str.1, i32 459, ptr null }, { ptr, ptr, ptr, i32, ptr } { ptr @logia_intrinsics_bin_bitwise_shr_i8_i8, ptr @.str.54, ptr @.str.1, i32 465, ptr null }, { ptr, ptr, ptr, i32, ptr } { ptr @logia__intrinsics_bin_add_i16_i16, ptr @.str.28, ptr @.str.1, i32 471, ptr null }, { ptr, ptr, ptr, i32, ptr } { ptr @logia_intrinsics_bin_sub_i16_i16, ptr @.str.29, ptr @.str.1, i32 477, ptr null }, { ptr, ptr, ptr, i32, ptr } { ptr @logia_intrinsics_bin_mul_i16_i16, ptr @.str.30, ptr @.str.1, i32 483, ptr null }, { ptr, ptr, ptr, i32, ptr } { ptr @logia_intrinsics_bin_div_i16_i16, ptr @.str.31, ptr @.str.1, i32 489, ptr null }, { ptr, ptr, ptr, i32, ptr } { ptr @logia_intrinsics_bin_mod_i16_i16, ptr @.str.32, ptr @.str.1, i32 495, ptr null }, { ptr, ptr, ptr, i32, ptr } { ptr @logia_intrinsics_bin_eq_i16_i16, ptr @.str.33, ptr @.str.1, i32 501, ptr null }, { ptr, ptr, ptr, i32, ptr } { ptr @logia_intrinsics_bin_neq_i16_i16, ptr @.str.34, ptr @.str.1, i32 507, ptr null }, { ptr, ptr, ptr, i32, ptr } { ptr @logia_intrinsics_bin_lt_i16_i16, ptr @.str.35, ptr @.str.1, i32 513, ptr null }, { ptr, ptr, ptr, i32, ptr } { ptr @logia_intrinsics_bin_gt_i16_i16, ptr @.str.36, ptr @.str.1, i32 519, ptr null }, { ptr, ptr, ptr, i32, ptr } { ptr @logia_intrinsics_bin_lte_i16_i16, ptr @.str.37, ptr @.str.1, i32 525, ptr null }, { ptr, ptr, ptr, i32, ptr } { ptr @logia_intrinsics_bin_gte_i16_i16, ptr @.str.38, ptr @.str.1, i32 531, ptr null }, { ptr, ptr, ptr, i32, ptr } { ptr @logia_intrinsics_bin_assign_i16_i16, ptr @.str.39, ptr @.str.1, i32 537, ptr null }, { ptr, ptr, ptr, i32, ptr } { ptr @logia_intrinsics_bin_add_assign_i16_i16, ptr @.str.40, ptr @.str.1, i32 544, ptr null }, { ptr, ptr, ptr, i32, ptr } { ptr @logia_intrinsics_bin_sub_assign_i16_i16, ptr @.str.41, ptr @.str.1, i32 551, ptr null }, { ptr, ptr, ptr, i32, ptr } { ptr @logia_intrinsics_bin_mul_assign_i16_i16, ptr @.str.42, ptr @.str.1, i32 558, ptr null }, { ptr, ptr, ptr, i32, ptr } { ptr @logia_intrinsics_bin_div_assign_i16_i16, ptr @.str.43, ptr @.str.1, i32 565, ptr null }, { ptr, ptr, ptr, i32, ptr } { ptr @logia_intrinsics_bin_mod_assign_i16_i16, ptr @.str.44, ptr @.str.1, i32 572, ptr null }, { ptr, ptr, ptr, i32, ptr } { ptr @logia_intrinsics_bin_bitwise_and_assign_i16_i16, ptr @.str.45, ptr @.str.1, i32 579, ptr null }, { ptr, ptr, ptr, i32, ptr } { ptr @logia_intrinsics_bin_bitwise_or_assign_i16_i16, ptr @.str.46, ptr @.str.1, i32 586, ptr null }, { ptr, ptr, ptr, i32, ptr } { ptr @logia_intrinsics_bin_bitwise_xor_assign_i16_i16, ptr @.str.47, ptr @.str.1, i32 593, ptr null }, { ptr, ptr, ptr, i32, ptr } { ptr @logia_intrinsics_bin_bitwise_shr_assign_i16_i16, ptr @.str.48, ptr @.str.1, i32 600, ptr null }, { ptr, ptr, ptr, i32, ptr } { ptr @logia_intrinsics_bin_bitwise_shl_assign_i16_i16, ptr @.str.49, ptr @.str.1, i32 607, ptr null }, { ptr, ptr, ptr, i32, ptr } { ptr @logia_intrinsics_bin_bitwise_and_i16_i16, ptr @.str.50, ptr @.str.1, i32 614, ptr null }, { ptr, ptr, ptr, i32, ptr } { ptr @logia_intrinsics_bin_bitwise_or_i16_i16, ptr @.str.51, ptr @.str.1, i32 620, ptr null }, { ptr, ptr, ptr, i32, ptr } { ptr @logia_intrinsics_bin_bitwise_xor_i16_i16, ptr @.str.52, ptr @.str.1, i32 626, ptr null }, { ptr, ptr, ptr, i32, ptr } { ptr @logia_intrinsics_bin_bitwise_shl_i16_i16, ptr @.str.53, ptr @.str.1, i32 632, ptr null }, { ptr, ptr, ptr, i32, ptr } { ptr @logia_intrinsics_bin_bitwise_shr_i16_i16, ptr @.str.54, ptr @.str.1, i32 638, ptr null }, { ptr, ptr, ptr, i32, ptr } { ptr @logia__intrinsics_bin_add_i32_i32, ptr @.str.28, ptr @.str.1, i32 644, ptr null }, { ptr, ptr, ptr, i32, ptr } { ptr @logia_intrinsics_bin_sub_i32_i32, ptr @.str.29, ptr @.str.1, i32 650, ptr null }, { ptr, ptr, ptr, i32, ptr } { ptr @logia_intrinsics_bin_mul_i32_i32, ptr @.str.30, ptr @.str.1, i32 656, ptr null }, { ptr, ptr, ptr, i32, ptr } { ptr @logia_intrinsics_bin_div_i32_i32, ptr @.str.31, ptr @.str.1, i32 662, ptr null }, { ptr, ptr, ptr, i32, ptr } { ptr @logia_intrinsics_bin_mod_i32_i32, ptr @.str.32, ptr @.str.1, i32 668, ptr null }, { ptr, ptr, ptr, i32, ptr } { ptr @logia_intrinsics_bin_eq_i32_i32, ptr @.str.33, ptr @.str.1, i32 674, ptr null }, { ptr, ptr, ptr, i32, ptr } { ptr @logia_intrinsics_bin_neq_i32_i32, ptr @.str.34, ptr @.str.1, i32 680, ptr null }, { ptr, ptr, ptr, i32, ptr } { ptr @logia_intrinsics_bin_lt_i32_i32, ptr @.str.35, ptr @.str.1, i32 686, ptr null }, { ptr, ptr, ptr, i32, ptr } { ptr @logia_intrinsics_bin_gt_i32_i32, ptr @.str.36, ptr @.str.1, i32 692, ptr null }, { ptr, ptr, ptr, i32, ptr } { ptr @logia_intrinsics_bin_lte_i32_i32, ptr @.str.37, ptr @.str.1, i32 698, ptr null }, { ptr, ptr, ptr, i32, ptr } { ptr @logia_intrinsics_bin_gte_i32_i32, ptr @.str.38, ptr @.str.1, i32 704, ptr null }, { ptr, ptr, ptr, i32, ptr } { ptr @logia_intrinsics_bin_assign_i32_i32, ptr @.str.39, ptr @.str.1, i32 710, ptr null }, { ptr, ptr, ptr, i32, ptr } { ptr @logia_intrinsics_bin_add_assign_i32_i32, ptr @.str.40, ptr @.str.1, i32 717, ptr null }, { ptr, ptr, ptr, i32, ptr } { ptr @logia_intrinsics_bin_sub_assign_i32_i32, ptr @.str.41, ptr @.str.1, i32 724, ptr null }, { ptr, ptr, ptr, i32, ptr } { ptr @logia_intrinsics_bin_mul_assign_i32_i32, ptr @.str.42, ptr @.str.1, i32 731, ptr null }, { ptr, ptr, ptr, i32, ptr } { ptr @logia_intrinsics_bin_div_assign_i32_i32, ptr @.str.43, ptr @.str.1, i32 738, ptr null }, { ptr, ptr, ptr, i32, ptr } { ptr @logia_intrinsics_bin_mod_assign_i32_i32, ptr @.str.44, ptr @.str.1, i32 745, ptr null }, { ptr, ptr, ptr, i32, ptr } { ptr @logia_intrinsics_bin_bitwise_and_assign_i32_i32, ptr @.str.45, ptr @.str.1, i32 752, ptr null }, { ptr, ptr, ptr, i32, ptr } { ptr @logia_intrinsics_bin_bitwise_or_assign_i32_i32, ptr @.str.46, ptr @.str.1, i32 759, ptr null }, { ptr, ptr, ptr, i32, ptr } { ptr @logia_intrinsics_bin_bitwise_xor_assign_i32_i32, ptr @.str.47, ptr @.str.1, i32 766, ptr null }, { ptr, ptr, ptr, i32, ptr } { ptr @logia_intrinsics_bin_bitwise_shr_assign_i32_i32, ptr @.str.48, ptr @.str.1, i32 773, ptr null }, { ptr, ptr, ptr, i32, ptr } { ptr @logia_intrinsics_bin_bitwise_shl_assign_i32_i32, ptr @.str.49, ptr @.str.1, i32 780, ptr null }, { ptr, ptr, ptr, i32, ptr } { ptr @logia_intrinsics_bin_bitwise_and_i32_i32, ptr @.str.50, ptr @.str.1, i32 787, ptr null }, { ptr, ptr, ptr, i32, ptr } { ptr @logia_intrinsics_bin_bitwise_or_i32_i32, ptr @.str.51, ptr @.str.1, i32 793, ptr null }, { ptr, ptr, ptr, i32, ptr } { ptr @logia_intrinsics_bin_bitwise_xor_i32_i32, ptr @.str.52, ptr @.str.1, i32 799, ptr null }, { ptr, ptr, ptr, i32, ptr } { ptr @logia_intrinsics_bin_bitwise_shl_i32_i32, ptr @.str.53, ptr @.str.1, i32 805, ptr null }, { ptr, ptr, ptr, i32, ptr } { ptr @logia_intrinsics_bin_bitwise_shr_i32_i32, ptr @.str.54, ptr @.str.1, i32 811, ptr null }, { ptr, ptr, ptr, i32, ptr } { ptr @logia__intrinsics_bin_add_i64_i64, ptr @.str.28, ptr @.str.1, i32 817, ptr null }, { ptr, ptr, ptr, i32, ptr } { ptr @logia_intrinsics_bin_sub_i64_i64, ptr @.str.29, ptr @.str.1, i32 823, ptr null }, { ptr, ptr, ptr, i32, ptr } { ptr @logia_intrinsics_bin_mul_i64_i64, ptr @.str.30, ptr @.str.1, i32 829, ptr null }, { ptr, ptr, ptr, i32, ptr } { ptr @logia_intrinsics_bin_div_i64_i64, ptr @.str.31, ptr @.str.1, i32 835, ptr null }, { ptr, ptr, ptr, i32, ptr } { ptr @logia_intrinsics_bin_mod_i64_i64, ptr @.str.32, ptr @.str.1, i32 841, ptr null }, { ptr, ptr, ptr, i32, ptr } { ptr @logia_intrinsics_bin_eq_i64_i64, ptr @.str.33, ptr @.str.1, i32 847, ptr null }, { ptr, ptr, ptr, i32, ptr } { ptr @logia_intrinsics_bin_neq_i64_i64, ptr @.str.34, ptr @.str.1, i32 853, ptr null }, { ptr, ptr, ptr, i32, ptr } { ptr @logia_intrinsics_bin_lt_i64_i64, ptr @.str.35, ptr @.str.1, i32 859, ptr null }, { ptr, ptr, ptr, i32, ptr } { ptr @logia_intrinsics_bin_gt_i64_i64, ptr @.str.36, ptr @.str.1, i32 865, ptr null }, { ptr, ptr, ptr, i32, ptr } { ptr @logia_intrinsics_bin_lte_i64_i64, ptr @.str.37, ptr @.str.1, i32 871, ptr null }, { ptr, ptr, ptr, i32, ptr } { ptr @logia_intrinsics_bin_gte_i64_i64, ptr @.str.38, ptr @.str.1, i32 877, ptr null }, { ptr, ptr, ptr, i32, ptr } { ptr @logia_intrinsics_bin_assign_i64_i64, ptr @.str.39, ptr @.str.1, i32 883, ptr null }, { ptr, ptr, ptr, i32, ptr } { ptr @logia_intrinsics_bin_add_assign_i64_i64, ptr @.str.40, ptr @.str.1, i32 890, ptr null }, { ptr, ptr, ptr, i32, ptr } { ptr @logia_intrinsics_bin_sub_assign_i64_i64, ptr @.str.41, ptr @.str.1, i32 897, ptr null }, { ptr, ptr, ptr, i32, ptr } { ptr @logia_intrinsics_bin_mul_assign_i64_i64, ptr @.str.42, ptr @.str.1, i32 904, ptr null }, { ptr, ptr, ptr, i32, ptr } { ptr @logia_intrinsics_bin_div_assign_i64_i64, ptr @.str.43, ptr @.str.1, i32 911, ptr null }, { ptr, ptr, ptr, i32, ptr } { ptr @logia_intrinsics_bin_mod_assign_i64_i64, ptr @.str.44, ptr @.str.1, i32 918, ptr null }, { ptr, ptr, ptr, i32, ptr } { ptr @logia_intrinsics_bin_bitwise_and_assign_i64_i64, ptr @.str.45, ptr @.str.1, i32 925, ptr null }, { ptr, ptr, ptr, i32, ptr } { ptr @logia_intrinsics_bin_bitwise_or_assign_i64_i64, ptr @.str.46, ptr @.str.1, i32 932, ptr null }, { ptr, ptr, ptr, i32, ptr } { ptr @logia_intrinsics_bin_bitwise_xor_assign_i64_i64, ptr @.str.47, ptr @.str.1, i32 939, ptr null }, { ptr, ptr, ptr, i32, ptr } { ptr @logia_intrinsics_bin_bitwise_shr_assign_i64_i64, ptr @.str.48, ptr @.str.1, i32 946, ptr null }, { ptr, ptr, ptr, i32, ptr } { ptr @logia_intrinsics_bin_bitwise_shl_assign_i64_i64, ptr @.str.49, ptr @.str.1, i32 953, ptr null }, { ptr, ptr, ptr, i32, ptr } { ptr @logia_intrinsics_bin_bitwise_and_i64_i64, ptr @.str.50, ptr @.str.1, i32 960, ptr null }, { ptr, ptr, ptr, i32, ptr } { ptr @logia_intrinsics_bin_bitwise_or_i64_i64, ptr @.str.51, ptr @.str.1, i32 966, ptr null }, { ptr, ptr, ptr, i32, ptr } { ptr @logia_intrinsics_bin_bitwise_xor_i64_i64, ptr @.str.52, ptr @.str.1, i32 972, ptr null }, { ptr, ptr, ptr, i32, ptr } { ptr @logia_intrinsics_bin_bitwise_shl_i64_i64, ptr @.str.53, ptr @.str.1, i32 978, ptr null }, { ptr, ptr, ptr, i32, ptr } { ptr @logia_intrinsics_bin_bitwise_shr_i64_i64, ptr @.str.54, ptr @.str.1, i32 984, ptr null }, { ptr, ptr, ptr, i32, ptr } { ptr @logia__intrinsics_bin_add_u8_u8, ptr @.str.28, ptr @.str.1, i32 990, ptr null }, { ptr, ptr, ptr, i32, ptr } { ptr @logia_intrinsics_bin_sub_u8_u8, ptr @.str.29, ptr @.str.1, i32 996, ptr null }, { ptr, ptr, ptr, i32, ptr } { ptr @logia_intrinsics_bin_mul_u8_u8, ptr @.str.30, ptr @.str.1, i32 1002, ptr null }, { ptr, ptr, ptr, i32, ptr } { ptr @logia_intrinsics_bin_div_u8_u8, ptr @.str.31, ptr @.str.1, i32 1008, ptr null }, { ptr, ptr, ptr, i32, ptr } { ptr @logia_intrinsics_bin_mod_u8_u8, ptr @.str.32, ptr @.str.1, i32 1014, ptr null }, { ptr, ptr, ptr, i32, ptr } { ptr @logia_intrinsics_bin_eq_u8_u8, ptr @.str.33, ptr @.str.1, i32 1020, ptr null }, { ptr, ptr, ptr, i32, ptr } { ptr @logia_intrinsics_bin_neq_u8_u8, ptr @.str.34, ptr @.str.1, i32 1026, ptr null }, { ptr, ptr, ptr, i32, ptr } { ptr @logia_intrinsics_bin_lt_u8_u8, ptr @.str.35, ptr @.str.1, i32 1032, ptr null }, { ptr, ptr, ptr, i32, ptr } { ptr @logia_intrinsics_bin_gt_u8_u8, ptr @.str.36, ptr @.str.1, i32 1038, ptr null }, { ptr, ptr, ptr, i32, ptr } { ptr @logia_intrinsics_bin_lte_u8_u8, ptr @.str.37, ptr @.str.1, i32 1044, ptr null }, { ptr, ptr, ptr, i32, ptr } { ptr @logia_intrinsics_bin_gte_u8_u8, ptr @.str.38, ptr @.str.1, i32 1050, ptr null }, { ptr, ptr, ptr, i32, ptr } { ptr @logia_intrinsics_bin_assign_u8_u8, ptr @.str.39, ptr @.str.1, i32 1056, ptr null }, { ptr, ptr, ptr, i32, ptr } { ptr @logia_intrinsics_bin_add_assign_u8_u8, ptr @.str.40, ptr @.str.1, i32 1063, ptr null }, { ptr, ptr, ptr, i32, ptr } { ptr @logia_intrinsics_bin_sub_assign_u8_u8, ptr @.str.41, ptr @.str.1, i32 1070, ptr null }, { ptr, ptr, ptr, i32, ptr } { ptr @logia_intrinsics_bin_mul_assign_u8_u8, ptr @.str.42, ptr @.str.1, i32 1077, ptr null }, { ptr, ptr, ptr, i32, ptr } { ptr @logia_intrinsics_bin_div_assign_u8_u8, ptr @.str.43, ptr @.str.1, i32 1084, ptr null }, { ptr, ptr, ptr, i32, ptr } { ptr @logia_intrinsics_bin_mod_assign_u8_u8, ptr @.str.44, ptr @.str.1, i32 1091, ptr null }, { ptr, ptr, ptr, i32, ptr } { ptr @logia_intrinsics_bin_bitwise_and_assign_u8_u8, ptr @.str.45, ptr @.str.1, i32 1098, ptr null }, { ptr, ptr, ptr, i32, ptr } { ptr @logia_intrinsics_bin_bitwise_or_assign_u8_u8, ptr @.str.46, ptr @.str.1, i32 1105, ptr null }, { ptr, ptr, ptr, i32, ptr } { ptr @logia_intrinsics_bin_bitwise_xor_assign_u8_u8, ptr @.str.47, ptr @.str.1, i32 1112, ptr null }, { ptr, ptr, ptr, i32, ptr } { ptr @logia_intrinsics_bin_bitwise_shr_assign_u8_u8, ptr @.str.48, ptr @.str.1, i32 1119, ptr null }, { ptr, ptr, ptr, i32, ptr } { ptr @logia_intrinsics_bin_bitwise_shl_assign_u8_u8, ptr @.str.49, ptr @.str.1, i32 1126, ptr null }, { ptr, ptr, ptr, i32, ptr } { ptr @logia_intrinsics_bin_bitwise_and_u8_u8, ptr @.str.50, ptr @.str.1, i32 1133, ptr null }, { ptr, ptr, ptr, i32, ptr } { ptr @logia_intrinsics_bin_bitwise_or_u8_u8, ptr @.str.51, ptr @.str.1, i32 1139, ptr null }, { ptr, ptr, ptr, i32, ptr } { ptr @logia_intrinsics_bin_bitwise_xor_u8_u8, ptr @.str.52, ptr @.str.1, i32 1145, ptr null }, { ptr, ptr, ptr, i32, ptr } { ptr @logia_intrinsics_bin_bitwise_shl_u8_u8, ptr @.str.53, ptr @.str.1, i32 1151, ptr null }, { ptr, ptr, ptr, i32, ptr } { ptr @logia_intrinsics_bin_bitwise_shr_u8_u8, ptr @.str.54, ptr @.str.1, i32 1157, ptr null }, { ptr, ptr, ptr, i32, ptr } { ptr @logia__intrinsics_bin_add_u16_u16, ptr @.str.28, ptr @.str.1, i32 1163, ptr null }, { ptr, ptr, ptr, i32, ptr } { ptr @logia_intrinsics_bin_sub_u16_u16, ptr @.str.29, ptr @.str.1, i32 1169, ptr null }, { ptr, ptr, ptr, i32, ptr } { ptr @logia_intrinsics_bin_mul_u16_u16, ptr @.str.30, ptr @.str.1, i32 1175, ptr null }, { ptr, ptr, ptr, i32, ptr } { ptr @logia_intrinsics_bin_div_u16_u16, ptr @.str.31, ptr @.str.1, i32 1181, ptr null }, { ptr, ptr, ptr, i32, ptr } { ptr @logia_intrinsics_bin_mod_u16_u16, ptr @.str.32, ptr @.str.1, i32 1187, ptr null }, { ptr, ptr, ptr, i32, ptr } { ptr @logia_intrinsics_bin_eq_u16_u16, ptr @.str.33, ptr @.str.1, i32 1193, ptr null }, { ptr, ptr, ptr, i32, ptr } { ptr @logia_intrinsics_bin_neq_u16_u16, ptr @.str.34, ptr @.str.1, i32 1199, ptr null }, { ptr, ptr, ptr, i32, ptr } { ptr @logia_intrinsics_bin_lt_u16_u16, ptr @.str.35, ptr @.str.1, i32 1205, ptr null }, { ptr, ptr, ptr, i32, ptr } { ptr @logia_intrinsics_bin_gt_u16_u16, ptr @.str.36, ptr @.str.1, i32 1211, ptr null }, { ptr, ptr, ptr, i32, ptr } { ptr @logia_intrinsics_bin_lte_u16_u16, ptr @.str.37, ptr @.str.1, i32 1217, ptr null }, { ptr, ptr, ptr, i32, ptr } { ptr @logia_intrinsics_bin_gte_u16_u16, ptr @.str.38, ptr @.str.1, i32 1223, ptr null }, { ptr, ptr, ptr, i32, ptr } { ptr @logia_intrinsics_bin_assign_u16_u16, ptr @.str.39, ptr @.str.1, i32 1229, ptr null }, { ptr, ptr, ptr, i32, ptr } { ptr @logia_intrinsics_bin_add_assign_u16_u16, ptr @.str.40, ptr @.str.1, i32 1236, ptr null }, { ptr, ptr, ptr, i32, ptr } { ptr @logia_intrinsics_bin_sub_assign_u16_u16, ptr @.str.41, ptr @.str.1, i32 1243, ptr null }, { ptr, ptr, ptr, i32, ptr } { ptr @logia_intrinsics_bin_mul_assign_u16_u16, ptr @.str.42, ptr @.str.1, i32 1250, ptr null }, { ptr, ptr, ptr, i32, ptr } { ptr @logia_intrinsics_bin_div_assign_u16_u16, ptr @.str.43, ptr @.str.1, i32 1257, ptr null }, { ptr, ptr, ptr, i32, ptr } { ptr @logia_intrinsics_bin_mod_assign_u16_u16, ptr @.str.44, ptr @.str.1, i32 1264, ptr null }, { ptr, ptr, ptr, i32, ptr } { ptr @logia_intrinsics_bin_bitwise_and_assign_u16_u16, ptr @.str.45, ptr @.str.1, i32 1271, ptr null }, { ptr, ptr, ptr, i32, ptr } { ptr @logia_intrinsics_bin_bitwise_or_assign_u16_u16, ptr @.str.46, ptr @.str.1, i32 1278, ptr null }, { ptr, ptr, ptr, i32, ptr } { ptr @logia_intrinsics_bin_bitwise_xor_assign_u16_u16, ptr @.str.47, ptr @.str.1, i32 1285, ptr null }, { ptr, ptr, ptr, i32, ptr } { ptr @logia_intrinsics_bin_bitwise_shr_assign_u16_u16, ptr @.str.48, ptr @.str.1, i32 1292, ptr null }, { ptr, ptr, ptr, i32, ptr } { ptr @logia_intrinsics_bin_bitwise_shl_assign_u16_u16, ptr @.str.49, ptr @.str.1, i32 1299, ptr null }, { ptr, ptr, ptr, i32, ptr } { ptr @logia_intrinsics_bin_bitwise_and_u16_u16, ptr @.str.50, ptr @.str.1, i32 1306, ptr null }, { ptr, ptr, ptr, i32, ptr } { ptr @logia_intrinsics_bin_bitwise_or_u16_u16, ptr @.str.51, ptr @.str.1, i32 1312, ptr null }, { ptr, ptr, ptr, i32, ptr } { ptr @logia_intrinsics_bin_bitwise_xor_u16_u16, ptr @.str.52, ptr @.str.1, i32 1318, ptr null }, { ptr, ptr, ptr, i32, ptr } { ptr @logia_intrinsics_bin_bitwise_shl_u16_u16, ptr @.str.53, ptr @.str.1, i32 1324, ptr null }, { ptr, ptr, ptr, i32, ptr } { ptr @logia_intrinsics_bin_bitwise_shr_u16_u16, ptr @.str.54, ptr @.str.1, i32 1330, ptr null }, { ptr, ptr, ptr, i32, ptr } { ptr @logia__intrinsics_bin_add_u32_u32, ptr @.str.28, ptr @.str.1, i32 1336, ptr null }, { ptr, ptr, ptr, i32, ptr } { ptr @logia_intrinsics_bin_sub_u32_u32, ptr @.str.29, ptr @.str.1, i32 1342, ptr null }, { ptr, ptr, ptr, i32, ptr } { ptr @logia_intrinsics_bin_mul_u32_u32, ptr @.str.30, ptr @.str.1, i32 1348, ptr null }, { ptr, ptr, ptr, i32, ptr } { ptr @logia_intrinsics_bin_div_u32_u32, ptr @.str.31, ptr @.str.1, i32 1354, ptr null }, { ptr, ptr, ptr, i32, ptr } { ptr @logia_intrinsics_bin_mod_u32_u32, ptr @.str.32, ptr @.str.1, i32 1360, ptr null }, { ptr, ptr, ptr, i32, ptr } { ptr @logia_intrinsics_bin_eq_u32_u32, ptr @.str.33, ptr @.str.1, i32 1366, ptr null }, { ptr, ptr, ptr, i32, ptr } { ptr @logia_intrinsics_bin_neq_u32_u32, ptr @.str.34, ptr @.str.1, i32 1372, ptr null }, { ptr, ptr, ptr, i32, ptr } { ptr @logia_intrinsics_bin_lt_u32_u32, ptr @.str.35, ptr @.str.1, i32 1378, ptr null }, { ptr, ptr, ptr, i32, ptr } { ptr @logia_intrinsics_bin_gt_u32_u32, ptr @.str.36, ptr @.str.1, i32 1384, ptr null }, { ptr, ptr, ptr, i32, ptr } { ptr @logia_intrinsics_bin_lte_u32_u32, ptr @.str.37, ptr @.str.1, i32 1390, ptr null }, { ptr, ptr, ptr, i32, ptr } { ptr @logia_intrinsics_bin_gte_u32_u32, ptr @.str.38, ptr @.str.1, i32 1396, ptr null }, { ptr, ptr, ptr, i32, ptr } { ptr @logia_intrinsics_bin_assign_u32_u32, ptr @.str.39, ptr @.str.1, i32 1402, ptr null }, { ptr, ptr, ptr, i32, ptr } { ptr @logia_intrinsics_bin_add_assign_u32_u32, ptr @.str.40, ptr @.str.1, i32 1409, ptr null }, { ptr, ptr, ptr, i32, ptr } { ptr @logia_intrinsics_bin_sub_assign_u32_u32, ptr @.str.41, ptr @.str.1, i32 1416, ptr null }, { ptr, ptr, ptr, i32, ptr } { ptr @logia_intrinsics_bin_mul_assign_u32_u32, ptr @.str.42, ptr @.str.1, i32 1423, ptr null }, { ptr, ptr, ptr, i32, ptr } { ptr @logia_intrinsics_bin_div_assign_u32_u32, ptr @.str.43, ptr @.str.1, i32 1430, ptr null }, { ptr, ptr, ptr, i32, ptr } { ptr @logia_intrinsics_bin_mod_assign_u32_u32, ptr @.str.44, ptr @.str.1, i32 1437, ptr null }, { ptr, ptr, ptr, i32, ptr } { ptr @logia_intrinsics_bin_bitwise_and_assign_u32_u32, ptr @.str.45, ptr @.str.1, i32 1444, ptr null }, { ptr, ptr, ptr, i32, ptr } { ptr @logia_intrinsics_bin_bitwise_or_assign_u32_u32, ptr @.str.46, ptr @.str.1, i32 1451, ptr null }, { ptr, ptr, ptr, i32, ptr } { ptr @logia_intrinsics_bin_bitwise_xor_assign_u32_u32, ptr @.str.47, ptr @.str.1, i32 1458, ptr null }, { ptr, ptr, ptr, i32, ptr } { ptr @logia_intrinsics_bin_bitwise_shr_assign_u32_u32, ptr @.str.48, ptr @.str.1, i32 1465, ptr null }, { ptr, ptr, ptr, i32, ptr } { ptr @logia_intrinsics_bin_bitwise_shl_assign_u32_u32, ptr @.str.49, ptr @.str.1, i32 1472, ptr null }, { ptr, ptr, ptr, i32, ptr } { ptr @logia_intrinsics_bin_bitwise_and_u32_u32, ptr @.str.50, ptr @.str.1, i32 1479, ptr null }, { ptr, ptr, ptr, i32, ptr } { ptr @logia_intrinsics_bin_bitwise_or_u32_u32, ptr @.str.51, ptr @.str.1, i32 1485, ptr null }, { ptr, ptr, ptr, i32, ptr } { ptr @logia_intrinsics_bin_bitwise_xor_u32_u32, ptr @.str.52, ptr @.str.1, i32 1491, ptr null }, { ptr, ptr, ptr, i32, ptr } { ptr @logia_intrinsics_bin_bitwise_shl_u32_u32, ptr @.str.53, ptr @.str.1, i32 1497, ptr null }, { ptr, ptr, ptr, i32, ptr } { ptr @logia_intrinsics_bin_bitwise_shr_u32_u32, ptr @.str.54, ptr @.str.1, i32 1503, ptr null }, { ptr, ptr, ptr, i32, ptr } { ptr @logia__intrinsics_bin_add_u64_u64, ptr @.str.28, ptr @.str.1, i32 1509, ptr null }, { ptr, ptr, ptr, i32, ptr } { ptr @logia_intrinsics_bin_sub_u64_u64, ptr @.str.29, ptr @.str.1, i32 1515, ptr null }, { ptr, ptr, ptr, i32, ptr } { ptr @logia_intrinsics_bin_mul_u64_u64, ptr @.str.30, ptr @.str.1, i32 1521, ptr null }, { ptr, ptr, ptr, i32, ptr } { ptr @logia_intrinsics_bin_div_u64_u64, ptr @.str.31, ptr @.str.1, i32 1527, ptr null }, { ptr, ptr, ptr, i32, ptr } { ptr @logia_intrinsics_bin_mod_u64_u64, ptr @.str.32, ptr @.str.1, i32 1533, ptr null }, { ptr, ptr, ptr, i32, ptr } { ptr @logia_intrinsics_bin_eq_u64_u64, ptr @.str.33, ptr @.str.1, i32 1539, ptr null }, { ptr, ptr, ptr, i32, ptr } { ptr @logia_intrinsics_bin_neq_u64_u64, ptr @.str.34, ptr @.str.1, i32 1545, ptr null }, { ptr, ptr, ptr, i32, ptr } { ptr @logia_intrinsics_bin_lt_u64_u64, ptr @.str.35, ptr @.str.1, i32 1551, ptr null }, { ptr, ptr, ptr, i32, ptr } { ptr @logia_intrinsics_bin_gt_u64_u64, ptr @.str.36, ptr @.str.1, i32 1557, ptr null }, { ptr, ptr, ptr, i32, ptr } { ptr @logia_intrinsics_bin_lte_u64_u64, ptr @.str.37, ptr @.str.1, i32 1563, ptr null }, { ptr, ptr, ptr, i32, ptr } { ptr @logia_intrinsics_bin_gte_u64_u64, ptr @.str.38, ptr @.str.1, i32 1569, ptr null }, { ptr, ptr, ptr, i32, ptr } { ptr @logia_intrinsics_bin_assign_u64_u64, ptr @.str.39, ptr @.str.1, i32 1575, ptr null }, { ptr, ptr, ptr, i32, ptr } { ptr @logia_intrinsics_bin_add_assign_u64_u64, ptr @.str.40, ptr @.str.1, i32 1582, ptr null }, { ptr, ptr, ptr, i32, ptr } { ptr @logia_intrinsics_bin_sub_assign_u64_u64, ptr @.str.41, ptr @.str.1, i32 1589, ptr null }, { ptr, ptr, ptr, i32, ptr } { ptr @logia_intrinsics_bin_mul_assign_u64_u64, ptr @.str.42, ptr @.str.1, i32 1596, ptr null }, { ptr, ptr, ptr, i32, ptr } { ptr @logia_intrinsics_bin_div_assign_u64_u64, ptr @.str.43, ptr @.str.1, i32 1603, ptr null }, { ptr, ptr, ptr, i32, ptr } { ptr @logia_intrinsics_bin_mod_assign_u64_u64, ptr @.str.44, ptr @.str.1, i32 1610, ptr null }, { ptr, ptr, ptr, i32, ptr } { ptr @logia_intrinsics_bin_bitwise_and_assign_u64_u64, ptr @.str.45, ptr @.str.1, i32 1617, ptr null }, { ptr, ptr, ptr, i32, ptr } { ptr @logia_intrinsics_bin_bitwise_or_assign_u64_u64, ptr @.str.46, ptr @.str.1, i32 1624, ptr null }, { ptr, ptr, ptr, i32, ptr } { ptr @logia_intrinsics_bin_bitwise_xor_assign_u64_u64, ptr @.str.47, ptr @.str.1, i32 1631, ptr null }, { ptr, ptr, ptr, i32, ptr } { ptr @logia_intrinsics_bin_bitwise_shr_assign_u64_u64, ptr @.str.48, ptr @.str.1, i32 1638, ptr null }, { ptr, ptr, ptr, i32, ptr } { ptr @logia_intrinsics_bin_bitwise_shl_assign_u64_u64, ptr @.str.49, ptr @.str.1, i32 1645, ptr null }, { ptr, ptr, ptr, i32, ptr } { ptr @logia_intrinsics_bin_bitwise_and_u64_u64, ptr @.str.50, ptr @.str.1, i32 1652, ptr null }, { ptr, ptr, ptr, i32, ptr } { ptr @logia_intrinsics_bin_bitwise_or_u64_u64, ptr @.str.51, ptr @.str.1, i32 1658, ptr null }, { ptr, ptr, ptr, i32, ptr } { ptr @logia_intrinsics_bin_bitwise_xor_u64_u64, ptr @.str.52, ptr @.str.1, i32 1664, ptr null }, { ptr, ptr, ptr, i32, ptr } { ptr @logia_intrinsics_bin_bitwise_shl_u64_u64, ptr @.str.53, ptr @.str.1, i32 1670, ptr null }, { ptr, ptr, ptr, i32, ptr } { ptr @logia_intrinsics_bin_bitwise_shr_u64_u64, ptr @.str.54, ptr @.str.1, i32 1676, ptr null }, { ptr, ptr, ptr, i32, ptr } { ptr @logia__intrinsics_bin_add_f32_f32, ptr @.str.28, ptr @.str.1, i32 1683, ptr null }, { ptr, ptr, ptr, i32, ptr } { ptr @logia_intrinsics_bin_sub_f32_f32, ptr @.str.29, ptr @.str.1, i32 1689, ptr null }, { ptr, ptr, ptr, i32, ptr } { ptr @logia_intrinsics_bin_mul_f32_f32, ptr @.str.30, ptr @.str.1, i32 1695, ptr null }, { ptr, ptr, ptr, i32, ptr } { ptr @logia_intrinsics_bin_div_f32_f32, ptr @.str.31, ptr @.str.1, i32 1701, ptr null }, { ptr, ptr, ptr, i32, ptr } { ptr @logia_intrinsics_bin_eq_f32_f32, ptr @.str.33, ptr @.str.1, i32 1707, ptr null }, { ptr, ptr, ptr, i32, ptr } { ptr @logia_intrinsics_bin_neq_f32_f32, ptr @.str.34, ptr @.str.1, i32 1713, ptr null }, { ptr, ptr, ptr, i32, ptr } { ptr @logia_intrinsics_bin_lt_f32_f32, ptr @.str.35, ptr @.str.1, i32 1719, ptr null }, { ptr, ptr, ptr, i32, ptr } { ptr @logia_intrinsics_bin_lte_f32_f32, ptr @.str.37, ptr @.str.1, i32 1725, ptr null }, { ptr, ptr, ptr, i32, ptr } { ptr @logia_intrinsics_bin_gte_f32_f32, ptr @.str.38, ptr @.str.1, i32 1731, ptr null }, { ptr, ptr, ptr, i32, ptr } { ptr @logia_intrinsics_bin_assign_f32_f32, ptr @.str.39, ptr @.str.1, i32 1737, ptr null }, { ptr, ptr, ptr, i32, ptr } { ptr @logia_intrinsics_bin_add_assign_f32_f32, ptr @.str.40, ptr @.str.1, i32 1744, ptr null }, { ptr, ptr, ptr, i32, ptr } { ptr @logia_intrinsics_bin_sub_assign_f32_f32, ptr @.str.41, ptr @.str.1, i32 1751, ptr null }, { ptr, ptr, ptr, i32, ptr } { ptr @logia_intrinsics_bin_mul_assign_f32_f32, ptr @.str.42, ptr @.str.1, i32 1758, ptr null }, { ptr, ptr, ptr, i32, ptr } { ptr @logia_intrinsics_bin_div_assign_f32_f32, ptr @.str.43, ptr @.str.1, i32 1765, ptr null }, { ptr, ptr, ptr, i32, ptr } { ptr @logia__intrinsics_bin_add_f64_f64, ptr @.str.28, ptr @.str.1, i32 1772, ptr null }, { ptr, ptr, ptr, i32, ptr } { ptr @logia_intrinsics_bin_sub_f64_f64, ptr @.str.29, ptr @.str.1, i32 1778, ptr null }, { ptr, ptr, ptr, i32, ptr } { ptr @logia_intrinsics_bin_mul_f64_f64, ptr @.str.30, ptr @.str.1, i32 1784, ptr null }, { ptr, ptr, ptr, i32, ptr } { ptr @logia_intrinsics_bin_div_f64_f64, ptr @.str.31, ptr @.str.1, i32 1790, ptr null }, { ptr, ptr, ptr, i32, ptr } { ptr @logia_intrinsics_bin_eq_f64_f64, ptr @.str.33, ptr @.str.1, i32 1796, ptr null }, { ptr, ptr, ptr, i32, ptr } { ptr @logia_intrinsics_bin_neq_f64_f64, ptr @.str.34, ptr @.str.1, i32 1802, ptr null }, { ptr, ptr, ptr, i32, ptr } { ptr @logia_intrinsics_bin_lt_f64_f64, ptr @.str.35, ptr @.str.1, i32 1808, ptr null }, { ptr, ptr, ptr, i32, ptr } { ptr @logia_intrinsics_bin_lte_f64_f64, ptr @.str.37, ptr @.str.1, i32 1814, ptr null }, { ptr, ptr, ptr, i32, ptr } { ptr @logia_intrinsics_bin_gte_f64_f64, ptr @.str.38, ptr @.str.1, i32 1820, ptr null }, { ptr, ptr, ptr, i32, ptr } { ptr @logia_intrinsics_bin_assign_f64_f64, ptr @.str.39, ptr @.str.1, i32 1826, ptr null }, { ptr, ptr, ptr, i32, ptr } { ptr @logia_intrinsics_bin_add_assign_f64_f64, ptr @.str.40, ptr @.str.1, i32 1833, ptr null }, { ptr, ptr, ptr, i32, ptr } { ptr @logia_intrinsics_bin_sub_assign_f64_f64, ptr @.str.41, ptr @.str.1, i32 1840, ptr null }, { ptr, ptr, ptr, i32, ptr } { ptr @logia_intrinsics_bin_mul_assign_f64_f64, ptr @.str.42, ptr @.str.1, i32 1847, ptr null }, { ptr, ptr, ptr, i32, ptr } { ptr @logia_intrinsics_bin_div_assign_f64_f64, ptr @.str.43, ptr @.str.1, i32 1854, ptr null }], section "llvm.metadata"

; Function Attrs: mustprogress noinline optnone uwtable
define dso_local ptr @logia_get_stdout() #0 {
  %1 = call ptr @__acrt_iob_func(i32 noundef 1)
  ret ptr %1
}

declare dso_local ptr @__acrt_iob_func(i32 noundef) #1

; Function Attrs: mustprogress noinline optnone uwtable
define dso_local ptr @logia_get_stdin() #0 {
  %1 = call ptr @__acrt_iob_func(i32 noundef 0)
  ret ptr %1
}

; Function Attrs: mustprogress noinline optnone uwtable
define dso_local ptr @logia_get_stderr() #0 {
  %1 = call ptr @__acrt_iob_func(i32 noundef 2)
  ret ptr %1
}

; Function Attrs: mustprogress noinline optnone uwtable
define dso_local void @logia_print_stdout(ptr noundef %0) #0 {
  %2 = alloca ptr, align 8
  store ptr %0, ptr %2, align 8
  %3 = call ptr @__acrt_iob_func(i32 noundef 1)
  %4 = load ptr, ptr %2, align 8
  %5 = call i32 @fputs(ptr noundef %4, ptr noundef %3)
  ret void
}

declare dso_local i32 @fputs(ptr noundef, ptr noundef) #1

; Function Attrs: mustprogress noinline optnone uwtable
define dso_local void @logia_fwrite(ptr noundef %0, i64 noundef %1, ptr noundef %2) #0 {
  %4 = alloca ptr, align 8
  %5 = alloca i64, align 8
  %6 = alloca ptr, align 8
  store ptr %2, ptr %4, align 8
  store i64 %1, ptr %5, align 8
  store ptr %0, ptr %6, align 8
  %7 = load ptr, ptr %4, align 8
  %8 = load i64, ptr %5, align 8
  %9 = load ptr, ptr %6, align 8
  %10 = call i64 @fwrite(ptr noundef %9, i64 noundef 1, i64 noundef %8, ptr noundef %7)
  ret void
}

declare dso_local i64 @fwrite(ptr noundef, i64 noundef, i64 noundef, ptr noundef) #1

; Function Attrs: mustprogress noinline nounwind optnone uwtable
define dso_local i8 @logia_intrinsics_prefix_neg_i8(i8 noundef %0) #2 {
  %2 = alloca i8, align 1
  store i8 %0, ptr %2, align 1
  call void @llvm.var.annotation.p0.p0(ptr %2, ptr @.str, ptr @.str.1, i32 56, ptr null)
  %3 = load i8, ptr %2, align 1
  %4 = sext i8 %3 to i32
  %5 = sub nsw i32 0, %4
  %6 = trunc i32 %5 to i8
  ret i8 %6
}

; Function Attrs: nocallback nofree nosync nounwind willreturn memory(inaccessiblemem: readwrite)
declare void @llvm.var.annotation.p0.p0(ptr, ptr, ptr, i32, ptr) #3

; Function Attrs: mustprogress noinline nounwind optnone uwtable
define dso_local i16 @logia_intrinsics_prefix_neg_i16(i16 noundef %0) #2 {
  %2 = alloca i16, align 2
  store i16 %0, ptr %2, align 2
  call void @llvm.var.annotation.p0.p0(ptr %2, ptr @.str.2, ptr @.str.1, i32 62, ptr null)
  %3 = load i16, ptr %2, align 2
  %4 = sext i16 %3 to i32
  %5 = sub nsw i32 0, %4
  %6 = trunc i32 %5 to i16
  ret i16 %6
}

; Function Attrs: mustprogress noinline nounwind optnone uwtable
define dso_local i32 @logia_intrinsics_prefix_neg_i32(i32 noundef %0) #2 {
  %2 = alloca i32, align 4
  store i32 %0, ptr %2, align 4
  call void @llvm.var.annotation.p0.p0(ptr %2, ptr @.str.3, ptr @.str.1, i32 68, ptr null)
  %3 = load i32, ptr %2, align 4
  %4 = sub nsw i32 0, %3
  ret i32 %4
}

; Function Attrs: mustprogress noinline nounwind optnone uwtable
define dso_local i64 @logia_intrinsics_prefix_neg_i64(i64 noundef %0) #2 {
  %2 = alloca i64, align 8
  store i64 %0, ptr %2, align 8
  call void @llvm.var.annotation.p0.p0(ptr %2, ptr @.str.4, ptr @.str.1, i32 74, ptr null)
  %3 = load i64, ptr %2, align 8
  %4 = sub nsw i64 0, %3
  ret i64 %4
}

; Function Attrs: mustprogress noinline nounwind optnone uwtable
define dso_local i8 @logia_intrinsics_prefix_neg_u8(i8 noundef %0) #2 {
  %2 = alloca i8, align 1
  store i8 %0, ptr %2, align 1
  call void @llvm.var.annotation.p0.p0(ptr %2, ptr @.str.5, ptr @.str.1, i32 80, ptr null)
  %3 = load i8, ptr %2, align 1
  %4 = zext i8 %3 to i32
  %5 = sub nsw i32 0, %4
  %6 = trunc i32 %5 to i8
  ret i8 %6
}

; Function Attrs: mustprogress noinline nounwind optnone uwtable
define dso_local i16 @logia_intrinsics_prefix_neg_u16(i16 noundef %0) #2 {
  %2 = alloca i16, align 2
  store i16 %0, ptr %2, align 2
  call void @llvm.var.annotation.p0.p0(ptr %2, ptr @.str.6, ptr @.str.1, i32 86, ptr null)
  %3 = load i16, ptr %2, align 2
  %4 = zext i16 %3 to i32
  %5 = sub nsw i32 0, %4
  %6 = trunc i32 %5 to i16
  ret i16 %6
}

; Function Attrs: mustprogress noinline nounwind optnone uwtable
define dso_local i32 @logia_intrinsics_prefix_neg_u32(i32 noundef %0) #2 {
  %2 = alloca i32, align 4
  store i32 %0, ptr %2, align 4
  call void @llvm.var.annotation.p0.p0(ptr %2, ptr @.str.7, ptr @.str.1, i32 92, ptr null)
  %3 = load i32, ptr %2, align 4
  %4 = sub i32 0, %3
  ret i32 %4
}

; Function Attrs: mustprogress noinline nounwind optnone uwtable
define dso_local i64 @logia_intrinsics_prefix_neg_u64(i64 noundef %0) #2 {
  %2 = alloca i64, align 8
  store i64 %0, ptr %2, align 8
  call void @llvm.var.annotation.p0.p0(ptr %2, ptr @.str.8, ptr @.str.1, i32 98, ptr null)
  %3 = load i64, ptr %2, align 8
  %4 = sub i64 0, %3
  ret i64 %4
}

; Function Attrs: mustprogress noinline nounwind optnone uwtable
define dso_local i8 @logia_intrinsics_prefix_bitwise_not_i8(i8 noundef %0) #2 {
  %2 = alloca i8, align 1
  store i8 %0, ptr %2, align 1
  call void @llvm.var.annotation.p0.p0(ptr %2, ptr @.str, ptr @.str.1, i32 104, ptr null)
  %3 = load i8, ptr %2, align 1
  %4 = sext i8 %3 to i32
  %5 = xor i32 %4, -1
  %6 = trunc i32 %5 to i8
  ret i8 %6
}

; Function Attrs: mustprogress noinline nounwind optnone uwtable
define dso_local i16 @logia_intrinsics_prefix_bitwise_not_i16(i16 noundef %0) #2 {
  %2 = alloca i16, align 2
  store i16 %0, ptr %2, align 2
  call void @llvm.var.annotation.p0.p0(ptr %2, ptr @.str.2, ptr @.str.1, i32 110, ptr null)
  %3 = load i16, ptr %2, align 2
  %4 = sext i16 %3 to i32
  %5 = xor i32 %4, -1
  %6 = trunc i32 %5 to i16
  ret i16 %6
}

; Function Attrs: mustprogress noinline nounwind optnone uwtable
define dso_local i32 @logia_intrinsics_prefix_bitwise_not_i32(i32 noundef %0) #2 {
  %2 = alloca i32, align 4
  store i32 %0, ptr %2, align 4
  call void @llvm.var.annotation.p0.p0(ptr %2, ptr @.str.3, ptr @.str.1, i32 116, ptr null)
  %3 = load i32, ptr %2, align 4
  %4 = xor i32 %3, -1
  ret i32 %4
}

; Function Attrs: mustprogress noinline nounwind optnone uwtable
define dso_local i64 @logia_intrinsics_prefix_bitwise_not_i64(i64 noundef %0) #2 {
  %2 = alloca i64, align 8
  store i64 %0, ptr %2, align 8
  call void @llvm.var.annotation.p0.p0(ptr %2, ptr @.str.4, ptr @.str.1, i32 122, ptr null)
  %3 = load i64, ptr %2, align 8
  %4 = xor i64 %3, -1
  ret i64 %4
}

; Function Attrs: mustprogress noinline nounwind optnone uwtable
define dso_local i8 @logia_intrinsics_prefix_bitwise_not_u8(i8 noundef %0) #2 {
  %2 = alloca i8, align 1
  store i8 %0, ptr %2, align 1
  call void @llvm.var.annotation.p0.p0(ptr %2, ptr @.str.5, ptr @.str.1, i32 128, ptr null)
  %3 = load i8, ptr %2, align 1
  %4 = zext i8 %3 to i32
  %5 = xor i32 %4, -1
  %6 = trunc i32 %5 to i8
  ret i8 %6
}

; Function Attrs: mustprogress noinline nounwind optnone uwtable
define dso_local i16 @logia_intrinsics_prefix_bitwise_not_u16(i16 noundef %0) #2 {
  %2 = alloca i16, align 2
  store i16 %0, ptr %2, align 2
  call void @llvm.var.annotation.p0.p0(ptr %2, ptr @.str.6, ptr @.str.1, i32 134, ptr null)
  %3 = load i16, ptr %2, align 2
  %4 = zext i16 %3 to i32
  %5 = xor i32 %4, -1
  %6 = trunc i32 %5 to i16
  ret i16 %6
}

; Function Attrs: mustprogress noinline nounwind optnone uwtable
define dso_local i32 @logia_intrinsics_prefix_bitwise_not_u32(i32 noundef %0) #2 {
  %2 = alloca i32, align 4
  store i32 %0, ptr %2, align 4
  call void @llvm.var.annotation.p0.p0(ptr %2, ptr @.str.7, ptr @.str.1, i32 140, ptr null)
  %3 = load i32, ptr %2, align 4
  %4 = xor i32 %3, -1
  ret i32 %4
}

; Function Attrs: mustprogress noinline nounwind optnone uwtable
define dso_local i64 @logia_intrinsics_prefix_bitwise_not_u64(i64 noundef %0) #2 {
  %2 = alloca i64, align 8
  store i64 %0, ptr %2, align 8
  call void @llvm.var.annotation.p0.p0(ptr %2, ptr @.str.8, ptr @.str.1, i32 146, ptr null)
  %3 = load i64, ptr %2, align 8
  %4 = xor i64 %3, -1
  ret i64 %4
}

; Function Attrs: mustprogress noinline nounwind optnone uwtable
define dso_local zeroext i1 @logia_intrinsics_prefix_logical_not_i8(i8 noundef %0) #2 {
  %2 = alloca i8, align 1
  store i8 %0, ptr %2, align 1
  call void @llvm.var.annotation.p0.p0(ptr %2, ptr @.str, ptr @.str.1, i32 152, ptr null)
  %3 = load i8, ptr %2, align 1
  %4 = icmp ne i8 %3, 0
  %5 = xor i1 %4, true
  ret i1 %5
}

; Function Attrs: mustprogress noinline nounwind optnone uwtable
define dso_local zeroext i1 @logia_intrinsics_prefix_logical_not_i16(i16 noundef %0) #2 {
  %2 = alloca i16, align 2
  store i16 %0, ptr %2, align 2
  call void @llvm.var.annotation.p0.p0(ptr %2, ptr @.str.2, ptr @.str.1, i32 158, ptr null)
  %3 = load i16, ptr %2, align 2
  %4 = icmp ne i16 %3, 0
  %5 = xor i1 %4, true
  ret i1 %5
}

; Function Attrs: mustprogress noinline nounwind optnone uwtable
define dso_local zeroext i1 @logia_intrinsics_prefix_logical_not_i32(i32 noundef %0) #2 {
  %2 = alloca i32, align 4
  store i32 %0, ptr %2, align 4
  call void @llvm.var.annotation.p0.p0(ptr %2, ptr @.str.3, ptr @.str.1, i32 164, ptr null)
  %3 = load i32, ptr %2, align 4
  %4 = icmp ne i32 %3, 0
  %5 = xor i1 %4, true
  ret i1 %5
}

; Function Attrs: mustprogress noinline nounwind optnone uwtable
define dso_local zeroext i1 @logia_intrinsics_prefix_logical_not_i64(i64 noundef %0) #2 {
  %2 = alloca i64, align 8
  store i64 %0, ptr %2, align 8
  call void @llvm.var.annotation.p0.p0(ptr %2, ptr @.str.4, ptr @.str.1, i32 170, ptr null)
  %3 = load i64, ptr %2, align 8
  %4 = icmp ne i64 %3, 0
  %5 = xor i1 %4, true
  ret i1 %5
}

; Function Attrs: mustprogress noinline nounwind optnone uwtable
define dso_local zeroext i1 @logia_intrinsics_prefix_logical_not_u8(i8 noundef %0) #2 {
  %2 = alloca i8, align 1
  store i8 %0, ptr %2, align 1
  call void @llvm.var.annotation.p0.p0(ptr %2, ptr @.str.5, ptr @.str.1, i32 176, ptr null)
  %3 = load i8, ptr %2, align 1
  %4 = icmp ne i8 %3, 0
  %5 = xor i1 %4, true
  ret i1 %5
}

; Function Attrs: mustprogress noinline nounwind optnone uwtable
define dso_local zeroext i1 @logia_intrinsics_prefix_logical_not_u16(i16 noundef %0) #2 {
  %2 = alloca i16, align 2
  store i16 %0, ptr %2, align 2
  call void @llvm.var.annotation.p0.p0(ptr %2, ptr @.str.6, ptr @.str.1, i32 182, ptr null)
  %3 = load i16, ptr %2, align 2
  %4 = icmp ne i16 %3, 0
  %5 = xor i1 %4, true
  ret i1 %5
}

; Function Attrs: mustprogress noinline nounwind optnone uwtable
define dso_local zeroext i1 @logia_intrinsics_prefix_logical_not_u32(i32 noundef %0) #2 {
  %2 = alloca i32, align 4
  store i32 %0, ptr %2, align 4
  call void @llvm.var.annotation.p0.p0(ptr %2, ptr @.str.7, ptr @.str.1, i32 188, ptr null)
  %3 = load i32, ptr %2, align 4
  %4 = icmp ne i32 %3, 0
  %5 = xor i1 %4, true
  ret i1 %5
}

; Function Attrs: mustprogress noinline nounwind optnone uwtable
define dso_local zeroext i1 @logia_intrinsics_prefix_logical_not_u64(i64 noundef %0) #2 {
  %2 = alloca i64, align 8
  store i64 %0, ptr %2, align 8
  call void @llvm.var.annotation.p0.p0(ptr %2, ptr @.str.8, ptr @.str.1, i32 194, ptr null)
  %3 = load i64, ptr %2, align 8
  %4 = icmp ne i64 %3, 0
  %5 = xor i1 %4, true
  ret i1 %5
}

; Function Attrs: mustprogress noinline nounwind optnone uwtable
define dso_local i8 @logia_intrinsics_prefix_inc_i8(ptr noundef %0) #2 {
  %2 = alloca ptr, align 8
  store ptr %0, ptr %2, align 8
  call void @llvm.var.annotation.p0.p0(ptr %2, ptr @.str.9, ptr @.str.1, i32 200, ptr null)
  %3 = load ptr, ptr %2, align 8
  %4 = load i8, ptr %3, align 1
  %5 = add i8 %4, 1
  store i8 %5, ptr %3, align 1
  ret i8 %5
}

; Function Attrs: mustprogress noinline nounwind optnone uwtable
define dso_local i16 @logia_intrinsics_prefix_inc_i16(ptr noundef %0) #2 {
  %2 = alloca ptr, align 8
  store ptr %0, ptr %2, align 8
  call void @llvm.var.annotation.p0.p0(ptr %2, ptr @.str.10, ptr @.str.1, i32 206, ptr null)
  %3 = load ptr, ptr %2, align 8
  %4 = load i16, ptr %3, align 2
  %5 = add i16 %4, 1
  store i16 %5, ptr %3, align 2
  ret i16 %5
}

; Function Attrs: mustprogress noinline nounwind optnone uwtable
define dso_local i32 @logia_intrinsics_prefix_inc_i32(ptr noundef %0) #2 {
  %2 = alloca ptr, align 8
  store ptr %0, ptr %2, align 8
  call void @llvm.var.annotation.p0.p0(ptr %2, ptr @.str.11, ptr @.str.1, i32 212, ptr null)
  %3 = load ptr, ptr %2, align 8
  %4 = load i32, ptr %3, align 4
  %5 = add nsw i32 %4, 1
  store i32 %5, ptr %3, align 4
  ret i32 %5
}

; Function Attrs: mustprogress noinline nounwind optnone uwtable
define dso_local i64 @logia_intrinsics_prefix_inc_i64(ptr noundef %0) #2 {
  %2 = alloca ptr, align 8
  store ptr %0, ptr %2, align 8
  call void @llvm.var.annotation.p0.p0(ptr %2, ptr @.str.12, ptr @.str.1, i32 218, ptr null)
  %3 = load ptr, ptr %2, align 8
  %4 = load i64, ptr %3, align 8
  %5 = add nsw i64 %4, 1
  store i64 %5, ptr %3, align 8
  ret i64 %5
}

; Function Attrs: mustprogress noinline nounwind optnone uwtable
define dso_local i8 @logia_intrinsics_prefix_inc_u8(ptr noundef %0) #2 {
  %2 = alloca ptr, align 8
  store ptr %0, ptr %2, align 8
  call void @llvm.var.annotation.p0.p0(ptr %2, ptr @.str.13, ptr @.str.1, i32 224, ptr null)
  %3 = load ptr, ptr %2, align 8
  %4 = load i8, ptr %3, align 1
  %5 = add i8 %4, 1
  store i8 %5, ptr %3, align 1
  ret i8 %5
}

; Function Attrs: mustprogress noinline nounwind optnone uwtable
define dso_local i16 @logia_intrinsics_prefix_inc_u16(ptr noundef %0) #2 {
  %2 = alloca ptr, align 8
  store ptr %0, ptr %2, align 8
  call void @llvm.var.annotation.p0.p0(ptr %2, ptr @.str.14, ptr @.str.1, i32 230, ptr null)
  %3 = load ptr, ptr %2, align 8
  %4 = load i16, ptr %3, align 2
  %5 = add i16 %4, 1
  store i16 %5, ptr %3, align 2
  ret i16 %5
}

; Function Attrs: mustprogress noinline nounwind optnone uwtable
define dso_local i32 @logia_intrinsics_prefix_inc_u32(ptr noundef %0) #2 {
  %2 = alloca ptr, align 8
  store ptr %0, ptr %2, align 8
  call void @llvm.var.annotation.p0.p0(ptr %2, ptr @.str.15, ptr @.str.1, i32 236, ptr null)
  %3 = load ptr, ptr %2, align 8
  %4 = load i32, ptr %3, align 4
  %5 = add i32 %4, 1
  store i32 %5, ptr %3, align 4
  ret i32 %5
}

; Function Attrs: mustprogress noinline nounwind optnone uwtable
define dso_local i64 @logia_intrinsics_prefix_inc_u64(ptr noundef %0) #2 {
  %2 = alloca ptr, align 8
  store ptr %0, ptr %2, align 8
  call void @llvm.var.annotation.p0.p0(ptr %2, ptr @.str.16, ptr @.str.1, i32 242, ptr null)
  %3 = load ptr, ptr %2, align 8
  %4 = load i64, ptr %3, align 8
  %5 = add i64 %4, 1
  store i64 %5, ptr %3, align 8
  ret i64 %5
}

; Function Attrs: mustprogress noinline nounwind optnone uwtable
define dso_local i8 @logia_intrinsics_prefix_dec_i8(ptr noundef %0) #2 {
  %2 = alloca ptr, align 8
  store ptr %0, ptr %2, align 8
  call void @llvm.var.annotation.p0.p0(ptr %2, ptr @.str.9, ptr @.str.1, i32 248, ptr null)
  %3 = load ptr, ptr %2, align 8
  %4 = load i8, ptr %3, align 1
  %5 = add i8 %4, -1
  store i8 %5, ptr %3, align 1
  ret i8 %5
}

; Function Attrs: mustprogress noinline nounwind optnone uwtable
define dso_local i16 @logia_intrinsics_prefix_dec_i16(ptr noundef %0) #2 {
  %2 = alloca ptr, align 8
  store ptr %0, ptr %2, align 8
  call void @llvm.var.annotation.p0.p0(ptr %2, ptr @.str.10, ptr @.str.1, i32 254, ptr null)
  %3 = load ptr, ptr %2, align 8
  %4 = load i16, ptr %3, align 2
  %5 = add i16 %4, -1
  store i16 %5, ptr %3, align 2
  ret i16 %5
}

; Function Attrs: mustprogress noinline nounwind optnone uwtable
define dso_local i32 @logia_intrinsics_prefix_dec_i32(ptr noundef %0) #2 {
  %2 = alloca ptr, align 8
  store ptr %0, ptr %2, align 8
  call void @llvm.var.annotation.p0.p0(ptr %2, ptr @.str.11, ptr @.str.1, i32 260, ptr null)
  %3 = load ptr, ptr %2, align 8
  %4 = load i32, ptr %3, align 4
  %5 = add nsw i32 %4, -1
  store i32 %5, ptr %3, align 4
  ret i32 %5
}

; Function Attrs: mustprogress noinline nounwind optnone uwtable
define dso_local i64 @logia_intrinsics_prefix_dec_i64(ptr noundef %0) #2 {
  %2 = alloca ptr, align 8
  store ptr %0, ptr %2, align 8
  call void @llvm.var.annotation.p0.p0(ptr %2, ptr @.str.12, ptr @.str.1, i32 266, ptr null)
  %3 = load ptr, ptr %2, align 8
  %4 = load i64, ptr %3, align 8
  %5 = add nsw i64 %4, -1
  store i64 %5, ptr %3, align 8
  ret i64 %5
}

; Function Attrs: mustprogress noinline nounwind optnone uwtable
define dso_local i8 @logia_intrinsics_prefix_dec_u8(ptr noundef %0) #2 {
  %2 = alloca ptr, align 8
  store ptr %0, ptr %2, align 8
  call void @llvm.var.annotation.p0.p0(ptr %2, ptr @.str.13, ptr @.str.1, i32 272, ptr null)
  %3 = load ptr, ptr %2, align 8
  %4 = load i8, ptr %3, align 1
  %5 = add i8 %4, -1
  store i8 %5, ptr %3, align 1
  ret i8 %5
}

; Function Attrs: mustprogress noinline nounwind optnone uwtable
define dso_local i16 @logia_intrinsics_prefix_dec_u16(ptr noundef %0) #2 {
  %2 = alloca ptr, align 8
  store ptr %0, ptr %2, align 8
  call void @llvm.var.annotation.p0.p0(ptr %2, ptr @.str.14, ptr @.str.1, i32 278, ptr null)
  %3 = load ptr, ptr %2, align 8
  %4 = load i16, ptr %3, align 2
  %5 = add i16 %4, -1
  store i16 %5, ptr %3, align 2
  ret i16 %5
}

; Function Attrs: mustprogress noinline nounwind optnone uwtable
define dso_local i32 @logia_intrinsics_prefix_dec_u32(ptr noundef %0) #2 {
  %2 = alloca ptr, align 8
  store ptr %0, ptr %2, align 8
  call void @llvm.var.annotation.p0.p0(ptr %2, ptr @.str.15, ptr @.str.1, i32 284, ptr null)
  %3 = load ptr, ptr %2, align 8
  %4 = load i32, ptr %3, align 4
  %5 = add i32 %4, -1
  store i32 %5, ptr %3, align 4
  ret i32 %5
}

; Function Attrs: mustprogress noinline nounwind optnone uwtable
define dso_local i64 @logia_intrinsics_prefix_dec_u64(ptr noundef %0) #2 {
  %2 = alloca ptr, align 8
  store ptr %0, ptr %2, align 8
  call void @llvm.var.annotation.p0.p0(ptr %2, ptr @.str.16, ptr @.str.1, i32 290, ptr null)
  %3 = load ptr, ptr %2, align 8
  %4 = load i64, ptr %3, align 8
  %5 = add i64 %4, -1
  store i64 %5, ptr %3, align 8
  ret i64 %5
}

; Function Attrs: mustprogress noinline nounwind optnone uwtable
define dso_local i8 @logia__intrinsics_bin_add_i8_i8(i8 noundef %0, i8 noundef %1) #2 {
  %3 = alloca i8, align 1
  %4 = alloca i8, align 1
  store i8 %1, ptr %3, align 1
  call void @llvm.var.annotation.p0.p0(ptr %3, ptr @.str, ptr @.str.1, i32 298, ptr null)
  store i8 %0, ptr %4, align 1
  call void @llvm.var.annotation.p0.p0(ptr %4, ptr @.str, ptr @.str.1, i32 298, ptr null)
  %5 = load i8, ptr %4, align 1
  %6 = sext i8 %5 to i32
  %7 = load i8, ptr %3, align 1
  %8 = sext i8 %7 to i32
  %9 = add nsw i32 %6, %8
  %10 = trunc i32 %9 to i8
  ret i8 %10
}

; Function Attrs: mustprogress noinline nounwind optnone uwtable
define dso_local i8 @logia_intrinsics_bin_sub_i8_i8(i8 noundef %0, i8 noundef %1) #2 {
  %3 = alloca i8, align 1
  %4 = alloca i8, align 1
  store i8 %1, ptr %3, align 1
  call void @llvm.var.annotation.p0.p0(ptr %3, ptr @.str, ptr @.str.1, i32 304, ptr null)
  store i8 %0, ptr %4, align 1
  call void @llvm.var.annotation.p0.p0(ptr %4, ptr @.str, ptr @.str.1, i32 304, ptr null)
  %5 = load i8, ptr %4, align 1
  %6 = sext i8 %5 to i32
  %7 = load i8, ptr %3, align 1
  %8 = sext i8 %7 to i32
  %9 = sub nsw i32 %6, %8
  %10 = trunc i32 %9 to i8
  ret i8 %10
}

; Function Attrs: mustprogress noinline nounwind optnone uwtable
define dso_local i8 @logia_intrinsics_bin_mul_i8_i8(i8 noundef %0, i8 noundef %1) #2 {
  %3 = alloca i8, align 1
  %4 = alloca i8, align 1
  store i8 %1, ptr %3, align 1
  call void @llvm.var.annotation.p0.p0(ptr %3, ptr @.str, ptr @.str.1, i32 310, ptr null)
  store i8 %0, ptr %4, align 1
  call void @llvm.var.annotation.p0.p0(ptr %4, ptr @.str, ptr @.str.1, i32 310, ptr null)
  %5 = load i8, ptr %4, align 1
  %6 = sext i8 %5 to i32
  %7 = load i8, ptr %3, align 1
  %8 = sext i8 %7 to i32
  %9 = mul nsw i32 %6, %8
  %10 = trunc i32 %9 to i8
  ret i8 %10
}

; Function Attrs: mustprogress noinline nounwind optnone uwtable
define dso_local i8 @logia_intrinsics_bin_div_i8_i8(i8 noundef %0, i8 noundef %1) #2 {
  %3 = alloca i8, align 1
  %4 = alloca i8, align 1
  store i8 %1, ptr %3, align 1
  call void @llvm.var.annotation.p0.p0(ptr %3, ptr @.str, ptr @.str.1, i32 316, ptr null)
  store i8 %0, ptr %4, align 1
  call void @llvm.var.annotation.p0.p0(ptr %4, ptr @.str, ptr @.str.1, i32 316, ptr null)
  %5 = load i8, ptr %4, align 1
  %6 = sext i8 %5 to i32
  %7 = load i8, ptr %3, align 1
  %8 = sext i8 %7 to i32
  %9 = sdiv i32 %6, %8
  %10 = trunc i32 %9 to i8
  ret i8 %10
}

; Function Attrs: mustprogress noinline nounwind optnone uwtable
define dso_local i8 @logia_intrinsics_bin_mod_i8_i8(i8 noundef %0, i8 noundef %1) #2 {
  %3 = alloca i8, align 1
  %4 = alloca i8, align 1
  store i8 %1, ptr %3, align 1
  call void @llvm.var.annotation.p0.p0(ptr %3, ptr @.str, ptr @.str.1, i32 322, ptr null)
  store i8 %0, ptr %4, align 1
  call void @llvm.var.annotation.p0.p0(ptr %4, ptr @.str, ptr @.str.1, i32 322, ptr null)
  %5 = load i8, ptr %4, align 1
  %6 = sext i8 %5 to i32
  %7 = load i8, ptr %3, align 1
  %8 = sext i8 %7 to i32
  %9 = srem i32 %6, %8
  %10 = trunc i32 %9 to i8
  ret i8 %10
}

; Function Attrs: mustprogress noinline nounwind optnone uwtable
define dso_local zeroext i1 @logia_intrinsics_bin_eq_i8_i8(i8 noundef %0, i8 noundef %1) #2 {
  %3 = alloca i8, align 1
  %4 = alloca i8, align 1
  store i8 %1, ptr %3, align 1
  call void @llvm.var.annotation.p0.p0(ptr %3, ptr @.str, ptr @.str.1, i32 328, ptr null)
  store i8 %0, ptr %4, align 1
  call void @llvm.var.annotation.p0.p0(ptr %4, ptr @.str, ptr @.str.1, i32 328, ptr null)
  %5 = load i8, ptr %4, align 1
  %6 = sext i8 %5 to i32
  %7 = load i8, ptr %3, align 1
  %8 = sext i8 %7 to i32
  %9 = icmp eq i32 %6, %8
  ret i1 %9
}

; Function Attrs: mustprogress noinline nounwind optnone uwtable
define dso_local zeroext i1 @logia_intrinsics_bin_neq_i8_i8(i8 noundef %0, i8 noundef %1) #2 {
  %3 = alloca i8, align 1
  %4 = alloca i8, align 1
  store i8 %1, ptr %3, align 1
  call void @llvm.var.annotation.p0.p0(ptr %3, ptr @.str, ptr @.str.1, i32 334, ptr null)
  store i8 %0, ptr %4, align 1
  call void @llvm.var.annotation.p0.p0(ptr %4, ptr @.str, ptr @.str.1, i32 334, ptr null)
  %5 = load i8, ptr %4, align 1
  %6 = sext i8 %5 to i32
  %7 = load i8, ptr %3, align 1
  %8 = sext i8 %7 to i32
  %9 = icmp ne i32 %6, %8
  ret i1 %9
}

; Function Attrs: mustprogress noinline nounwind optnone uwtable
define dso_local zeroext i1 @logia_intrinsics_bin_lt_i8_i8(i8 noundef %0, i8 noundef %1) #2 {
  %3 = alloca i8, align 1
  %4 = alloca i8, align 1
  store i8 %1, ptr %3, align 1
  call void @llvm.var.annotation.p0.p0(ptr %3, ptr @.str, ptr @.str.1, i32 340, ptr null)
  store i8 %0, ptr %4, align 1
  call void @llvm.var.annotation.p0.p0(ptr %4, ptr @.str, ptr @.str.1, i32 340, ptr null)
  %5 = load i8, ptr %4, align 1
  %6 = sext i8 %5 to i32
  %7 = load i8, ptr %3, align 1
  %8 = sext i8 %7 to i32
  %9 = icmp slt i32 %6, %8
  ret i1 %9
}

; Function Attrs: mustprogress noinline nounwind optnone uwtable
define dso_local zeroext i1 @logia_intrinsics_bin_gt_i8_i8(i8 noundef %0, i8 noundef %1) #2 {
  %3 = alloca i8, align 1
  %4 = alloca i8, align 1
  store i8 %1, ptr %3, align 1
  call void @llvm.var.annotation.p0.p0(ptr %3, ptr @.str, ptr @.str.1, i32 346, ptr null)
  store i8 %0, ptr %4, align 1
  call void @llvm.var.annotation.p0.p0(ptr %4, ptr @.str, ptr @.str.1, i32 346, ptr null)
  %5 = load i8, ptr %4, align 1
  %6 = sext i8 %5 to i32
  %7 = load i8, ptr %3, align 1
  %8 = sext i8 %7 to i32
  %9 = icmp sgt i32 %6, %8
  ret i1 %9
}

; Function Attrs: mustprogress noinline nounwind optnone uwtable
define dso_local zeroext i1 @logia_intrinsics_bin_lte_i8_i8(i8 noundef %0, i8 noundef %1) #2 {
  %3 = alloca i8, align 1
  %4 = alloca i8, align 1
  store i8 %1, ptr %3, align 1
  call void @llvm.var.annotation.p0.p0(ptr %3, ptr @.str, ptr @.str.1, i32 352, ptr null)
  store i8 %0, ptr %4, align 1
  call void @llvm.var.annotation.p0.p0(ptr %4, ptr @.str, ptr @.str.1, i32 352, ptr null)
  %5 = load i8, ptr %4, align 1
  %6 = sext i8 %5 to i32
  %7 = load i8, ptr %3, align 1
  %8 = sext i8 %7 to i32
  %9 = icmp sle i32 %6, %8
  ret i1 %9
}

; Function Attrs: mustprogress noinline nounwind optnone uwtable
define dso_local zeroext i1 @logia_intrinsics_bin_gte_i8_i8(i8 noundef %0, i8 noundef %1) #2 {
  %3 = alloca i8, align 1
  %4 = alloca i8, align 1
  store i8 %1, ptr %3, align 1
  call void @llvm.var.annotation.p0.p0(ptr %3, ptr @.str, ptr @.str.1, i32 358, ptr null)
  store i8 %0, ptr %4, align 1
  call void @llvm.var.annotation.p0.p0(ptr %4, ptr @.str, ptr @.str.1, i32 358, ptr null)
  %5 = load i8, ptr %4, align 1
  %6 = sext i8 %5 to i32
  %7 = load i8, ptr %3, align 1
  %8 = sext i8 %7 to i32
  %9 = icmp sge i32 %6, %8
  ret i1 %9
}

; Function Attrs: mustprogress noinline nounwind optnone uwtable
define dso_local i8 @logia_intrinsics_bin_assign_i8_i8(ptr noundef %0, i8 noundef %1) #2 {
  %3 = alloca i8, align 1
  %4 = alloca ptr, align 8
  store i8 %1, ptr %3, align 1
  call void @llvm.var.annotation.p0.p0(ptr %3, ptr @.str, ptr @.str.1, i32 364, ptr null)
  store ptr %0, ptr %4, align 8
  call void @llvm.var.annotation.p0.p0(ptr %4, ptr @.str.9, ptr @.str.1, i32 364, ptr null)
  %5 = load i8, ptr %3, align 1
  %6 = load ptr, ptr %4, align 8
  store i8 %5, ptr %6, align 1
  %7 = load ptr, ptr %4, align 8
  %8 = load i8, ptr %7, align 1
  ret i8 %8
}

; Function Attrs: mustprogress noinline nounwind optnone uwtable
define dso_local i8 @logia_intrinsics_bin_add_assign_i8_i8(ptr noundef %0, i8 noundef %1) #2 {
  %3 = alloca i8, align 1
  %4 = alloca ptr, align 8
  store i8 %1, ptr %3, align 1
  call void @llvm.var.annotation.p0.p0(ptr %3, ptr @.str, ptr @.str.1, i32 371, ptr null)
  store ptr %0, ptr %4, align 8
  call void @llvm.var.annotation.p0.p0(ptr %4, ptr @.str.9, ptr @.str.1, i32 371, ptr null)
  %5 = load i8, ptr %3, align 1
  %6 = sext i8 %5 to i32
  %7 = load ptr, ptr %4, align 8
  %8 = load i8, ptr %7, align 1
  %9 = sext i8 %8 to i32
  %10 = add nsw i32 %9, %6
  %11 = trunc i32 %10 to i8
  store i8 %11, ptr %7, align 1
  %12 = load ptr, ptr %4, align 8
  %13 = load i8, ptr %12, align 1
  ret i8 %13
}

; Function Attrs: mustprogress noinline nounwind optnone uwtable
define dso_local i8 @logia_intrinsics_bin_sub_assign_i8_i8(ptr noundef %0, i8 noundef %1) #2 {
  %3 = alloca i8, align 1
  %4 = alloca ptr, align 8
  store i8 %1, ptr %3, align 1
  call void @llvm.var.annotation.p0.p0(ptr %3, ptr @.str, ptr @.str.1, i32 378, ptr null)
  store ptr %0, ptr %4, align 8
  call void @llvm.var.annotation.p0.p0(ptr %4, ptr @.str.9, ptr @.str.1, i32 378, ptr null)
  %5 = load i8, ptr %3, align 1
  %6 = sext i8 %5 to i32
  %7 = load ptr, ptr %4, align 8
  %8 = load i8, ptr %7, align 1
  %9 = sext i8 %8 to i32
  %10 = sub nsw i32 %9, %6
  %11 = trunc i32 %10 to i8
  store i8 %11, ptr %7, align 1
  %12 = load ptr, ptr %4, align 8
  %13 = load i8, ptr %12, align 1
  ret i8 %13
}

; Function Attrs: mustprogress noinline nounwind optnone uwtable
define dso_local i8 @logia_intrinsics_bin_mul_assign_i8_i8(ptr noundef %0, i8 noundef %1) #2 {
  %3 = alloca i8, align 1
  %4 = alloca ptr, align 8
  store i8 %1, ptr %3, align 1
  call void @llvm.var.annotation.p0.p0(ptr %3, ptr @.str, ptr @.str.1, i32 385, ptr null)
  store ptr %0, ptr %4, align 8
  call void @llvm.var.annotation.p0.p0(ptr %4, ptr @.str.9, ptr @.str.1, i32 385, ptr null)
  %5 = load i8, ptr %3, align 1
  %6 = sext i8 %5 to i32
  %7 = load ptr, ptr %4, align 8
  %8 = load i8, ptr %7, align 1
  %9 = sext i8 %8 to i32
  %10 = mul nsw i32 %9, %6
  %11 = trunc i32 %10 to i8
  store i8 %11, ptr %7, align 1
  %12 = load ptr, ptr %4, align 8
  %13 = load i8, ptr %12, align 1
  ret i8 %13
}

; Function Attrs: mustprogress noinline nounwind optnone uwtable
define dso_local i8 @logia_intrinsics_bin_div_assign_i8_i8(ptr noundef %0, i8 noundef %1) #2 {
  %3 = alloca i8, align 1
  %4 = alloca ptr, align 8
  store i8 %1, ptr %3, align 1
  call void @llvm.var.annotation.p0.p0(ptr %3, ptr @.str, ptr @.str.1, i32 392, ptr null)
  store ptr %0, ptr %4, align 8
  call void @llvm.var.annotation.p0.p0(ptr %4, ptr @.str.9, ptr @.str.1, i32 392, ptr null)
  %5 = load i8, ptr %3, align 1
  %6 = sext i8 %5 to i32
  %7 = load ptr, ptr %4, align 8
  %8 = load i8, ptr %7, align 1
  %9 = sext i8 %8 to i32
  %10 = sdiv i32 %9, %6
  %11 = trunc i32 %10 to i8
  store i8 %11, ptr %7, align 1
  %12 = load ptr, ptr %4, align 8
  %13 = load i8, ptr %12, align 1
  ret i8 %13
}

; Function Attrs: mustprogress noinline nounwind optnone uwtable
define dso_local i8 @logia_intrinsics_bin_mod_assign_i8_i8(ptr noundef %0, i8 noundef %1) #2 {
  %3 = alloca i8, align 1
  %4 = alloca ptr, align 8
  store i8 %1, ptr %3, align 1
  call void @llvm.var.annotation.p0.p0(ptr %3, ptr @.str, ptr @.str.1, i32 399, ptr null)
  store ptr %0, ptr %4, align 8
  call void @llvm.var.annotation.p0.p0(ptr %4, ptr @.str.9, ptr @.str.1, i32 399, ptr null)
  %5 = load i8, ptr %3, align 1
  %6 = sext i8 %5 to i32
  %7 = load ptr, ptr %4, align 8
  %8 = load i8, ptr %7, align 1
  %9 = sext i8 %8 to i32
  %10 = srem i32 %9, %6
  %11 = trunc i32 %10 to i8
  store i8 %11, ptr %7, align 1
  %12 = load ptr, ptr %4, align 8
  %13 = load i8, ptr %12, align 1
  ret i8 %13
}

; Function Attrs: mustprogress noinline nounwind optnone uwtable
define dso_local i8 @logia_intrinsics_bin_bitwise_and_assign_i8_i8(ptr noundef %0, i8 noundef %1) #2 {
  %3 = alloca i8, align 1
  %4 = alloca ptr, align 8
  store i8 %1, ptr %3, align 1
  call void @llvm.var.annotation.p0.p0(ptr %3, ptr @.str, ptr @.str.1, i32 406, ptr null)
  store ptr %0, ptr %4, align 8
  call void @llvm.var.annotation.p0.p0(ptr %4, ptr @.str.9, ptr @.str.1, i32 406, ptr null)
  %5 = load i8, ptr %3, align 1
  %6 = sext i8 %5 to i32
  %7 = load ptr, ptr %4, align 8
  %8 = load i8, ptr %7, align 1
  %9 = sext i8 %8 to i32
  %10 = and i32 %9, %6
  %11 = trunc i32 %10 to i8
  store i8 %11, ptr %7, align 1
  %12 = load ptr, ptr %4, align 8
  %13 = load i8, ptr %12, align 1
  ret i8 %13
}

; Function Attrs: mustprogress noinline nounwind optnone uwtable
define dso_local i8 @logia_intrinsics_bin_bitwise_or_assign_i8_i8(ptr noundef %0, i8 noundef %1) #2 {
  %3 = alloca i8, align 1
  %4 = alloca ptr, align 8
  store i8 %1, ptr %3, align 1
  call void @llvm.var.annotation.p0.p0(ptr %3, ptr @.str, ptr @.str.1, i32 413, ptr null)
  store ptr %0, ptr %4, align 8
  call void @llvm.var.annotation.p0.p0(ptr %4, ptr @.str.9, ptr @.str.1, i32 413, ptr null)
  %5 = load i8, ptr %3, align 1
  %6 = sext i8 %5 to i32
  %7 = load ptr, ptr %4, align 8
  %8 = load i8, ptr %7, align 1
  %9 = sext i8 %8 to i32
  %10 = or i32 %9, %6
  %11 = trunc i32 %10 to i8
  store i8 %11, ptr %7, align 1
  %12 = load ptr, ptr %4, align 8
  %13 = load i8, ptr %12, align 1
  ret i8 %13
}

; Function Attrs: mustprogress noinline nounwind optnone uwtable
define dso_local i8 @logia_intrinsics_bin_bitwise_xor_assign_i8_i8(ptr noundef %0, i8 noundef %1) #2 {
  %3 = alloca i8, align 1
  %4 = alloca ptr, align 8
  store i8 %1, ptr %3, align 1
  call void @llvm.var.annotation.p0.p0(ptr %3, ptr @.str, ptr @.str.1, i32 420, ptr null)
  store ptr %0, ptr %4, align 8
  call void @llvm.var.annotation.p0.p0(ptr %4, ptr @.str.9, ptr @.str.1, i32 420, ptr null)
  %5 = load i8, ptr %3, align 1
  %6 = sext i8 %5 to i32
  %7 = load ptr, ptr %4, align 8
  %8 = load i8, ptr %7, align 1
  %9 = sext i8 %8 to i32
  %10 = xor i32 %9, %6
  %11 = trunc i32 %10 to i8
  store i8 %11, ptr %7, align 1
  %12 = load ptr, ptr %4, align 8
  %13 = load i8, ptr %12, align 1
  ret i8 %13
}

; Function Attrs: mustprogress noinline nounwind optnone uwtable
define dso_local i8 @logia_intrinsics_bin_bitwise_shr_assign_i8_i8(ptr noundef %0, i8 noundef %1) #2 {
  %3 = alloca i8, align 1
  %4 = alloca ptr, align 8
  store i8 %1, ptr %3, align 1
  call void @llvm.var.annotation.p0.p0(ptr %3, ptr @.str, ptr @.str.1, i32 427, ptr null)
  store ptr %0, ptr %4, align 8
  call void @llvm.var.annotation.p0.p0(ptr %4, ptr @.str.9, ptr @.str.1, i32 427, ptr null)
  %5 = load i8, ptr %3, align 1
  %6 = sext i8 %5 to i32
  %7 = load ptr, ptr %4, align 8
  %8 = load i8, ptr %7, align 1
  %9 = sext i8 %8 to i32
  %10 = ashr i32 %9, %6
  %11 = trunc i32 %10 to i8
  store i8 %11, ptr %7, align 1
  %12 = load ptr, ptr %4, align 8
  %13 = load i8, ptr %12, align 1
  ret i8 %13
}

; Function Attrs: mustprogress noinline nounwind optnone uwtable
define dso_local i8 @logia_intrinsics_bin_bitwise_shl_assign_i8_i8(ptr noundef %0, i8 noundef %1) #2 {
  %3 = alloca i8, align 1
  %4 = alloca ptr, align 8
  store i8 %1, ptr %3, align 1
  call void @llvm.var.annotation.p0.p0(ptr %3, ptr @.str, ptr @.str.1, i32 434, ptr null)
  store ptr %0, ptr %4, align 8
  call void @llvm.var.annotation.p0.p0(ptr %4, ptr @.str.9, ptr @.str.1, i32 434, ptr null)
  %5 = load i8, ptr %3, align 1
  %6 = sext i8 %5 to i32
  %7 = load ptr, ptr %4, align 8
  %8 = load i8, ptr %7, align 1
  %9 = sext i8 %8 to i32
  %10 = shl i32 %9, %6
  %11 = trunc i32 %10 to i8
  store i8 %11, ptr %7, align 1
  %12 = load ptr, ptr %4, align 8
  %13 = load i8, ptr %12, align 1
  ret i8 %13
}

; Function Attrs: mustprogress noinline nounwind optnone uwtable
define dso_local i8 @logia_intrinsics_bin_bitwise_and_i8_i8(i8 noundef %0, i8 noundef %1) #2 {
  %3 = alloca i8, align 1
  %4 = alloca i8, align 1
  store i8 %1, ptr %3, align 1
  call void @llvm.var.annotation.p0.p0(ptr %3, ptr @.str, ptr @.str.1, i32 441, ptr null)
  store i8 %0, ptr %4, align 1
  call void @llvm.var.annotation.p0.p0(ptr %4, ptr @.str, ptr @.str.1, i32 441, ptr null)
  %5 = load i8, ptr %4, align 1
  %6 = sext i8 %5 to i32
  %7 = load i8, ptr %3, align 1
  %8 = sext i8 %7 to i32
  %9 = and i32 %6, %8
  %10 = trunc i32 %9 to i8
  ret i8 %10
}

; Function Attrs: mustprogress noinline nounwind optnone uwtable
define dso_local i8 @logia_intrinsics_bin_bitwise_or_i8_i8(i8 noundef %0, i8 noundef %1) #2 {
  %3 = alloca i8, align 1
  %4 = alloca i8, align 1
  store i8 %1, ptr %3, align 1
  call void @llvm.var.annotation.p0.p0(ptr %3, ptr @.str, ptr @.str.1, i32 447, ptr null)
  store i8 %0, ptr %4, align 1
  call void @llvm.var.annotation.p0.p0(ptr %4, ptr @.str, ptr @.str.1, i32 447, ptr null)
  %5 = load i8, ptr %4, align 1
  %6 = sext i8 %5 to i32
  %7 = load i8, ptr %3, align 1
  %8 = sext i8 %7 to i32
  %9 = or i32 %6, %8
  %10 = trunc i32 %9 to i8
  ret i8 %10
}

; Function Attrs: mustprogress noinline nounwind optnone uwtable
define dso_local i8 @logia_intrinsics_bin_bitwise_xor_i8_i8(i8 noundef %0, i8 noundef %1) #2 {
  %3 = alloca i8, align 1
  %4 = alloca i8, align 1
  store i8 %1, ptr %3, align 1
  call void @llvm.var.annotation.p0.p0(ptr %3, ptr @.str, ptr @.str.1, i32 453, ptr null)
  store i8 %0, ptr %4, align 1
  call void @llvm.var.annotation.p0.p0(ptr %4, ptr @.str, ptr @.str.1, i32 453, ptr null)
  %5 = load i8, ptr %4, align 1
  %6 = sext i8 %5 to i32
  %7 = load i8, ptr %3, align 1
  %8 = sext i8 %7 to i32
  %9 = xor i32 %6, %8
  %10 = trunc i32 %9 to i8
  ret i8 %10
}

; Function Attrs: mustprogress noinline nounwind optnone uwtable
define dso_local i8 @logia_intrinsics_bin_bitwise_shl_i8_i8(i8 noundef %0, i8 noundef %1) #2 {
  %3 = alloca i8, align 1
  %4 = alloca i8, align 1
  store i8 %1, ptr %3, align 1
  call void @llvm.var.annotation.p0.p0(ptr %3, ptr @.str, ptr @.str.1, i32 459, ptr null)
  store i8 %0, ptr %4, align 1
  call void @llvm.var.annotation.p0.p0(ptr %4, ptr @.str, ptr @.str.1, i32 459, ptr null)
  %5 = load i8, ptr %4, align 1
  %6 = sext i8 %5 to i32
  %7 = load i8, ptr %3, align 1
  %8 = sext i8 %7 to i32
  %9 = shl i32 %6, %8
  %10 = trunc i32 %9 to i8
  ret i8 %10
}

; Function Attrs: mustprogress noinline nounwind optnone uwtable
define dso_local i8 @logia_intrinsics_bin_bitwise_shr_i8_i8(i8 noundef %0, i8 noundef %1) #2 {
  %3 = alloca i8, align 1
  %4 = alloca i8, align 1
  store i8 %1, ptr %3, align 1
  call void @llvm.var.annotation.p0.p0(ptr %3, ptr @.str, ptr @.str.1, i32 465, ptr null)
  store i8 %0, ptr %4, align 1
  call void @llvm.var.annotation.p0.p0(ptr %4, ptr @.str, ptr @.str.1, i32 465, ptr null)
  %5 = load i8, ptr %4, align 1
  %6 = sext i8 %5 to i32
  %7 = load i8, ptr %3, align 1
  %8 = sext i8 %7 to i32
  %9 = ashr i32 %6, %8
  %10 = trunc i32 %9 to i8
  ret i8 %10
}

; Function Attrs: mustprogress noinline nounwind optnone uwtable
define dso_local i16 @logia__intrinsics_bin_add_i16_i16(i16 noundef %0, i16 noundef %1) #2 {
  %3 = alloca i16, align 2
  %4 = alloca i16, align 2
  store i16 %1, ptr %3, align 2
  call void @llvm.var.annotation.p0.p0(ptr %3, ptr @.str.2, ptr @.str.1, i32 471, ptr null)
  store i16 %0, ptr %4, align 2
  call void @llvm.var.annotation.p0.p0(ptr %4, ptr @.str.2, ptr @.str.1, i32 471, ptr null)
  %5 = load i16, ptr %4, align 2
  %6 = sext i16 %5 to i32
  %7 = load i16, ptr %3, align 2
  %8 = sext i16 %7 to i32
  %9 = add nsw i32 %6, %8
  %10 = trunc i32 %9 to i16
  ret i16 %10
}

; Function Attrs: mustprogress noinline nounwind optnone uwtable
define dso_local i16 @logia_intrinsics_bin_sub_i16_i16(i16 noundef %0, i16 noundef %1) #2 {
  %3 = alloca i16, align 2
  %4 = alloca i16, align 2
  store i16 %1, ptr %3, align 2
  call void @llvm.var.annotation.p0.p0(ptr %3, ptr @.str.2, ptr @.str.1, i32 477, ptr null)
  store i16 %0, ptr %4, align 2
  call void @llvm.var.annotation.p0.p0(ptr %4, ptr @.str.2, ptr @.str.1, i32 477, ptr null)
  %5 = load i16, ptr %4, align 2
  %6 = sext i16 %5 to i32
  %7 = load i16, ptr %3, align 2
  %8 = sext i16 %7 to i32
  %9 = sub nsw i32 %6, %8
  %10 = trunc i32 %9 to i16
  ret i16 %10
}

; Function Attrs: mustprogress noinline nounwind optnone uwtable
define dso_local i16 @logia_intrinsics_bin_mul_i16_i16(i16 noundef %0, i16 noundef %1) #2 {
  %3 = alloca i16, align 2
  %4 = alloca i16, align 2
  store i16 %1, ptr %3, align 2
  call void @llvm.var.annotation.p0.p0(ptr %3, ptr @.str.2, ptr @.str.1, i32 483, ptr null)
  store i16 %0, ptr %4, align 2
  call void @llvm.var.annotation.p0.p0(ptr %4, ptr @.str.2, ptr @.str.1, i32 483, ptr null)
  %5 = load i16, ptr %4, align 2
  %6 = sext i16 %5 to i32
  %7 = load i16, ptr %3, align 2
  %8 = sext i16 %7 to i32
  %9 = mul nsw i32 %6, %8
  %10 = trunc i32 %9 to i16
  ret i16 %10
}

; Function Attrs: mustprogress noinline nounwind optnone uwtable
define dso_local i16 @logia_intrinsics_bin_div_i16_i16(i16 noundef %0, i16 noundef %1) #2 {
  %3 = alloca i16, align 2
  %4 = alloca i16, align 2
  store i16 %1, ptr %3, align 2
  call void @llvm.var.annotation.p0.p0(ptr %3, ptr @.str.2, ptr @.str.1, i32 489, ptr null)
  store i16 %0, ptr %4, align 2
  call void @llvm.var.annotation.p0.p0(ptr %4, ptr @.str.2, ptr @.str.1, i32 489, ptr null)
  %5 = load i16, ptr %4, align 2
  %6 = sext i16 %5 to i32
  %7 = load i16, ptr %3, align 2
  %8 = sext i16 %7 to i32
  %9 = sdiv i32 %6, %8
  %10 = trunc i32 %9 to i16
  ret i16 %10
}

; Function Attrs: mustprogress noinline nounwind optnone uwtable
define dso_local i16 @logia_intrinsics_bin_mod_i16_i16(i16 noundef %0, i16 noundef %1) #2 {
  %3 = alloca i16, align 2
  %4 = alloca i16, align 2
  store i16 %1, ptr %3, align 2
  call void @llvm.var.annotation.p0.p0(ptr %3, ptr @.str.2, ptr @.str.1, i32 495, ptr null)
  store i16 %0, ptr %4, align 2
  call void @llvm.var.annotation.p0.p0(ptr %4, ptr @.str.2, ptr @.str.1, i32 495, ptr null)
  %5 = load i16, ptr %4, align 2
  %6 = sext i16 %5 to i32
  %7 = load i16, ptr %3, align 2
  %8 = sext i16 %7 to i32
  %9 = srem i32 %6, %8
  %10 = trunc i32 %9 to i16
  ret i16 %10
}

; Function Attrs: mustprogress noinline nounwind optnone uwtable
define dso_local zeroext i1 @logia_intrinsics_bin_eq_i16_i16(i16 noundef %0, i16 noundef %1) #2 {
  %3 = alloca i16, align 2
  %4 = alloca i16, align 2
  store i16 %1, ptr %3, align 2
  call void @llvm.var.annotation.p0.p0(ptr %3, ptr @.str.2, ptr @.str.1, i32 501, ptr null)
  store i16 %0, ptr %4, align 2
  call void @llvm.var.annotation.p0.p0(ptr %4, ptr @.str.2, ptr @.str.1, i32 501, ptr null)
  %5 = load i16, ptr %4, align 2
  %6 = sext i16 %5 to i32
  %7 = load i16, ptr %3, align 2
  %8 = sext i16 %7 to i32
  %9 = icmp eq i32 %6, %8
  ret i1 %9
}

; Function Attrs: mustprogress noinline nounwind optnone uwtable
define dso_local zeroext i1 @logia_intrinsics_bin_neq_i16_i16(i16 noundef %0, i16 noundef %1) #2 {
  %3 = alloca i16, align 2
  %4 = alloca i16, align 2
  store i16 %1, ptr %3, align 2
  call void @llvm.var.annotation.p0.p0(ptr %3, ptr @.str.2, ptr @.str.1, i32 507, ptr null)
  store i16 %0, ptr %4, align 2
  call void @llvm.var.annotation.p0.p0(ptr %4, ptr @.str.2, ptr @.str.1, i32 507, ptr null)
  %5 = load i16, ptr %4, align 2
  %6 = sext i16 %5 to i32
  %7 = load i16, ptr %3, align 2
  %8 = sext i16 %7 to i32
  %9 = icmp ne i32 %6, %8
  ret i1 %9
}

; Function Attrs: mustprogress noinline nounwind optnone uwtable
define dso_local zeroext i1 @logia_intrinsics_bin_lt_i16_i16(i16 noundef %0, i16 noundef %1) #2 {
  %3 = alloca i16, align 2
  %4 = alloca i16, align 2
  store i16 %1, ptr %3, align 2
  call void @llvm.var.annotation.p0.p0(ptr %3, ptr @.str.2, ptr @.str.1, i32 513, ptr null)
  store i16 %0, ptr %4, align 2
  call void @llvm.var.annotation.p0.p0(ptr %4, ptr @.str.2, ptr @.str.1, i32 513, ptr null)
  %5 = load i16, ptr %4, align 2
  %6 = sext i16 %5 to i32
  %7 = load i16, ptr %3, align 2
  %8 = sext i16 %7 to i32
  %9 = icmp slt i32 %6, %8
  ret i1 %9
}

; Function Attrs: mustprogress noinline nounwind optnone uwtable
define dso_local zeroext i1 @logia_intrinsics_bin_gt_i16_i16(i16 noundef %0, i16 noundef %1) #2 {
  %3 = alloca i16, align 2
  %4 = alloca i16, align 2
  store i16 %1, ptr %3, align 2
  call void @llvm.var.annotation.p0.p0(ptr %3, ptr @.str.2, ptr @.str.1, i32 519, ptr null)
  store i16 %0, ptr %4, align 2
  call void @llvm.var.annotation.p0.p0(ptr %4, ptr @.str.2, ptr @.str.1, i32 519, ptr null)
  %5 = load i16, ptr %4, align 2
  %6 = sext i16 %5 to i32
  %7 = load i16, ptr %3, align 2
  %8 = sext i16 %7 to i32
  %9 = icmp sgt i32 %6, %8
  ret i1 %9
}

; Function Attrs: mustprogress noinline nounwind optnone uwtable
define dso_local zeroext i1 @logia_intrinsics_bin_lte_i16_i16(i16 noundef %0, i16 noundef %1) #2 {
  %3 = alloca i16, align 2
  %4 = alloca i16, align 2
  store i16 %1, ptr %3, align 2
  call void @llvm.var.annotation.p0.p0(ptr %3, ptr @.str.2, ptr @.str.1, i32 525, ptr null)
  store i16 %0, ptr %4, align 2
  call void @llvm.var.annotation.p0.p0(ptr %4, ptr @.str.2, ptr @.str.1, i32 525, ptr null)
  %5 = load i16, ptr %4, align 2
  %6 = sext i16 %5 to i32
  %7 = load i16, ptr %3, align 2
  %8 = sext i16 %7 to i32
  %9 = icmp sle i32 %6, %8
  ret i1 %9
}

; Function Attrs: mustprogress noinline nounwind optnone uwtable
define dso_local zeroext i1 @logia_intrinsics_bin_gte_i16_i16(i16 noundef %0, i16 noundef %1) #2 {
  %3 = alloca i16, align 2
  %4 = alloca i16, align 2
  store i16 %1, ptr %3, align 2
  call void @llvm.var.annotation.p0.p0(ptr %3, ptr @.str.2, ptr @.str.1, i32 531, ptr null)
  store i16 %0, ptr %4, align 2
  call void @llvm.var.annotation.p0.p0(ptr %4, ptr @.str.2, ptr @.str.1, i32 531, ptr null)
  %5 = load i16, ptr %4, align 2
  %6 = sext i16 %5 to i32
  %7 = load i16, ptr %3, align 2
  %8 = sext i16 %7 to i32
  %9 = icmp sge i32 %6, %8
  ret i1 %9
}

; Function Attrs: mustprogress noinline nounwind optnone uwtable
define dso_local i16 @logia_intrinsics_bin_assign_i16_i16(ptr noundef %0, i16 noundef %1) #2 {
  %3 = alloca i16, align 2
  %4 = alloca ptr, align 8
  store i16 %1, ptr %3, align 2
  call void @llvm.var.annotation.p0.p0(ptr %3, ptr @.str.2, ptr @.str.1, i32 537, ptr null)
  store ptr %0, ptr %4, align 8
  call void @llvm.var.annotation.p0.p0(ptr %4, ptr @.str.10, ptr @.str.1, i32 537, ptr null)
  %5 = load i16, ptr %3, align 2
  %6 = load ptr, ptr %4, align 8
  store i16 %5, ptr %6, align 2
  %7 = load ptr, ptr %4, align 8
  %8 = load i16, ptr %7, align 2
  ret i16 %8
}

; Function Attrs: mustprogress noinline nounwind optnone uwtable
define dso_local i16 @logia_intrinsics_bin_add_assign_i16_i16(ptr noundef %0, i16 noundef %1) #2 {
  %3 = alloca i16, align 2
  %4 = alloca ptr, align 8
  store i16 %1, ptr %3, align 2
  call void @llvm.var.annotation.p0.p0(ptr %3, ptr @.str.2, ptr @.str.1, i32 544, ptr null)
  store ptr %0, ptr %4, align 8
  call void @llvm.var.annotation.p0.p0(ptr %4, ptr @.str.10, ptr @.str.1, i32 544, ptr null)
  %5 = load i16, ptr %3, align 2
  %6 = sext i16 %5 to i32
  %7 = load ptr, ptr %4, align 8
  %8 = load i16, ptr %7, align 2
  %9 = sext i16 %8 to i32
  %10 = add nsw i32 %9, %6
  %11 = trunc i32 %10 to i16
  store i16 %11, ptr %7, align 2
  %12 = load ptr, ptr %4, align 8
  %13 = load i16, ptr %12, align 2
  ret i16 %13
}

; Function Attrs: mustprogress noinline nounwind optnone uwtable
define dso_local i16 @logia_intrinsics_bin_sub_assign_i16_i16(ptr noundef %0, i16 noundef %1) #2 {
  %3 = alloca i16, align 2
  %4 = alloca ptr, align 8
  store i16 %1, ptr %3, align 2
  call void @llvm.var.annotation.p0.p0(ptr %3, ptr @.str.2, ptr @.str.1, i32 551, ptr null)
  store ptr %0, ptr %4, align 8
  call void @llvm.var.annotation.p0.p0(ptr %4, ptr @.str.10, ptr @.str.1, i32 551, ptr null)
  %5 = load i16, ptr %3, align 2
  %6 = sext i16 %5 to i32
  %7 = load ptr, ptr %4, align 8
  %8 = load i16, ptr %7, align 2
  %9 = sext i16 %8 to i32
  %10 = sub nsw i32 %9, %6
  %11 = trunc i32 %10 to i16
  store i16 %11, ptr %7, align 2
  %12 = load ptr, ptr %4, align 8
  %13 = load i16, ptr %12, align 2
  ret i16 %13
}

; Function Attrs: mustprogress noinline nounwind optnone uwtable
define dso_local i16 @logia_intrinsics_bin_mul_assign_i16_i16(ptr noundef %0, i16 noundef %1) #2 {
  %3 = alloca i16, align 2
  %4 = alloca ptr, align 8
  store i16 %1, ptr %3, align 2
  call void @llvm.var.annotation.p0.p0(ptr %3, ptr @.str.2, ptr @.str.1, i32 558, ptr null)
  store ptr %0, ptr %4, align 8
  call void @llvm.var.annotation.p0.p0(ptr %4, ptr @.str.10, ptr @.str.1, i32 558, ptr null)
  %5 = load i16, ptr %3, align 2
  %6 = sext i16 %5 to i32
  %7 = load ptr, ptr %4, align 8
  %8 = load i16, ptr %7, align 2
  %9 = sext i16 %8 to i32
  %10 = mul nsw i32 %9, %6
  %11 = trunc i32 %10 to i16
  store i16 %11, ptr %7, align 2
  %12 = load ptr, ptr %4, align 8
  %13 = load i16, ptr %12, align 2
  ret i16 %13
}

; Function Attrs: mustprogress noinline nounwind optnone uwtable
define dso_local i16 @logia_intrinsics_bin_div_assign_i16_i16(ptr noundef %0, i16 noundef %1) #2 {
  %3 = alloca i16, align 2
  %4 = alloca ptr, align 8
  store i16 %1, ptr %3, align 2
  call void @llvm.var.annotation.p0.p0(ptr %3, ptr @.str.2, ptr @.str.1, i32 565, ptr null)
  store ptr %0, ptr %4, align 8
  call void @llvm.var.annotation.p0.p0(ptr %4, ptr @.str.10, ptr @.str.1, i32 565, ptr null)
  %5 = load i16, ptr %3, align 2
  %6 = sext i16 %5 to i32
  %7 = load ptr, ptr %4, align 8
  %8 = load i16, ptr %7, align 2
  %9 = sext i16 %8 to i32
  %10 = sdiv i32 %9, %6
  %11 = trunc i32 %10 to i16
  store i16 %11, ptr %7, align 2
  %12 = load ptr, ptr %4, align 8
  %13 = load i16, ptr %12, align 2
  ret i16 %13
}

; Function Attrs: mustprogress noinline nounwind optnone uwtable
define dso_local i16 @logia_intrinsics_bin_mod_assign_i16_i16(ptr noundef %0, i16 noundef %1) #2 {
  %3 = alloca i16, align 2
  %4 = alloca ptr, align 8
  store i16 %1, ptr %3, align 2
  call void @llvm.var.annotation.p0.p0(ptr %3, ptr @.str.2, ptr @.str.1, i32 572, ptr null)
  store ptr %0, ptr %4, align 8
  call void @llvm.var.annotation.p0.p0(ptr %4, ptr @.str.10, ptr @.str.1, i32 572, ptr null)
  %5 = load i16, ptr %3, align 2
  %6 = sext i16 %5 to i32
  %7 = load ptr, ptr %4, align 8
  %8 = load i16, ptr %7, align 2
  %9 = sext i16 %8 to i32
  %10 = srem i32 %9, %6
  %11 = trunc i32 %10 to i16
  store i16 %11, ptr %7, align 2
  %12 = load ptr, ptr %4, align 8
  %13 = load i16, ptr %12, align 2
  ret i16 %13
}

; Function Attrs: mustprogress noinline nounwind optnone uwtable
define dso_local i16 @logia_intrinsics_bin_bitwise_and_assign_i16_i16(ptr noundef %0, i16 noundef %1) #2 {
  %3 = alloca i16, align 2
  %4 = alloca ptr, align 8
  store i16 %1, ptr %3, align 2
  call void @llvm.var.annotation.p0.p0(ptr %3, ptr @.str.2, ptr @.str.1, i32 579, ptr null)
  store ptr %0, ptr %4, align 8
  call void @llvm.var.annotation.p0.p0(ptr %4, ptr @.str.10, ptr @.str.1, i32 579, ptr null)
  %5 = load i16, ptr %3, align 2
  %6 = sext i16 %5 to i32
  %7 = load ptr, ptr %4, align 8
  %8 = load i16, ptr %7, align 2
  %9 = sext i16 %8 to i32
  %10 = and i32 %9, %6
  %11 = trunc i32 %10 to i16
  store i16 %11, ptr %7, align 2
  %12 = load ptr, ptr %4, align 8
  %13 = load i16, ptr %12, align 2
  ret i16 %13
}

; Function Attrs: mustprogress noinline nounwind optnone uwtable
define dso_local i16 @logia_intrinsics_bin_bitwise_or_assign_i16_i16(ptr noundef %0, i16 noundef %1) #2 {
  %3 = alloca i16, align 2
  %4 = alloca ptr, align 8
  store i16 %1, ptr %3, align 2
  call void @llvm.var.annotation.p0.p0(ptr %3, ptr @.str.2, ptr @.str.1, i32 586, ptr null)
  store ptr %0, ptr %4, align 8
  call void @llvm.var.annotation.p0.p0(ptr %4, ptr @.str.10, ptr @.str.1, i32 586, ptr null)
  %5 = load i16, ptr %3, align 2
  %6 = sext i16 %5 to i32
  %7 = load ptr, ptr %4, align 8
  %8 = load i16, ptr %7, align 2
  %9 = sext i16 %8 to i32
  %10 = or i32 %9, %6
  %11 = trunc i32 %10 to i16
  store i16 %11, ptr %7, align 2
  %12 = load ptr, ptr %4, align 8
  %13 = load i16, ptr %12, align 2
  ret i16 %13
}

; Function Attrs: mustprogress noinline nounwind optnone uwtable
define dso_local i16 @logia_intrinsics_bin_bitwise_xor_assign_i16_i16(ptr noundef %0, i16 noundef %1) #2 {
  %3 = alloca i16, align 2
  %4 = alloca ptr, align 8
  store i16 %1, ptr %3, align 2
  call void @llvm.var.annotation.p0.p0(ptr %3, ptr @.str.2, ptr @.str.1, i32 593, ptr null)
  store ptr %0, ptr %4, align 8
  call void @llvm.var.annotation.p0.p0(ptr %4, ptr @.str.10, ptr @.str.1, i32 593, ptr null)
  %5 = load i16, ptr %3, align 2
  %6 = sext i16 %5 to i32
  %7 = load ptr, ptr %4, align 8
  %8 = load i16, ptr %7, align 2
  %9 = sext i16 %8 to i32
  %10 = xor i32 %9, %6
  %11 = trunc i32 %10 to i16
  store i16 %11, ptr %7, align 2
  %12 = load ptr, ptr %4, align 8
  %13 = load i16, ptr %12, align 2
  ret i16 %13
}

; Function Attrs: mustprogress noinline nounwind optnone uwtable
define dso_local i16 @logia_intrinsics_bin_bitwise_shr_assign_i16_i16(ptr noundef %0, i16 noundef %1) #2 {
  %3 = alloca i16, align 2
  %4 = alloca ptr, align 8
  store i16 %1, ptr %3, align 2
  call void @llvm.var.annotation.p0.p0(ptr %3, ptr @.str.2, ptr @.str.1, i32 600, ptr null)
  store ptr %0, ptr %4, align 8
  call void @llvm.var.annotation.p0.p0(ptr %4, ptr @.str.10, ptr @.str.1, i32 600, ptr null)
  %5 = load i16, ptr %3, align 2
  %6 = sext i16 %5 to i32
  %7 = load ptr, ptr %4, align 8
  %8 = load i16, ptr %7, align 2
  %9 = sext i16 %8 to i32
  %10 = ashr i32 %9, %6
  %11 = trunc i32 %10 to i16
  store i16 %11, ptr %7, align 2
  %12 = load ptr, ptr %4, align 8
  %13 = load i16, ptr %12, align 2
  ret i16 %13
}

; Function Attrs: mustprogress noinline nounwind optnone uwtable
define dso_local i16 @logia_intrinsics_bin_bitwise_shl_assign_i16_i16(ptr noundef %0, i16 noundef %1) #2 {
  %3 = alloca i16, align 2
  %4 = alloca ptr, align 8
  store i16 %1, ptr %3, align 2
  call void @llvm.var.annotation.p0.p0(ptr %3, ptr @.str.2, ptr @.str.1, i32 607, ptr null)
  store ptr %0, ptr %4, align 8
  call void @llvm.var.annotation.p0.p0(ptr %4, ptr @.str.10, ptr @.str.1, i32 607, ptr null)
  %5 = load i16, ptr %3, align 2
  %6 = sext i16 %5 to i32
  %7 = load ptr, ptr %4, align 8
  %8 = load i16, ptr %7, align 2
  %9 = sext i16 %8 to i32
  %10 = shl i32 %9, %6
  %11 = trunc i32 %10 to i16
  store i16 %11, ptr %7, align 2
  %12 = load ptr, ptr %4, align 8
  %13 = load i16, ptr %12, align 2
  ret i16 %13
}

; Function Attrs: mustprogress noinline nounwind optnone uwtable
define dso_local i16 @logia_intrinsics_bin_bitwise_and_i16_i16(i16 noundef %0, i16 noundef %1) #2 {
  %3 = alloca i16, align 2
  %4 = alloca i16, align 2
  store i16 %1, ptr %3, align 2
  call void @llvm.var.annotation.p0.p0(ptr %3, ptr @.str.2, ptr @.str.1, i32 614, ptr null)
  store i16 %0, ptr %4, align 2
  call void @llvm.var.annotation.p0.p0(ptr %4, ptr @.str.2, ptr @.str.1, i32 614, ptr null)
  %5 = load i16, ptr %4, align 2
  %6 = sext i16 %5 to i32
  %7 = load i16, ptr %3, align 2
  %8 = sext i16 %7 to i32
  %9 = and i32 %6, %8
  %10 = trunc i32 %9 to i16
  ret i16 %10
}

; Function Attrs: mustprogress noinline nounwind optnone uwtable
define dso_local i16 @logia_intrinsics_bin_bitwise_or_i16_i16(i16 noundef %0, i16 noundef %1) #2 {
  %3 = alloca i16, align 2
  %4 = alloca i16, align 2
  store i16 %1, ptr %3, align 2
  call void @llvm.var.annotation.p0.p0(ptr %3, ptr @.str.2, ptr @.str.1, i32 620, ptr null)
  store i16 %0, ptr %4, align 2
  call void @llvm.var.annotation.p0.p0(ptr %4, ptr @.str.2, ptr @.str.1, i32 620, ptr null)
  %5 = load i16, ptr %4, align 2
  %6 = sext i16 %5 to i32
  %7 = load i16, ptr %3, align 2
  %8 = sext i16 %7 to i32
  %9 = or i32 %6, %8
  %10 = trunc i32 %9 to i16
  ret i16 %10
}

; Function Attrs: mustprogress noinline nounwind optnone uwtable
define dso_local i16 @logia_intrinsics_bin_bitwise_xor_i16_i16(i16 noundef %0, i16 noundef %1) #2 {
  %3 = alloca i16, align 2
  %4 = alloca i16, align 2
  store i16 %1, ptr %3, align 2
  call void @llvm.var.annotation.p0.p0(ptr %3, ptr @.str.2, ptr @.str.1, i32 626, ptr null)
  store i16 %0, ptr %4, align 2
  call void @llvm.var.annotation.p0.p0(ptr %4, ptr @.str.2, ptr @.str.1, i32 626, ptr null)
  %5 = load i16, ptr %4, align 2
  %6 = sext i16 %5 to i32
  %7 = load i16, ptr %3, align 2
  %8 = sext i16 %7 to i32
  %9 = xor i32 %6, %8
  %10 = trunc i32 %9 to i16
  ret i16 %10
}

; Function Attrs: mustprogress noinline nounwind optnone uwtable
define dso_local i16 @logia_intrinsics_bin_bitwise_shl_i16_i16(i16 noundef %0, i16 noundef %1) #2 {
  %3 = alloca i16, align 2
  %4 = alloca i16, align 2
  store i16 %1, ptr %3, align 2
  call void @llvm.var.annotation.p0.p0(ptr %3, ptr @.str.2, ptr @.str.1, i32 632, ptr null)
  store i16 %0, ptr %4, align 2
  call void @llvm.var.annotation.p0.p0(ptr %4, ptr @.str.2, ptr @.str.1, i32 632, ptr null)
  %5 = load i16, ptr %4, align 2
  %6 = sext i16 %5 to i32
  %7 = load i16, ptr %3, align 2
  %8 = sext i16 %7 to i32
  %9 = shl i32 %6, %8
  %10 = trunc i32 %9 to i16
  ret i16 %10
}

; Function Attrs: mustprogress noinline nounwind optnone uwtable
define dso_local i16 @logia_intrinsics_bin_bitwise_shr_i16_i16(i16 noundef %0, i16 noundef %1) #2 {
  %3 = alloca i16, align 2
  %4 = alloca i16, align 2
  store i16 %1, ptr %3, align 2
  call void @llvm.var.annotation.p0.p0(ptr %3, ptr @.str.2, ptr @.str.1, i32 638, ptr null)
  store i16 %0, ptr %4, align 2
  call void @llvm.var.annotation.p0.p0(ptr %4, ptr @.str.2, ptr @.str.1, i32 638, ptr null)
  %5 = load i16, ptr %4, align 2
  %6 = sext i16 %5 to i32
  %7 = load i16, ptr %3, align 2
  %8 = sext i16 %7 to i32
  %9 = ashr i32 %6, %8
  %10 = trunc i32 %9 to i16
  ret i16 %10
}

; Function Attrs: mustprogress noinline nounwind optnone uwtable
define dso_local i32 @logia__intrinsics_bin_add_i32_i32(i32 noundef %0, i32 noundef %1) #2 {
  %3 = alloca i32, align 4
  %4 = alloca i32, align 4
  store i32 %1, ptr %3, align 4
  call void @llvm.var.annotation.p0.p0(ptr %3, ptr @.str.3, ptr @.str.1, i32 644, ptr null)
  store i32 %0, ptr %4, align 4
  call void @llvm.var.annotation.p0.p0(ptr %4, ptr @.str.3, ptr @.str.1, i32 644, ptr null)
  %5 = load i32, ptr %4, align 4
  %6 = load i32, ptr %3, align 4
  %7 = add nsw i32 %5, %6
  ret i32 %7
}

; Function Attrs: mustprogress noinline nounwind optnone uwtable
define dso_local i32 @logia_intrinsics_bin_sub_i32_i32(i32 noundef %0, i32 noundef %1) #2 {
  %3 = alloca i32, align 4
  %4 = alloca i32, align 4
  store i32 %1, ptr %3, align 4
  call void @llvm.var.annotation.p0.p0(ptr %3, ptr @.str.3, ptr @.str.1, i32 650, ptr null)
  store i32 %0, ptr %4, align 4
  call void @llvm.var.annotation.p0.p0(ptr %4, ptr @.str.3, ptr @.str.1, i32 650, ptr null)
  %5 = load i32, ptr %4, align 4
  %6 = load i32, ptr %3, align 4
  %7 = sub nsw i32 %5, %6
  ret i32 %7
}

; Function Attrs: mustprogress noinline nounwind optnone uwtable
define dso_local i32 @logia_intrinsics_bin_mul_i32_i32(i32 noundef %0, i32 noundef %1) #2 {
  %3 = alloca i32, align 4
  %4 = alloca i32, align 4
  store i32 %1, ptr %3, align 4
  call void @llvm.var.annotation.p0.p0(ptr %3, ptr @.str.3, ptr @.str.1, i32 656, ptr null)
  store i32 %0, ptr %4, align 4
  call void @llvm.var.annotation.p0.p0(ptr %4, ptr @.str.3, ptr @.str.1, i32 656, ptr null)
  %5 = load i32, ptr %4, align 4
  %6 = load i32, ptr %3, align 4
  %7 = mul nsw i32 %5, %6
  ret i32 %7
}

; Function Attrs: mustprogress noinline nounwind optnone uwtable
define dso_local i32 @logia_intrinsics_bin_div_i32_i32(i32 noundef %0, i32 noundef %1) #2 {
  %3 = alloca i32, align 4
  %4 = alloca i32, align 4
  store i32 %1, ptr %3, align 4
  call void @llvm.var.annotation.p0.p0(ptr %3, ptr @.str.3, ptr @.str.1, i32 662, ptr null)
  store i32 %0, ptr %4, align 4
  call void @llvm.var.annotation.p0.p0(ptr %4, ptr @.str.3, ptr @.str.1, i32 662, ptr null)
  %5 = load i32, ptr %4, align 4
  %6 = load i32, ptr %3, align 4
  %7 = sdiv i32 %5, %6
  ret i32 %7
}

; Function Attrs: mustprogress noinline nounwind optnone uwtable
define dso_local i32 @logia_intrinsics_bin_mod_i32_i32(i32 noundef %0, i32 noundef %1) #2 {
  %3 = alloca i32, align 4
  %4 = alloca i32, align 4
  store i32 %1, ptr %3, align 4
  call void @llvm.var.annotation.p0.p0(ptr %3, ptr @.str.3, ptr @.str.1, i32 668, ptr null)
  store i32 %0, ptr %4, align 4
  call void @llvm.var.annotation.p0.p0(ptr %4, ptr @.str.3, ptr @.str.1, i32 668, ptr null)
  %5 = load i32, ptr %4, align 4
  %6 = load i32, ptr %3, align 4
  %7 = srem i32 %5, %6
  ret i32 %7
}

; Function Attrs: mustprogress noinline nounwind optnone uwtable
define dso_local zeroext i1 @logia_intrinsics_bin_eq_i32_i32(i32 noundef %0, i32 noundef %1) #2 {
  %3 = alloca i32, align 4
  %4 = alloca i32, align 4
  store i32 %1, ptr %3, align 4
  call void @llvm.var.annotation.p0.p0(ptr %3, ptr @.str.3, ptr @.str.1, i32 674, ptr null)
  store i32 %0, ptr %4, align 4
  call void @llvm.var.annotation.p0.p0(ptr %4, ptr @.str.3, ptr @.str.1, i32 674, ptr null)
  %5 = load i32, ptr %4, align 4
  %6 = load i32, ptr %3, align 4
  %7 = icmp eq i32 %5, %6
  ret i1 %7
}

; Function Attrs: mustprogress noinline nounwind optnone uwtable
define dso_local zeroext i1 @logia_intrinsics_bin_neq_i32_i32(i32 noundef %0, i32 noundef %1) #2 {
  %3 = alloca i32, align 4
  %4 = alloca i32, align 4
  store i32 %1, ptr %3, align 4
  call void @llvm.var.annotation.p0.p0(ptr %3, ptr @.str.3, ptr @.str.1, i32 680, ptr null)
  store i32 %0, ptr %4, align 4
  call void @llvm.var.annotation.p0.p0(ptr %4, ptr @.str.3, ptr @.str.1, i32 680, ptr null)
  %5 = load i32, ptr %4, align 4
  %6 = load i32, ptr %3, align 4
  %7 = icmp ne i32 %5, %6
  ret i1 %7
}

; Function Attrs: mustprogress noinline nounwind optnone uwtable
define dso_local zeroext i1 @logia_intrinsics_bin_lt_i32_i32(i32 noundef %0, i32 noundef %1) #2 {
  %3 = alloca i32, align 4
  %4 = alloca i32, align 4
  store i32 %1, ptr %3, align 4
  call void @llvm.var.annotation.p0.p0(ptr %3, ptr @.str.3, ptr @.str.1, i32 686, ptr null)
  store i32 %0, ptr %4, align 4
  call void @llvm.var.annotation.p0.p0(ptr %4, ptr @.str.3, ptr @.str.1, i32 686, ptr null)
  %5 = load i32, ptr %4, align 4
  %6 = load i32, ptr %3, align 4
  %7 = icmp slt i32 %5, %6
  ret i1 %7
}

; Function Attrs: mustprogress noinline nounwind optnone uwtable
define dso_local zeroext i1 @logia_intrinsics_bin_gt_i32_i32(i32 noundef %0, i32 noundef %1) #2 {
  %3 = alloca i32, align 4
  %4 = alloca i32, align 4
  store i32 %1, ptr %3, align 4
  call void @llvm.var.annotation.p0.p0(ptr %3, ptr @.str.3, ptr @.str.1, i32 692, ptr null)
  store i32 %0, ptr %4, align 4
  call void @llvm.var.annotation.p0.p0(ptr %4, ptr @.str.3, ptr @.str.1, i32 692, ptr null)
  %5 = load i32, ptr %4, align 4
  %6 = load i32, ptr %3, align 4
  %7 = icmp sgt i32 %5, %6
  ret i1 %7
}

; Function Attrs: mustprogress noinline nounwind optnone uwtable
define dso_local zeroext i1 @logia_intrinsics_bin_lte_i32_i32(i32 noundef %0, i32 noundef %1) #2 {
  %3 = alloca i32, align 4
  %4 = alloca i32, align 4
  store i32 %1, ptr %3, align 4
  call void @llvm.var.annotation.p0.p0(ptr %3, ptr @.str.3, ptr @.str.1, i32 698, ptr null)
  store i32 %0, ptr %4, align 4
  call void @llvm.var.annotation.p0.p0(ptr %4, ptr @.str.3, ptr @.str.1, i32 698, ptr null)
  %5 = load i32, ptr %4, align 4
  %6 = load i32, ptr %3, align 4
  %7 = icmp sle i32 %5, %6
  ret i1 %7
}

; Function Attrs: mustprogress noinline nounwind optnone uwtable
define dso_local zeroext i1 @logia_intrinsics_bin_gte_i32_i32(i32 noundef %0, i32 noundef %1) #2 {
  %3 = alloca i32, align 4
  %4 = alloca i32, align 4
  store i32 %1, ptr %3, align 4
  call void @llvm.var.annotation.p0.p0(ptr %3, ptr @.str.3, ptr @.str.1, i32 704, ptr null)
  store i32 %0, ptr %4, align 4
  call void @llvm.var.annotation.p0.p0(ptr %4, ptr @.str.3, ptr @.str.1, i32 704, ptr null)
  %5 = load i32, ptr %4, align 4
  %6 = load i32, ptr %3, align 4
  %7 = icmp sge i32 %5, %6
  ret i1 %7
}

; Function Attrs: mustprogress noinline nounwind optnone uwtable
define dso_local i32 @logia_intrinsics_bin_assign_i32_i32(ptr noundef %0, i32 noundef %1) #2 {
  %3 = alloca i32, align 4
  %4 = alloca ptr, align 8
  store i32 %1, ptr %3, align 4
  call void @llvm.var.annotation.p0.p0(ptr %3, ptr @.str.3, ptr @.str.1, i32 710, ptr null)
  store ptr %0, ptr %4, align 8
  call void @llvm.var.annotation.p0.p0(ptr %4, ptr @.str.11, ptr @.str.1, i32 710, ptr null)
  %5 = load i32, ptr %3, align 4
  %6 = load ptr, ptr %4, align 8
  store i32 %5, ptr %6, align 4
  %7 = load ptr, ptr %4, align 8
  %8 = load i32, ptr %7, align 4
  ret i32 %8
}

; Function Attrs: mustprogress noinline nounwind optnone uwtable
define dso_local i32 @logia_intrinsics_bin_add_assign_i32_i32(ptr noundef %0, i32 noundef %1) #2 {
  %3 = alloca i32, align 4
  %4 = alloca ptr, align 8
  store i32 %1, ptr %3, align 4
  call void @llvm.var.annotation.p0.p0(ptr %3, ptr @.str.3, ptr @.str.1, i32 717, ptr null)
  store ptr %0, ptr %4, align 8
  call void @llvm.var.annotation.p0.p0(ptr %4, ptr @.str.11, ptr @.str.1, i32 717, ptr null)
  %5 = load i32, ptr %3, align 4
  %6 = load ptr, ptr %4, align 8
  %7 = load i32, ptr %6, align 4
  %8 = add nsw i32 %7, %5
  store i32 %8, ptr %6, align 4
  %9 = load ptr, ptr %4, align 8
  %10 = load i32, ptr %9, align 4
  ret i32 %10
}

; Function Attrs: mustprogress noinline nounwind optnone uwtable
define dso_local i32 @logia_intrinsics_bin_sub_assign_i32_i32(ptr noundef %0, i32 noundef %1) #2 {
  %3 = alloca i32, align 4
  %4 = alloca ptr, align 8
  store i32 %1, ptr %3, align 4
  call void @llvm.var.annotation.p0.p0(ptr %3, ptr @.str.3, ptr @.str.1, i32 724, ptr null)
  store ptr %0, ptr %4, align 8
  call void @llvm.var.annotation.p0.p0(ptr %4, ptr @.str.11, ptr @.str.1, i32 724, ptr null)
  %5 = load i32, ptr %3, align 4
  %6 = load ptr, ptr %4, align 8
  %7 = load i32, ptr %6, align 4
  %8 = sub nsw i32 %7, %5
  store i32 %8, ptr %6, align 4
  %9 = load ptr, ptr %4, align 8
  %10 = load i32, ptr %9, align 4
  ret i32 %10
}

; Function Attrs: mustprogress noinline nounwind optnone uwtable
define dso_local i32 @logia_intrinsics_bin_mul_assign_i32_i32(ptr noundef %0, i32 noundef %1) #2 {
  %3 = alloca i32, align 4
  %4 = alloca ptr, align 8
  store i32 %1, ptr %3, align 4
  call void @llvm.var.annotation.p0.p0(ptr %3, ptr @.str.3, ptr @.str.1, i32 731, ptr null)
  store ptr %0, ptr %4, align 8
  call void @llvm.var.annotation.p0.p0(ptr %4, ptr @.str.11, ptr @.str.1, i32 731, ptr null)
  %5 = load i32, ptr %3, align 4
  %6 = load ptr, ptr %4, align 8
  %7 = load i32, ptr %6, align 4
  %8 = mul nsw i32 %7, %5
  store i32 %8, ptr %6, align 4
  %9 = load ptr, ptr %4, align 8
  %10 = load i32, ptr %9, align 4
  ret i32 %10
}

; Function Attrs: mustprogress noinline nounwind optnone uwtable
define dso_local i32 @logia_intrinsics_bin_div_assign_i32_i32(ptr noundef %0, i32 noundef %1) #2 {
  %3 = alloca i32, align 4
  %4 = alloca ptr, align 8
  store i32 %1, ptr %3, align 4
  call void @llvm.var.annotation.p0.p0(ptr %3, ptr @.str.3, ptr @.str.1, i32 738, ptr null)
  store ptr %0, ptr %4, align 8
  call void @llvm.var.annotation.p0.p0(ptr %4, ptr @.str.11, ptr @.str.1, i32 738, ptr null)
  %5 = load i32, ptr %3, align 4
  %6 = load ptr, ptr %4, align 8
  %7 = load i32, ptr %6, align 4
  %8 = sdiv i32 %7, %5
  store i32 %8, ptr %6, align 4
  %9 = load ptr, ptr %4, align 8
  %10 = load i32, ptr %9, align 4
  ret i32 %10
}

; Function Attrs: mustprogress noinline nounwind optnone uwtable
define dso_local i32 @logia_intrinsics_bin_mod_assign_i32_i32(ptr noundef %0, i32 noundef %1) #2 {
  %3 = alloca i32, align 4
  %4 = alloca ptr, align 8
  store i32 %1, ptr %3, align 4
  call void @llvm.var.annotation.p0.p0(ptr %3, ptr @.str.3, ptr @.str.1, i32 745, ptr null)
  store ptr %0, ptr %4, align 8
  call void @llvm.var.annotation.p0.p0(ptr %4, ptr @.str.11, ptr @.str.1, i32 745, ptr null)
  %5 = load i32, ptr %3, align 4
  %6 = load ptr, ptr %4, align 8
  %7 = load i32, ptr %6, align 4
  %8 = srem i32 %7, %5
  store i32 %8, ptr %6, align 4
  %9 = load ptr, ptr %4, align 8
  %10 = load i32, ptr %9, align 4
  ret i32 %10
}

; Function Attrs: mustprogress noinline nounwind optnone uwtable
define dso_local i32 @logia_intrinsics_bin_bitwise_and_assign_i32_i32(ptr noundef %0, i32 noundef %1) #2 {
  %3 = alloca i32, align 4
  %4 = alloca ptr, align 8
  store i32 %1, ptr %3, align 4
  call void @llvm.var.annotation.p0.p0(ptr %3, ptr @.str.3, ptr @.str.1, i32 752, ptr null)
  store ptr %0, ptr %4, align 8
  call void @llvm.var.annotation.p0.p0(ptr %4, ptr @.str.11, ptr @.str.1, i32 752, ptr null)
  %5 = load i32, ptr %3, align 4
  %6 = load ptr, ptr %4, align 8
  %7 = load i32, ptr %6, align 4
  %8 = and i32 %7, %5
  store i32 %8, ptr %6, align 4
  %9 = load ptr, ptr %4, align 8
  %10 = load i32, ptr %9, align 4
  ret i32 %10
}

; Function Attrs: mustprogress noinline nounwind optnone uwtable
define dso_local i32 @logia_intrinsics_bin_bitwise_or_assign_i32_i32(ptr noundef %0, i32 noundef %1) #2 {
  %3 = alloca i32, align 4
  %4 = alloca ptr, align 8
  store i32 %1, ptr %3, align 4
  call void @llvm.var.annotation.p0.p0(ptr %3, ptr @.str.3, ptr @.str.1, i32 759, ptr null)
  store ptr %0, ptr %4, align 8
  call void @llvm.var.annotation.p0.p0(ptr %4, ptr @.str.11, ptr @.str.1, i32 759, ptr null)
  %5 = load i32, ptr %3, align 4
  %6 = load ptr, ptr %4, align 8
  %7 = load i32, ptr %6, align 4
  %8 = or i32 %7, %5
  store i32 %8, ptr %6, align 4
  %9 = load ptr, ptr %4, align 8
  %10 = load i32, ptr %9, align 4
  ret i32 %10
}

; Function Attrs: mustprogress noinline nounwind optnone uwtable
define dso_local i32 @logia_intrinsics_bin_bitwise_xor_assign_i32_i32(ptr noundef %0, i32 noundef %1) #2 {
  %3 = alloca i32, align 4
  %4 = alloca ptr, align 8
  store i32 %1, ptr %3, align 4
  call void @llvm.var.annotation.p0.p0(ptr %3, ptr @.str.3, ptr @.str.1, i32 766, ptr null)
  store ptr %0, ptr %4, align 8
  call void @llvm.var.annotation.p0.p0(ptr %4, ptr @.str.11, ptr @.str.1, i32 766, ptr null)
  %5 = load i32, ptr %3, align 4
  %6 = load ptr, ptr %4, align 8
  %7 = load i32, ptr %6, align 4
  %8 = xor i32 %7, %5
  store i32 %8, ptr %6, align 4
  %9 = load ptr, ptr %4, align 8
  %10 = load i32, ptr %9, align 4
  ret i32 %10
}

; Function Attrs: mustprogress noinline nounwind optnone uwtable
define dso_local i32 @logia_intrinsics_bin_bitwise_shr_assign_i32_i32(ptr noundef %0, i32 noundef %1) #2 {
  %3 = alloca i32, align 4
  %4 = alloca ptr, align 8
  store i32 %1, ptr %3, align 4
  call void @llvm.var.annotation.p0.p0(ptr %3, ptr @.str.3, ptr @.str.1, i32 773, ptr null)
  store ptr %0, ptr %4, align 8
  call void @llvm.var.annotation.p0.p0(ptr %4, ptr @.str.11, ptr @.str.1, i32 773, ptr null)
  %5 = load i32, ptr %3, align 4
  %6 = load ptr, ptr %4, align 8
  %7 = load i32, ptr %6, align 4
  %8 = ashr i32 %7, %5
  store i32 %8, ptr %6, align 4
  %9 = load ptr, ptr %4, align 8
  %10 = load i32, ptr %9, align 4
  ret i32 %10
}

; Function Attrs: mustprogress noinline nounwind optnone uwtable
define dso_local i32 @logia_intrinsics_bin_bitwise_shl_assign_i32_i32(ptr noundef %0, i32 noundef %1) #2 {
  %3 = alloca i32, align 4
  %4 = alloca ptr, align 8
  store i32 %1, ptr %3, align 4
  call void @llvm.var.annotation.p0.p0(ptr %3, ptr @.str.3, ptr @.str.1, i32 780, ptr null)
  store ptr %0, ptr %4, align 8
  call void @llvm.var.annotation.p0.p0(ptr %4, ptr @.str.11, ptr @.str.1, i32 780, ptr null)
  %5 = load i32, ptr %3, align 4
  %6 = load ptr, ptr %4, align 8
  %7 = load i32, ptr %6, align 4
  %8 = shl i32 %7, %5
  store i32 %8, ptr %6, align 4
  %9 = load ptr, ptr %4, align 8
  %10 = load i32, ptr %9, align 4
  ret i32 %10
}

; Function Attrs: mustprogress noinline nounwind optnone uwtable
define dso_local i32 @logia_intrinsics_bin_bitwise_and_i32_i32(i32 noundef %0, i32 noundef %1) #2 {
  %3 = alloca i32, align 4
  %4 = alloca i32, align 4
  store i32 %1, ptr %3, align 4
  call void @llvm.var.annotation.p0.p0(ptr %3, ptr @.str.3, ptr @.str.1, i32 787, ptr null)
  store i32 %0, ptr %4, align 4
  call void @llvm.var.annotation.p0.p0(ptr %4, ptr @.str.3, ptr @.str.1, i32 787, ptr null)
  %5 = load i32, ptr %4, align 4
  %6 = load i32, ptr %3, align 4
  %7 = and i32 %5, %6
  ret i32 %7
}

; Function Attrs: mustprogress noinline nounwind optnone uwtable
define dso_local i32 @logia_intrinsics_bin_bitwise_or_i32_i32(i32 noundef %0, i32 noundef %1) #2 {
  %3 = alloca i32, align 4
  %4 = alloca i32, align 4
  store i32 %1, ptr %3, align 4
  call void @llvm.var.annotation.p0.p0(ptr %3, ptr @.str.3, ptr @.str.1, i32 793, ptr null)
  store i32 %0, ptr %4, align 4
  call void @llvm.var.annotation.p0.p0(ptr %4, ptr @.str.3, ptr @.str.1, i32 793, ptr null)
  %5 = load i32, ptr %4, align 4
  %6 = load i32, ptr %3, align 4
  %7 = or i32 %5, %6
  ret i32 %7
}

; Function Attrs: mustprogress noinline nounwind optnone uwtable
define dso_local i32 @logia_intrinsics_bin_bitwise_xor_i32_i32(i32 noundef %0, i32 noundef %1) #2 {
  %3 = alloca i32, align 4
  %4 = alloca i32, align 4
  store i32 %1, ptr %3, align 4
  call void @llvm.var.annotation.p0.p0(ptr %3, ptr @.str.3, ptr @.str.1, i32 799, ptr null)
  store i32 %0, ptr %4, align 4
  call void @llvm.var.annotation.p0.p0(ptr %4, ptr @.str.3, ptr @.str.1, i32 799, ptr null)
  %5 = load i32, ptr %4, align 4
  %6 = load i32, ptr %3, align 4
  %7 = xor i32 %5, %6
  ret i32 %7
}

; Function Attrs: mustprogress noinline nounwind optnone uwtable
define dso_local i32 @logia_intrinsics_bin_bitwise_shl_i32_i32(i32 noundef %0, i32 noundef %1) #2 {
  %3 = alloca i32, align 4
  %4 = alloca i32, align 4
  store i32 %1, ptr %3, align 4
  call void @llvm.var.annotation.p0.p0(ptr %3, ptr @.str.3, ptr @.str.1, i32 805, ptr null)
  store i32 %0, ptr %4, align 4
  call void @llvm.var.annotation.p0.p0(ptr %4, ptr @.str.3, ptr @.str.1, i32 805, ptr null)
  %5 = load i32, ptr %4, align 4
  %6 = load i32, ptr %3, align 4
  %7 = shl i32 %5, %6
  ret i32 %7
}

; Function Attrs: mustprogress noinline nounwind optnone uwtable
define dso_local i32 @logia_intrinsics_bin_bitwise_shr_i32_i32(i32 noundef %0, i32 noundef %1) #2 {
  %3 = alloca i32, align 4
  %4 = alloca i32, align 4
  store i32 %1, ptr %3, align 4
  call void @llvm.var.annotation.p0.p0(ptr %3, ptr @.str.3, ptr @.str.1, i32 811, ptr null)
  store i32 %0, ptr %4, align 4
  call void @llvm.var.annotation.p0.p0(ptr %4, ptr @.str.3, ptr @.str.1, i32 811, ptr null)
  %5 = load i32, ptr %4, align 4
  %6 = load i32, ptr %3, align 4
  %7 = ashr i32 %5, %6
  ret i32 %7
}

; Function Attrs: mustprogress noinline nounwind optnone uwtable
define dso_local i64 @logia__intrinsics_bin_add_i64_i64(i64 noundef %0, i64 noundef %1) #2 {
  %3 = alloca i64, align 8
  %4 = alloca i64, align 8
  store i64 %1, ptr %3, align 8
  call void @llvm.var.annotation.p0.p0(ptr %3, ptr @.str.4, ptr @.str.1, i32 817, ptr null)
  store i64 %0, ptr %4, align 8
  call void @llvm.var.annotation.p0.p0(ptr %4, ptr @.str.4, ptr @.str.1, i32 817, ptr null)
  %5 = load i64, ptr %4, align 8
  %6 = load i64, ptr %3, align 8
  %7 = add nsw i64 %5, %6
  ret i64 %7
}

; Function Attrs: mustprogress noinline nounwind optnone uwtable
define dso_local i64 @logia_intrinsics_bin_sub_i64_i64(i64 noundef %0, i64 noundef %1) #2 {
  %3 = alloca i64, align 8
  %4 = alloca i64, align 8
  store i64 %1, ptr %3, align 8
  call void @llvm.var.annotation.p0.p0(ptr %3, ptr @.str.4, ptr @.str.1, i32 823, ptr null)
  store i64 %0, ptr %4, align 8
  call void @llvm.var.annotation.p0.p0(ptr %4, ptr @.str.4, ptr @.str.1, i32 823, ptr null)
  %5 = load i64, ptr %4, align 8
  %6 = load i64, ptr %3, align 8
  %7 = sub nsw i64 %5, %6
  ret i64 %7
}

; Function Attrs: mustprogress noinline nounwind optnone uwtable
define dso_local i64 @logia_intrinsics_bin_mul_i64_i64(i64 noundef %0, i64 noundef %1) #2 {
  %3 = alloca i64, align 8
  %4 = alloca i64, align 8
  store i64 %1, ptr %3, align 8
  call void @llvm.var.annotation.p0.p0(ptr %3, ptr @.str.4, ptr @.str.1, i32 829, ptr null)
  store i64 %0, ptr %4, align 8
  call void @llvm.var.annotation.p0.p0(ptr %4, ptr @.str.4, ptr @.str.1, i32 829, ptr null)
  %5 = load i64, ptr %4, align 8
  %6 = load i64, ptr %3, align 8
  %7 = mul nsw i64 %5, %6
  ret i64 %7
}

; Function Attrs: mustprogress noinline nounwind optnone uwtable
define dso_local i64 @logia_intrinsics_bin_div_i64_i64(i64 noundef %0, i64 noundef %1) #2 {
  %3 = alloca i64, align 8
  %4 = alloca i64, align 8
  store i64 %1, ptr %3, align 8
  call void @llvm.var.annotation.p0.p0(ptr %3, ptr @.str.4, ptr @.str.1, i32 835, ptr null)
  store i64 %0, ptr %4, align 8
  call void @llvm.var.annotation.p0.p0(ptr %4, ptr @.str.4, ptr @.str.1, i32 835, ptr null)
  %5 = load i64, ptr %4, align 8
  %6 = load i64, ptr %3, align 8
  %7 = sdiv i64 %5, %6
  ret i64 %7
}

; Function Attrs: mustprogress noinline nounwind optnone uwtable
define dso_local i64 @logia_intrinsics_bin_mod_i64_i64(i64 noundef %0, i64 noundef %1) #2 {
  %3 = alloca i64, align 8
  %4 = alloca i64, align 8
  store i64 %1, ptr %3, align 8
  call void @llvm.var.annotation.p0.p0(ptr %3, ptr @.str.4, ptr @.str.1, i32 841, ptr null)
  store i64 %0, ptr %4, align 8
  call void @llvm.var.annotation.p0.p0(ptr %4, ptr @.str.4, ptr @.str.1, i32 841, ptr null)
  %5 = load i64, ptr %4, align 8
  %6 = load i64, ptr %3, align 8
  %7 = srem i64 %5, %6
  ret i64 %7
}

; Function Attrs: mustprogress noinline nounwind optnone uwtable
define dso_local zeroext i1 @logia_intrinsics_bin_eq_i64_i64(i64 noundef %0, i64 noundef %1) #2 {
  %3 = alloca i64, align 8
  %4 = alloca i64, align 8
  store i64 %1, ptr %3, align 8
  call void @llvm.var.annotation.p0.p0(ptr %3, ptr @.str.4, ptr @.str.1, i32 847, ptr null)
  store i64 %0, ptr %4, align 8
  call void @llvm.var.annotation.p0.p0(ptr %4, ptr @.str.4, ptr @.str.1, i32 847, ptr null)
  %5 = load i64, ptr %4, align 8
  %6 = load i64, ptr %3, align 8
  %7 = icmp eq i64 %5, %6
  ret i1 %7
}

; Function Attrs: mustprogress noinline nounwind optnone uwtable
define dso_local zeroext i1 @logia_intrinsics_bin_neq_i64_i64(i64 noundef %0, i64 noundef %1) #2 {
  %3 = alloca i64, align 8
  %4 = alloca i64, align 8
  store i64 %1, ptr %3, align 8
  call void @llvm.var.annotation.p0.p0(ptr %3, ptr @.str.4, ptr @.str.1, i32 853, ptr null)
  store i64 %0, ptr %4, align 8
  call void @llvm.var.annotation.p0.p0(ptr %4, ptr @.str.4, ptr @.str.1, i32 853, ptr null)
  %5 = load i64, ptr %4, align 8
  %6 = load i64, ptr %3, align 8
  %7 = icmp ne i64 %5, %6
  ret i1 %7
}

; Function Attrs: mustprogress noinline nounwind optnone uwtable
define dso_local zeroext i1 @logia_intrinsics_bin_lt_i64_i64(i64 noundef %0, i64 noundef %1) #2 {
  %3 = alloca i64, align 8
  %4 = alloca i64, align 8
  store i64 %1, ptr %3, align 8
  call void @llvm.var.annotation.p0.p0(ptr %3, ptr @.str.4, ptr @.str.1, i32 859, ptr null)
  store i64 %0, ptr %4, align 8
  call void @llvm.var.annotation.p0.p0(ptr %4, ptr @.str.4, ptr @.str.1, i32 859, ptr null)
  %5 = load i64, ptr %4, align 8
  %6 = load i64, ptr %3, align 8
  %7 = icmp slt i64 %5, %6
  ret i1 %7
}

; Function Attrs: mustprogress noinline nounwind optnone uwtable
define dso_local zeroext i1 @logia_intrinsics_bin_gt_i64_i64(i64 noundef %0, i64 noundef %1) #2 {
  %3 = alloca i64, align 8
  %4 = alloca i64, align 8
  store i64 %1, ptr %3, align 8
  call void @llvm.var.annotation.p0.p0(ptr %3, ptr @.str.4, ptr @.str.1, i32 865, ptr null)
  store i64 %0, ptr %4, align 8
  call void @llvm.var.annotation.p0.p0(ptr %4, ptr @.str.4, ptr @.str.1, i32 865, ptr null)
  %5 = load i64, ptr %4, align 8
  %6 = load i64, ptr %3, align 8
  %7 = icmp sgt i64 %5, %6
  ret i1 %7
}

; Function Attrs: mustprogress noinline nounwind optnone uwtable
define dso_local zeroext i1 @logia_intrinsics_bin_lte_i64_i64(i64 noundef %0, i64 noundef %1) #2 {
  %3 = alloca i64, align 8
  %4 = alloca i64, align 8
  store i64 %1, ptr %3, align 8
  call void @llvm.var.annotation.p0.p0(ptr %3, ptr @.str.4, ptr @.str.1, i32 871, ptr null)
  store i64 %0, ptr %4, align 8
  call void @llvm.var.annotation.p0.p0(ptr %4, ptr @.str.4, ptr @.str.1, i32 871, ptr null)
  %5 = load i64, ptr %4, align 8
  %6 = load i64, ptr %3, align 8
  %7 = icmp sle i64 %5, %6
  ret i1 %7
}

; Function Attrs: mustprogress noinline nounwind optnone uwtable
define dso_local zeroext i1 @logia_intrinsics_bin_gte_i64_i64(i64 noundef %0, i64 noundef %1) #2 {
  %3 = alloca i64, align 8
  %4 = alloca i64, align 8
  store i64 %1, ptr %3, align 8
  call void @llvm.var.annotation.p0.p0(ptr %3, ptr @.str.4, ptr @.str.1, i32 877, ptr null)
  store i64 %0, ptr %4, align 8
  call void @llvm.var.annotation.p0.p0(ptr %4, ptr @.str.4, ptr @.str.1, i32 877, ptr null)
  %5 = load i64, ptr %4, align 8
  %6 = load i64, ptr %3, align 8
  %7 = icmp sge i64 %5, %6
  ret i1 %7
}

; Function Attrs: mustprogress noinline nounwind optnone uwtable
define dso_local i64 @logia_intrinsics_bin_assign_i64_i64(ptr noundef %0, i64 noundef %1) #2 {
  %3 = alloca i64, align 8
  %4 = alloca ptr, align 8
  store i64 %1, ptr %3, align 8
  call void @llvm.var.annotation.p0.p0(ptr %3, ptr @.str.4, ptr @.str.1, i32 883, ptr null)
  store ptr %0, ptr %4, align 8
  call void @llvm.var.annotation.p0.p0(ptr %4, ptr @.str.12, ptr @.str.1, i32 883, ptr null)
  %5 = load i64, ptr %3, align 8
  %6 = load ptr, ptr %4, align 8
  store i64 %5, ptr %6, align 8
  %7 = load ptr, ptr %4, align 8
  %8 = load i64, ptr %7, align 8
  ret i64 %8
}

; Function Attrs: mustprogress noinline nounwind optnone uwtable
define dso_local i64 @logia_intrinsics_bin_add_assign_i64_i64(ptr noundef %0, i64 noundef %1) #2 {
  %3 = alloca i64, align 8
  %4 = alloca ptr, align 8
  store i64 %1, ptr %3, align 8
  call void @llvm.var.annotation.p0.p0(ptr %3, ptr @.str.4, ptr @.str.1, i32 890, ptr null)
  store ptr %0, ptr %4, align 8
  call void @llvm.var.annotation.p0.p0(ptr %4, ptr @.str.12, ptr @.str.1, i32 890, ptr null)
  %5 = load i64, ptr %3, align 8
  %6 = load ptr, ptr %4, align 8
  %7 = load i64, ptr %6, align 8
  %8 = add nsw i64 %7, %5
  store i64 %8, ptr %6, align 8
  %9 = load ptr, ptr %4, align 8
  %10 = load i64, ptr %9, align 8
  ret i64 %10
}

; Function Attrs: mustprogress noinline nounwind optnone uwtable
define dso_local i64 @logia_intrinsics_bin_sub_assign_i64_i64(ptr noundef %0, i64 noundef %1) #2 {
  %3 = alloca i64, align 8
  %4 = alloca ptr, align 8
  store i64 %1, ptr %3, align 8
  call void @llvm.var.annotation.p0.p0(ptr %3, ptr @.str.4, ptr @.str.1, i32 897, ptr null)
  store ptr %0, ptr %4, align 8
  call void @llvm.var.annotation.p0.p0(ptr %4, ptr @.str.12, ptr @.str.1, i32 897, ptr null)
  %5 = load i64, ptr %3, align 8
  %6 = load ptr, ptr %4, align 8
  %7 = load i64, ptr %6, align 8
  %8 = sub nsw i64 %7, %5
  store i64 %8, ptr %6, align 8
  %9 = load ptr, ptr %4, align 8
  %10 = load i64, ptr %9, align 8
  ret i64 %10
}

; Function Attrs: mustprogress noinline nounwind optnone uwtable
define dso_local i64 @logia_intrinsics_bin_mul_assign_i64_i64(ptr noundef %0, i64 noundef %1) #2 {
  %3 = alloca i64, align 8
  %4 = alloca ptr, align 8
  store i64 %1, ptr %3, align 8
  call void @llvm.var.annotation.p0.p0(ptr %3, ptr @.str.4, ptr @.str.1, i32 904, ptr null)
  store ptr %0, ptr %4, align 8
  call void @llvm.var.annotation.p0.p0(ptr %4, ptr @.str.12, ptr @.str.1, i32 904, ptr null)
  %5 = load i64, ptr %3, align 8
  %6 = load ptr, ptr %4, align 8
  %7 = load i64, ptr %6, align 8
  %8 = mul nsw i64 %7, %5
  store i64 %8, ptr %6, align 8
  %9 = load ptr, ptr %4, align 8
  %10 = load i64, ptr %9, align 8
  ret i64 %10
}

; Function Attrs: mustprogress noinline nounwind optnone uwtable
define dso_local i64 @logia_intrinsics_bin_div_assign_i64_i64(ptr noundef %0, i64 noundef %1) #2 {
  %3 = alloca i64, align 8
  %4 = alloca ptr, align 8
  store i64 %1, ptr %3, align 8
  call void @llvm.var.annotation.p0.p0(ptr %3, ptr @.str.4, ptr @.str.1, i32 911, ptr null)
  store ptr %0, ptr %4, align 8
  call void @llvm.var.annotation.p0.p0(ptr %4, ptr @.str.12, ptr @.str.1, i32 911, ptr null)
  %5 = load i64, ptr %3, align 8
  %6 = load ptr, ptr %4, align 8
  %7 = load i64, ptr %6, align 8
  %8 = sdiv i64 %7, %5
  store i64 %8, ptr %6, align 8
  %9 = load ptr, ptr %4, align 8
  %10 = load i64, ptr %9, align 8
  ret i64 %10
}

; Function Attrs: mustprogress noinline nounwind optnone uwtable
define dso_local i64 @logia_intrinsics_bin_mod_assign_i64_i64(ptr noundef %0, i64 noundef %1) #2 {
  %3 = alloca i64, align 8
  %4 = alloca ptr, align 8
  store i64 %1, ptr %3, align 8
  call void @llvm.var.annotation.p0.p0(ptr %3, ptr @.str.4, ptr @.str.1, i32 918, ptr null)
  store ptr %0, ptr %4, align 8
  call void @llvm.var.annotation.p0.p0(ptr %4, ptr @.str.12, ptr @.str.1, i32 918, ptr null)
  %5 = load i64, ptr %3, align 8
  %6 = load ptr, ptr %4, align 8
  %7 = load i64, ptr %6, align 8
  %8 = srem i64 %7, %5
  store i64 %8, ptr %6, align 8
  %9 = load ptr, ptr %4, align 8
  %10 = load i64, ptr %9, align 8
  ret i64 %10
}

; Function Attrs: mustprogress noinline nounwind optnone uwtable
define dso_local i64 @logia_intrinsics_bin_bitwise_and_assign_i64_i64(ptr noundef %0, i64 noundef %1) #2 {
  %3 = alloca i64, align 8
  %4 = alloca ptr, align 8
  store i64 %1, ptr %3, align 8
  call void @llvm.var.annotation.p0.p0(ptr %3, ptr @.str.4, ptr @.str.1, i32 925, ptr null)
  store ptr %0, ptr %4, align 8
  call void @llvm.var.annotation.p0.p0(ptr %4, ptr @.str.12, ptr @.str.1, i32 925, ptr null)
  %5 = load i64, ptr %3, align 8
  %6 = load ptr, ptr %4, align 8
  %7 = load i64, ptr %6, align 8
  %8 = and i64 %7, %5
  store i64 %8, ptr %6, align 8
  %9 = load ptr, ptr %4, align 8
  %10 = load i64, ptr %9, align 8
  ret i64 %10
}

; Function Attrs: mustprogress noinline nounwind optnone uwtable
define dso_local i64 @logia_intrinsics_bin_bitwise_or_assign_i64_i64(ptr noundef %0, i64 noundef %1) #2 {
  %3 = alloca i64, align 8
  %4 = alloca ptr, align 8
  store i64 %1, ptr %3, align 8
  call void @llvm.var.annotation.p0.p0(ptr %3, ptr @.str.4, ptr @.str.1, i32 932, ptr null)
  store ptr %0, ptr %4, align 8
  call void @llvm.var.annotation.p0.p0(ptr %4, ptr @.str.12, ptr @.str.1, i32 932, ptr null)
  %5 = load i64, ptr %3, align 8
  %6 = load ptr, ptr %4, align 8
  %7 = load i64, ptr %6, align 8
  %8 = or i64 %7, %5
  store i64 %8, ptr %6, align 8
  %9 = load ptr, ptr %4, align 8
  %10 = load i64, ptr %9, align 8
  ret i64 %10
}

; Function Attrs: mustprogress noinline nounwind optnone uwtable
define dso_local i64 @logia_intrinsics_bin_bitwise_xor_assign_i64_i64(ptr noundef %0, i64 noundef %1) #2 {
  %3 = alloca i64, align 8
  %4 = alloca ptr, align 8
  store i64 %1, ptr %3, align 8
  call void @llvm.var.annotation.p0.p0(ptr %3, ptr @.str.4, ptr @.str.1, i32 939, ptr null)
  store ptr %0, ptr %4, align 8
  call void @llvm.var.annotation.p0.p0(ptr %4, ptr @.str.12, ptr @.str.1, i32 939, ptr null)
  %5 = load i64, ptr %3, align 8
  %6 = load ptr, ptr %4, align 8
  %7 = load i64, ptr %6, align 8
  %8 = xor i64 %7, %5
  store i64 %8, ptr %6, align 8
  %9 = load ptr, ptr %4, align 8
  %10 = load i64, ptr %9, align 8
  ret i64 %10
}

; Function Attrs: mustprogress noinline nounwind optnone uwtable
define dso_local i64 @logia_intrinsics_bin_bitwise_shr_assign_i64_i64(ptr noundef %0, i64 noundef %1) #2 {
  %3 = alloca i64, align 8
  %4 = alloca ptr, align 8
  store i64 %1, ptr %3, align 8
  call void @llvm.var.annotation.p0.p0(ptr %3, ptr @.str.4, ptr @.str.1, i32 946, ptr null)
  store ptr %0, ptr %4, align 8
  call void @llvm.var.annotation.p0.p0(ptr %4, ptr @.str.12, ptr @.str.1, i32 946, ptr null)
  %5 = load i64, ptr %3, align 8
  %6 = load ptr, ptr %4, align 8
  %7 = load i64, ptr %6, align 8
  %8 = ashr i64 %7, %5
  store i64 %8, ptr %6, align 8
  %9 = load ptr, ptr %4, align 8
  %10 = load i64, ptr %9, align 8
  ret i64 %10
}

; Function Attrs: mustprogress noinline nounwind optnone uwtable
define dso_local i64 @logia_intrinsics_bin_bitwise_shl_assign_i64_i64(ptr noundef %0, i64 noundef %1) #2 {
  %3 = alloca i64, align 8
  %4 = alloca ptr, align 8
  store i64 %1, ptr %3, align 8
  call void @llvm.var.annotation.p0.p0(ptr %3, ptr @.str.4, ptr @.str.1, i32 953, ptr null)
  store ptr %0, ptr %4, align 8
  call void @llvm.var.annotation.p0.p0(ptr %4, ptr @.str.12, ptr @.str.1, i32 953, ptr null)
  %5 = load i64, ptr %3, align 8
  %6 = load ptr, ptr %4, align 8
  %7 = load i64, ptr %6, align 8
  %8 = shl i64 %7, %5
  store i64 %8, ptr %6, align 8
  %9 = load ptr, ptr %4, align 8
  %10 = load i64, ptr %9, align 8
  ret i64 %10
}

; Function Attrs: mustprogress noinline nounwind optnone uwtable
define dso_local i64 @logia_intrinsics_bin_bitwise_and_i64_i64(i64 noundef %0, i64 noundef %1) #2 {
  %3 = alloca i64, align 8
  %4 = alloca i64, align 8
  store i64 %1, ptr %3, align 8
  call void @llvm.var.annotation.p0.p0(ptr %3, ptr @.str.4, ptr @.str.1, i32 960, ptr null)
  store i64 %0, ptr %4, align 8
  call void @llvm.var.annotation.p0.p0(ptr %4, ptr @.str.4, ptr @.str.1, i32 960, ptr null)
  %5 = load i64, ptr %4, align 8
  %6 = load i64, ptr %3, align 8
  %7 = and i64 %5, %6
  ret i64 %7
}

; Function Attrs: mustprogress noinline nounwind optnone uwtable
define dso_local i64 @logia_intrinsics_bin_bitwise_or_i64_i64(i64 noundef %0, i64 noundef %1) #2 {
  %3 = alloca i64, align 8
  %4 = alloca i64, align 8
  store i64 %1, ptr %3, align 8
  call void @llvm.var.annotation.p0.p0(ptr %3, ptr @.str.4, ptr @.str.1, i32 966, ptr null)
  store i64 %0, ptr %4, align 8
  call void @llvm.var.annotation.p0.p0(ptr %4, ptr @.str.4, ptr @.str.1, i32 966, ptr null)
  %5 = load i64, ptr %4, align 8
  %6 = load i64, ptr %3, align 8
  %7 = or i64 %5, %6
  ret i64 %7
}

; Function Attrs: mustprogress noinline nounwind optnone uwtable
define dso_local i64 @logia_intrinsics_bin_bitwise_xor_i64_i64(i64 noundef %0, i64 noundef %1) #2 {
  %3 = alloca i64, align 8
  %4 = alloca i64, align 8
  store i64 %1, ptr %3, align 8
  call void @llvm.var.annotation.p0.p0(ptr %3, ptr @.str.4, ptr @.str.1, i32 972, ptr null)
  store i64 %0, ptr %4, align 8
  call void @llvm.var.annotation.p0.p0(ptr %4, ptr @.str.4, ptr @.str.1, i32 972, ptr null)
  %5 = load i64, ptr %4, align 8
  %6 = load i64, ptr %3, align 8
  %7 = xor i64 %5, %6
  ret i64 %7
}

; Function Attrs: mustprogress noinline nounwind optnone uwtable
define dso_local i64 @logia_intrinsics_bin_bitwise_shl_i64_i64(i64 noundef %0, i64 noundef %1) #2 {
  %3 = alloca i64, align 8
  %4 = alloca i64, align 8
  store i64 %1, ptr %3, align 8
  call void @llvm.var.annotation.p0.p0(ptr %3, ptr @.str.4, ptr @.str.1, i32 978, ptr null)
  store i64 %0, ptr %4, align 8
  call void @llvm.var.annotation.p0.p0(ptr %4, ptr @.str.4, ptr @.str.1, i32 978, ptr null)
  %5 = load i64, ptr %4, align 8
  %6 = load i64, ptr %3, align 8
  %7 = shl i64 %5, %6
  ret i64 %7
}

; Function Attrs: mustprogress noinline nounwind optnone uwtable
define dso_local i64 @logia_intrinsics_bin_bitwise_shr_i64_i64(i64 noundef %0, i64 noundef %1) #2 {
  %3 = alloca i64, align 8
  %4 = alloca i64, align 8
  store i64 %1, ptr %3, align 8
  call void @llvm.var.annotation.p0.p0(ptr %3, ptr @.str.4, ptr @.str.1, i32 984, ptr null)
  store i64 %0, ptr %4, align 8
  call void @llvm.var.annotation.p0.p0(ptr %4, ptr @.str.4, ptr @.str.1, i32 984, ptr null)
  %5 = load i64, ptr %4, align 8
  %6 = load i64, ptr %3, align 8
  %7 = ashr i64 %5, %6
  ret i64 %7
}

; Function Attrs: mustprogress noinline nounwind optnone uwtable
define dso_local i8 @logia__intrinsics_bin_add_u8_u8(i8 noundef %0, i8 noundef %1) #2 {
  %3 = alloca i8, align 1
  %4 = alloca i8, align 1
  store i8 %1, ptr %3, align 1
  call void @llvm.var.annotation.p0.p0(ptr %3, ptr @.str.5, ptr @.str.1, i32 990, ptr null)
  store i8 %0, ptr %4, align 1
  call void @llvm.var.annotation.p0.p0(ptr %4, ptr @.str.5, ptr @.str.1, i32 990, ptr null)
  %5 = load i8, ptr %4, align 1
  %6 = zext i8 %5 to i32
  %7 = load i8, ptr %3, align 1
  %8 = zext i8 %7 to i32
  %9 = add nsw i32 %6, %8
  %10 = trunc i32 %9 to i8
  ret i8 %10
}

; Function Attrs: mustprogress noinline nounwind optnone uwtable
define dso_local i8 @logia_intrinsics_bin_sub_u8_u8(i8 noundef %0, i8 noundef %1) #2 {
  %3 = alloca i8, align 1
  %4 = alloca i8, align 1
  store i8 %1, ptr %3, align 1
  call void @llvm.var.annotation.p0.p0(ptr %3, ptr @.str.5, ptr @.str.1, i32 996, ptr null)
  store i8 %0, ptr %4, align 1
  call void @llvm.var.annotation.p0.p0(ptr %4, ptr @.str.5, ptr @.str.1, i32 996, ptr null)
  %5 = load i8, ptr %4, align 1
  %6 = zext i8 %5 to i32
  %7 = load i8, ptr %3, align 1
  %8 = zext i8 %7 to i32
  %9 = sub nsw i32 %6, %8
  %10 = trunc i32 %9 to i8
  ret i8 %10
}

; Function Attrs: mustprogress noinline nounwind optnone uwtable
define dso_local i8 @logia_intrinsics_bin_mul_u8_u8(i8 noundef %0, i8 noundef %1) #2 {
  %3 = alloca i8, align 1
  %4 = alloca i8, align 1
  store i8 %1, ptr %3, align 1
  call void @llvm.var.annotation.p0.p0(ptr %3, ptr @.str.5, ptr @.str.1, i32 1002, ptr null)
  store i8 %0, ptr %4, align 1
  call void @llvm.var.annotation.p0.p0(ptr %4, ptr @.str.5, ptr @.str.1, i32 1002, ptr null)
  %5 = load i8, ptr %4, align 1
  %6 = zext i8 %5 to i32
  %7 = load i8, ptr %3, align 1
  %8 = zext i8 %7 to i32
  %9 = mul nsw i32 %6, %8
  %10 = trunc i32 %9 to i8
  ret i8 %10
}

; Function Attrs: mustprogress noinline nounwind optnone uwtable
define dso_local i8 @logia_intrinsics_bin_div_u8_u8(i8 noundef %0, i8 noundef %1) #2 {
  %3 = alloca i8, align 1
  %4 = alloca i8, align 1
  store i8 %1, ptr %3, align 1
  call void @llvm.var.annotation.p0.p0(ptr %3, ptr @.str.5, ptr @.str.1, i32 1008, ptr null)
  store i8 %0, ptr %4, align 1
  call void @llvm.var.annotation.p0.p0(ptr %4, ptr @.str.5, ptr @.str.1, i32 1008, ptr null)
  %5 = load i8, ptr %4, align 1
  %6 = zext i8 %5 to i32
  %7 = load i8, ptr %3, align 1
  %8 = zext i8 %7 to i32
  %9 = sdiv i32 %6, %8
  %10 = trunc i32 %9 to i8
  ret i8 %10
}

; Function Attrs: mustprogress noinline nounwind optnone uwtable
define dso_local i8 @logia_intrinsics_bin_mod_u8_u8(i8 noundef %0, i8 noundef %1) #2 {
  %3 = alloca i8, align 1
  %4 = alloca i8, align 1
  store i8 %1, ptr %3, align 1
  call void @llvm.var.annotation.p0.p0(ptr %3, ptr @.str.5, ptr @.str.1, i32 1014, ptr null)
  store i8 %0, ptr %4, align 1
  call void @llvm.var.annotation.p0.p0(ptr %4, ptr @.str.5, ptr @.str.1, i32 1014, ptr null)
  %5 = load i8, ptr %4, align 1
  %6 = zext i8 %5 to i32
  %7 = load i8, ptr %3, align 1
  %8 = zext i8 %7 to i32
  %9 = srem i32 %6, %8
  %10 = trunc i32 %9 to i8
  ret i8 %10
}

; Function Attrs: mustprogress noinline nounwind optnone uwtable
define dso_local zeroext i1 @logia_intrinsics_bin_eq_u8_u8(i8 noundef %0, i8 noundef %1) #2 {
  %3 = alloca i8, align 1
  %4 = alloca i8, align 1
  store i8 %1, ptr %3, align 1
  call void @llvm.var.annotation.p0.p0(ptr %3, ptr @.str.5, ptr @.str.1, i32 1020, ptr null)
  store i8 %0, ptr %4, align 1
  call void @llvm.var.annotation.p0.p0(ptr %4, ptr @.str.5, ptr @.str.1, i32 1020, ptr null)
  %5 = load i8, ptr %4, align 1
  %6 = zext i8 %5 to i32
  %7 = load i8, ptr %3, align 1
  %8 = zext i8 %7 to i32
  %9 = icmp eq i32 %6, %8
  ret i1 %9
}

; Function Attrs: mustprogress noinline nounwind optnone uwtable
define dso_local zeroext i1 @logia_intrinsics_bin_neq_u8_u8(i8 noundef %0, i8 noundef %1) #2 {
  %3 = alloca i8, align 1
  %4 = alloca i8, align 1
  store i8 %1, ptr %3, align 1
  call void @llvm.var.annotation.p0.p0(ptr %3, ptr @.str.5, ptr @.str.1, i32 1026, ptr null)
  store i8 %0, ptr %4, align 1
  call void @llvm.var.annotation.p0.p0(ptr %4, ptr @.str.5, ptr @.str.1, i32 1026, ptr null)
  %5 = load i8, ptr %4, align 1
  %6 = zext i8 %5 to i32
  %7 = load i8, ptr %3, align 1
  %8 = zext i8 %7 to i32
  %9 = icmp ne i32 %6, %8
  ret i1 %9
}

; Function Attrs: mustprogress noinline nounwind optnone uwtable
define dso_local zeroext i1 @logia_intrinsics_bin_lt_u8_u8(i8 noundef %0, i8 noundef %1) #2 {
  %3 = alloca i8, align 1
  %4 = alloca i8, align 1
  store i8 %1, ptr %3, align 1
  call void @llvm.var.annotation.p0.p0(ptr %3, ptr @.str.5, ptr @.str.1, i32 1032, ptr null)
  store i8 %0, ptr %4, align 1
  call void @llvm.var.annotation.p0.p0(ptr %4, ptr @.str.5, ptr @.str.1, i32 1032, ptr null)
  %5 = load i8, ptr %4, align 1
  %6 = zext i8 %5 to i32
  %7 = load i8, ptr %3, align 1
  %8 = zext i8 %7 to i32
  %9 = icmp slt i32 %6, %8
  ret i1 %9
}

; Function Attrs: mustprogress noinline nounwind optnone uwtable
define dso_local zeroext i1 @logia_intrinsics_bin_gt_u8_u8(i8 noundef %0, i8 noundef %1) #2 {
  %3 = alloca i8, align 1
  %4 = alloca i8, align 1
  store i8 %1, ptr %3, align 1
  call void @llvm.var.annotation.p0.p0(ptr %3, ptr @.str.5, ptr @.str.1, i32 1038, ptr null)
  store i8 %0, ptr %4, align 1
  call void @llvm.var.annotation.p0.p0(ptr %4, ptr @.str.5, ptr @.str.1, i32 1038, ptr null)
  %5 = load i8, ptr %4, align 1
  %6 = zext i8 %5 to i32
  %7 = load i8, ptr %3, align 1
  %8 = zext i8 %7 to i32
  %9 = icmp sgt i32 %6, %8
  ret i1 %9
}

; Function Attrs: mustprogress noinline nounwind optnone uwtable
define dso_local zeroext i1 @logia_intrinsics_bin_lte_u8_u8(i8 noundef %0, i8 noundef %1) #2 {
  %3 = alloca i8, align 1
  %4 = alloca i8, align 1
  store i8 %1, ptr %3, align 1
  call void @llvm.var.annotation.p0.p0(ptr %3, ptr @.str.5, ptr @.str.1, i32 1044, ptr null)
  store i8 %0, ptr %4, align 1
  call void @llvm.var.annotation.p0.p0(ptr %4, ptr @.str.5, ptr @.str.1, i32 1044, ptr null)
  %5 = load i8, ptr %4, align 1
  %6 = zext i8 %5 to i32
  %7 = load i8, ptr %3, align 1
  %8 = zext i8 %7 to i32
  %9 = icmp sle i32 %6, %8
  ret i1 %9
}

; Function Attrs: mustprogress noinline nounwind optnone uwtable
define dso_local zeroext i1 @logia_intrinsics_bin_gte_u8_u8(i8 noundef %0, i8 noundef %1) #2 {
  %3 = alloca i8, align 1
  %4 = alloca i8, align 1
  store i8 %1, ptr %3, align 1
  call void @llvm.var.annotation.p0.p0(ptr %3, ptr @.str.5, ptr @.str.1, i32 1050, ptr null)
  store i8 %0, ptr %4, align 1
  call void @llvm.var.annotation.p0.p0(ptr %4, ptr @.str.5, ptr @.str.1, i32 1050, ptr null)
  %5 = load i8, ptr %4, align 1
  %6 = zext i8 %5 to i32
  %7 = load i8, ptr %3, align 1
  %8 = zext i8 %7 to i32
  %9 = icmp sge i32 %6, %8
  ret i1 %9
}

; Function Attrs: mustprogress noinline nounwind optnone uwtable
define dso_local i8 @logia_intrinsics_bin_assign_u8_u8(ptr noundef %0, i8 noundef %1) #2 {
  %3 = alloca i8, align 1
  %4 = alloca ptr, align 8
  store i8 %1, ptr %3, align 1
  call void @llvm.var.annotation.p0.p0(ptr %3, ptr @.str.5, ptr @.str.1, i32 1056, ptr null)
  store ptr %0, ptr %4, align 8
  call void @llvm.var.annotation.p0.p0(ptr %4, ptr @.str.13, ptr @.str.1, i32 1056, ptr null)
  %5 = load i8, ptr %3, align 1
  %6 = load ptr, ptr %4, align 8
  store i8 %5, ptr %6, align 1
  %7 = load ptr, ptr %4, align 8
  %8 = load i8, ptr %7, align 1
  ret i8 %8
}

; Function Attrs: mustprogress noinline nounwind optnone uwtable
define dso_local i8 @logia_intrinsics_bin_add_assign_u8_u8(ptr noundef %0, i8 noundef %1) #2 {
  %3 = alloca i8, align 1
  %4 = alloca ptr, align 8
  store i8 %1, ptr %3, align 1
  call void @llvm.var.annotation.p0.p0(ptr %3, ptr @.str.5, ptr @.str.1, i32 1063, ptr null)
  store ptr %0, ptr %4, align 8
  call void @llvm.var.annotation.p0.p0(ptr %4, ptr @.str.13, ptr @.str.1, i32 1063, ptr null)
  %5 = load i8, ptr %3, align 1
  %6 = zext i8 %5 to i32
  %7 = load ptr, ptr %4, align 8
  %8 = load i8, ptr %7, align 1
  %9 = zext i8 %8 to i32
  %10 = add nsw i32 %9, %6
  %11 = trunc i32 %10 to i8
  store i8 %11, ptr %7, align 1
  %12 = load ptr, ptr %4, align 8
  %13 = load i8, ptr %12, align 1
  ret i8 %13
}

; Function Attrs: mustprogress noinline nounwind optnone uwtable
define dso_local i8 @logia_intrinsics_bin_sub_assign_u8_u8(ptr noundef %0, i8 noundef %1) #2 {
  %3 = alloca i8, align 1
  %4 = alloca ptr, align 8
  store i8 %1, ptr %3, align 1
  call void @llvm.var.annotation.p0.p0(ptr %3, ptr @.str.5, ptr @.str.1, i32 1070, ptr null)
  store ptr %0, ptr %4, align 8
  call void @llvm.var.annotation.p0.p0(ptr %4, ptr @.str.13, ptr @.str.1, i32 1070, ptr null)
  %5 = load i8, ptr %3, align 1
  %6 = zext i8 %5 to i32
  %7 = load ptr, ptr %4, align 8
  %8 = load i8, ptr %7, align 1
  %9 = zext i8 %8 to i32
  %10 = sub nsw i32 %9, %6
  %11 = trunc i32 %10 to i8
  store i8 %11, ptr %7, align 1
  %12 = load ptr, ptr %4, align 8
  %13 = load i8, ptr %12, align 1
  ret i8 %13
}

; Function Attrs: mustprogress noinline nounwind optnone uwtable
define dso_local i8 @logia_intrinsics_bin_mul_assign_u8_u8(ptr noundef %0, i8 noundef %1) #2 {
  %3 = alloca i8, align 1
  %4 = alloca ptr, align 8
  store i8 %1, ptr %3, align 1
  call void @llvm.var.annotation.p0.p0(ptr %3, ptr @.str.5, ptr @.str.1, i32 1077, ptr null)
  store ptr %0, ptr %4, align 8
  call void @llvm.var.annotation.p0.p0(ptr %4, ptr @.str.13, ptr @.str.1, i32 1077, ptr null)
  %5 = load i8, ptr %3, align 1
  %6 = zext i8 %5 to i32
  %7 = load ptr, ptr %4, align 8
  %8 = load i8, ptr %7, align 1
  %9 = zext i8 %8 to i32
  %10 = mul nsw i32 %9, %6
  %11 = trunc i32 %10 to i8
  store i8 %11, ptr %7, align 1
  %12 = load ptr, ptr %4, align 8
  %13 = load i8, ptr %12, align 1
  ret i8 %13
}

; Function Attrs: mustprogress noinline nounwind optnone uwtable
define dso_local i8 @logia_intrinsics_bin_div_assign_u8_u8(ptr noundef %0, i8 noundef %1) #2 {
  %3 = alloca i8, align 1
  %4 = alloca ptr, align 8
  store i8 %1, ptr %3, align 1
  call void @llvm.var.annotation.p0.p0(ptr %3, ptr @.str.5, ptr @.str.1, i32 1084, ptr null)
  store ptr %0, ptr %4, align 8
  call void @llvm.var.annotation.p0.p0(ptr %4, ptr @.str.13, ptr @.str.1, i32 1084, ptr null)
  %5 = load i8, ptr %3, align 1
  %6 = zext i8 %5 to i32
  %7 = load ptr, ptr %4, align 8
  %8 = load i8, ptr %7, align 1
  %9 = zext i8 %8 to i32
  %10 = sdiv i32 %9, %6
  %11 = trunc i32 %10 to i8
  store i8 %11, ptr %7, align 1
  %12 = load ptr, ptr %4, align 8
  %13 = load i8, ptr %12, align 1
  ret i8 %13
}

; Function Attrs: mustprogress noinline nounwind optnone uwtable
define dso_local i8 @logia_intrinsics_bin_mod_assign_u8_u8(ptr noundef %0, i8 noundef %1) #2 {
  %3 = alloca i8, align 1
  %4 = alloca ptr, align 8
  store i8 %1, ptr %3, align 1
  call void @llvm.var.annotation.p0.p0(ptr %3, ptr @.str.5, ptr @.str.1, i32 1091, ptr null)
  store ptr %0, ptr %4, align 8
  call void @llvm.var.annotation.p0.p0(ptr %4, ptr @.str.13, ptr @.str.1, i32 1091, ptr null)
  %5 = load i8, ptr %3, align 1
  %6 = zext i8 %5 to i32
  %7 = load ptr, ptr %4, align 8
  %8 = load i8, ptr %7, align 1
  %9 = zext i8 %8 to i32
  %10 = srem i32 %9, %6
  %11 = trunc i32 %10 to i8
  store i8 %11, ptr %7, align 1
  %12 = load ptr, ptr %4, align 8
  %13 = load i8, ptr %12, align 1
  ret i8 %13
}

; Function Attrs: mustprogress noinline nounwind optnone uwtable
define dso_local i8 @logia_intrinsics_bin_bitwise_and_assign_u8_u8(ptr noundef %0, i8 noundef %1) #2 {
  %3 = alloca i8, align 1
  %4 = alloca ptr, align 8
  store i8 %1, ptr %3, align 1
  call void @llvm.var.annotation.p0.p0(ptr %3, ptr @.str.5, ptr @.str.1, i32 1098, ptr null)
  store ptr %0, ptr %4, align 8
  call void @llvm.var.annotation.p0.p0(ptr %4, ptr @.str.13, ptr @.str.1, i32 1098, ptr null)
  %5 = load i8, ptr %3, align 1
  %6 = zext i8 %5 to i32
  %7 = load ptr, ptr %4, align 8
  %8 = load i8, ptr %7, align 1
  %9 = zext i8 %8 to i32
  %10 = and i32 %9, %6
  %11 = trunc i32 %10 to i8
  store i8 %11, ptr %7, align 1
  %12 = load ptr, ptr %4, align 8
  %13 = load i8, ptr %12, align 1
  ret i8 %13
}

; Function Attrs: mustprogress noinline nounwind optnone uwtable
define dso_local i8 @logia_intrinsics_bin_bitwise_or_assign_u8_u8(ptr noundef %0, i8 noundef %1) #2 {
  %3 = alloca i8, align 1
  %4 = alloca ptr, align 8
  store i8 %1, ptr %3, align 1
  call void @llvm.var.annotation.p0.p0(ptr %3, ptr @.str.5, ptr @.str.1, i32 1105, ptr null)
  store ptr %0, ptr %4, align 8
  call void @llvm.var.annotation.p0.p0(ptr %4, ptr @.str.13, ptr @.str.1, i32 1105, ptr null)
  %5 = load i8, ptr %3, align 1
  %6 = zext i8 %5 to i32
  %7 = load ptr, ptr %4, align 8
  %8 = load i8, ptr %7, align 1
  %9 = zext i8 %8 to i32
  %10 = or i32 %9, %6
  %11 = trunc i32 %10 to i8
  store i8 %11, ptr %7, align 1
  %12 = load ptr, ptr %4, align 8
  %13 = load i8, ptr %12, align 1
  ret i8 %13
}

; Function Attrs: mustprogress noinline nounwind optnone uwtable
define dso_local i8 @logia_intrinsics_bin_bitwise_xor_assign_u8_u8(ptr noundef %0, i8 noundef %1) #2 {
  %3 = alloca i8, align 1
  %4 = alloca ptr, align 8
  store i8 %1, ptr %3, align 1
  call void @llvm.var.annotation.p0.p0(ptr %3, ptr @.str.5, ptr @.str.1, i32 1112, ptr null)
  store ptr %0, ptr %4, align 8
  call void @llvm.var.annotation.p0.p0(ptr %4, ptr @.str.13, ptr @.str.1, i32 1112, ptr null)
  %5 = load i8, ptr %3, align 1
  %6 = zext i8 %5 to i32
  %7 = load ptr, ptr %4, align 8
  %8 = load i8, ptr %7, align 1
  %9 = zext i8 %8 to i32
  %10 = xor i32 %9, %6
  %11 = trunc i32 %10 to i8
  store i8 %11, ptr %7, align 1
  %12 = load ptr, ptr %4, align 8
  %13 = load i8, ptr %12, align 1
  ret i8 %13
}

; Function Attrs: mustprogress noinline nounwind optnone uwtable
define dso_local i8 @logia_intrinsics_bin_bitwise_shr_assign_u8_u8(ptr noundef %0, i8 noundef %1) #2 {
  %3 = alloca i8, align 1
  %4 = alloca ptr, align 8
  store i8 %1, ptr %3, align 1
  call void @llvm.var.annotation.p0.p0(ptr %3, ptr @.str.5, ptr @.str.1, i32 1119, ptr null)
  store ptr %0, ptr %4, align 8
  call void @llvm.var.annotation.p0.p0(ptr %4, ptr @.str.13, ptr @.str.1, i32 1119, ptr null)
  %5 = load i8, ptr %3, align 1
  %6 = zext i8 %5 to i32
  %7 = load ptr, ptr %4, align 8
  %8 = load i8, ptr %7, align 1
  %9 = zext i8 %8 to i32
  %10 = ashr i32 %9, %6
  %11 = trunc i32 %10 to i8
  store i8 %11, ptr %7, align 1
  %12 = load ptr, ptr %4, align 8
  %13 = load i8, ptr %12, align 1
  ret i8 %13
}

; Function Attrs: mustprogress noinline nounwind optnone uwtable
define dso_local i8 @logia_intrinsics_bin_bitwise_shl_assign_u8_u8(ptr noundef %0, i8 noundef %1) #2 {
  %3 = alloca i8, align 1
  %4 = alloca ptr, align 8
  store i8 %1, ptr %3, align 1
  call void @llvm.var.annotation.p0.p0(ptr %3, ptr @.str.5, ptr @.str.1, i32 1126, ptr null)
  store ptr %0, ptr %4, align 8
  call void @llvm.var.annotation.p0.p0(ptr %4, ptr @.str.13, ptr @.str.1, i32 1126, ptr null)
  %5 = load i8, ptr %3, align 1
  %6 = zext i8 %5 to i32
  %7 = load ptr, ptr %4, align 8
  %8 = load i8, ptr %7, align 1
  %9 = zext i8 %8 to i32
  %10 = shl i32 %9, %6
  %11 = trunc i32 %10 to i8
  store i8 %11, ptr %7, align 1
  %12 = load ptr, ptr %4, align 8
  %13 = load i8, ptr %12, align 1
  ret i8 %13
}

; Function Attrs: mustprogress noinline nounwind optnone uwtable
define dso_local i8 @logia_intrinsics_bin_bitwise_and_u8_u8(i8 noundef %0, i8 noundef %1) #2 {
  %3 = alloca i8, align 1
  %4 = alloca i8, align 1
  store i8 %1, ptr %3, align 1
  call void @llvm.var.annotation.p0.p0(ptr %3, ptr @.str.5, ptr @.str.1, i32 1133, ptr null)
  store i8 %0, ptr %4, align 1
  call void @llvm.var.annotation.p0.p0(ptr %4, ptr @.str.5, ptr @.str.1, i32 1133, ptr null)
  %5 = load i8, ptr %4, align 1
  %6 = zext i8 %5 to i32
  %7 = load i8, ptr %3, align 1
  %8 = zext i8 %7 to i32
  %9 = and i32 %6, %8
  %10 = trunc i32 %9 to i8
  ret i8 %10
}

; Function Attrs: mustprogress noinline nounwind optnone uwtable
define dso_local i8 @logia_intrinsics_bin_bitwise_or_u8_u8(i8 noundef %0, i8 noundef %1) #2 {
  %3 = alloca i8, align 1
  %4 = alloca i8, align 1
  store i8 %1, ptr %3, align 1
  call void @llvm.var.annotation.p0.p0(ptr %3, ptr @.str.5, ptr @.str.1, i32 1139, ptr null)
  store i8 %0, ptr %4, align 1
  call void @llvm.var.annotation.p0.p0(ptr %4, ptr @.str.5, ptr @.str.1, i32 1139, ptr null)
  %5 = load i8, ptr %4, align 1
  %6 = zext i8 %5 to i32
  %7 = load i8, ptr %3, align 1
  %8 = zext i8 %7 to i32
  %9 = or i32 %6, %8
  %10 = trunc i32 %9 to i8
  ret i8 %10
}

; Function Attrs: mustprogress noinline nounwind optnone uwtable
define dso_local i8 @logia_intrinsics_bin_bitwise_xor_u8_u8(i8 noundef %0, i8 noundef %1) #2 {
  %3 = alloca i8, align 1
  %4 = alloca i8, align 1
  store i8 %1, ptr %3, align 1
  call void @llvm.var.annotation.p0.p0(ptr %3, ptr @.str.5, ptr @.str.1, i32 1145, ptr null)
  store i8 %0, ptr %4, align 1
  call void @llvm.var.annotation.p0.p0(ptr %4, ptr @.str.5, ptr @.str.1, i32 1145, ptr null)
  %5 = load i8, ptr %4, align 1
  %6 = zext i8 %5 to i32
  %7 = load i8, ptr %3, align 1
  %8 = zext i8 %7 to i32
  %9 = xor i32 %6, %8
  %10 = trunc i32 %9 to i8
  ret i8 %10
}

; Function Attrs: mustprogress noinline nounwind optnone uwtable
define dso_local i8 @logia_intrinsics_bin_bitwise_shl_u8_u8(i8 noundef %0, i8 noundef %1) #2 {
  %3 = alloca i8, align 1
  %4 = alloca i8, align 1
  store i8 %1, ptr %3, align 1
  call void @llvm.var.annotation.p0.p0(ptr %3, ptr @.str.5, ptr @.str.1, i32 1151, ptr null)
  store i8 %0, ptr %4, align 1
  call void @llvm.var.annotation.p0.p0(ptr %4, ptr @.str.5, ptr @.str.1, i32 1151, ptr null)
  %5 = load i8, ptr %4, align 1
  %6 = zext i8 %5 to i32
  %7 = load i8, ptr %3, align 1
  %8 = zext i8 %7 to i32
  %9 = shl i32 %6, %8
  %10 = trunc i32 %9 to i8
  ret i8 %10
}

; Function Attrs: mustprogress noinline nounwind optnone uwtable
define dso_local i8 @logia_intrinsics_bin_bitwise_shr_u8_u8(i8 noundef %0, i8 noundef %1) #2 {
  %3 = alloca i8, align 1
  %4 = alloca i8, align 1
  store i8 %1, ptr %3, align 1
  call void @llvm.var.annotation.p0.p0(ptr %3, ptr @.str.5, ptr @.str.1, i32 1157, ptr null)
  store i8 %0, ptr %4, align 1
  call void @llvm.var.annotation.p0.p0(ptr %4, ptr @.str.5, ptr @.str.1, i32 1157, ptr null)
  %5 = load i8, ptr %4, align 1
  %6 = zext i8 %5 to i32
  %7 = load i8, ptr %3, align 1
  %8 = zext i8 %7 to i32
  %9 = ashr i32 %6, %8
  %10 = trunc i32 %9 to i8
  ret i8 %10
}

; Function Attrs: mustprogress noinline nounwind optnone uwtable
define dso_local i16 @logia__intrinsics_bin_add_u16_u16(i16 noundef %0, i16 noundef %1) #2 {
  %3 = alloca i16, align 2
  %4 = alloca i16, align 2
  store i16 %1, ptr %3, align 2
  call void @llvm.var.annotation.p0.p0(ptr %3, ptr @.str.6, ptr @.str.1, i32 1163, ptr null)
  store i16 %0, ptr %4, align 2
  call void @llvm.var.annotation.p0.p0(ptr %4, ptr @.str.6, ptr @.str.1, i32 1163, ptr null)
  %5 = load i16, ptr %4, align 2
  %6 = zext i16 %5 to i32
  %7 = load i16, ptr %3, align 2
  %8 = zext i16 %7 to i32
  %9 = add nsw i32 %6, %8
  %10 = trunc i32 %9 to i16
  ret i16 %10
}

; Function Attrs: mustprogress noinline nounwind optnone uwtable
define dso_local i16 @logia_intrinsics_bin_sub_u16_u16(i16 noundef %0, i16 noundef %1) #2 {
  %3 = alloca i16, align 2
  %4 = alloca i16, align 2
  store i16 %1, ptr %3, align 2
  call void @llvm.var.annotation.p0.p0(ptr %3, ptr @.str.6, ptr @.str.1, i32 1169, ptr null)
  store i16 %0, ptr %4, align 2
  call void @llvm.var.annotation.p0.p0(ptr %4, ptr @.str.6, ptr @.str.1, i32 1169, ptr null)
  %5 = load i16, ptr %4, align 2
  %6 = zext i16 %5 to i32
  %7 = load i16, ptr %3, align 2
  %8 = zext i16 %7 to i32
  %9 = sub nsw i32 %6, %8
  %10 = trunc i32 %9 to i16
  ret i16 %10
}

; Function Attrs: mustprogress noinline nounwind optnone uwtable
define dso_local i16 @logia_intrinsics_bin_mul_u16_u16(i16 noundef %0, i16 noundef %1) #2 {
  %3 = alloca i16, align 2
  %4 = alloca i16, align 2
  store i16 %1, ptr %3, align 2
  call void @llvm.var.annotation.p0.p0(ptr %3, ptr @.str.6, ptr @.str.1, i32 1175, ptr null)
  store i16 %0, ptr %4, align 2
  call void @llvm.var.annotation.p0.p0(ptr %4, ptr @.str.6, ptr @.str.1, i32 1175, ptr null)
  %5 = load i16, ptr %4, align 2
  %6 = zext i16 %5 to i32
  %7 = load i16, ptr %3, align 2
  %8 = zext i16 %7 to i32
  %9 = mul nsw i32 %6, %8
  %10 = trunc i32 %9 to i16
  ret i16 %10
}

; Function Attrs: mustprogress noinline nounwind optnone uwtable
define dso_local i16 @logia_intrinsics_bin_div_u16_u16(i16 noundef %0, i16 noundef %1) #2 {
  %3 = alloca i16, align 2
  %4 = alloca i16, align 2
  store i16 %1, ptr %3, align 2
  call void @llvm.var.annotation.p0.p0(ptr %3, ptr @.str.6, ptr @.str.1, i32 1181, ptr null)
  store i16 %0, ptr %4, align 2
  call void @llvm.var.annotation.p0.p0(ptr %4, ptr @.str.6, ptr @.str.1, i32 1181, ptr null)
  %5 = load i16, ptr %4, align 2
  %6 = zext i16 %5 to i32
  %7 = load i16, ptr %3, align 2
  %8 = zext i16 %7 to i32
  %9 = sdiv i32 %6, %8
  %10 = trunc i32 %9 to i16
  ret i16 %10
}

; Function Attrs: mustprogress noinline nounwind optnone uwtable
define dso_local i16 @logia_intrinsics_bin_mod_u16_u16(i16 noundef %0, i16 noundef %1) #2 {
  %3 = alloca i16, align 2
  %4 = alloca i16, align 2
  store i16 %1, ptr %3, align 2
  call void @llvm.var.annotation.p0.p0(ptr %3, ptr @.str.6, ptr @.str.1, i32 1187, ptr null)
  store i16 %0, ptr %4, align 2
  call void @llvm.var.annotation.p0.p0(ptr %4, ptr @.str.6, ptr @.str.1, i32 1187, ptr null)
  %5 = load i16, ptr %4, align 2
  %6 = zext i16 %5 to i32
  %7 = load i16, ptr %3, align 2
  %8 = zext i16 %7 to i32
  %9 = srem i32 %6, %8
  %10 = trunc i32 %9 to i16
  ret i16 %10
}

; Function Attrs: mustprogress noinline nounwind optnone uwtable
define dso_local zeroext i1 @logia_intrinsics_bin_eq_u16_u16(i16 noundef %0, i16 noundef %1) #2 {
  %3 = alloca i16, align 2
  %4 = alloca i16, align 2
  store i16 %1, ptr %3, align 2
  call void @llvm.var.annotation.p0.p0(ptr %3, ptr @.str.6, ptr @.str.1, i32 1193, ptr null)
  store i16 %0, ptr %4, align 2
  call void @llvm.var.annotation.p0.p0(ptr %4, ptr @.str.6, ptr @.str.1, i32 1193, ptr null)
  %5 = load i16, ptr %4, align 2
  %6 = zext i16 %5 to i32
  %7 = load i16, ptr %3, align 2
  %8 = zext i16 %7 to i32
  %9 = icmp eq i32 %6, %8
  ret i1 %9
}

; Function Attrs: mustprogress noinline nounwind optnone uwtable
define dso_local zeroext i1 @logia_intrinsics_bin_neq_u16_u16(i16 noundef %0, i16 noundef %1) #2 {
  %3 = alloca i16, align 2
  %4 = alloca i16, align 2
  store i16 %1, ptr %3, align 2
  call void @llvm.var.annotation.p0.p0(ptr %3, ptr @.str.6, ptr @.str.1, i32 1199, ptr null)
  store i16 %0, ptr %4, align 2
  call void @llvm.var.annotation.p0.p0(ptr %4, ptr @.str.6, ptr @.str.1, i32 1199, ptr null)
  %5 = load i16, ptr %4, align 2
  %6 = zext i16 %5 to i32
  %7 = load i16, ptr %3, align 2
  %8 = zext i16 %7 to i32
  %9 = icmp ne i32 %6, %8
  ret i1 %9
}

; Function Attrs: mustprogress noinline nounwind optnone uwtable
define dso_local zeroext i1 @logia_intrinsics_bin_lt_u16_u16(i16 noundef %0, i16 noundef %1) #2 {
  %3 = alloca i16, align 2
  %4 = alloca i16, align 2
  store i16 %1, ptr %3, align 2
  call void @llvm.var.annotation.p0.p0(ptr %3, ptr @.str.6, ptr @.str.1, i32 1205, ptr null)
  store i16 %0, ptr %4, align 2
  call void @llvm.var.annotation.p0.p0(ptr %4, ptr @.str.6, ptr @.str.1, i32 1205, ptr null)
  %5 = load i16, ptr %4, align 2
  %6 = zext i16 %5 to i32
  %7 = load i16, ptr %3, align 2
  %8 = zext i16 %7 to i32
  %9 = icmp slt i32 %6, %8
  ret i1 %9
}

; Function Attrs: mustprogress noinline nounwind optnone uwtable
define dso_local zeroext i1 @logia_intrinsics_bin_gt_u16_u16(i16 noundef %0, i16 noundef %1) #2 {
  %3 = alloca i16, align 2
  %4 = alloca i16, align 2
  store i16 %1, ptr %3, align 2
  call void @llvm.var.annotation.p0.p0(ptr %3, ptr @.str.6, ptr @.str.1, i32 1211, ptr null)
  store i16 %0, ptr %4, align 2
  call void @llvm.var.annotation.p0.p0(ptr %4, ptr @.str.6, ptr @.str.1, i32 1211, ptr null)
  %5 = load i16, ptr %4, align 2
  %6 = zext i16 %5 to i32
  %7 = load i16, ptr %3, align 2
  %8 = zext i16 %7 to i32
  %9 = icmp sgt i32 %6, %8
  ret i1 %9
}

; Function Attrs: mustprogress noinline nounwind optnone uwtable
define dso_local zeroext i1 @logia_intrinsics_bin_lte_u16_u16(i16 noundef %0, i16 noundef %1) #2 {
  %3 = alloca i16, align 2
  %4 = alloca i16, align 2
  store i16 %1, ptr %3, align 2
  call void @llvm.var.annotation.p0.p0(ptr %3, ptr @.str.6, ptr @.str.1, i32 1217, ptr null)
  store i16 %0, ptr %4, align 2
  call void @llvm.var.annotation.p0.p0(ptr %4, ptr @.str.6, ptr @.str.1, i32 1217, ptr null)
  %5 = load i16, ptr %4, align 2
  %6 = zext i16 %5 to i32
  %7 = load i16, ptr %3, align 2
  %8 = zext i16 %7 to i32
  %9 = icmp sle i32 %6, %8
  ret i1 %9
}

; Function Attrs: mustprogress noinline nounwind optnone uwtable
define dso_local zeroext i1 @logia_intrinsics_bin_gte_u16_u16(i16 noundef %0, i16 noundef %1) #2 {
  %3 = alloca i16, align 2
  %4 = alloca i16, align 2
  store i16 %1, ptr %3, align 2
  call void @llvm.var.annotation.p0.p0(ptr %3, ptr @.str.6, ptr @.str.1, i32 1223, ptr null)
  store i16 %0, ptr %4, align 2
  call void @llvm.var.annotation.p0.p0(ptr %4, ptr @.str.6, ptr @.str.1, i32 1223, ptr null)
  %5 = load i16, ptr %4, align 2
  %6 = zext i16 %5 to i32
  %7 = load i16, ptr %3, align 2
  %8 = zext i16 %7 to i32
  %9 = icmp sge i32 %6, %8
  ret i1 %9
}

; Function Attrs: mustprogress noinline nounwind optnone uwtable
define dso_local i16 @logia_intrinsics_bin_assign_u16_u16(ptr noundef %0, i16 noundef %1) #2 {
  %3 = alloca i16, align 2
  %4 = alloca ptr, align 8
  store i16 %1, ptr %3, align 2
  call void @llvm.var.annotation.p0.p0(ptr %3, ptr @.str.6, ptr @.str.1, i32 1229, ptr null)
  store ptr %0, ptr %4, align 8
  call void @llvm.var.annotation.p0.p0(ptr %4, ptr @.str.14, ptr @.str.1, i32 1229, ptr null)
  %5 = load i16, ptr %3, align 2
  %6 = load ptr, ptr %4, align 8
  store i16 %5, ptr %6, align 2
  %7 = load ptr, ptr %4, align 8
  %8 = load i16, ptr %7, align 2
  ret i16 %8
}

; Function Attrs: mustprogress noinline nounwind optnone uwtable
define dso_local i16 @logia_intrinsics_bin_add_assign_u16_u16(ptr noundef %0, i16 noundef %1) #2 {
  %3 = alloca i16, align 2
  %4 = alloca ptr, align 8
  store i16 %1, ptr %3, align 2
  call void @llvm.var.annotation.p0.p0(ptr %3, ptr @.str.6, ptr @.str.1, i32 1236, ptr null)
  store ptr %0, ptr %4, align 8
  call void @llvm.var.annotation.p0.p0(ptr %4, ptr @.str.14, ptr @.str.1, i32 1236, ptr null)
  %5 = load i16, ptr %3, align 2
  %6 = zext i16 %5 to i32
  %7 = load ptr, ptr %4, align 8
  %8 = load i16, ptr %7, align 2
  %9 = zext i16 %8 to i32
  %10 = add nsw i32 %9, %6
  %11 = trunc i32 %10 to i16
  store i16 %11, ptr %7, align 2
  %12 = load ptr, ptr %4, align 8
  %13 = load i16, ptr %12, align 2
  ret i16 %13
}

; Function Attrs: mustprogress noinline nounwind optnone uwtable
define dso_local i16 @logia_intrinsics_bin_sub_assign_u16_u16(ptr noundef %0, i16 noundef %1) #2 {
  %3 = alloca i16, align 2
  %4 = alloca ptr, align 8
  store i16 %1, ptr %3, align 2
  call void @llvm.var.annotation.p0.p0(ptr %3, ptr @.str.6, ptr @.str.1, i32 1243, ptr null)
  store ptr %0, ptr %4, align 8
  call void @llvm.var.annotation.p0.p0(ptr %4, ptr @.str.14, ptr @.str.1, i32 1243, ptr null)
  %5 = load i16, ptr %3, align 2
  %6 = zext i16 %5 to i32
  %7 = load ptr, ptr %4, align 8
  %8 = load i16, ptr %7, align 2
  %9 = zext i16 %8 to i32
  %10 = sub nsw i32 %9, %6
  %11 = trunc i32 %10 to i16
  store i16 %11, ptr %7, align 2
  %12 = load ptr, ptr %4, align 8
  %13 = load i16, ptr %12, align 2
  ret i16 %13
}

; Function Attrs: mustprogress noinline nounwind optnone uwtable
define dso_local i16 @logia_intrinsics_bin_mul_assign_u16_u16(ptr noundef %0, i16 noundef %1) #2 {
  %3 = alloca i16, align 2
  %4 = alloca ptr, align 8
  store i16 %1, ptr %3, align 2
  call void @llvm.var.annotation.p0.p0(ptr %3, ptr @.str.6, ptr @.str.1, i32 1250, ptr null)
  store ptr %0, ptr %4, align 8
  call void @llvm.var.annotation.p0.p0(ptr %4, ptr @.str.14, ptr @.str.1, i32 1250, ptr null)
  %5 = load i16, ptr %3, align 2
  %6 = zext i16 %5 to i32
  %7 = load ptr, ptr %4, align 8
  %8 = load i16, ptr %7, align 2
  %9 = zext i16 %8 to i32
  %10 = mul nsw i32 %9, %6
  %11 = trunc i32 %10 to i16
  store i16 %11, ptr %7, align 2
  %12 = load ptr, ptr %4, align 8
  %13 = load i16, ptr %12, align 2
  ret i16 %13
}

; Function Attrs: mustprogress noinline nounwind optnone uwtable
define dso_local i16 @logia_intrinsics_bin_div_assign_u16_u16(ptr noundef %0, i16 noundef %1) #2 {
  %3 = alloca i16, align 2
  %4 = alloca ptr, align 8
  store i16 %1, ptr %3, align 2
  call void @llvm.var.annotation.p0.p0(ptr %3, ptr @.str.6, ptr @.str.1, i32 1257, ptr null)
  store ptr %0, ptr %4, align 8
  call void @llvm.var.annotation.p0.p0(ptr %4, ptr @.str.14, ptr @.str.1, i32 1257, ptr null)
  %5 = load i16, ptr %3, align 2
  %6 = zext i16 %5 to i32
  %7 = load ptr, ptr %4, align 8
  %8 = load i16, ptr %7, align 2
  %9 = zext i16 %8 to i32
  %10 = sdiv i32 %9, %6
  %11 = trunc i32 %10 to i16
  store i16 %11, ptr %7, align 2
  %12 = load ptr, ptr %4, align 8
  %13 = load i16, ptr %12, align 2
  ret i16 %13
}

; Function Attrs: mustprogress noinline nounwind optnone uwtable
define dso_local i16 @logia_intrinsics_bin_mod_assign_u16_u16(ptr noundef %0, i16 noundef %1) #2 {
  %3 = alloca i16, align 2
  %4 = alloca ptr, align 8
  store i16 %1, ptr %3, align 2
  call void @llvm.var.annotation.p0.p0(ptr %3, ptr @.str.6, ptr @.str.1, i32 1264, ptr null)
  store ptr %0, ptr %4, align 8
  call void @llvm.var.annotation.p0.p0(ptr %4, ptr @.str.14, ptr @.str.1, i32 1264, ptr null)
  %5 = load i16, ptr %3, align 2
  %6 = zext i16 %5 to i32
  %7 = load ptr, ptr %4, align 8
  %8 = load i16, ptr %7, align 2
  %9 = zext i16 %8 to i32
  %10 = srem i32 %9, %6
  %11 = trunc i32 %10 to i16
  store i16 %11, ptr %7, align 2
  %12 = load ptr, ptr %4, align 8
  %13 = load i16, ptr %12, align 2
  ret i16 %13
}

; Function Attrs: mustprogress noinline nounwind optnone uwtable
define dso_local i16 @logia_intrinsics_bin_bitwise_and_assign_u16_u16(ptr noundef %0, i16 noundef %1) #2 {
  %3 = alloca i16, align 2
  %4 = alloca ptr, align 8
  store i16 %1, ptr %3, align 2
  call void @llvm.var.annotation.p0.p0(ptr %3, ptr @.str.6, ptr @.str.1, i32 1271, ptr null)
  store ptr %0, ptr %4, align 8
  call void @llvm.var.annotation.p0.p0(ptr %4, ptr @.str.14, ptr @.str.1, i32 1271, ptr null)
  %5 = load i16, ptr %3, align 2
  %6 = zext i16 %5 to i32
  %7 = load ptr, ptr %4, align 8
  %8 = load i16, ptr %7, align 2
  %9 = zext i16 %8 to i32
  %10 = and i32 %9, %6
  %11 = trunc i32 %10 to i16
  store i16 %11, ptr %7, align 2
  %12 = load ptr, ptr %4, align 8
  %13 = load i16, ptr %12, align 2
  ret i16 %13
}

; Function Attrs: mustprogress noinline nounwind optnone uwtable
define dso_local i16 @logia_intrinsics_bin_bitwise_or_assign_u16_u16(ptr noundef %0, i16 noundef %1) #2 {
  %3 = alloca i16, align 2
  %4 = alloca ptr, align 8
  store i16 %1, ptr %3, align 2
  call void @llvm.var.annotation.p0.p0(ptr %3, ptr @.str.6, ptr @.str.1, i32 1278, ptr null)
  store ptr %0, ptr %4, align 8
  call void @llvm.var.annotation.p0.p0(ptr %4, ptr @.str.14, ptr @.str.1, i32 1278, ptr null)
  %5 = load i16, ptr %3, align 2
  %6 = zext i16 %5 to i32
  %7 = load ptr, ptr %4, align 8
  %8 = load i16, ptr %7, align 2
  %9 = zext i16 %8 to i32
  %10 = or i32 %9, %6
  %11 = trunc i32 %10 to i16
  store i16 %11, ptr %7, align 2
  %12 = load ptr, ptr %4, align 8
  %13 = load i16, ptr %12, align 2
  ret i16 %13
}

; Function Attrs: mustprogress noinline nounwind optnone uwtable
define dso_local i16 @logia_intrinsics_bin_bitwise_xor_assign_u16_u16(ptr noundef %0, i16 noundef %1) #2 {
  %3 = alloca i16, align 2
  %4 = alloca ptr, align 8
  store i16 %1, ptr %3, align 2
  call void @llvm.var.annotation.p0.p0(ptr %3, ptr @.str.6, ptr @.str.1, i32 1285, ptr null)
  store ptr %0, ptr %4, align 8
  call void @llvm.var.annotation.p0.p0(ptr %4, ptr @.str.14, ptr @.str.1, i32 1285, ptr null)
  %5 = load i16, ptr %3, align 2
  %6 = zext i16 %5 to i32
  %7 = load ptr, ptr %4, align 8
  %8 = load i16, ptr %7, align 2
  %9 = zext i16 %8 to i32
  %10 = xor i32 %9, %6
  %11 = trunc i32 %10 to i16
  store i16 %11, ptr %7, align 2
  %12 = load ptr, ptr %4, align 8
  %13 = load i16, ptr %12, align 2
  ret i16 %13
}

; Function Attrs: mustprogress noinline nounwind optnone uwtable
define dso_local i16 @logia_intrinsics_bin_bitwise_shr_assign_u16_u16(ptr noundef %0, i16 noundef %1) #2 {
  %3 = alloca i16, align 2
  %4 = alloca ptr, align 8
  store i16 %1, ptr %3, align 2
  call void @llvm.var.annotation.p0.p0(ptr %3, ptr @.str.6, ptr @.str.1, i32 1292, ptr null)
  store ptr %0, ptr %4, align 8
  call void @llvm.var.annotation.p0.p0(ptr %4, ptr @.str.14, ptr @.str.1, i32 1292, ptr null)
  %5 = load i16, ptr %3, align 2
  %6 = zext i16 %5 to i32
  %7 = load ptr, ptr %4, align 8
  %8 = load i16, ptr %7, align 2
  %9 = zext i16 %8 to i32
  %10 = ashr i32 %9, %6
  %11 = trunc i32 %10 to i16
  store i16 %11, ptr %7, align 2
  %12 = load ptr, ptr %4, align 8
  %13 = load i16, ptr %12, align 2
  ret i16 %13
}

; Function Attrs: mustprogress noinline nounwind optnone uwtable
define dso_local i16 @logia_intrinsics_bin_bitwise_shl_assign_u16_u16(ptr noundef %0, i16 noundef %1) #2 {
  %3 = alloca i16, align 2
  %4 = alloca ptr, align 8
  store i16 %1, ptr %3, align 2
  call void @llvm.var.annotation.p0.p0(ptr %3, ptr @.str.6, ptr @.str.1, i32 1299, ptr null)
  store ptr %0, ptr %4, align 8
  call void @llvm.var.annotation.p0.p0(ptr %4, ptr @.str.14, ptr @.str.1, i32 1299, ptr null)
  %5 = load i16, ptr %3, align 2
  %6 = zext i16 %5 to i32
  %7 = load ptr, ptr %4, align 8
  %8 = load i16, ptr %7, align 2
  %9 = zext i16 %8 to i32
  %10 = shl i32 %9, %6
  %11 = trunc i32 %10 to i16
  store i16 %11, ptr %7, align 2
  %12 = load ptr, ptr %4, align 8
  %13 = load i16, ptr %12, align 2
  ret i16 %13
}

; Function Attrs: mustprogress noinline nounwind optnone uwtable
define dso_local i16 @logia_intrinsics_bin_bitwise_and_u16_u16(i16 noundef %0, i16 noundef %1) #2 {
  %3 = alloca i16, align 2
  %4 = alloca i16, align 2
  store i16 %1, ptr %3, align 2
  call void @llvm.var.annotation.p0.p0(ptr %3, ptr @.str.6, ptr @.str.1, i32 1306, ptr null)
  store i16 %0, ptr %4, align 2
  call void @llvm.var.annotation.p0.p0(ptr %4, ptr @.str.6, ptr @.str.1, i32 1306, ptr null)
  %5 = load i16, ptr %4, align 2
  %6 = zext i16 %5 to i32
  %7 = load i16, ptr %3, align 2
  %8 = zext i16 %7 to i32
  %9 = and i32 %6, %8
  %10 = trunc i32 %9 to i16
  ret i16 %10
}

; Function Attrs: mustprogress noinline nounwind optnone uwtable
define dso_local i16 @logia_intrinsics_bin_bitwise_or_u16_u16(i16 noundef %0, i16 noundef %1) #2 {
  %3 = alloca i16, align 2
  %4 = alloca i16, align 2
  store i16 %1, ptr %3, align 2
  call void @llvm.var.annotation.p0.p0(ptr %3, ptr @.str.6, ptr @.str.1, i32 1312, ptr null)
  store i16 %0, ptr %4, align 2
  call void @llvm.var.annotation.p0.p0(ptr %4, ptr @.str.6, ptr @.str.1, i32 1312, ptr null)
  %5 = load i16, ptr %4, align 2
  %6 = zext i16 %5 to i32
  %7 = load i16, ptr %3, align 2
  %8 = zext i16 %7 to i32
  %9 = or i32 %6, %8
  %10 = trunc i32 %9 to i16
  ret i16 %10
}

; Function Attrs: mustprogress noinline nounwind optnone uwtable
define dso_local i16 @logia_intrinsics_bin_bitwise_xor_u16_u16(i16 noundef %0, i16 noundef %1) #2 {
  %3 = alloca i16, align 2
  %4 = alloca i16, align 2
  store i16 %1, ptr %3, align 2
  call void @llvm.var.annotation.p0.p0(ptr %3, ptr @.str.6, ptr @.str.1, i32 1318, ptr null)
  store i16 %0, ptr %4, align 2
  call void @llvm.var.annotation.p0.p0(ptr %4, ptr @.str.6, ptr @.str.1, i32 1318, ptr null)
  %5 = load i16, ptr %4, align 2
  %6 = zext i16 %5 to i32
  %7 = load i16, ptr %3, align 2
  %8 = zext i16 %7 to i32
  %9 = xor i32 %6, %8
  %10 = trunc i32 %9 to i16
  ret i16 %10
}

; Function Attrs: mustprogress noinline nounwind optnone uwtable
define dso_local i16 @logia_intrinsics_bin_bitwise_shl_u16_u16(i16 noundef %0, i16 noundef %1) #2 {
  %3 = alloca i16, align 2
  %4 = alloca i16, align 2
  store i16 %1, ptr %3, align 2
  call void @llvm.var.annotation.p0.p0(ptr %3, ptr @.str.6, ptr @.str.1, i32 1324, ptr null)
  store i16 %0, ptr %4, align 2
  call void @llvm.var.annotation.p0.p0(ptr %4, ptr @.str.6, ptr @.str.1, i32 1324, ptr null)
  %5 = load i16, ptr %4, align 2
  %6 = zext i16 %5 to i32
  %7 = load i16, ptr %3, align 2
  %8 = zext i16 %7 to i32
  %9 = shl i32 %6, %8
  %10 = trunc i32 %9 to i16
  ret i16 %10
}

; Function Attrs: mustprogress noinline nounwind optnone uwtable
define dso_local i16 @logia_intrinsics_bin_bitwise_shr_u16_u16(i16 noundef %0, i16 noundef %1) #2 {
  %3 = alloca i16, align 2
  %4 = alloca i16, align 2
  store i16 %1, ptr %3, align 2
  call void @llvm.var.annotation.p0.p0(ptr %3, ptr @.str.6, ptr @.str.1, i32 1330, ptr null)
  store i16 %0, ptr %4, align 2
  call void @llvm.var.annotation.p0.p0(ptr %4, ptr @.str.6, ptr @.str.1, i32 1330, ptr null)
  %5 = load i16, ptr %4, align 2
  %6 = zext i16 %5 to i32
  %7 = load i16, ptr %3, align 2
  %8 = zext i16 %7 to i32
  %9 = ashr i32 %6, %8
  %10 = trunc i32 %9 to i16
  ret i16 %10
}

; Function Attrs: mustprogress noinline nounwind optnone uwtable
define dso_local i32 @logia__intrinsics_bin_add_u32_u32(i32 noundef %0, i32 noundef %1) #2 {
  %3 = alloca i32, align 4
  %4 = alloca i32, align 4
  store i32 %1, ptr %3, align 4
  call void @llvm.var.annotation.p0.p0(ptr %3, ptr @.str.7, ptr @.str.1, i32 1336, ptr null)
  store i32 %0, ptr %4, align 4
  call void @llvm.var.annotation.p0.p0(ptr %4, ptr @.str.7, ptr @.str.1, i32 1336, ptr null)
  %5 = load i32, ptr %4, align 4
  %6 = load i32, ptr %3, align 4
  %7 = add i32 %5, %6
  ret i32 %7
}

; Function Attrs: mustprogress noinline nounwind optnone uwtable
define dso_local i32 @logia_intrinsics_bin_sub_u32_u32(i32 noundef %0, i32 noundef %1) #2 {
  %3 = alloca i32, align 4
  %4 = alloca i32, align 4
  store i32 %1, ptr %3, align 4
  call void @llvm.var.annotation.p0.p0(ptr %3, ptr @.str.7, ptr @.str.1, i32 1342, ptr null)
  store i32 %0, ptr %4, align 4
  call void @llvm.var.annotation.p0.p0(ptr %4, ptr @.str.7, ptr @.str.1, i32 1342, ptr null)
  %5 = load i32, ptr %4, align 4
  %6 = load i32, ptr %3, align 4
  %7 = sub i32 %5, %6
  ret i32 %7
}

; Function Attrs: mustprogress noinline nounwind optnone uwtable
define dso_local i32 @logia_intrinsics_bin_mul_u32_u32(i32 noundef %0, i32 noundef %1) #2 {
  %3 = alloca i32, align 4
  %4 = alloca i32, align 4
  store i32 %1, ptr %3, align 4
  call void @llvm.var.annotation.p0.p0(ptr %3, ptr @.str.7, ptr @.str.1, i32 1348, ptr null)
  store i32 %0, ptr %4, align 4
  call void @llvm.var.annotation.p0.p0(ptr %4, ptr @.str.7, ptr @.str.1, i32 1348, ptr null)
  %5 = load i32, ptr %4, align 4
  %6 = load i32, ptr %3, align 4
  %7 = mul i32 %5, %6
  ret i32 %7
}

; Function Attrs: mustprogress noinline nounwind optnone uwtable
define dso_local i32 @logia_intrinsics_bin_div_u32_u32(i32 noundef %0, i32 noundef %1) #2 {
  %3 = alloca i32, align 4
  %4 = alloca i32, align 4
  store i32 %1, ptr %3, align 4
  call void @llvm.var.annotation.p0.p0(ptr %3, ptr @.str.7, ptr @.str.1, i32 1354, ptr null)
  store i32 %0, ptr %4, align 4
  call void @llvm.var.annotation.p0.p0(ptr %4, ptr @.str.7, ptr @.str.1, i32 1354, ptr null)
  %5 = load i32, ptr %4, align 4
  %6 = load i32, ptr %3, align 4
  %7 = udiv i32 %5, %6
  ret i32 %7
}

; Function Attrs: mustprogress noinline nounwind optnone uwtable
define dso_local i32 @logia_intrinsics_bin_mod_u32_u32(i32 noundef %0, i32 noundef %1) #2 {
  %3 = alloca i32, align 4
  %4 = alloca i32, align 4
  store i32 %1, ptr %3, align 4
  call void @llvm.var.annotation.p0.p0(ptr %3, ptr @.str.7, ptr @.str.1, i32 1360, ptr null)
  store i32 %0, ptr %4, align 4
  call void @llvm.var.annotation.p0.p0(ptr %4, ptr @.str.7, ptr @.str.1, i32 1360, ptr null)
  %5 = load i32, ptr %4, align 4
  %6 = load i32, ptr %3, align 4
  %7 = urem i32 %5, %6
  ret i32 %7
}

; Function Attrs: mustprogress noinline nounwind optnone uwtable
define dso_local zeroext i1 @logia_intrinsics_bin_eq_u32_u32(i32 noundef %0, i32 noundef %1) #2 {
  %3 = alloca i32, align 4
  %4 = alloca i32, align 4
  store i32 %1, ptr %3, align 4
  call void @llvm.var.annotation.p0.p0(ptr %3, ptr @.str.7, ptr @.str.1, i32 1366, ptr null)
  store i32 %0, ptr %4, align 4
  call void @llvm.var.annotation.p0.p0(ptr %4, ptr @.str.7, ptr @.str.1, i32 1366, ptr null)
  %5 = load i32, ptr %4, align 4
  %6 = load i32, ptr %3, align 4
  %7 = icmp eq i32 %5, %6
  ret i1 %7
}

; Function Attrs: mustprogress noinline nounwind optnone uwtable
define dso_local zeroext i1 @logia_intrinsics_bin_neq_u32_u32(i32 noundef %0, i32 noundef %1) #2 {
  %3 = alloca i32, align 4
  %4 = alloca i32, align 4
  store i32 %1, ptr %3, align 4
  call void @llvm.var.annotation.p0.p0(ptr %3, ptr @.str.7, ptr @.str.1, i32 1372, ptr null)
  store i32 %0, ptr %4, align 4
  call void @llvm.var.annotation.p0.p0(ptr %4, ptr @.str.7, ptr @.str.1, i32 1372, ptr null)
  %5 = load i32, ptr %4, align 4
  %6 = load i32, ptr %3, align 4
  %7 = icmp ne i32 %5, %6
  ret i1 %7
}

; Function Attrs: mustprogress noinline nounwind optnone uwtable
define dso_local zeroext i1 @logia_intrinsics_bin_lt_u32_u32(i32 noundef %0, i32 noundef %1) #2 {
  %3 = alloca i32, align 4
  %4 = alloca i32, align 4
  store i32 %1, ptr %3, align 4
  call void @llvm.var.annotation.p0.p0(ptr %3, ptr @.str.7, ptr @.str.1, i32 1378, ptr null)
  store i32 %0, ptr %4, align 4
  call void @llvm.var.annotation.p0.p0(ptr %4, ptr @.str.7, ptr @.str.1, i32 1378, ptr null)
  %5 = load i32, ptr %4, align 4
  %6 = load i32, ptr %3, align 4
  %7 = icmp ult i32 %5, %6
  ret i1 %7
}

; Function Attrs: mustprogress noinline nounwind optnone uwtable
define dso_local zeroext i1 @logia_intrinsics_bin_gt_u32_u32(i32 noundef %0, i32 noundef %1) #2 {
  %3 = alloca i32, align 4
  %4 = alloca i32, align 4
  store i32 %1, ptr %3, align 4
  call void @llvm.var.annotation.p0.p0(ptr %3, ptr @.str.7, ptr @.str.1, i32 1384, ptr null)
  store i32 %0, ptr %4, align 4
  call void @llvm.var.annotation.p0.p0(ptr %4, ptr @.str.7, ptr @.str.1, i32 1384, ptr null)
  %5 = load i32, ptr %4, align 4
  %6 = load i32, ptr %3, align 4
  %7 = icmp ugt i32 %5, %6
  ret i1 %7
}

; Function Attrs: mustprogress noinline nounwind optnone uwtable
define dso_local zeroext i1 @logia_intrinsics_bin_lte_u32_u32(i32 noundef %0, i32 noundef %1) #2 {
  %3 = alloca i32, align 4
  %4 = alloca i32, align 4
  store i32 %1, ptr %3, align 4
  call void @llvm.var.annotation.p0.p0(ptr %3, ptr @.str.7, ptr @.str.1, i32 1390, ptr null)
  store i32 %0, ptr %4, align 4
  call void @llvm.var.annotation.p0.p0(ptr %4, ptr @.str.7, ptr @.str.1, i32 1390, ptr null)
  %5 = load i32, ptr %4, align 4
  %6 = load i32, ptr %3, align 4
  %7 = icmp ule i32 %5, %6
  ret i1 %7
}

; Function Attrs: mustprogress noinline nounwind optnone uwtable
define dso_local zeroext i1 @logia_intrinsics_bin_gte_u32_u32(i32 noundef %0, i32 noundef %1) #2 {
  %3 = alloca i32, align 4
  %4 = alloca i32, align 4
  store i32 %1, ptr %3, align 4
  call void @llvm.var.annotation.p0.p0(ptr %3, ptr @.str.7, ptr @.str.1, i32 1396, ptr null)
  store i32 %0, ptr %4, align 4
  call void @llvm.var.annotation.p0.p0(ptr %4, ptr @.str.7, ptr @.str.1, i32 1396, ptr null)
  %5 = load i32, ptr %4, align 4
  %6 = load i32, ptr %3, align 4
  %7 = icmp uge i32 %5, %6
  ret i1 %7
}

; Function Attrs: mustprogress noinline nounwind optnone uwtable
define dso_local i32 @logia_intrinsics_bin_assign_u32_u32(ptr noundef %0, i32 noundef %1) #2 {
  %3 = alloca i32, align 4
  %4 = alloca ptr, align 8
  store i32 %1, ptr %3, align 4
  call void @llvm.var.annotation.p0.p0(ptr %3, ptr @.str.7, ptr @.str.1, i32 1402, ptr null)
  store ptr %0, ptr %4, align 8
  call void @llvm.var.annotation.p0.p0(ptr %4, ptr @.str.15, ptr @.str.1, i32 1402, ptr null)
  %5 = load i32, ptr %3, align 4
  %6 = load ptr, ptr %4, align 8
  store i32 %5, ptr %6, align 4
  %7 = load ptr, ptr %4, align 8
  %8 = load i32, ptr %7, align 4
  ret i32 %8
}

; Function Attrs: mustprogress noinline nounwind optnone uwtable
define dso_local i32 @logia_intrinsics_bin_add_assign_u32_u32(ptr noundef %0, i32 noundef %1) #2 {
  %3 = alloca i32, align 4
  %4 = alloca ptr, align 8
  store i32 %1, ptr %3, align 4
  call void @llvm.var.annotation.p0.p0(ptr %3, ptr @.str.7, ptr @.str.1, i32 1409, ptr null)
  store ptr %0, ptr %4, align 8
  call void @llvm.var.annotation.p0.p0(ptr %4, ptr @.str.15, ptr @.str.1, i32 1409, ptr null)
  %5 = load i32, ptr %3, align 4
  %6 = load ptr, ptr %4, align 8
  %7 = load i32, ptr %6, align 4
  %8 = add i32 %7, %5
  store i32 %8, ptr %6, align 4
  %9 = load ptr, ptr %4, align 8
  %10 = load i32, ptr %9, align 4
  ret i32 %10
}

; Function Attrs: mustprogress noinline nounwind optnone uwtable
define dso_local i32 @logia_intrinsics_bin_sub_assign_u32_u32(ptr noundef %0, i32 noundef %1) #2 {
  %3 = alloca i32, align 4
  %4 = alloca ptr, align 8
  store i32 %1, ptr %3, align 4
  call void @llvm.var.annotation.p0.p0(ptr %3, ptr @.str.7, ptr @.str.1, i32 1416, ptr null)
  store ptr %0, ptr %4, align 8
  call void @llvm.var.annotation.p0.p0(ptr %4, ptr @.str.15, ptr @.str.1, i32 1416, ptr null)
  %5 = load i32, ptr %3, align 4
  %6 = load ptr, ptr %4, align 8
  %7 = load i32, ptr %6, align 4
  %8 = sub i32 %7, %5
  store i32 %8, ptr %6, align 4
  %9 = load ptr, ptr %4, align 8
  %10 = load i32, ptr %9, align 4
  ret i32 %10
}

; Function Attrs: mustprogress noinline nounwind optnone uwtable
define dso_local i32 @logia_intrinsics_bin_mul_assign_u32_u32(ptr noundef %0, i32 noundef %1) #2 {
  %3 = alloca i32, align 4
  %4 = alloca ptr, align 8
  store i32 %1, ptr %3, align 4
  call void @llvm.var.annotation.p0.p0(ptr %3, ptr @.str.7, ptr @.str.1, i32 1423, ptr null)
  store ptr %0, ptr %4, align 8
  call void @llvm.var.annotation.p0.p0(ptr %4, ptr @.str.15, ptr @.str.1, i32 1423, ptr null)
  %5 = load i32, ptr %3, align 4
  %6 = load ptr, ptr %4, align 8
  %7 = load i32, ptr %6, align 4
  %8 = mul i32 %7, %5
  store i32 %8, ptr %6, align 4
  %9 = load ptr, ptr %4, align 8
  %10 = load i32, ptr %9, align 4
  ret i32 %10
}

; Function Attrs: mustprogress noinline nounwind optnone uwtable
define dso_local i32 @logia_intrinsics_bin_div_assign_u32_u32(ptr noundef %0, i32 noundef %1) #2 {
  %3 = alloca i32, align 4
  %4 = alloca ptr, align 8
  store i32 %1, ptr %3, align 4
  call void @llvm.var.annotation.p0.p0(ptr %3, ptr @.str.7, ptr @.str.1, i32 1430, ptr null)
  store ptr %0, ptr %4, align 8
  call void @llvm.var.annotation.p0.p0(ptr %4, ptr @.str.15, ptr @.str.1, i32 1430, ptr null)
  %5 = load i32, ptr %3, align 4
  %6 = load ptr, ptr %4, align 8
  %7 = load i32, ptr %6, align 4
  %8 = udiv i32 %7, %5
  store i32 %8, ptr %6, align 4
  %9 = load ptr, ptr %4, align 8
  %10 = load i32, ptr %9, align 4
  ret i32 %10
}

; Function Attrs: mustprogress noinline nounwind optnone uwtable
define dso_local i32 @logia_intrinsics_bin_mod_assign_u32_u32(ptr noundef %0, i32 noundef %1) #2 {
  %3 = alloca i32, align 4
  %4 = alloca ptr, align 8
  store i32 %1, ptr %3, align 4
  call void @llvm.var.annotation.p0.p0(ptr %3, ptr @.str.7, ptr @.str.1, i32 1437, ptr null)
  store ptr %0, ptr %4, align 8
  call void @llvm.var.annotation.p0.p0(ptr %4, ptr @.str.15, ptr @.str.1, i32 1437, ptr null)
  %5 = load i32, ptr %3, align 4
  %6 = load ptr, ptr %4, align 8
  %7 = load i32, ptr %6, align 4
  %8 = urem i32 %7, %5
  store i32 %8, ptr %6, align 4
  %9 = load ptr, ptr %4, align 8
  %10 = load i32, ptr %9, align 4
  ret i32 %10
}

; Function Attrs: mustprogress noinline nounwind optnone uwtable
define dso_local i32 @logia_intrinsics_bin_bitwise_and_assign_u32_u32(ptr noundef %0, i32 noundef %1) #2 {
  %3 = alloca i32, align 4
  %4 = alloca ptr, align 8
  store i32 %1, ptr %3, align 4
  call void @llvm.var.annotation.p0.p0(ptr %3, ptr @.str.7, ptr @.str.1, i32 1444, ptr null)
  store ptr %0, ptr %4, align 8
  call void @llvm.var.annotation.p0.p0(ptr %4, ptr @.str.15, ptr @.str.1, i32 1444, ptr null)
  %5 = load i32, ptr %3, align 4
  %6 = load ptr, ptr %4, align 8
  %7 = load i32, ptr %6, align 4
  %8 = and i32 %7, %5
  store i32 %8, ptr %6, align 4
  %9 = load ptr, ptr %4, align 8
  %10 = load i32, ptr %9, align 4
  ret i32 %10
}

; Function Attrs: mustprogress noinline nounwind optnone uwtable
define dso_local i32 @logia_intrinsics_bin_bitwise_or_assign_u32_u32(ptr noundef %0, i32 noundef %1) #2 {
  %3 = alloca i32, align 4
  %4 = alloca ptr, align 8
  store i32 %1, ptr %3, align 4
  call void @llvm.var.annotation.p0.p0(ptr %3, ptr @.str.7, ptr @.str.1, i32 1451, ptr null)
  store ptr %0, ptr %4, align 8
  call void @llvm.var.annotation.p0.p0(ptr %4, ptr @.str.15, ptr @.str.1, i32 1451, ptr null)
  %5 = load i32, ptr %3, align 4
  %6 = load ptr, ptr %4, align 8
  %7 = load i32, ptr %6, align 4
  %8 = or i32 %7, %5
  store i32 %8, ptr %6, align 4
  %9 = load ptr, ptr %4, align 8
  %10 = load i32, ptr %9, align 4
  ret i32 %10
}

; Function Attrs: mustprogress noinline nounwind optnone uwtable
define dso_local i32 @logia_intrinsics_bin_bitwise_xor_assign_u32_u32(ptr noundef %0, i32 noundef %1) #2 {
  %3 = alloca i32, align 4
  %4 = alloca ptr, align 8
  store i32 %1, ptr %3, align 4
  call void @llvm.var.annotation.p0.p0(ptr %3, ptr @.str.7, ptr @.str.1, i32 1458, ptr null)
  store ptr %0, ptr %4, align 8
  call void @llvm.var.annotation.p0.p0(ptr %4, ptr @.str.15, ptr @.str.1, i32 1458, ptr null)
  %5 = load i32, ptr %3, align 4
  %6 = load ptr, ptr %4, align 8
  %7 = load i32, ptr %6, align 4
  %8 = xor i32 %7, %5
  store i32 %8, ptr %6, align 4
  %9 = load ptr, ptr %4, align 8
  %10 = load i32, ptr %9, align 4
  ret i32 %10
}

; Function Attrs: mustprogress noinline nounwind optnone uwtable
define dso_local i32 @logia_intrinsics_bin_bitwise_shr_assign_u32_u32(ptr noundef %0, i32 noundef %1) #2 {
  %3 = alloca i32, align 4
  %4 = alloca ptr, align 8
  store i32 %1, ptr %3, align 4
  call void @llvm.var.annotation.p0.p0(ptr %3, ptr @.str.7, ptr @.str.1, i32 1465, ptr null)
  store ptr %0, ptr %4, align 8
  call void @llvm.var.annotation.p0.p0(ptr %4, ptr @.str.15, ptr @.str.1, i32 1465, ptr null)
  %5 = load i32, ptr %3, align 4
  %6 = load ptr, ptr %4, align 8
  %7 = load i32, ptr %6, align 4
  %8 = lshr i32 %7, %5
  store i32 %8, ptr %6, align 4
  %9 = load ptr, ptr %4, align 8
  %10 = load i32, ptr %9, align 4
  ret i32 %10
}

; Function Attrs: mustprogress noinline nounwind optnone uwtable
define dso_local i32 @logia_intrinsics_bin_bitwise_shl_assign_u32_u32(ptr noundef %0, i32 noundef %1) #2 {
  %3 = alloca i32, align 4
  %4 = alloca ptr, align 8
  store i32 %1, ptr %3, align 4
  call void @llvm.var.annotation.p0.p0(ptr %3, ptr @.str.7, ptr @.str.1, i32 1472, ptr null)
  store ptr %0, ptr %4, align 8
  call void @llvm.var.annotation.p0.p0(ptr %4, ptr @.str.15, ptr @.str.1, i32 1472, ptr null)
  %5 = load i32, ptr %3, align 4
  %6 = load ptr, ptr %4, align 8
  %7 = load i32, ptr %6, align 4
  %8 = shl i32 %7, %5
  store i32 %8, ptr %6, align 4
  %9 = load ptr, ptr %4, align 8
  %10 = load i32, ptr %9, align 4
  ret i32 %10
}

; Function Attrs: mustprogress noinline nounwind optnone uwtable
define dso_local i32 @logia_intrinsics_bin_bitwise_and_u32_u32(i32 noundef %0, i32 noundef %1) #2 {
  %3 = alloca i32, align 4
  %4 = alloca i32, align 4
  store i32 %1, ptr %3, align 4
  call void @llvm.var.annotation.p0.p0(ptr %3, ptr @.str.7, ptr @.str.1, i32 1479, ptr null)
  store i32 %0, ptr %4, align 4
  call void @llvm.var.annotation.p0.p0(ptr %4, ptr @.str.7, ptr @.str.1, i32 1479, ptr null)
  %5 = load i32, ptr %4, align 4
  %6 = load i32, ptr %3, align 4
  %7 = and i32 %5, %6
  ret i32 %7
}

; Function Attrs: mustprogress noinline nounwind optnone uwtable
define dso_local i32 @logia_intrinsics_bin_bitwise_or_u32_u32(i32 noundef %0, i32 noundef %1) #2 {
  %3 = alloca i32, align 4
  %4 = alloca i32, align 4
  store i32 %1, ptr %3, align 4
  call void @llvm.var.annotation.p0.p0(ptr %3, ptr @.str.7, ptr @.str.1, i32 1485, ptr null)
  store i32 %0, ptr %4, align 4
  call void @llvm.var.annotation.p0.p0(ptr %4, ptr @.str.7, ptr @.str.1, i32 1485, ptr null)
  %5 = load i32, ptr %4, align 4
  %6 = load i32, ptr %3, align 4
  %7 = or i32 %5, %6
  ret i32 %7
}

; Function Attrs: mustprogress noinline nounwind optnone uwtable
define dso_local i32 @logia_intrinsics_bin_bitwise_xor_u32_u32(i32 noundef %0, i32 noundef %1) #2 {
  %3 = alloca i32, align 4
  %4 = alloca i32, align 4
  store i32 %1, ptr %3, align 4
  call void @llvm.var.annotation.p0.p0(ptr %3, ptr @.str.7, ptr @.str.1, i32 1491, ptr null)
  store i32 %0, ptr %4, align 4
  call void @llvm.var.annotation.p0.p0(ptr %4, ptr @.str.7, ptr @.str.1, i32 1491, ptr null)
  %5 = load i32, ptr %4, align 4
  %6 = load i32, ptr %3, align 4
  %7 = xor i32 %5, %6
  ret i32 %7
}

; Function Attrs: mustprogress noinline nounwind optnone uwtable
define dso_local i32 @logia_intrinsics_bin_bitwise_shl_u32_u32(i32 noundef %0, i32 noundef %1) #2 {
  %3 = alloca i32, align 4
  %4 = alloca i32, align 4
  store i32 %1, ptr %3, align 4
  call void @llvm.var.annotation.p0.p0(ptr %3, ptr @.str.7, ptr @.str.1, i32 1497, ptr null)
  store i32 %0, ptr %4, align 4
  call void @llvm.var.annotation.p0.p0(ptr %4, ptr @.str.7, ptr @.str.1, i32 1497, ptr null)
  %5 = load i32, ptr %4, align 4
  %6 = load i32, ptr %3, align 4
  %7 = shl i32 %5, %6
  ret i32 %7
}

; Function Attrs: mustprogress noinline nounwind optnone uwtable
define dso_local i32 @logia_intrinsics_bin_bitwise_shr_u32_u32(i32 noundef %0, i32 noundef %1) #2 {
  %3 = alloca i32, align 4
  %4 = alloca i32, align 4
  store i32 %1, ptr %3, align 4
  call void @llvm.var.annotation.p0.p0(ptr %3, ptr @.str.7, ptr @.str.1, i32 1503, ptr null)
  store i32 %0, ptr %4, align 4
  call void @llvm.var.annotation.p0.p0(ptr %4, ptr @.str.7, ptr @.str.1, i32 1503, ptr null)
  %5 = load i32, ptr %4, align 4
  %6 = load i32, ptr %3, align 4
  %7 = lshr i32 %5, %6
  ret i32 %7
}

; Function Attrs: mustprogress noinline nounwind optnone uwtable
define dso_local i64 @logia__intrinsics_bin_add_u64_u64(i64 noundef %0, i64 noundef %1) #2 {
  %3 = alloca i64, align 8
  %4 = alloca i64, align 8
  store i64 %1, ptr %3, align 8
  call void @llvm.var.annotation.p0.p0(ptr %3, ptr @.str.8, ptr @.str.1, i32 1509, ptr null)
  store i64 %0, ptr %4, align 8
  call void @llvm.var.annotation.p0.p0(ptr %4, ptr @.str.8, ptr @.str.1, i32 1509, ptr null)
  %5 = load i64, ptr %4, align 8
  %6 = load i64, ptr %3, align 8
  %7 = add i64 %5, %6
  ret i64 %7
}

; Function Attrs: mustprogress noinline nounwind optnone uwtable
define dso_local i64 @logia_intrinsics_bin_sub_u64_u64(i64 noundef %0, i64 noundef %1) #2 {
  %3 = alloca i64, align 8
  %4 = alloca i64, align 8
  store i64 %1, ptr %3, align 8
  call void @llvm.var.annotation.p0.p0(ptr %3, ptr @.str.8, ptr @.str.1, i32 1515, ptr null)
  store i64 %0, ptr %4, align 8
  call void @llvm.var.annotation.p0.p0(ptr %4, ptr @.str.8, ptr @.str.1, i32 1515, ptr null)
  %5 = load i64, ptr %4, align 8
  %6 = load i64, ptr %3, align 8
  %7 = sub i64 %5, %6
  ret i64 %7
}

; Function Attrs: mustprogress noinline nounwind optnone uwtable
define dso_local i64 @logia_intrinsics_bin_mul_u64_u64(i64 noundef %0, i64 noundef %1) #2 {
  %3 = alloca i64, align 8
  %4 = alloca i64, align 8
  store i64 %1, ptr %3, align 8
  call void @llvm.var.annotation.p0.p0(ptr %3, ptr @.str.8, ptr @.str.1, i32 1521, ptr null)
  store i64 %0, ptr %4, align 8
  call void @llvm.var.annotation.p0.p0(ptr %4, ptr @.str.8, ptr @.str.1, i32 1521, ptr null)
  %5 = load i64, ptr %4, align 8
  %6 = load i64, ptr %3, align 8
  %7 = mul i64 %5, %6
  ret i64 %7
}

; Function Attrs: mustprogress noinline nounwind optnone uwtable
define dso_local i64 @logia_intrinsics_bin_div_u64_u64(i64 noundef %0, i64 noundef %1) #2 {
  %3 = alloca i64, align 8
  %4 = alloca i64, align 8
  store i64 %1, ptr %3, align 8
  call void @llvm.var.annotation.p0.p0(ptr %3, ptr @.str.8, ptr @.str.1, i32 1527, ptr null)
  store i64 %0, ptr %4, align 8
  call void @llvm.var.annotation.p0.p0(ptr %4, ptr @.str.8, ptr @.str.1, i32 1527, ptr null)
  %5 = load i64, ptr %4, align 8
  %6 = load i64, ptr %3, align 8
  %7 = udiv i64 %5, %6
  ret i64 %7
}

; Function Attrs: mustprogress noinline nounwind optnone uwtable
define dso_local i64 @logia_intrinsics_bin_mod_u64_u64(i64 noundef %0, i64 noundef %1) #2 {
  %3 = alloca i64, align 8
  %4 = alloca i64, align 8
  store i64 %1, ptr %3, align 8
  call void @llvm.var.annotation.p0.p0(ptr %3, ptr @.str.8, ptr @.str.1, i32 1533, ptr null)
  store i64 %0, ptr %4, align 8
  call void @llvm.var.annotation.p0.p0(ptr %4, ptr @.str.8, ptr @.str.1, i32 1533, ptr null)
  %5 = load i64, ptr %4, align 8
  %6 = load i64, ptr %3, align 8
  %7 = urem i64 %5, %6
  ret i64 %7
}

; Function Attrs: mustprogress noinline nounwind optnone uwtable
define dso_local zeroext i1 @logia_intrinsics_bin_eq_u64_u64(i64 noundef %0, i64 noundef %1) #2 {
  %3 = alloca i64, align 8
  %4 = alloca i64, align 8
  store i64 %1, ptr %3, align 8
  call void @llvm.var.annotation.p0.p0(ptr %3, ptr @.str.8, ptr @.str.1, i32 1539, ptr null)
  store i64 %0, ptr %4, align 8
  call void @llvm.var.annotation.p0.p0(ptr %4, ptr @.str.8, ptr @.str.1, i32 1539, ptr null)
  %5 = load i64, ptr %4, align 8
  %6 = load i64, ptr %3, align 8
  %7 = icmp eq i64 %5, %6
  ret i1 %7
}

; Function Attrs: mustprogress noinline nounwind optnone uwtable
define dso_local zeroext i1 @logia_intrinsics_bin_neq_u64_u64(i64 noundef %0, i64 noundef %1) #2 {
  %3 = alloca i64, align 8
  %4 = alloca i64, align 8
  store i64 %1, ptr %3, align 8
  call void @llvm.var.annotation.p0.p0(ptr %3, ptr @.str.8, ptr @.str.1, i32 1545, ptr null)
  store i64 %0, ptr %4, align 8
  call void @llvm.var.annotation.p0.p0(ptr %4, ptr @.str.8, ptr @.str.1, i32 1545, ptr null)
  %5 = load i64, ptr %4, align 8
  %6 = load i64, ptr %3, align 8
  %7 = icmp ne i64 %5, %6
  ret i1 %7
}

; Function Attrs: mustprogress noinline nounwind optnone uwtable
define dso_local zeroext i1 @logia_intrinsics_bin_lt_u64_u64(i64 noundef %0, i64 noundef %1) #2 {
  %3 = alloca i64, align 8
  %4 = alloca i64, align 8
  store i64 %1, ptr %3, align 8
  call void @llvm.var.annotation.p0.p0(ptr %3, ptr @.str.8, ptr @.str.1, i32 1551, ptr null)
  store i64 %0, ptr %4, align 8
  call void @llvm.var.annotation.p0.p0(ptr %4, ptr @.str.8, ptr @.str.1, i32 1551, ptr null)
  %5 = load i64, ptr %4, align 8
  %6 = load i64, ptr %3, align 8
  %7 = icmp ult i64 %5, %6
  ret i1 %7
}

; Function Attrs: mustprogress noinline nounwind optnone uwtable
define dso_local zeroext i1 @logia_intrinsics_bin_gt_u64_u64(i64 noundef %0, i64 noundef %1) #2 {
  %3 = alloca i64, align 8
  %4 = alloca i64, align 8
  store i64 %1, ptr %3, align 8
  call void @llvm.var.annotation.p0.p0(ptr %3, ptr @.str.8, ptr @.str.1, i32 1557, ptr null)
  store i64 %0, ptr %4, align 8
  call void @llvm.var.annotation.p0.p0(ptr %4, ptr @.str.8, ptr @.str.1, i32 1557, ptr null)
  %5 = load i64, ptr %4, align 8
  %6 = load i64, ptr %3, align 8
  %7 = icmp ugt i64 %5, %6
  ret i1 %7
}

; Function Attrs: mustprogress noinline nounwind optnone uwtable
define dso_local zeroext i1 @logia_intrinsics_bin_lte_u64_u64(i64 noundef %0, i64 noundef %1) #2 {
  %3 = alloca i64, align 8
  %4 = alloca i64, align 8
  store i64 %1, ptr %3, align 8
  call void @llvm.var.annotation.p0.p0(ptr %3, ptr @.str.8, ptr @.str.1, i32 1563, ptr null)
  store i64 %0, ptr %4, align 8
  call void @llvm.var.annotation.p0.p0(ptr %4, ptr @.str.8, ptr @.str.1, i32 1563, ptr null)
  %5 = load i64, ptr %4, align 8
  %6 = load i64, ptr %3, align 8
  %7 = icmp ule i64 %5, %6
  ret i1 %7
}

; Function Attrs: mustprogress noinline nounwind optnone uwtable
define dso_local zeroext i1 @logia_intrinsics_bin_gte_u64_u64(i64 noundef %0, i64 noundef %1) #2 {
  %3 = alloca i64, align 8
  %4 = alloca i64, align 8
  store i64 %1, ptr %3, align 8
  call void @llvm.var.annotation.p0.p0(ptr %3, ptr @.str.8, ptr @.str.1, i32 1569, ptr null)
  store i64 %0, ptr %4, align 8
  call void @llvm.var.annotation.p0.p0(ptr %4, ptr @.str.8, ptr @.str.1, i32 1569, ptr null)
  %5 = load i64, ptr %4, align 8
  %6 = load i64, ptr %3, align 8
  %7 = icmp uge i64 %5, %6
  ret i1 %7
}

; Function Attrs: mustprogress noinline nounwind optnone uwtable
define dso_local i64 @logia_intrinsics_bin_assign_u64_u64(ptr noundef %0, i64 noundef %1) #2 {
  %3 = alloca i64, align 8
  %4 = alloca ptr, align 8
  store i64 %1, ptr %3, align 8
  call void @llvm.var.annotation.p0.p0(ptr %3, ptr @.str.8, ptr @.str.1, i32 1575, ptr null)
  store ptr %0, ptr %4, align 8
  call void @llvm.var.annotation.p0.p0(ptr %4, ptr @.str.16, ptr @.str.1, i32 1575, ptr null)
  %5 = load i64, ptr %3, align 8
  %6 = load ptr, ptr %4, align 8
  store i64 %5, ptr %6, align 8
  %7 = load ptr, ptr %4, align 8
  %8 = load i64, ptr %7, align 8
  ret i64 %8
}

; Function Attrs: mustprogress noinline nounwind optnone uwtable
define dso_local i64 @logia_intrinsics_bin_add_assign_u64_u64(ptr noundef %0, i64 noundef %1) #2 {
  %3 = alloca i64, align 8
  %4 = alloca ptr, align 8
  store i64 %1, ptr %3, align 8
  call void @llvm.var.annotation.p0.p0(ptr %3, ptr @.str.8, ptr @.str.1, i32 1582, ptr null)
  store ptr %0, ptr %4, align 8
  call void @llvm.var.annotation.p0.p0(ptr %4, ptr @.str.16, ptr @.str.1, i32 1582, ptr null)
  %5 = load i64, ptr %3, align 8
  %6 = load ptr, ptr %4, align 8
  %7 = load i64, ptr %6, align 8
  %8 = add i64 %7, %5
  store i64 %8, ptr %6, align 8
  %9 = load ptr, ptr %4, align 8
  %10 = load i64, ptr %9, align 8
  ret i64 %10
}

; Function Attrs: mustprogress noinline nounwind optnone uwtable
define dso_local i64 @logia_intrinsics_bin_sub_assign_u64_u64(ptr noundef %0, i64 noundef %1) #2 {
  %3 = alloca i64, align 8
  %4 = alloca ptr, align 8
  store i64 %1, ptr %3, align 8
  call void @llvm.var.annotation.p0.p0(ptr %3, ptr @.str.8, ptr @.str.1, i32 1589, ptr null)
  store ptr %0, ptr %4, align 8
  call void @llvm.var.annotation.p0.p0(ptr %4, ptr @.str.16, ptr @.str.1, i32 1589, ptr null)
  %5 = load i64, ptr %3, align 8
  %6 = load ptr, ptr %4, align 8
  %7 = load i64, ptr %6, align 8
  %8 = sub i64 %7, %5
  store i64 %8, ptr %6, align 8
  %9 = load ptr, ptr %4, align 8
  %10 = load i64, ptr %9, align 8
  ret i64 %10
}

; Function Attrs: mustprogress noinline nounwind optnone uwtable
define dso_local i64 @logia_intrinsics_bin_mul_assign_u64_u64(ptr noundef %0, i64 noundef %1) #2 {
  %3 = alloca i64, align 8
  %4 = alloca ptr, align 8
  store i64 %1, ptr %3, align 8
  call void @llvm.var.annotation.p0.p0(ptr %3, ptr @.str.8, ptr @.str.1, i32 1596, ptr null)
  store ptr %0, ptr %4, align 8
  call void @llvm.var.annotation.p0.p0(ptr %4, ptr @.str.16, ptr @.str.1, i32 1596, ptr null)
  %5 = load i64, ptr %3, align 8
  %6 = load ptr, ptr %4, align 8
  %7 = load i64, ptr %6, align 8
  %8 = mul i64 %7, %5
  store i64 %8, ptr %6, align 8
  %9 = load ptr, ptr %4, align 8
  %10 = load i64, ptr %9, align 8
  ret i64 %10
}

; Function Attrs: mustprogress noinline nounwind optnone uwtable
define dso_local i64 @logia_intrinsics_bin_div_assign_u64_u64(ptr noundef %0, i64 noundef %1) #2 {
  %3 = alloca i64, align 8
  %4 = alloca ptr, align 8
  store i64 %1, ptr %3, align 8
  call void @llvm.var.annotation.p0.p0(ptr %3, ptr @.str.8, ptr @.str.1, i32 1603, ptr null)
  store ptr %0, ptr %4, align 8
  call void @llvm.var.annotation.p0.p0(ptr %4, ptr @.str.16, ptr @.str.1, i32 1603, ptr null)
  %5 = load i64, ptr %3, align 8
  %6 = load ptr, ptr %4, align 8
  %7 = load i64, ptr %6, align 8
  %8 = udiv i64 %7, %5
  store i64 %8, ptr %6, align 8
  %9 = load ptr, ptr %4, align 8
  %10 = load i64, ptr %9, align 8
  ret i64 %10
}

; Function Attrs: mustprogress noinline nounwind optnone uwtable
define dso_local i64 @logia_intrinsics_bin_mod_assign_u64_u64(ptr noundef %0, i64 noundef %1) #2 {
  %3 = alloca i64, align 8
  %4 = alloca ptr, align 8
  store i64 %1, ptr %3, align 8
  call void @llvm.var.annotation.p0.p0(ptr %3, ptr @.str.8, ptr @.str.1, i32 1610, ptr null)
  store ptr %0, ptr %4, align 8
  call void @llvm.var.annotation.p0.p0(ptr %4, ptr @.str.16, ptr @.str.1, i32 1610, ptr null)
  %5 = load i64, ptr %3, align 8
  %6 = load ptr, ptr %4, align 8
  %7 = load i64, ptr %6, align 8
  %8 = urem i64 %7, %5
  store i64 %8, ptr %6, align 8
  %9 = load ptr, ptr %4, align 8
  %10 = load i64, ptr %9, align 8
  ret i64 %10
}

; Function Attrs: mustprogress noinline nounwind optnone uwtable
define dso_local i64 @logia_intrinsics_bin_bitwise_and_assign_u64_u64(ptr noundef %0, i64 noundef %1) #2 {
  %3 = alloca i64, align 8
  %4 = alloca ptr, align 8
  store i64 %1, ptr %3, align 8
  call void @llvm.var.annotation.p0.p0(ptr %3, ptr @.str.8, ptr @.str.1, i32 1617, ptr null)
  store ptr %0, ptr %4, align 8
  call void @llvm.var.annotation.p0.p0(ptr %4, ptr @.str.16, ptr @.str.1, i32 1617, ptr null)
  %5 = load i64, ptr %3, align 8
  %6 = load ptr, ptr %4, align 8
  %7 = load i64, ptr %6, align 8
  %8 = and i64 %7, %5
  store i64 %8, ptr %6, align 8
  %9 = load ptr, ptr %4, align 8
  %10 = load i64, ptr %9, align 8
  ret i64 %10
}

; Function Attrs: mustprogress noinline nounwind optnone uwtable
define dso_local i64 @logia_intrinsics_bin_bitwise_or_assign_u64_u64(ptr noundef %0, i64 noundef %1) #2 {
  %3 = alloca i64, align 8
  %4 = alloca ptr, align 8
  store i64 %1, ptr %3, align 8
  call void @llvm.var.annotation.p0.p0(ptr %3, ptr @.str.8, ptr @.str.1, i32 1624, ptr null)
  store ptr %0, ptr %4, align 8
  call void @llvm.var.annotation.p0.p0(ptr %4, ptr @.str.16, ptr @.str.1, i32 1624, ptr null)
  %5 = load i64, ptr %3, align 8
  %6 = load ptr, ptr %4, align 8
  %7 = load i64, ptr %6, align 8
  %8 = or i64 %7, %5
  store i64 %8, ptr %6, align 8
  %9 = load ptr, ptr %4, align 8
  %10 = load i64, ptr %9, align 8
  ret i64 %10
}

; Function Attrs: mustprogress noinline nounwind optnone uwtable
define dso_local i64 @logia_intrinsics_bin_bitwise_xor_assign_u64_u64(ptr noundef %0, i64 noundef %1) #2 {
  %3 = alloca i64, align 8
  %4 = alloca ptr, align 8
  store i64 %1, ptr %3, align 8
  call void @llvm.var.annotation.p0.p0(ptr %3, ptr @.str.8, ptr @.str.1, i32 1631, ptr null)
  store ptr %0, ptr %4, align 8
  call void @llvm.var.annotation.p0.p0(ptr %4, ptr @.str.16, ptr @.str.1, i32 1631, ptr null)
  %5 = load i64, ptr %3, align 8
  %6 = load ptr, ptr %4, align 8
  %7 = load i64, ptr %6, align 8
  %8 = xor i64 %7, %5
  store i64 %8, ptr %6, align 8
  %9 = load ptr, ptr %4, align 8
  %10 = load i64, ptr %9, align 8
  ret i64 %10
}

; Function Attrs: mustprogress noinline nounwind optnone uwtable
define dso_local i64 @logia_intrinsics_bin_bitwise_shr_assign_u64_u64(ptr noundef %0, i64 noundef %1) #2 {
  %3 = alloca i64, align 8
  %4 = alloca ptr, align 8
  store i64 %1, ptr %3, align 8
  call void @llvm.var.annotation.p0.p0(ptr %3, ptr @.str.8, ptr @.str.1, i32 1638, ptr null)
  store ptr %0, ptr %4, align 8
  call void @llvm.var.annotation.p0.p0(ptr %4, ptr @.str.16, ptr @.str.1, i32 1638, ptr null)
  %5 = load i64, ptr %3, align 8
  %6 = load ptr, ptr %4, align 8
  %7 = load i64, ptr %6, align 8
  %8 = lshr i64 %7, %5
  store i64 %8, ptr %6, align 8
  %9 = load ptr, ptr %4, align 8
  %10 = load i64, ptr %9, align 8
  ret i64 %10
}

; Function Attrs: mustprogress noinline nounwind optnone uwtable
define dso_local i64 @logia_intrinsics_bin_bitwise_shl_assign_u64_u64(ptr noundef %0, i64 noundef %1) #2 {
  %3 = alloca i64, align 8
  %4 = alloca ptr, align 8
  store i64 %1, ptr %3, align 8
  call void @llvm.var.annotation.p0.p0(ptr %3, ptr @.str.8, ptr @.str.1, i32 1645, ptr null)
  store ptr %0, ptr %4, align 8
  call void @llvm.var.annotation.p0.p0(ptr %4, ptr @.str.16, ptr @.str.1, i32 1645, ptr null)
  %5 = load i64, ptr %3, align 8
  %6 = load ptr, ptr %4, align 8
  %7 = load i64, ptr %6, align 8
  %8 = shl i64 %7, %5
  store i64 %8, ptr %6, align 8
  %9 = load ptr, ptr %4, align 8
  %10 = load i64, ptr %9, align 8
  ret i64 %10
}

; Function Attrs: mustprogress noinline nounwind optnone uwtable
define dso_local i64 @logia_intrinsics_bin_bitwise_and_u64_u64(i64 noundef %0, i64 noundef %1) #2 {
  %3 = alloca i64, align 8
  %4 = alloca i64, align 8
  store i64 %1, ptr %3, align 8
  call void @llvm.var.annotation.p0.p0(ptr %3, ptr @.str.8, ptr @.str.1, i32 1652, ptr null)
  store i64 %0, ptr %4, align 8
  call void @llvm.var.annotation.p0.p0(ptr %4, ptr @.str.8, ptr @.str.1, i32 1652, ptr null)
  %5 = load i64, ptr %4, align 8
  %6 = load i64, ptr %3, align 8
  %7 = and i64 %5, %6
  ret i64 %7
}

; Function Attrs: mustprogress noinline nounwind optnone uwtable
define dso_local i64 @logia_intrinsics_bin_bitwise_or_u64_u64(i64 noundef %0, i64 noundef %1) #2 {
  %3 = alloca i64, align 8
  %4 = alloca i64, align 8
  store i64 %1, ptr %3, align 8
  call void @llvm.var.annotation.p0.p0(ptr %3, ptr @.str.8, ptr @.str.1, i32 1658, ptr null)
  store i64 %0, ptr %4, align 8
  call void @llvm.var.annotation.p0.p0(ptr %4, ptr @.str.8, ptr @.str.1, i32 1658, ptr null)
  %5 = load i64, ptr %4, align 8
  %6 = load i64, ptr %3, align 8
  %7 = or i64 %5, %6
  ret i64 %7
}

; Function Attrs: mustprogress noinline nounwind optnone uwtable
define dso_local i64 @logia_intrinsics_bin_bitwise_xor_u64_u64(i64 noundef %0, i64 noundef %1) #2 {
  %3 = alloca i64, align 8
  %4 = alloca i64, align 8
  store i64 %1, ptr %3, align 8
  call void @llvm.var.annotation.p0.p0(ptr %3, ptr @.str.8, ptr @.str.1, i32 1664, ptr null)
  store i64 %0, ptr %4, align 8
  call void @llvm.var.annotation.p0.p0(ptr %4, ptr @.str.8, ptr @.str.1, i32 1664, ptr null)
  %5 = load i64, ptr %4, align 8
  %6 = load i64, ptr %3, align 8
  %7 = xor i64 %5, %6
  ret i64 %7
}

; Function Attrs: mustprogress noinline nounwind optnone uwtable
define dso_local i64 @logia_intrinsics_bin_bitwise_shl_u64_u64(i64 noundef %0, i64 noundef %1) #2 {
  %3 = alloca i64, align 8
  %4 = alloca i64, align 8
  store i64 %1, ptr %3, align 8
  call void @llvm.var.annotation.p0.p0(ptr %3, ptr @.str.8, ptr @.str.1, i32 1670, ptr null)
  store i64 %0, ptr %4, align 8
  call void @llvm.var.annotation.p0.p0(ptr %4, ptr @.str.8, ptr @.str.1, i32 1670, ptr null)
  %5 = load i64, ptr %4, align 8
  %6 = load i64, ptr %3, align 8
  %7 = shl i64 %5, %6
  ret i64 %7
}

; Function Attrs: mustprogress noinline nounwind optnone uwtable
define dso_local i64 @logia_intrinsics_bin_bitwise_shr_u64_u64(i64 noundef %0, i64 noundef %1) #2 {
  %3 = alloca i64, align 8
  %4 = alloca i64, align 8
  store i64 %1, ptr %3, align 8
  call void @llvm.var.annotation.p0.p0(ptr %3, ptr @.str.8, ptr @.str.1, i32 1676, ptr null)
  store i64 %0, ptr %4, align 8
  call void @llvm.var.annotation.p0.p0(ptr %4, ptr @.str.8, ptr @.str.1, i32 1676, ptr null)
  %5 = load i64, ptr %4, align 8
  %6 = load i64, ptr %3, align 8
  %7 = lshr i64 %5, %6
  ret i64 %7
}

; Function Attrs: mustprogress noinline nounwind optnone uwtable
define dso_local float @logia__intrinsics_bin_add_f32_f32(float noundef %0, float noundef %1) #2 {
  %3 = alloca float, align 4
  %4 = alloca float, align 4
  store float %1, ptr %3, align 4
  call void @llvm.var.annotation.p0.p0(ptr %3, ptr @.str.17, ptr @.str.1, i32 1683, ptr null)
  store float %0, ptr %4, align 4
  call void @llvm.var.annotation.p0.p0(ptr %4, ptr @.str.17, ptr @.str.1, i32 1683, ptr null)
  %5 = load float, ptr %4, align 4
  %6 = load float, ptr %3, align 4
  %7 = fadd float %5, %6
  ret float %7
}

; Function Attrs: mustprogress noinline nounwind optnone uwtable
define dso_local float @logia_intrinsics_bin_sub_f32_f32(float noundef %0, float noundef %1) #2 {
  %3 = alloca float, align 4
  %4 = alloca float, align 4
  store float %1, ptr %3, align 4
  call void @llvm.var.annotation.p0.p0(ptr %3, ptr @.str.17, ptr @.str.1, i32 1689, ptr null)
  store float %0, ptr %4, align 4
  call void @llvm.var.annotation.p0.p0(ptr %4, ptr @.str.17, ptr @.str.1, i32 1689, ptr null)
  %5 = load float, ptr %4, align 4
  %6 = load float, ptr %3, align 4
  %7 = fsub float %5, %6
  ret float %7
}

; Function Attrs: mustprogress noinline nounwind optnone uwtable
define dso_local float @logia_intrinsics_bin_mul_f32_f32(float noundef %0, float noundef %1) #2 {
  %3 = alloca float, align 4
  %4 = alloca float, align 4
  store float %1, ptr %3, align 4
  call void @llvm.var.annotation.p0.p0(ptr %3, ptr @.str.17, ptr @.str.1, i32 1695, ptr null)
  store float %0, ptr %4, align 4
  call void @llvm.var.annotation.p0.p0(ptr %4, ptr @.str.17, ptr @.str.1, i32 1695, ptr null)
  %5 = load float, ptr %4, align 4
  %6 = load float, ptr %3, align 4
  %7 = fmul float %5, %6
  ret float %7
}

; Function Attrs: mustprogress noinline nounwind optnone uwtable
define dso_local float @logia_intrinsics_bin_div_f32_f32(float noundef %0, float noundef %1) #2 {
  %3 = alloca float, align 4
  %4 = alloca float, align 4
  store float %1, ptr %3, align 4
  call void @llvm.var.annotation.p0.p0(ptr %3, ptr @.str.17, ptr @.str.1, i32 1701, ptr null)
  store float %0, ptr %4, align 4
  call void @llvm.var.annotation.p0.p0(ptr %4, ptr @.str.17, ptr @.str.1, i32 1701, ptr null)
  %5 = load float, ptr %4, align 4
  %6 = load float, ptr %3, align 4
  %7 = fdiv float %5, %6
  ret float %7
}

; Function Attrs: mustprogress noinline nounwind optnone uwtable
define dso_local zeroext i1 @logia_intrinsics_bin_eq_f32_f32(float noundef %0, float noundef %1) #2 {
  %3 = alloca float, align 4
  %4 = alloca float, align 4
  store float %1, ptr %3, align 4
  call void @llvm.var.annotation.p0.p0(ptr %3, ptr @.str.17, ptr @.str.1, i32 1707, ptr null)
  store float %0, ptr %4, align 4
  call void @llvm.var.annotation.p0.p0(ptr %4, ptr @.str.17, ptr @.str.1, i32 1707, ptr null)
  %5 = load float, ptr %4, align 4
  %6 = load float, ptr %3, align 4
  %7 = fcmp oeq float %5, %6
  ret i1 %7
}

; Function Attrs: mustprogress noinline nounwind optnone uwtable
define dso_local zeroext i1 @logia_intrinsics_bin_neq_f32_f32(float noundef %0, float noundef %1) #2 {
  %3 = alloca float, align 4
  %4 = alloca float, align 4
  store float %1, ptr %3, align 4
  call void @llvm.var.annotation.p0.p0(ptr %3, ptr @.str.17, ptr @.str.1, i32 1713, ptr null)
  store float %0, ptr %4, align 4
  call void @llvm.var.annotation.p0.p0(ptr %4, ptr @.str.17, ptr @.str.1, i32 1713, ptr null)
  %5 = load float, ptr %4, align 4
  %6 = load float, ptr %3, align 4
  %7 = fcmp une float %5, %6
  ret i1 %7
}

; Function Attrs: mustprogress noinline nounwind optnone uwtable
define dso_local zeroext i1 @logia_intrinsics_bin_lt_f32_f32(float noundef %0, float noundef %1) #2 {
  %3 = alloca float, align 4
  %4 = alloca float, align 4
  store float %1, ptr %3, align 4
  call void @llvm.var.annotation.p0.p0(ptr %3, ptr @.str.17, ptr @.str.1, i32 1719, ptr null)
  store float %0, ptr %4, align 4
  call void @llvm.var.annotation.p0.p0(ptr %4, ptr @.str.17, ptr @.str.1, i32 1719, ptr null)
  %5 = load float, ptr %4, align 4
  %6 = load float, ptr %3, align 4
  %7 = fcmp olt float %5, %6
  ret i1 %7
}

; Function Attrs: mustprogress noinline nounwind optnone uwtable
define dso_local zeroext i1 @logia_intrinsics_bin_lte_f32_f32(float noundef %0, float noundef %1) #2 {
  %3 = alloca float, align 4
  %4 = alloca float, align 4
  store float %1, ptr %3, align 4
  call void @llvm.var.annotation.p0.p0(ptr %3, ptr @.str.17, ptr @.str.1, i32 1725, ptr null)
  store float %0, ptr %4, align 4
  call void @llvm.var.annotation.p0.p0(ptr %4, ptr @.str.17, ptr @.str.1, i32 1725, ptr null)
  %5 = load float, ptr %4, align 4
  %6 = load float, ptr %3, align 4
  %7 = fcmp ole float %5, %6
  ret i1 %7
}

; Function Attrs: mustprogress noinline nounwind optnone uwtable
define dso_local zeroext i1 @logia_intrinsics_bin_gte_f32_f32(float noundef %0, float noundef %1) #2 {
  %3 = alloca float, align 4
  %4 = alloca float, align 4
  store float %1, ptr %3, align 4
  call void @llvm.var.annotation.p0.p0(ptr %3, ptr @.str.17, ptr @.str.1, i32 1731, ptr null)
  store float %0, ptr %4, align 4
  call void @llvm.var.annotation.p0.p0(ptr %4, ptr @.str.17, ptr @.str.1, i32 1731, ptr null)
  %5 = load float, ptr %4, align 4
  %6 = load float, ptr %3, align 4
  %7 = fcmp oge float %5, %6
  ret i1 %7
}

; Function Attrs: mustprogress noinline nounwind optnone uwtable
define dso_local float @logia_intrinsics_bin_assign_f32_f32(ptr noundef %0, float noundef %1) #2 {
  %3 = alloca float, align 4
  %4 = alloca ptr, align 8
  store float %1, ptr %3, align 4
  call void @llvm.var.annotation.p0.p0(ptr %3, ptr @.str.18, ptr @.str.1, i32 1737, ptr null)
  store ptr %0, ptr %4, align 8
  call void @llvm.var.annotation.p0.p0(ptr %4, ptr @.str.19, ptr @.str.1, i32 1737, ptr null)
  %5 = load float, ptr %3, align 4
  %6 = load ptr, ptr %4, align 8
  store float %5, ptr %6, align 4
  %7 = load ptr, ptr %4, align 8
  %8 = load float, ptr %7, align 4
  ret float %8
}

; Function Attrs: mustprogress noinline nounwind optnone uwtable
define dso_local float @logia_intrinsics_bin_add_assign_f32_f32(ptr noundef %0, float noundef %1) #2 {
  %3 = alloca float, align 4
  %4 = alloca ptr, align 8
  store float %1, ptr %3, align 4
  call void @llvm.var.annotation.p0.p0(ptr %3, ptr @.str.18, ptr @.str.1, i32 1744, ptr null)
  store ptr %0, ptr %4, align 8
  call void @llvm.var.annotation.p0.p0(ptr %4, ptr @.str.19, ptr @.str.1, i32 1744, ptr null)
  %5 = load float, ptr %3, align 4
  %6 = load ptr, ptr %4, align 8
  %7 = load float, ptr %6, align 4
  %8 = fadd float %7, %5
  store float %8, ptr %6, align 4
  %9 = load ptr, ptr %4, align 8
  %10 = load float, ptr %9, align 4
  ret float %10
}

; Function Attrs: mustprogress noinline nounwind optnone uwtable
define dso_local float @logia_intrinsics_bin_sub_assign_f32_f32(ptr noundef %0, float noundef %1) #2 {
  %3 = alloca float, align 4
  %4 = alloca ptr, align 8
  store float %1, ptr %3, align 4
  call void @llvm.var.annotation.p0.p0(ptr %3, ptr @.str.18, ptr @.str.1, i32 1751, ptr null)
  store ptr %0, ptr %4, align 8
  call void @llvm.var.annotation.p0.p0(ptr %4, ptr @.str.19, ptr @.str.1, i32 1751, ptr null)
  %5 = load float, ptr %3, align 4
  %6 = load ptr, ptr %4, align 8
  %7 = load float, ptr %6, align 4
  %8 = fsub float %7, %5
  store float %8, ptr %6, align 4
  %9 = load ptr, ptr %4, align 8
  %10 = load float, ptr %9, align 4
  ret float %10
}

; Function Attrs: mustprogress noinline nounwind optnone uwtable
define dso_local float @logia_intrinsics_bin_mul_assign_f32_f32(ptr noundef %0, float noundef %1) #2 {
  %3 = alloca float, align 4
  %4 = alloca ptr, align 8
  store float %1, ptr %3, align 4
  call void @llvm.var.annotation.p0.p0(ptr %3, ptr @.str.18, ptr @.str.1, i32 1758, ptr null)
  store ptr %0, ptr %4, align 8
  call void @llvm.var.annotation.p0.p0(ptr %4, ptr @.str.19, ptr @.str.1, i32 1758, ptr null)
  %5 = load float, ptr %3, align 4
  %6 = load ptr, ptr %4, align 8
  %7 = load float, ptr %6, align 4
  %8 = fmul float %7, %5
  store float %8, ptr %6, align 4
  %9 = load ptr, ptr %4, align 8
  %10 = load float, ptr %9, align 4
  ret float %10
}

; Function Attrs: mustprogress noinline nounwind optnone uwtable
define dso_local float @logia_intrinsics_bin_div_assign_f32_f32(ptr noundef %0, float noundef %1) #2 {
  %3 = alloca float, align 4
  %4 = alloca ptr, align 8
  store float %1, ptr %3, align 4
  call void @llvm.var.annotation.p0.p0(ptr %3, ptr @.str.18, ptr @.str.1, i32 1765, ptr null)
  store ptr %0, ptr %4, align 8
  call void @llvm.var.annotation.p0.p0(ptr %4, ptr @.str.19, ptr @.str.1, i32 1765, ptr null)
  %5 = load float, ptr %3, align 4
  %6 = load ptr, ptr %4, align 8
  %7 = load float, ptr %6, align 4
  %8 = fdiv float %7, %5
  store float %8, ptr %6, align 4
  %9 = load ptr, ptr %4, align 8
  %10 = load float, ptr %9, align 4
  ret float %10
}

; Function Attrs: mustprogress noinline nounwind optnone uwtable
define dso_local double @logia__intrinsics_bin_add_f64_f64(double noundef %0, double noundef %1) #2 {
  %3 = alloca double, align 8
  %4 = alloca double, align 8
  store double %1, ptr %3, align 8
  call void @llvm.var.annotation.p0.p0(ptr %3, ptr @.str.20, ptr @.str.1, i32 1772, ptr null)
  store double %0, ptr %4, align 8
  call void @llvm.var.annotation.p0.p0(ptr %4, ptr @.str.20, ptr @.str.1, i32 1772, ptr null)
  %5 = load double, ptr %4, align 8
  %6 = load double, ptr %3, align 8
  %7 = fadd double %5, %6
  ret double %7
}

; Function Attrs: mustprogress noinline nounwind optnone uwtable
define dso_local double @logia_intrinsics_bin_sub_f64_f64(double noundef %0, double noundef %1) #2 {
  %3 = alloca double, align 8
  %4 = alloca double, align 8
  store double %1, ptr %3, align 8
  call void @llvm.var.annotation.p0.p0(ptr %3, ptr @.str.20, ptr @.str.1, i32 1778, ptr null)
  store double %0, ptr %4, align 8
  call void @llvm.var.annotation.p0.p0(ptr %4, ptr @.str.20, ptr @.str.1, i32 1778, ptr null)
  %5 = load double, ptr %4, align 8
  %6 = load double, ptr %3, align 8
  %7 = fsub double %5, %6
  ret double %7
}

; Function Attrs: mustprogress noinline nounwind optnone uwtable
define dso_local double @logia_intrinsics_bin_mul_f64_f64(double noundef %0, double noundef %1) #2 {
  %3 = alloca double, align 8
  %4 = alloca double, align 8
  store double %1, ptr %3, align 8
  call void @llvm.var.annotation.p0.p0(ptr %3, ptr @.str.20, ptr @.str.1, i32 1784, ptr null)
  store double %0, ptr %4, align 8
  call void @llvm.var.annotation.p0.p0(ptr %4, ptr @.str.20, ptr @.str.1, i32 1784, ptr null)
  %5 = load double, ptr %4, align 8
  %6 = load double, ptr %3, align 8
  %7 = fmul double %5, %6
  ret double %7
}

; Function Attrs: mustprogress noinline nounwind optnone uwtable
define dso_local double @logia_intrinsics_bin_div_f64_f64(double noundef %0, double noundef %1) #2 {
  %3 = alloca double, align 8
  %4 = alloca double, align 8
  store double %1, ptr %3, align 8
  call void @llvm.var.annotation.p0.p0(ptr %3, ptr @.str.20, ptr @.str.1, i32 1790, ptr null)
  store double %0, ptr %4, align 8
  call void @llvm.var.annotation.p0.p0(ptr %4, ptr @.str.20, ptr @.str.1, i32 1790, ptr null)
  %5 = load double, ptr %4, align 8
  %6 = load double, ptr %3, align 8
  %7 = fdiv double %5, %6
  ret double %7
}

; Function Attrs: mustprogress noinline nounwind optnone uwtable
define dso_local zeroext i1 @logia_intrinsics_bin_eq_f64_f64(double noundef %0, double noundef %1) #2 {
  %3 = alloca double, align 8
  %4 = alloca double, align 8
  store double %1, ptr %3, align 8
  call void @llvm.var.annotation.p0.p0(ptr %3, ptr @.str.20, ptr @.str.1, i32 1796, ptr null)
  store double %0, ptr %4, align 8
  call void @llvm.var.annotation.p0.p0(ptr %4, ptr @.str.20, ptr @.str.1, i32 1796, ptr null)
  %5 = load double, ptr %4, align 8
  %6 = load double, ptr %3, align 8
  %7 = fcmp oeq double %5, %6
  ret i1 %7
}

; Function Attrs: mustprogress noinline nounwind optnone uwtable
define dso_local zeroext i1 @logia_intrinsics_bin_neq_f64_f64(double noundef %0, double noundef %1) #2 {
  %3 = alloca double, align 8
  %4 = alloca double, align 8
  store double %1, ptr %3, align 8
  call void @llvm.var.annotation.p0.p0(ptr %3, ptr @.str.20, ptr @.str.1, i32 1802, ptr null)
  store double %0, ptr %4, align 8
  call void @llvm.var.annotation.p0.p0(ptr %4, ptr @.str.20, ptr @.str.1, i32 1802, ptr null)
  %5 = load double, ptr %4, align 8
  %6 = load double, ptr %3, align 8
  %7 = fcmp une double %5, %6
  ret i1 %7
}

; Function Attrs: mustprogress noinline nounwind optnone uwtable
define dso_local zeroext i1 @logia_intrinsics_bin_lt_f64_f64(double noundef %0, double noundef %1) #2 {
  %3 = alloca double, align 8
  %4 = alloca double, align 8
  store double %1, ptr %3, align 8
  call void @llvm.var.annotation.p0.p0(ptr %3, ptr @.str.20, ptr @.str.1, i32 1808, ptr null)
  store double %0, ptr %4, align 8
  call void @llvm.var.annotation.p0.p0(ptr %4, ptr @.str.20, ptr @.str.1, i32 1808, ptr null)
  %5 = load double, ptr %4, align 8
  %6 = load double, ptr %3, align 8
  %7 = fcmp olt double %5, %6
  ret i1 %7
}

; Function Attrs: mustprogress noinline nounwind optnone uwtable
define dso_local zeroext i1 @logia_intrinsics_bin_lte_f64_f64(double noundef %0, double noundef %1) #2 {
  %3 = alloca double, align 8
  %4 = alloca double, align 8
  store double %1, ptr %3, align 8
  call void @llvm.var.annotation.p0.p0(ptr %3, ptr @.str.20, ptr @.str.1, i32 1814, ptr null)
  store double %0, ptr %4, align 8
  call void @llvm.var.annotation.p0.p0(ptr %4, ptr @.str.20, ptr @.str.1, i32 1814, ptr null)
  %5 = load double, ptr %4, align 8
  %6 = load double, ptr %3, align 8
  %7 = fcmp ole double %5, %6
  ret i1 %7
}

; Function Attrs: mustprogress noinline nounwind optnone uwtable
define dso_local zeroext i1 @logia_intrinsics_bin_gte_f64_f64(double noundef %0, double noundef %1) #2 {
  %3 = alloca double, align 8
  %4 = alloca double, align 8
  store double %1, ptr %3, align 8
  call void @llvm.var.annotation.p0.p0(ptr %3, ptr @.str.20, ptr @.str.1, i32 1820, ptr null)
  store double %0, ptr %4, align 8
  call void @llvm.var.annotation.p0.p0(ptr %4, ptr @.str.20, ptr @.str.1, i32 1820, ptr null)
  %5 = load double, ptr %4, align 8
  %6 = load double, ptr %3, align 8
  %7 = fcmp oge double %5, %6
  ret i1 %7
}

; Function Attrs: mustprogress noinline nounwind optnone uwtable
define dso_local double @logia_intrinsics_bin_assign_f64_f64(ptr noundef %0, double noundef %1) #2 {
  %3 = alloca double, align 8
  %4 = alloca ptr, align 8
  store double %1, ptr %3, align 8
  call void @llvm.var.annotation.p0.p0(ptr %3, ptr @.str.21, ptr @.str.1, i32 1826, ptr null)
  store ptr %0, ptr %4, align 8
  call void @llvm.var.annotation.p0.p0(ptr %4, ptr @.str.22, ptr @.str.1, i32 1826, ptr null)
  %5 = load double, ptr %3, align 8
  %6 = load ptr, ptr %4, align 8
  store double %5, ptr %6, align 8
  %7 = load ptr, ptr %4, align 8
  %8 = load double, ptr %7, align 8
  ret double %8
}

; Function Attrs: mustprogress noinline nounwind optnone uwtable
define dso_local double @logia_intrinsics_bin_add_assign_f64_f64(ptr noundef %0, double noundef %1) #2 {
  %3 = alloca double, align 8
  %4 = alloca ptr, align 8
  store double %1, ptr %3, align 8
  call void @llvm.var.annotation.p0.p0(ptr %3, ptr @.str.21, ptr @.str.1, i32 1833, ptr null)
  store ptr %0, ptr %4, align 8
  call void @llvm.var.annotation.p0.p0(ptr %4, ptr @.str.22, ptr @.str.1, i32 1833, ptr null)
  %5 = load double, ptr %3, align 8
  %6 = load ptr, ptr %4, align 8
  %7 = load double, ptr %6, align 8
  %8 = fadd double %7, %5
  store double %8, ptr %6, align 8
  %9 = load ptr, ptr %4, align 8
  %10 = load double, ptr %9, align 8
  ret double %10
}

; Function Attrs: mustprogress noinline nounwind optnone uwtable
define dso_local double @logia_intrinsics_bin_sub_assign_f64_f64(ptr noundef %0, double noundef %1) #2 {
  %3 = alloca double, align 8
  %4 = alloca ptr, align 8
  store double %1, ptr %3, align 8
  call void @llvm.var.annotation.p0.p0(ptr %3, ptr @.str.21, ptr @.str.1, i32 1840, ptr null)
  store ptr %0, ptr %4, align 8
  call void @llvm.var.annotation.p0.p0(ptr %4, ptr @.str.22, ptr @.str.1, i32 1840, ptr null)
  %5 = load double, ptr %3, align 8
  %6 = load ptr, ptr %4, align 8
  %7 = load double, ptr %6, align 8
  %8 = fsub double %7, %5
  store double %8, ptr %6, align 8
  %9 = load ptr, ptr %4, align 8
  %10 = load double, ptr %9, align 8
  ret double %10
}

; Function Attrs: mustprogress noinline nounwind optnone uwtable
define dso_local double @logia_intrinsics_bin_mul_assign_f64_f64(ptr noundef %0, double noundef %1) #2 {
  %3 = alloca double, align 8
  %4 = alloca ptr, align 8
  store double %1, ptr %3, align 8
  call void @llvm.var.annotation.p0.p0(ptr %3, ptr @.str.21, ptr @.str.1, i32 1847, ptr null)
  store ptr %0, ptr %4, align 8
  call void @llvm.var.annotation.p0.p0(ptr %4, ptr @.str.22, ptr @.str.1, i32 1847, ptr null)
  %5 = load double, ptr %3, align 8
  %6 = load ptr, ptr %4, align 8
  %7 = load double, ptr %6, align 8
  %8 = fmul double %7, %5
  store double %8, ptr %6, align 8
  %9 = load ptr, ptr %4, align 8
  %10 = load double, ptr %9, align 8
  ret double %10
}

; Function Attrs: mustprogress noinline nounwind optnone uwtable
define dso_local double @logia_intrinsics_bin_div_assign_f64_f64(ptr noundef %0, double noundef %1) #2 {
  %3 = alloca double, align 8
  %4 = alloca ptr, align 8
  store double %1, ptr %3, align 8
  call void @llvm.var.annotation.p0.p0(ptr %3, ptr @.str.21, ptr @.str.1, i32 1854, ptr null)
  store ptr %0, ptr %4, align 8
  call void @llvm.var.annotation.p0.p0(ptr %4, ptr @.str.22, ptr @.str.1, i32 1854, ptr null)
  %5 = load double, ptr %3, align 8
  %6 = load ptr, ptr %4, align 8
  %7 = load double, ptr %6, align 8
  %8 = fdiv double %7, %5
  store double %8, ptr %6, align 8
  %9 = load ptr, ptr %4, align 8
  %10 = load double, ptr %9, align 8
  ret double %10
}

attributes #0 = { mustprogress noinline optnone uwtable "min-legal-vector-width"="0" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cmov,+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }
attributes #1 = { "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cmov,+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }
attributes #2 = { mustprogress noinline nounwind optnone uwtable "min-legal-vector-width"="0" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cmov,+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }
attributes #3 = { nocallback nofree nosync nounwind willreturn memory(inaccessiblemem: readwrite) }

!llvm.dbg.cu = !{!0}
!llvm.linker.options = !{!2}
!llvm.module.flags = !{!3, !4, !5, !6, !7}
!llvm.ident = !{!8}

!0 = distinct !DICompileUnit(language: DW_LANG_C_plus_plus_14, file: !1, producer: "clang version 22.1.0 (https://github.com/llvm/llvm-project 4434dabb69916856b824f68a64b029c67175e532)", isOptimized: false, runtimeVersion: 0, emissionKind: NoDebug, splitDebugInlining: false, nameTableKind: None)
!1 = !DIFile(filename: "intrinsics\\intrinsics.cpp", directory: "C:\\Users\\luis\\Desktop\\git\\logia")
!2 = !{!"/FAILIFMISMATCH:\22_CRT_STDIO_ISO_WIDE_SPECIFIERS=0\22"}
!3 = !{i32 2, !"Debug Info Version", i32 3}
!4 = !{i32 1, !"wchar_size", i32 2}
!5 = !{i32 8, !"PIC Level", i32 2}
!6 = !{i32 7, !"uwtable", i32 2}
!7 = !{i32 1, !"MaxTLSAlign", i32 65536}
!8 = !{!"clang version 22.1.0 (https://github.com/llvm/llvm-project 4434dabb69916856b824f68a64b029c67175e532)"}
