; ModuleID = '.\intrinsics\intrinsics.cpp'
source_filename = ".\\intrinsics\\intrinsics.cpp"
target datalayout = "e-m:w-p270:32:32-p271:32:32-p272:64:64-i64:64-i128:128-f80:128-n8:16:32:64-S128"
target triple = "x86_64-pc-windows-msvc19.44.35221"

@.str = private unnamed_addr constant [9 x i8] c"logia=i8\00", section "llvm.metadata"
@.str.1 = private unnamed_addr constant [28 x i8] c".\\intrinsics\\intrinsics.cpp\00", section "llvm.metadata"
@.str.2 = private unnamed_addr constant [10 x i8] c"logia=i16\00", section "llvm.metadata"
@.str.3 = private unnamed_addr constant [10 x i8] c"logia=i32\00", section "llvm.metadata"
@.str.4 = private unnamed_addr constant [10 x i8] c"logia=i64\00", section "llvm.metadata"
@.str.5 = private unnamed_addr constant [9 x i8] c"logia=u8\00", section "llvm.metadata"
@.str.6 = private unnamed_addr constant [10 x i8] c"logia=u16\00", section "llvm.metadata"
@.str.7 = private unnamed_addr constant [10 x i8] c"logia=u32\00", section "llvm.metadata"
@.str.8 = private unnamed_addr constant [10 x i8] c"logia=u64\00", section "llvm.metadata"
@.str.9 = private unnamed_addr constant [22 x i8] c"logia=__logia_bin_add\00", section "llvm.metadata"
@.str.10 = private unnamed_addr constant [22 x i8] c"logia=__logia_bin_sub\00", section "llvm.metadata"
@.str.11 = private unnamed_addr constant [22 x i8] c"logia=__logia_bin_mul\00", section "llvm.metadata"
@.str.12 = private unnamed_addr constant [22 x i8] c"logia=__logia_bin_div\00", section "llvm.metadata"
@.str.13 = private unnamed_addr constant [22 x i8] c"logia=__logia_bin_mod\00", section "llvm.metadata"
@.str.14 = private unnamed_addr constant [30 x i8] c"logia=__logia_bin_bitwise_and\00", section "llvm.metadata"
@.str.15 = private unnamed_addr constant [29 x i8] c"logia=__logia_bin_bitwise_or\00", section "llvm.metadata"
@.str.16 = private unnamed_addr constant [30 x i8] c"logia=__logia_bin_bitwise_xor\00", section "llvm.metadata"
@.str.17 = private unnamed_addr constant [30 x i8] c"logia=__logia_bin_bitwise_shl\00", section "llvm.metadata"
@.str.18 = private unnamed_addr constant [30 x i8] c"logia=__logia_bin_bitwise_shr\00", section "llvm.metadata"
@llvm.global.annotations = appending global [80 x { ptr, ptr, ptr, i32, ptr }] [{ ptr, ptr, ptr, i32, ptr } { ptr @logia__intrinsics_bin_add_i8_i8, ptr @.str.9, ptr @.str.1, i32 71, ptr null }, { ptr, ptr, ptr, i32, ptr } { ptr @logia_intrinsics_bin_sub_i8_i8, ptr @.str.10, ptr @.str.1, i32 77, ptr null }, { ptr, ptr, ptr, i32, ptr } { ptr @logia_intrinsics_bin_mul_i8_i8, ptr @.str.11, ptr @.str.1, i32 83, ptr null }, { ptr, ptr, ptr, i32, ptr } { ptr @logia_intrinsics_bin_div_i8_i8, ptr @.str.12, ptr @.str.1, i32 89, ptr null }, { ptr, ptr, ptr, i32, ptr } { ptr @logia_intrinsics_bin_mod_i8_i8, ptr @.str.13, ptr @.str.1, i32 95, ptr null }, { ptr, ptr, ptr, i32, ptr } { ptr @logia_intrinsics_bin_bitwise_and_i8_i8, ptr @.str.14, ptr @.str.1, i32 197, ptr null }, { ptr, ptr, ptr, i32, ptr } { ptr @logia_intrinsics_bin_bitwise_or_i8_i8, ptr @.str.15, ptr @.str.1, i32 203, ptr null }, { ptr, ptr, ptr, i32, ptr } { ptr @logia_intrinsics_bin_bitwise_xor_i8_i8, ptr @.str.16, ptr @.str.1, i32 209, ptr null }, { ptr, ptr, ptr, i32, ptr } { ptr @logia_intrinsics_bin_bitwise_shl_i8_i8, ptr @.str.17, ptr @.str.1, i32 215, ptr null }, { ptr, ptr, ptr, i32, ptr } { ptr @logia_intrinsics_bin_bitwise_shr_i8_i8, ptr @.str.18, ptr @.str.1, i32 221, ptr null }, { ptr, ptr, ptr, i32, ptr } { ptr @logia__intrinsics_bin_add_i16_i16, ptr @.str.9, ptr @.str.1, i32 227, ptr null }, { ptr, ptr, ptr, i32, ptr } { ptr @logia_intrinsics_bin_sub_i16_i16, ptr @.str.10, ptr @.str.1, i32 233, ptr null }, { ptr, ptr, ptr, i32, ptr } { ptr @logia_intrinsics_bin_mul_i16_i16, ptr @.str.11, ptr @.str.1, i32 239, ptr null }, { ptr, ptr, ptr, i32, ptr } { ptr @logia_intrinsics_bin_div_i16_i16, ptr @.str.12, ptr @.str.1, i32 245, ptr null }, { ptr, ptr, ptr, i32, ptr } { ptr @logia_intrinsics_bin_mod_i16_i16, ptr @.str.13, ptr @.str.1, i32 251, ptr null }, { ptr, ptr, ptr, i32, ptr } { ptr @logia_intrinsics_bin_bitwise_and_i16_i16, ptr @.str.14, ptr @.str.1, i32 353, ptr null }, { ptr, ptr, ptr, i32, ptr } { ptr @logia_intrinsics_bin_bitwise_or_i16_i16, ptr @.str.15, ptr @.str.1, i32 359, ptr null }, { ptr, ptr, ptr, i32, ptr } { ptr @logia_intrinsics_bin_bitwise_xor_i16_i16, ptr @.str.16, ptr @.str.1, i32 365, ptr null }, { ptr, ptr, ptr, i32, ptr } { ptr @logia_intrinsics_bin_bitwise_shl_i16_i16, ptr @.str.17, ptr @.str.1, i32 371, ptr null }, { ptr, ptr, ptr, i32, ptr } { ptr @logia_intrinsics_bin_bitwise_shr_i16_i16, ptr @.str.18, ptr @.str.1, i32 377, ptr null }, { ptr, ptr, ptr, i32, ptr } { ptr @logia__intrinsics_bin_add_i32_i32, ptr @.str.9, ptr @.str.1, i32 383, ptr null }, { ptr, ptr, ptr, i32, ptr } { ptr @logia_intrinsics_bin_sub_i32_i32, ptr @.str.10, ptr @.str.1, i32 389, ptr null }, { ptr, ptr, ptr, i32, ptr } { ptr @logia_intrinsics_bin_mul_i32_i32, ptr @.str.11, ptr @.str.1, i32 395, ptr null }, { ptr, ptr, ptr, i32, ptr } { ptr @logia_intrinsics_bin_div_i32_i32, ptr @.str.12, ptr @.str.1, i32 401, ptr null }, { ptr, ptr, ptr, i32, ptr } { ptr @logia_intrinsics_bin_mod_i32_i32, ptr @.str.13, ptr @.str.1, i32 407, ptr null }, { ptr, ptr, ptr, i32, ptr } { ptr @logia_intrinsics_bin_bitwise_and_i32_i32, ptr @.str.14, ptr @.str.1, i32 509, ptr null }, { ptr, ptr, ptr, i32, ptr } { ptr @logia_intrinsics_bin_bitwise_or_i32_i32, ptr @.str.15, ptr @.str.1, i32 515, ptr null }, { ptr, ptr, ptr, i32, ptr } { ptr @logia_intrinsics_bin_bitwise_xor_i32_i32, ptr @.str.16, ptr @.str.1, i32 521, ptr null }, { ptr, ptr, ptr, i32, ptr } { ptr @logia_intrinsics_bin_bitwise_shl_i32_i32, ptr @.str.17, ptr @.str.1, i32 527, ptr null }, { ptr, ptr, ptr, i32, ptr } { ptr @logia_intrinsics_bin_bitwise_shr_i32_i32, ptr @.str.18, ptr @.str.1, i32 533, ptr null }, { ptr, ptr, ptr, i32, ptr } { ptr @logia__intrinsics_bin_add_i64_i64, ptr @.str.9, ptr @.str.1, i32 539, ptr null }, { ptr, ptr, ptr, i32, ptr } { ptr @logia_intrinsics_bin_sub_i64_i64, ptr @.str.10, ptr @.str.1, i32 545, ptr null }, { ptr, ptr, ptr, i32, ptr } { ptr @logia_intrinsics_bin_mul_i64_i64, ptr @.str.11, ptr @.str.1, i32 551, ptr null }, { ptr, ptr, ptr, i32, ptr } { ptr @logia_intrinsics_bin_div_i64_i64, ptr @.str.12, ptr @.str.1, i32 557, ptr null }, { ptr, ptr, ptr, i32, ptr } { ptr @logia_intrinsics_bin_mod_i64_i64, ptr @.str.13, ptr @.str.1, i32 563, ptr null }, { ptr, ptr, ptr, i32, ptr } { ptr @logia_intrinsics_bin_bitwise_and_i64_i64, ptr @.str.14, ptr @.str.1, i32 665, ptr null }, { ptr, ptr, ptr, i32, ptr } { ptr @logia_intrinsics_bin_bitwise_or_i64_i64, ptr @.str.15, ptr @.str.1, i32 671, ptr null }, { ptr, ptr, ptr, i32, ptr } { ptr @logia_intrinsics_bin_bitwise_xor_i64_i64, ptr @.str.16, ptr @.str.1, i32 677, ptr null }, { ptr, ptr, ptr, i32, ptr } { ptr @logia_intrinsics_bin_bitwise_shl_i64_i64, ptr @.str.17, ptr @.str.1, i32 683, ptr null }, { ptr, ptr, ptr, i32, ptr } { ptr @logia_intrinsics_bin_bitwise_shr_i64_i64, ptr @.str.18, ptr @.str.1, i32 689, ptr null }, { ptr, ptr, ptr, i32, ptr } { ptr @logia__intrinsics_bin_add_u8_u8, ptr @.str.9, ptr @.str.1, i32 695, ptr null }, { ptr, ptr, ptr, i32, ptr } { ptr @logia_intrinsics_bin_sub_u8_u8, ptr @.str.10, ptr @.str.1, i32 701, ptr null }, { ptr, ptr, ptr, i32, ptr } { ptr @logia_intrinsics_bin_mul_u8_u8, ptr @.str.11, ptr @.str.1, i32 707, ptr null }, { ptr, ptr, ptr, i32, ptr } { ptr @logia_intrinsics_bin_div_u8_u8, ptr @.str.12, ptr @.str.1, i32 713, ptr null }, { ptr, ptr, ptr, i32, ptr } { ptr @logia_intrinsics_bin_mod_u8_u8, ptr @.str.13, ptr @.str.1, i32 719, ptr null }, { ptr, ptr, ptr, i32, ptr } { ptr @logia_intrinsics_bin_bitwise_and_u8_u8, ptr @.str.14, ptr @.str.1, i32 821, ptr null }, { ptr, ptr, ptr, i32, ptr } { ptr @logia_intrinsics_bin_bitwise_or_u8_u8, ptr @.str.15, ptr @.str.1, i32 827, ptr null }, { ptr, ptr, ptr, i32, ptr } { ptr @logia_intrinsics_bin_bitwise_xor_u8_u8, ptr @.str.16, ptr @.str.1, i32 833, ptr null }, { ptr, ptr, ptr, i32, ptr } { ptr @logia_intrinsics_bin_bitwise_shl_u8_u8, ptr @.str.17, ptr @.str.1, i32 839, ptr null }, { ptr, ptr, ptr, i32, ptr } { ptr @logia_intrinsics_bin_bitwise_shr_u8_u8, ptr @.str.18, ptr @.str.1, i32 845, ptr null }, { ptr, ptr, ptr, i32, ptr } { ptr @logia__intrinsics_bin_add_u16_u16, ptr @.str.9, ptr @.str.1, i32 851, ptr null }, { ptr, ptr, ptr, i32, ptr } { ptr @logia_intrinsics_bin_sub_u16_u16, ptr @.str.10, ptr @.str.1, i32 857, ptr null }, { ptr, ptr, ptr, i32, ptr } { ptr @logia_intrinsics_bin_mul_u16_u16, ptr @.str.11, ptr @.str.1, i32 863, ptr null }, { ptr, ptr, ptr, i32, ptr } { ptr @logia_intrinsics_bin_div_u16_u16, ptr @.str.12, ptr @.str.1, i32 869, ptr null }, { ptr, ptr, ptr, i32, ptr } { ptr @logia_intrinsics_bin_mod_u16_u16, ptr @.str.13, ptr @.str.1, i32 875, ptr null }, { ptr, ptr, ptr, i32, ptr } { ptr @logia_intrinsics_bin_bitwise_and_u16_u16, ptr @.str.14, ptr @.str.1, i32 977, ptr null }, { ptr, ptr, ptr, i32, ptr } { ptr @logia_intrinsics_bin_bitwise_or_u16_u16, ptr @.str.15, ptr @.str.1, i32 983, ptr null }, { ptr, ptr, ptr, i32, ptr } { ptr @logia_intrinsics_bin_bitwise_xor_u16_u16, ptr @.str.16, ptr @.str.1, i32 989, ptr null }, { ptr, ptr, ptr, i32, ptr } { ptr @logia_intrinsics_bin_bitwise_shl_u16_u16, ptr @.str.17, ptr @.str.1, i32 995, ptr null }, { ptr, ptr, ptr, i32, ptr } { ptr @logia_intrinsics_bin_bitwise_shr_u16_u16, ptr @.str.18, ptr @.str.1, i32 1001, ptr null }, { ptr, ptr, ptr, i32, ptr } { ptr @logia__intrinsics_bin_add_u32_u32, ptr @.str.9, ptr @.str.1, i32 1007, ptr null }, { ptr, ptr, ptr, i32, ptr } { ptr @logia_intrinsics_bin_sub_u32_u32, ptr @.str.10, ptr @.str.1, i32 1013, ptr null }, { ptr, ptr, ptr, i32, ptr } { ptr @logia_intrinsics_bin_mul_u32_u32, ptr @.str.11, ptr @.str.1, i32 1019, ptr null }, { ptr, ptr, ptr, i32, ptr } { ptr @logia_intrinsics_bin_div_u32_u32, ptr @.str.12, ptr @.str.1, i32 1025, ptr null }, { ptr, ptr, ptr, i32, ptr } { ptr @logia_intrinsics_bin_mod_u32_u32, ptr @.str.13, ptr @.str.1, i32 1031, ptr null }, { ptr, ptr, ptr, i32, ptr } { ptr @logia_intrinsics_bin_bitwise_and_u32_u32, ptr @.str.14, ptr @.str.1, i32 1133, ptr null }, { ptr, ptr, ptr, i32, ptr } { ptr @logia_intrinsics_bin_bitwise_or_u32_u32, ptr @.str.15, ptr @.str.1, i32 1139, ptr null }, { ptr, ptr, ptr, i32, ptr } { ptr @logia_intrinsics_bin_bitwise_xor_u32_u32, ptr @.str.16, ptr @.str.1, i32 1145, ptr null }, { ptr, ptr, ptr, i32, ptr } { ptr @logia_intrinsics_bin_bitwise_shl_u32_u32, ptr @.str.17, ptr @.str.1, i32 1151, ptr null }, { ptr, ptr, ptr, i32, ptr } { ptr @logia_intrinsics_bin_bitwise_shr_u32_u32, ptr @.str.18, ptr @.str.1, i32 1157, ptr null }, { ptr, ptr, ptr, i32, ptr } { ptr @logia__intrinsics_bin_add_u64_u64, ptr @.str.9, ptr @.str.1, i32 1163, ptr null }, { ptr, ptr, ptr, i32, ptr } { ptr @logia_intrinsics_bin_sub_u64_u64, ptr @.str.10, ptr @.str.1, i32 1169, ptr null }, { ptr, ptr, ptr, i32, ptr } { ptr @logia_intrinsics_bin_mul_u64_u64, ptr @.str.11, ptr @.str.1, i32 1175, ptr null }, { ptr, ptr, ptr, i32, ptr } { ptr @logia_intrinsics_bin_div_u64_u64, ptr @.str.12, ptr @.str.1, i32 1181, ptr null }, { ptr, ptr, ptr, i32, ptr } { ptr @logia_intrinsics_bin_mod_u64_u64, ptr @.str.13, ptr @.str.1, i32 1187, ptr null }, { ptr, ptr, ptr, i32, ptr } { ptr @logia_intrinsics_bin_bitwise_and_u64_u64, ptr @.str.14, ptr @.str.1, i32 1289, ptr null }, { ptr, ptr, ptr, i32, ptr } { ptr @logia_intrinsics_bin_bitwise_or_u64_u64, ptr @.str.15, ptr @.str.1, i32 1295, ptr null }, { ptr, ptr, ptr, i32, ptr } { ptr @logia_intrinsics_bin_bitwise_xor_u64_u64, ptr @.str.16, ptr @.str.1, i32 1301, ptr null }, { ptr, ptr, ptr, i32, ptr } { ptr @logia_intrinsics_bin_bitwise_shl_u64_u64, ptr @.str.17, ptr @.str.1, i32 1307, ptr null }, { ptr, ptr, ptr, i32, ptr } { ptr @logia_intrinsics_bin_bitwise_shr_u64_u64, ptr @.str.18, ptr @.str.1, i32 1313, ptr null }], section "llvm.metadata"

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
define dso_local i64 @logia_intrinsics_prefix_neg_i64(i64 noundef %0) #2 {
  %2 = alloca i64, align 8
  store i64 %0, ptr %2, align 8
  %3 = load i64, ptr %2, align 8
  %4 = sub nsw i64 0, %3
  ret i64 %4
}

; Function Attrs: mustprogress noinline nounwind optnone uwtable
define dso_local zeroext i1 @logia_intrinsics_prefix_logical_not_i64(i64 noundef %0) #2 {
  %2 = alloca i64, align 8
  store i64 %0, ptr %2, align 8
  %3 = load i64, ptr %2, align 8
  %4 = icmp ne i64 %3, 0
  %5 = xor i1 %4, true
  ret i1 %5
}

; Function Attrs: mustprogress noinline nounwind optnone uwtable
define dso_local i64 @logia_intrinsics_prefix_bitwise_not_i64(i64 noundef %0) #2 {
  %2 = alloca i64, align 8
  store i64 %0, ptr %2, align 8
  %3 = load i64, ptr %2, align 8
  %4 = xor i64 %3, -1
  ret i64 %4
}

; Function Attrs: mustprogress noinline nounwind optnone uwtable
define dso_local i8 @logia__intrinsics_bin_add_i8_i8(i8 noundef %0, i8 noundef %1) #2 {
  %3 = alloca i8, align 1
  %4 = alloca i8, align 1
  store i8 %1, ptr %3, align 1
  call void @llvm.var.annotation.p0.p0(ptr %3, ptr @.str, ptr @.str.1, i32 71, ptr null)
  store i8 %0, ptr %4, align 1
  call void @llvm.var.annotation.p0.p0(ptr %4, ptr @.str, ptr @.str.1, i32 71, ptr null)
  %5 = load i8, ptr %4, align 1
  %6 = sext i8 %5 to i32
  %7 = load i8, ptr %3, align 1
  %8 = sext i8 %7 to i32
  %9 = add nsw i32 %6, %8
  %10 = trunc i32 %9 to i8
  ret i8 %10
}

; Function Attrs: nocallback nofree nosync nounwind willreturn memory(inaccessiblemem: readwrite)
declare void @llvm.var.annotation.p0.p0(ptr, ptr, ptr, i32, ptr) #3

; Function Attrs: mustprogress noinline nounwind optnone uwtable
define dso_local i8 @logia_intrinsics_bin_sub_i8_i8(i8 noundef %0, i8 noundef %1) #2 {
  %3 = alloca i8, align 1
  %4 = alloca i8, align 1
  store i8 %1, ptr %3, align 1
  call void @llvm.var.annotation.p0.p0(ptr %3, ptr @.str, ptr @.str.1, i32 77, ptr null)
  store i8 %0, ptr %4, align 1
  call void @llvm.var.annotation.p0.p0(ptr %4, ptr @.str, ptr @.str.1, i32 77, ptr null)
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
  call void @llvm.var.annotation.p0.p0(ptr %3, ptr @.str, ptr @.str.1, i32 83, ptr null)
  store i8 %0, ptr %4, align 1
  call void @llvm.var.annotation.p0.p0(ptr %4, ptr @.str, ptr @.str.1, i32 83, ptr null)
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
  call void @llvm.var.annotation.p0.p0(ptr %3, ptr @.str, ptr @.str.1, i32 89, ptr null)
  store i8 %0, ptr %4, align 1
  call void @llvm.var.annotation.p0.p0(ptr %4, ptr @.str, ptr @.str.1, i32 89, ptr null)
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
  call void @llvm.var.annotation.p0.p0(ptr %3, ptr @.str, ptr @.str.1, i32 95, ptr null)
  store i8 %0, ptr %4, align 1
  call void @llvm.var.annotation.p0.p0(ptr %4, ptr @.str, ptr @.str.1, i32 95, ptr null)
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
  store i8 %0, ptr %4, align 1
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
  store i8 %0, ptr %4, align 1
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
  store i8 %0, ptr %4, align 1
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
  store i8 %0, ptr %4, align 1
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
  store i8 %0, ptr %4, align 1
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
  store i8 %0, ptr %4, align 1
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
  store ptr %0, ptr %4, align 8
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
  store ptr %0, ptr %4, align 8
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
  store ptr %0, ptr %4, align 8
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
  store ptr %0, ptr %4, align 8
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
  store ptr %0, ptr %4, align 8
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
  store ptr %0, ptr %4, align 8
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
  store ptr %0, ptr %4, align 8
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
  store ptr %0, ptr %4, align 8
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
  store ptr %0, ptr %4, align 8
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
  store ptr %0, ptr %4, align 8
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
  store ptr %0, ptr %4, align 8
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
  call void @llvm.var.annotation.p0.p0(ptr %3, ptr @.str, ptr @.str.1, i32 197, ptr null)
  store i8 %0, ptr %4, align 1
  call void @llvm.var.annotation.p0.p0(ptr %4, ptr @.str, ptr @.str.1, i32 197, ptr null)
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
  call void @llvm.var.annotation.p0.p0(ptr %3, ptr @.str, ptr @.str.1, i32 203, ptr null)
  store i8 %0, ptr %4, align 1
  call void @llvm.var.annotation.p0.p0(ptr %4, ptr @.str, ptr @.str.1, i32 203, ptr null)
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
  call void @llvm.var.annotation.p0.p0(ptr %3, ptr @.str, ptr @.str.1, i32 209, ptr null)
  store i8 %0, ptr %4, align 1
  call void @llvm.var.annotation.p0.p0(ptr %4, ptr @.str, ptr @.str.1, i32 209, ptr null)
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
  call void @llvm.var.annotation.p0.p0(ptr %3, ptr @.str, ptr @.str.1, i32 215, ptr null)
  store i8 %0, ptr %4, align 1
  call void @llvm.var.annotation.p0.p0(ptr %4, ptr @.str, ptr @.str.1, i32 215, ptr null)
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
  call void @llvm.var.annotation.p0.p0(ptr %3, ptr @.str, ptr @.str.1, i32 221, ptr null)
  store i8 %0, ptr %4, align 1
  call void @llvm.var.annotation.p0.p0(ptr %4, ptr @.str, ptr @.str.1, i32 221, ptr null)
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
  call void @llvm.var.annotation.p0.p0(ptr %3, ptr @.str.2, ptr @.str.1, i32 227, ptr null)
  store i16 %0, ptr %4, align 2
  call void @llvm.var.annotation.p0.p0(ptr %4, ptr @.str.2, ptr @.str.1, i32 227, ptr null)
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
  call void @llvm.var.annotation.p0.p0(ptr %3, ptr @.str.2, ptr @.str.1, i32 233, ptr null)
  store i16 %0, ptr %4, align 2
  call void @llvm.var.annotation.p0.p0(ptr %4, ptr @.str.2, ptr @.str.1, i32 233, ptr null)
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
  call void @llvm.var.annotation.p0.p0(ptr %3, ptr @.str.2, ptr @.str.1, i32 239, ptr null)
  store i16 %0, ptr %4, align 2
  call void @llvm.var.annotation.p0.p0(ptr %4, ptr @.str.2, ptr @.str.1, i32 239, ptr null)
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
  call void @llvm.var.annotation.p0.p0(ptr %3, ptr @.str.2, ptr @.str.1, i32 245, ptr null)
  store i16 %0, ptr %4, align 2
  call void @llvm.var.annotation.p0.p0(ptr %4, ptr @.str.2, ptr @.str.1, i32 245, ptr null)
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
  call void @llvm.var.annotation.p0.p0(ptr %3, ptr @.str.2, ptr @.str.1, i32 251, ptr null)
  store i16 %0, ptr %4, align 2
  call void @llvm.var.annotation.p0.p0(ptr %4, ptr @.str.2, ptr @.str.1, i32 251, ptr null)
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
  store i16 %0, ptr %4, align 2
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
  store i16 %0, ptr %4, align 2
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
  store i16 %0, ptr %4, align 2
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
  store i16 %0, ptr %4, align 2
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
  store i16 %0, ptr %4, align 2
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
  store i16 %0, ptr %4, align 2
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
  store ptr %0, ptr %4, align 8
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
  store ptr %0, ptr %4, align 8
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
  store ptr %0, ptr %4, align 8
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
  store ptr %0, ptr %4, align 8
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
  store ptr %0, ptr %4, align 8
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
  store ptr %0, ptr %4, align 8
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
  store ptr %0, ptr %4, align 8
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
  store ptr %0, ptr %4, align 8
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
  store ptr %0, ptr %4, align 8
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
  store ptr %0, ptr %4, align 8
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
  store ptr %0, ptr %4, align 8
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
  call void @llvm.var.annotation.p0.p0(ptr %3, ptr @.str.2, ptr @.str.1, i32 353, ptr null)
  store i16 %0, ptr %4, align 2
  call void @llvm.var.annotation.p0.p0(ptr %4, ptr @.str.2, ptr @.str.1, i32 353, ptr null)
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
  call void @llvm.var.annotation.p0.p0(ptr %3, ptr @.str.2, ptr @.str.1, i32 359, ptr null)
  store i16 %0, ptr %4, align 2
  call void @llvm.var.annotation.p0.p0(ptr %4, ptr @.str.2, ptr @.str.1, i32 359, ptr null)
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
  call void @llvm.var.annotation.p0.p0(ptr %3, ptr @.str.2, ptr @.str.1, i32 365, ptr null)
  store i16 %0, ptr %4, align 2
  call void @llvm.var.annotation.p0.p0(ptr %4, ptr @.str.2, ptr @.str.1, i32 365, ptr null)
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
  call void @llvm.var.annotation.p0.p0(ptr %3, ptr @.str.2, ptr @.str.1, i32 371, ptr null)
  store i16 %0, ptr %4, align 2
  call void @llvm.var.annotation.p0.p0(ptr %4, ptr @.str.2, ptr @.str.1, i32 371, ptr null)
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
  call void @llvm.var.annotation.p0.p0(ptr %3, ptr @.str.2, ptr @.str.1, i32 377, ptr null)
  store i16 %0, ptr %4, align 2
  call void @llvm.var.annotation.p0.p0(ptr %4, ptr @.str.2, ptr @.str.1, i32 377, ptr null)
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
  call void @llvm.var.annotation.p0.p0(ptr %3, ptr @.str.3, ptr @.str.1, i32 383, ptr null)
  store i32 %0, ptr %4, align 4
  call void @llvm.var.annotation.p0.p0(ptr %4, ptr @.str.3, ptr @.str.1, i32 383, ptr null)
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
  call void @llvm.var.annotation.p0.p0(ptr %3, ptr @.str.3, ptr @.str.1, i32 389, ptr null)
  store i32 %0, ptr %4, align 4
  call void @llvm.var.annotation.p0.p0(ptr %4, ptr @.str.3, ptr @.str.1, i32 389, ptr null)
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
  call void @llvm.var.annotation.p0.p0(ptr %3, ptr @.str.3, ptr @.str.1, i32 395, ptr null)
  store i32 %0, ptr %4, align 4
  call void @llvm.var.annotation.p0.p0(ptr %4, ptr @.str.3, ptr @.str.1, i32 395, ptr null)
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
  call void @llvm.var.annotation.p0.p0(ptr %3, ptr @.str.3, ptr @.str.1, i32 401, ptr null)
  store i32 %0, ptr %4, align 4
  call void @llvm.var.annotation.p0.p0(ptr %4, ptr @.str.3, ptr @.str.1, i32 401, ptr null)
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
  call void @llvm.var.annotation.p0.p0(ptr %3, ptr @.str.3, ptr @.str.1, i32 407, ptr null)
  store i32 %0, ptr %4, align 4
  call void @llvm.var.annotation.p0.p0(ptr %4, ptr @.str.3, ptr @.str.1, i32 407, ptr null)
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
  store i32 %0, ptr %4, align 4
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
  store i32 %0, ptr %4, align 4
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
  store i32 %0, ptr %4, align 4
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
  store i32 %0, ptr %4, align 4
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
  store i32 %0, ptr %4, align 4
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
  store i32 %0, ptr %4, align 4
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
  store ptr %0, ptr %4, align 8
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
  store ptr %0, ptr %4, align 8
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
  store ptr %0, ptr %4, align 8
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
  store ptr %0, ptr %4, align 8
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
  store ptr %0, ptr %4, align 8
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
  store ptr %0, ptr %4, align 8
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
  store ptr %0, ptr %4, align 8
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
  store ptr %0, ptr %4, align 8
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
  store ptr %0, ptr %4, align 8
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
  store ptr %0, ptr %4, align 8
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
  store ptr %0, ptr %4, align 8
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
  call void @llvm.var.annotation.p0.p0(ptr %3, ptr @.str.3, ptr @.str.1, i32 509, ptr null)
  store i32 %0, ptr %4, align 4
  call void @llvm.var.annotation.p0.p0(ptr %4, ptr @.str.3, ptr @.str.1, i32 509, ptr null)
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
  call void @llvm.var.annotation.p0.p0(ptr %3, ptr @.str.3, ptr @.str.1, i32 515, ptr null)
  store i32 %0, ptr %4, align 4
  call void @llvm.var.annotation.p0.p0(ptr %4, ptr @.str.3, ptr @.str.1, i32 515, ptr null)
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
  call void @llvm.var.annotation.p0.p0(ptr %3, ptr @.str.3, ptr @.str.1, i32 521, ptr null)
  store i32 %0, ptr %4, align 4
  call void @llvm.var.annotation.p0.p0(ptr %4, ptr @.str.3, ptr @.str.1, i32 521, ptr null)
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
  call void @llvm.var.annotation.p0.p0(ptr %3, ptr @.str.3, ptr @.str.1, i32 527, ptr null)
  store i32 %0, ptr %4, align 4
  call void @llvm.var.annotation.p0.p0(ptr %4, ptr @.str.3, ptr @.str.1, i32 527, ptr null)
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
  call void @llvm.var.annotation.p0.p0(ptr %3, ptr @.str.3, ptr @.str.1, i32 533, ptr null)
  store i32 %0, ptr %4, align 4
  call void @llvm.var.annotation.p0.p0(ptr %4, ptr @.str.3, ptr @.str.1, i32 533, ptr null)
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
  call void @llvm.var.annotation.p0.p0(ptr %3, ptr @.str.4, ptr @.str.1, i32 539, ptr null)
  store i64 %0, ptr %4, align 8
  call void @llvm.var.annotation.p0.p0(ptr %4, ptr @.str.4, ptr @.str.1, i32 539, ptr null)
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
  call void @llvm.var.annotation.p0.p0(ptr %3, ptr @.str.4, ptr @.str.1, i32 545, ptr null)
  store i64 %0, ptr %4, align 8
  call void @llvm.var.annotation.p0.p0(ptr %4, ptr @.str.4, ptr @.str.1, i32 545, ptr null)
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
  call void @llvm.var.annotation.p0.p0(ptr %3, ptr @.str.4, ptr @.str.1, i32 551, ptr null)
  store i64 %0, ptr %4, align 8
  call void @llvm.var.annotation.p0.p0(ptr %4, ptr @.str.4, ptr @.str.1, i32 551, ptr null)
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
  call void @llvm.var.annotation.p0.p0(ptr %3, ptr @.str.4, ptr @.str.1, i32 557, ptr null)
  store i64 %0, ptr %4, align 8
  call void @llvm.var.annotation.p0.p0(ptr %4, ptr @.str.4, ptr @.str.1, i32 557, ptr null)
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
  call void @llvm.var.annotation.p0.p0(ptr %3, ptr @.str.4, ptr @.str.1, i32 563, ptr null)
  store i64 %0, ptr %4, align 8
  call void @llvm.var.annotation.p0.p0(ptr %4, ptr @.str.4, ptr @.str.1, i32 563, ptr null)
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
  store i64 %0, ptr %4, align 8
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
  store i64 %0, ptr %4, align 8
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
  store i64 %0, ptr %4, align 8
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
  store i64 %0, ptr %4, align 8
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
  store i64 %0, ptr %4, align 8
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
  store i64 %0, ptr %4, align 8
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
  store ptr %0, ptr %4, align 8
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
  store ptr %0, ptr %4, align 8
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
  store ptr %0, ptr %4, align 8
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
  store ptr %0, ptr %4, align 8
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
  store ptr %0, ptr %4, align 8
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
  store ptr %0, ptr %4, align 8
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
  store ptr %0, ptr %4, align 8
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
  store ptr %0, ptr %4, align 8
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
  store ptr %0, ptr %4, align 8
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
  store ptr %0, ptr %4, align 8
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
  store ptr %0, ptr %4, align 8
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
  call void @llvm.var.annotation.p0.p0(ptr %3, ptr @.str.4, ptr @.str.1, i32 665, ptr null)
  store i64 %0, ptr %4, align 8
  call void @llvm.var.annotation.p0.p0(ptr %4, ptr @.str.4, ptr @.str.1, i32 665, ptr null)
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
  call void @llvm.var.annotation.p0.p0(ptr %3, ptr @.str.4, ptr @.str.1, i32 671, ptr null)
  store i64 %0, ptr %4, align 8
  call void @llvm.var.annotation.p0.p0(ptr %4, ptr @.str.4, ptr @.str.1, i32 671, ptr null)
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
  call void @llvm.var.annotation.p0.p0(ptr %3, ptr @.str.4, ptr @.str.1, i32 677, ptr null)
  store i64 %0, ptr %4, align 8
  call void @llvm.var.annotation.p0.p0(ptr %4, ptr @.str.4, ptr @.str.1, i32 677, ptr null)
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
  call void @llvm.var.annotation.p0.p0(ptr %3, ptr @.str.4, ptr @.str.1, i32 683, ptr null)
  store i64 %0, ptr %4, align 8
  call void @llvm.var.annotation.p0.p0(ptr %4, ptr @.str.4, ptr @.str.1, i32 683, ptr null)
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
  call void @llvm.var.annotation.p0.p0(ptr %3, ptr @.str.4, ptr @.str.1, i32 689, ptr null)
  store i64 %0, ptr %4, align 8
  call void @llvm.var.annotation.p0.p0(ptr %4, ptr @.str.4, ptr @.str.1, i32 689, ptr null)
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
  call void @llvm.var.annotation.p0.p0(ptr %3, ptr @.str.5, ptr @.str.1, i32 695, ptr null)
  store i8 %0, ptr %4, align 1
  call void @llvm.var.annotation.p0.p0(ptr %4, ptr @.str.5, ptr @.str.1, i32 695, ptr null)
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
  call void @llvm.var.annotation.p0.p0(ptr %3, ptr @.str.5, ptr @.str.1, i32 701, ptr null)
  store i8 %0, ptr %4, align 1
  call void @llvm.var.annotation.p0.p0(ptr %4, ptr @.str.5, ptr @.str.1, i32 701, ptr null)
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
  call void @llvm.var.annotation.p0.p0(ptr %3, ptr @.str.5, ptr @.str.1, i32 707, ptr null)
  store i8 %0, ptr %4, align 1
  call void @llvm.var.annotation.p0.p0(ptr %4, ptr @.str.5, ptr @.str.1, i32 707, ptr null)
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
  call void @llvm.var.annotation.p0.p0(ptr %3, ptr @.str.5, ptr @.str.1, i32 713, ptr null)
  store i8 %0, ptr %4, align 1
  call void @llvm.var.annotation.p0.p0(ptr %4, ptr @.str.5, ptr @.str.1, i32 713, ptr null)
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
  call void @llvm.var.annotation.p0.p0(ptr %3, ptr @.str.5, ptr @.str.1, i32 719, ptr null)
  store i8 %0, ptr %4, align 1
  call void @llvm.var.annotation.p0.p0(ptr %4, ptr @.str.5, ptr @.str.1, i32 719, ptr null)
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
  store i8 %0, ptr %4, align 1
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
  store i8 %0, ptr %4, align 1
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
  store i8 %0, ptr %4, align 1
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
  store i8 %0, ptr %4, align 1
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
  store i8 %0, ptr %4, align 1
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
  store i8 %0, ptr %4, align 1
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
  store ptr %0, ptr %4, align 8
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
  store ptr %0, ptr %4, align 8
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
  store ptr %0, ptr %4, align 8
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
  store ptr %0, ptr %4, align 8
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
  store ptr %0, ptr %4, align 8
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
  store ptr %0, ptr %4, align 8
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
  store ptr %0, ptr %4, align 8
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
  store ptr %0, ptr %4, align 8
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
  store ptr %0, ptr %4, align 8
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
  store ptr %0, ptr %4, align 8
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
  store ptr %0, ptr %4, align 8
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
  call void @llvm.var.annotation.p0.p0(ptr %3, ptr @.str.5, ptr @.str.1, i32 821, ptr null)
  store i8 %0, ptr %4, align 1
  call void @llvm.var.annotation.p0.p0(ptr %4, ptr @.str.5, ptr @.str.1, i32 821, ptr null)
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
  call void @llvm.var.annotation.p0.p0(ptr %3, ptr @.str.5, ptr @.str.1, i32 827, ptr null)
  store i8 %0, ptr %4, align 1
  call void @llvm.var.annotation.p0.p0(ptr %4, ptr @.str.5, ptr @.str.1, i32 827, ptr null)
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
  call void @llvm.var.annotation.p0.p0(ptr %3, ptr @.str.5, ptr @.str.1, i32 833, ptr null)
  store i8 %0, ptr %4, align 1
  call void @llvm.var.annotation.p0.p0(ptr %4, ptr @.str.5, ptr @.str.1, i32 833, ptr null)
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
  call void @llvm.var.annotation.p0.p0(ptr %3, ptr @.str.5, ptr @.str.1, i32 839, ptr null)
  store i8 %0, ptr %4, align 1
  call void @llvm.var.annotation.p0.p0(ptr %4, ptr @.str.5, ptr @.str.1, i32 839, ptr null)
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
  call void @llvm.var.annotation.p0.p0(ptr %3, ptr @.str.5, ptr @.str.1, i32 845, ptr null)
  store i8 %0, ptr %4, align 1
  call void @llvm.var.annotation.p0.p0(ptr %4, ptr @.str.5, ptr @.str.1, i32 845, ptr null)
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
  call void @llvm.var.annotation.p0.p0(ptr %3, ptr @.str.6, ptr @.str.1, i32 851, ptr null)
  store i16 %0, ptr %4, align 2
  call void @llvm.var.annotation.p0.p0(ptr %4, ptr @.str.6, ptr @.str.1, i32 851, ptr null)
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
  call void @llvm.var.annotation.p0.p0(ptr %3, ptr @.str.6, ptr @.str.1, i32 857, ptr null)
  store i16 %0, ptr %4, align 2
  call void @llvm.var.annotation.p0.p0(ptr %4, ptr @.str.6, ptr @.str.1, i32 857, ptr null)
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
  call void @llvm.var.annotation.p0.p0(ptr %3, ptr @.str.6, ptr @.str.1, i32 863, ptr null)
  store i16 %0, ptr %4, align 2
  call void @llvm.var.annotation.p0.p0(ptr %4, ptr @.str.6, ptr @.str.1, i32 863, ptr null)
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
  call void @llvm.var.annotation.p0.p0(ptr %3, ptr @.str.6, ptr @.str.1, i32 869, ptr null)
  store i16 %0, ptr %4, align 2
  call void @llvm.var.annotation.p0.p0(ptr %4, ptr @.str.6, ptr @.str.1, i32 869, ptr null)
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
  call void @llvm.var.annotation.p0.p0(ptr %3, ptr @.str.6, ptr @.str.1, i32 875, ptr null)
  store i16 %0, ptr %4, align 2
  call void @llvm.var.annotation.p0.p0(ptr %4, ptr @.str.6, ptr @.str.1, i32 875, ptr null)
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
  store i16 %0, ptr %4, align 2
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
  store i16 %0, ptr %4, align 2
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
  store i16 %0, ptr %4, align 2
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
  store i16 %0, ptr %4, align 2
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
  store i16 %0, ptr %4, align 2
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
  store i16 %0, ptr %4, align 2
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
  store ptr %0, ptr %4, align 8
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
  store ptr %0, ptr %4, align 8
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
  store ptr %0, ptr %4, align 8
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
  store ptr %0, ptr %4, align 8
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
  store ptr %0, ptr %4, align 8
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
  store ptr %0, ptr %4, align 8
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
  store ptr %0, ptr %4, align 8
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
  store ptr %0, ptr %4, align 8
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
  store ptr %0, ptr %4, align 8
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
  store ptr %0, ptr %4, align 8
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
  store ptr %0, ptr %4, align 8
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
  call void @llvm.var.annotation.p0.p0(ptr %3, ptr @.str.6, ptr @.str.1, i32 977, ptr null)
  store i16 %0, ptr %4, align 2
  call void @llvm.var.annotation.p0.p0(ptr %4, ptr @.str.6, ptr @.str.1, i32 977, ptr null)
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
  call void @llvm.var.annotation.p0.p0(ptr %3, ptr @.str.6, ptr @.str.1, i32 983, ptr null)
  store i16 %0, ptr %4, align 2
  call void @llvm.var.annotation.p0.p0(ptr %4, ptr @.str.6, ptr @.str.1, i32 983, ptr null)
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
  call void @llvm.var.annotation.p0.p0(ptr %3, ptr @.str.6, ptr @.str.1, i32 989, ptr null)
  store i16 %0, ptr %4, align 2
  call void @llvm.var.annotation.p0.p0(ptr %4, ptr @.str.6, ptr @.str.1, i32 989, ptr null)
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
  call void @llvm.var.annotation.p0.p0(ptr %3, ptr @.str.6, ptr @.str.1, i32 995, ptr null)
  store i16 %0, ptr %4, align 2
  call void @llvm.var.annotation.p0.p0(ptr %4, ptr @.str.6, ptr @.str.1, i32 995, ptr null)
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
  call void @llvm.var.annotation.p0.p0(ptr %3, ptr @.str.6, ptr @.str.1, i32 1001, ptr null)
  store i16 %0, ptr %4, align 2
  call void @llvm.var.annotation.p0.p0(ptr %4, ptr @.str.6, ptr @.str.1, i32 1001, ptr null)
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
  call void @llvm.var.annotation.p0.p0(ptr %3, ptr @.str.7, ptr @.str.1, i32 1007, ptr null)
  store i32 %0, ptr %4, align 4
  call void @llvm.var.annotation.p0.p0(ptr %4, ptr @.str.7, ptr @.str.1, i32 1007, ptr null)
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
  call void @llvm.var.annotation.p0.p0(ptr %3, ptr @.str.7, ptr @.str.1, i32 1013, ptr null)
  store i32 %0, ptr %4, align 4
  call void @llvm.var.annotation.p0.p0(ptr %4, ptr @.str.7, ptr @.str.1, i32 1013, ptr null)
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
  call void @llvm.var.annotation.p0.p0(ptr %3, ptr @.str.7, ptr @.str.1, i32 1019, ptr null)
  store i32 %0, ptr %4, align 4
  call void @llvm.var.annotation.p0.p0(ptr %4, ptr @.str.7, ptr @.str.1, i32 1019, ptr null)
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
  call void @llvm.var.annotation.p0.p0(ptr %3, ptr @.str.7, ptr @.str.1, i32 1025, ptr null)
  store i32 %0, ptr %4, align 4
  call void @llvm.var.annotation.p0.p0(ptr %4, ptr @.str.7, ptr @.str.1, i32 1025, ptr null)
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
  call void @llvm.var.annotation.p0.p0(ptr %3, ptr @.str.7, ptr @.str.1, i32 1031, ptr null)
  store i32 %0, ptr %4, align 4
  call void @llvm.var.annotation.p0.p0(ptr %4, ptr @.str.7, ptr @.str.1, i32 1031, ptr null)
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
  store i32 %0, ptr %4, align 4
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
  store i32 %0, ptr %4, align 4
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
  store i32 %0, ptr %4, align 4
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
  store i32 %0, ptr %4, align 4
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
  store i32 %0, ptr %4, align 4
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
  store i32 %0, ptr %4, align 4
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
  store ptr %0, ptr %4, align 8
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
  store ptr %0, ptr %4, align 8
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
  store ptr %0, ptr %4, align 8
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
  store ptr %0, ptr %4, align 8
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
  store ptr %0, ptr %4, align 8
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
  store ptr %0, ptr %4, align 8
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
  store ptr %0, ptr %4, align 8
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
  store ptr %0, ptr %4, align 8
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
  store ptr %0, ptr %4, align 8
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
  store ptr %0, ptr %4, align 8
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
  store ptr %0, ptr %4, align 8
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
  call void @llvm.var.annotation.p0.p0(ptr %3, ptr @.str.7, ptr @.str.1, i32 1133, ptr null)
  store i32 %0, ptr %4, align 4
  call void @llvm.var.annotation.p0.p0(ptr %4, ptr @.str.7, ptr @.str.1, i32 1133, ptr null)
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
  call void @llvm.var.annotation.p0.p0(ptr %3, ptr @.str.7, ptr @.str.1, i32 1139, ptr null)
  store i32 %0, ptr %4, align 4
  call void @llvm.var.annotation.p0.p0(ptr %4, ptr @.str.7, ptr @.str.1, i32 1139, ptr null)
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
  call void @llvm.var.annotation.p0.p0(ptr %3, ptr @.str.7, ptr @.str.1, i32 1145, ptr null)
  store i32 %0, ptr %4, align 4
  call void @llvm.var.annotation.p0.p0(ptr %4, ptr @.str.7, ptr @.str.1, i32 1145, ptr null)
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
  call void @llvm.var.annotation.p0.p0(ptr %3, ptr @.str.7, ptr @.str.1, i32 1151, ptr null)
  store i32 %0, ptr %4, align 4
  call void @llvm.var.annotation.p0.p0(ptr %4, ptr @.str.7, ptr @.str.1, i32 1151, ptr null)
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
  call void @llvm.var.annotation.p0.p0(ptr %3, ptr @.str.7, ptr @.str.1, i32 1157, ptr null)
  store i32 %0, ptr %4, align 4
  call void @llvm.var.annotation.p0.p0(ptr %4, ptr @.str.7, ptr @.str.1, i32 1157, ptr null)
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
  call void @llvm.var.annotation.p0.p0(ptr %3, ptr @.str.8, ptr @.str.1, i32 1163, ptr null)
  store i64 %0, ptr %4, align 8
  call void @llvm.var.annotation.p0.p0(ptr %4, ptr @.str.8, ptr @.str.1, i32 1163, ptr null)
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
  call void @llvm.var.annotation.p0.p0(ptr %3, ptr @.str.8, ptr @.str.1, i32 1169, ptr null)
  store i64 %0, ptr %4, align 8
  call void @llvm.var.annotation.p0.p0(ptr %4, ptr @.str.8, ptr @.str.1, i32 1169, ptr null)
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
  call void @llvm.var.annotation.p0.p0(ptr %3, ptr @.str.8, ptr @.str.1, i32 1175, ptr null)
  store i64 %0, ptr %4, align 8
  call void @llvm.var.annotation.p0.p0(ptr %4, ptr @.str.8, ptr @.str.1, i32 1175, ptr null)
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
  call void @llvm.var.annotation.p0.p0(ptr %3, ptr @.str.8, ptr @.str.1, i32 1181, ptr null)
  store i64 %0, ptr %4, align 8
  call void @llvm.var.annotation.p0.p0(ptr %4, ptr @.str.8, ptr @.str.1, i32 1181, ptr null)
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
  call void @llvm.var.annotation.p0.p0(ptr %3, ptr @.str.8, ptr @.str.1, i32 1187, ptr null)
  store i64 %0, ptr %4, align 8
  call void @llvm.var.annotation.p0.p0(ptr %4, ptr @.str.8, ptr @.str.1, i32 1187, ptr null)
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
  store i64 %0, ptr %4, align 8
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
  store i64 %0, ptr %4, align 8
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
  store i64 %0, ptr %4, align 8
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
  store i64 %0, ptr %4, align 8
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
  store i64 %0, ptr %4, align 8
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
  store i64 %0, ptr %4, align 8
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
  store ptr %0, ptr %4, align 8
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
  store ptr %0, ptr %4, align 8
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
  store ptr %0, ptr %4, align 8
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
  store ptr %0, ptr %4, align 8
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
  store ptr %0, ptr %4, align 8
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
  store ptr %0, ptr %4, align 8
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
  store ptr %0, ptr %4, align 8
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
  store ptr %0, ptr %4, align 8
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
  store ptr %0, ptr %4, align 8
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
  store ptr %0, ptr %4, align 8
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
  store ptr %0, ptr %4, align 8
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
  call void @llvm.var.annotation.p0.p0(ptr %3, ptr @.str.8, ptr @.str.1, i32 1289, ptr null)
  store i64 %0, ptr %4, align 8
  call void @llvm.var.annotation.p0.p0(ptr %4, ptr @.str.8, ptr @.str.1, i32 1289, ptr null)
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
  call void @llvm.var.annotation.p0.p0(ptr %3, ptr @.str.8, ptr @.str.1, i32 1295, ptr null)
  store i64 %0, ptr %4, align 8
  call void @llvm.var.annotation.p0.p0(ptr %4, ptr @.str.8, ptr @.str.1, i32 1295, ptr null)
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
  call void @llvm.var.annotation.p0.p0(ptr %3, ptr @.str.8, ptr @.str.1, i32 1301, ptr null)
  store i64 %0, ptr %4, align 8
  call void @llvm.var.annotation.p0.p0(ptr %4, ptr @.str.8, ptr @.str.1, i32 1301, ptr null)
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
  call void @llvm.var.annotation.p0.p0(ptr %3, ptr @.str.8, ptr @.str.1, i32 1307, ptr null)
  store i64 %0, ptr %4, align 8
  call void @llvm.var.annotation.p0.p0(ptr %4, ptr @.str.8, ptr @.str.1, i32 1307, ptr null)
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
  call void @llvm.var.annotation.p0.p0(ptr %3, ptr @.str.8, ptr @.str.1, i32 1313, ptr null)
  store i64 %0, ptr %4, align 8
  call void @llvm.var.annotation.p0.p0(ptr %4, ptr @.str.8, ptr @.str.1, i32 1313, ptr null)
  %5 = load i64, ptr %4, align 8
  %6 = load i64, ptr %3, align 8
  %7 = lshr i64 %5, %6
  ret i64 %7
}

; Function Attrs: mustprogress noinline nounwind optnone uwtable
define dso_local float @logia_intrinsics_bin_add_f32_f32(float noundef %0, float noundef %1) #2 {
  %3 = alloca float, align 4
  %4 = alloca float, align 4
  store float %1, ptr %3, align 4
  store float %0, ptr %4, align 4
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
  store float %0, ptr %4, align 4
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
  store float %0, ptr %4, align 4
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
  store float %0, ptr %4, align 4
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
  store float %0, ptr %4, align 4
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
  store float %0, ptr %4, align 4
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
  store float %0, ptr %4, align 4
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
  store float %0, ptr %4, align 4
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
  store float %0, ptr %4, align 4
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
  store ptr %0, ptr %4, align 8
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
  store ptr %0, ptr %4, align 8
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
  store ptr %0, ptr %4, align 8
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
  store ptr %0, ptr %4, align 8
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
  store ptr %0, ptr %4, align 8
  %5 = load float, ptr %3, align 4
  %6 = load ptr, ptr %4, align 8
  %7 = load float, ptr %6, align 4
  %8 = fdiv float %7, %5
  store float %8, ptr %6, align 4
  %9 = load ptr, ptr %4, align 8
  %10 = load float, ptr %9, align 4
  ret float %10
}

attributes #0 = { mustprogress noinline optnone uwtable "min-legal-vector-width"="0" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cmov,+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }
attributes #1 = { "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cmov,+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }
attributes #2 = { mustprogress noinline nounwind optnone uwtable "min-legal-vector-width"="0" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cmov,+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }
attributes #3 = { nocallback nofree nosync nounwind willreturn memory(inaccessiblemem: readwrite) }

!llvm.dbg.cu = !{!0}
!llvm.linker.options = !{!2}
!llvm.module.flags = !{!3, !4, !5, !6, !7}
!llvm.ident = !{!8}

!0 = distinct !DICompileUnit(language: DW_LANG_C_plus_plus_14, file: !1, producer: "clang version 22.1.1 (https://github.com/llvm/llvm-project fef02d48c08db859ef83f84232ed78bd9d1c323a)", isOptimized: false, runtimeVersion: 0, emissionKind: NoDebug, splitDebugInlining: false, nameTableKind: None)
!1 = !DIFile(filename: "intrinsics\\intrinsics.cpp", directory: "C:\\Users\\luis\\Desktop\\git\\logia")
!2 = !{!"/FAILIFMISMATCH:\22_CRT_STDIO_ISO_WIDE_SPECIFIERS=0\22"}
!3 = !{i32 2, !"Debug Info Version", i32 3}
!4 = !{i32 1, !"wchar_size", i32 2}
!5 = !{i32 8, !"PIC Level", i32 2}
!6 = !{i32 7, !"uwtable", i32 2}
!7 = !{i32 1, !"MaxTLSAlign", i32 65536}
!8 = !{!"clang version 22.1.1 (https://github.com/llvm/llvm-project fef02d48c08db859ef83f84232ed78bd9d1c323a)"}
