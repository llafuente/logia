; ModuleID = '.\intrinsics\intrinsics.cpp'
source_filename = ".\\intrinsics\\intrinsics.cpp"
target datalayout = "e-m:w-p270:32:32-p271:32:32-p272:64:64-i64:64-i128:128-f80:128-n8:16:32:64-S128"
target triple = "x86_64-pc-windows-msvc19.44.35221"

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
define dso_local i64 @logia_intrinsics_bin_add_i64_i64(i64 noundef %0, i64 noundef %1) #2 {
  %3 = alloca i64, align 8
  %4 = alloca i64, align 8
  store i64 %1, ptr %3, align 8
  store i64 %0, ptr %4, align 8
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
  store i64 %0, ptr %4, align 8
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
  store i64 %0, ptr %4, align 8
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
  store i64 %0, ptr %4, align 8
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
  store i64 %0, ptr %4, align 8
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
define dso_local i64 @logia_intrinsics_bin_bitwise_shl_assign_i64_i64(ptr noundef %0, i64 noundef %1) #2 {
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
define dso_local i64 @logia_intrinsics_bin_bitwise_shr_assign_i64_i64(ptr noundef %0, i64 noundef %1) #2 {
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
  store i64 %0, ptr %4, align 8
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
  store i64 %0, ptr %4, align 8
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
  store i64 %0, ptr %4, align 8
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
  store i64 %0, ptr %4, align 8
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
  store i64 %0, ptr %4, align 8
  %5 = load i64, ptr %4, align 8
  %6 = load i64, ptr %3, align 8
  %7 = ashr i64 %5, %6
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

; Function Attrs: mustprogress noinline nounwind optnone uwtable
define dso_local i32 @logia_intrinsics_bin_add_i32_i32(i32 noundef %0, i32 noundef %1) #2 {
  %3 = alloca i32, align 8
  %4 = alloca i32, align 8
  store i32 %1, ptr %3, align 8
  store i32 %0, ptr %4, align 8
  %5 = load i32, ptr %4, align 8
  %6 = load i32, ptr %3, align 8
  %7 = add nsw i32 %5, %6
  ret i32 %7
}


attributes #0 = { mustprogress noinline optnone uwtable "min-legal-vector-width"="0" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cmov,+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }
attributes #1 = { "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cmov,+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }
attributes #2 = { mustprogress noinline nounwind optnone uwtable "min-legal-vector-width"="0" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cmov,+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }

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
