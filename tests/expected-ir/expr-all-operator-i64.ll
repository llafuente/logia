; ModuleID = 'logia'
source_filename = "logia"

define i64 @main() {
function_body:
%a = alloca i64, align 8
store i64 100, ptr %a, align 4
%b = alloca i64, align 8
store i64 10, ptr %b, align 4
%c = alloca i64, align 8
store i64 0, ptr %c, align 4
%c1 = load i64, ptr %c, align 4
%deref = alloca ptr, align 8
store i64 %c1, ptr %deref, align 4
%a2 = load i64, ptr %a, align 4
%b3 = load i64, ptr %b, align 4
%0 = call i64 @logia_intrinsics_bin_add_i64_i64(i64 %a2, i64 %b3)
%1 = call i64 @logia_intrinsics_bin_assign_i64_i64(ptr %deref, i64 %0)
%c4 = load i64, ptr %c, align 4
%deref5 = alloca ptr, align 8
store i64 %c4, ptr %deref5, align 4
%a6 = load i64, ptr %a, align 4
%b7 = load i64, ptr %b, align 4
%2 = call i64 @logia_intrinsics_bin_sub_i64_i64(i64 %a6, i64 %b7)
%3 = call i64 @logia_intrinsics_bin_assign_i64_i64(ptr %deref5, i64 %2)
%c8 = load i64, ptr %c, align 4
%deref9 = alloca ptr, align 8
store i64 %c8, ptr %deref9, align 4
%a10 = load i64, ptr %a, align 4
%b11 = load i64, ptr %b, align 4
%4 = call i64 @logia_intrinsics_bin_mul_i64_i64(i64 %a10, i64 %b11)
%5 = call i64 @logia_intrinsics_bin_assign_i64_i64(ptr %deref9, i64 %4)
%c12 = load i64, ptr %c, align 4
%deref13 = alloca ptr, align 8
store i64 %c12, ptr %deref13, align 4
%a14 = load i64, ptr %a, align 4
%b15 = load i64, ptr %b, align 4
%6 = call i64 @logia_intrinsics_bin_div_i64_i64(i64 %a14, i64 %b15)
%7 = call i64 @logia_intrinsics_bin_assign_i64_i64(ptr %deref13, i64 %6)
%c16 = load i64, ptr %c, align 4
%deref17 = alloca ptr, align 8
store i64 %c16, ptr %deref17, align 4
%a18 = load i64, ptr %a, align 4
%b19 = load i64, ptr %b, align 4
%8 = call i64 @logia_intrinsics_bin_mod_i64_i64(i64 %a18, i64 %b19)
%9 = call i64 @logia_intrinsics_bin_assign_i64_i64(ptr %deref17, i64 %8)
%a20 = load i64, ptr %a, align 4
%b21 = load i64, ptr %b, align 4
%10 = call i1 @logia_intrinsics_bin_eq_i64_i64(i64 %a20, i64 %b21)
%a22 = load i64, ptr %a, align 4
%b23 = load i64, ptr %b, align 4
%11 = call i1 @logia_intrinsics_bin_neq_i64_i64(i64 %a22, i64 %b23)
%a24 = load i64, ptr %a, align 4
%b25 = load i64, ptr %b, align 4
%12 = call i1 @logia_intrinsics_bin_lt_i64_i64(i64 %a24, i64 %b25)
%a26 = load i64, ptr %a, align 4
%b27 = load i64, ptr %b, align 4
%13 = call i1 @logia_intrinsics_bin_gt_i64_i64(i64 %a26, i64 %b27)
%a28 = load i64, ptr %a, align 4
%b29 = load i64, ptr %b, align 4
%14 = call i1 @logia_intrinsics_bin_lte_i64_i64(i64 %a28, i64 %b29)
%a30 = load i64, ptr %a, align 4
%b31 = load i64, ptr %b, align 4
%15 = call i1 @logia_intrinsics_bin_gte_i64_i64(i64 %a30, i64 %b31)
%c32 = load i64, ptr %c, align 4
%deref33 = alloca ptr, align 8
store i64 %c32, ptr %deref33, align 4
%a34 = load i64, ptr %a, align 4
%16 = call i64 @logia_intrinsics_bin_add_assign_i64_i64(ptr %deref33, i64 %a34)
%c35 = load i64, ptr %c, align 4
%deref36 = alloca ptr, align 8
store i64 %c35, ptr %deref36, align 4
%a37 = load i64, ptr %a, align 4
%17 = call i64 @logia_intrinsics_bin_sub_assign_i64_i64(ptr %deref36, i64 %a37)
%c38 = load i64, ptr %c, align 4
%deref39 = alloca ptr, align 8
store i64 %c38, ptr %deref39, align 4
%a40 = load i64, ptr %a, align 4
%18 = call i64 @logia_intrinsics_bin_mul_assign_i64_i64(ptr %deref39, i64 %a40)
%c41 = load i64, ptr %c, align 4
%deref42 = alloca ptr, align 8
store i64 %c41, ptr %deref42, align 4
%a43 = load i64, ptr %a, align 4
%19 = call i64 @logia_intrinsics_bin_div_assign_i64_i64(ptr %deref42, i64 %a43)
%c44 = load i64, ptr %c, align 4
%deref45 = alloca ptr, align 8
store i64 %c44, ptr %deref45, align 4
%a46 = load i64, ptr %a, align 4
%b47 = load i64, ptr %b, align 4
%20 = call i64 @logia_intrinsics_bin_bitwise_and_i64_i64(i64 %a46, i64 %b47)
%21 = call i64 @logia_intrinsics_bin_assign_i64_i64(ptr %deref45, i64 %20)
%c48 = load i64, ptr %c, align 4
%deref49 = alloca ptr, align 8
store i64 %c48, ptr %deref49, align 4
%a50 = load i64, ptr %a, align 4
%b51 = load i64, ptr %b, align 4
%22 = call i64 @logia_intrinsics_bin_bitwise_or_i64_i64(i64 %a50, i64 %b51)
%23 = call i64 @logia_intrinsics_bin_assign_i64_i64(ptr %deref49, i64 %22)
%c52 = load i64, ptr %c, align 4
%deref53 = alloca ptr, align 8
store i64 %c52, ptr %deref53, align 4
%a54 = load i64, ptr %a, align 4
%b55 = load i64, ptr %b, align 4
%24 = call i64 @logia_intrinsics_bin_bitwise_xor_i64_i64(i64 %a54, i64 %b55)
%25 = call i64 @logia_intrinsics_bin_assign_i64_i64(ptr %deref53, i64 %24)
%c56 = load i64, ptr %c, align 4
%deref57 = alloca ptr, align 8
store i64 %c56, ptr %deref57, align 4
%a58 = load i64, ptr %a, align 4
%26 = call i64 @logia_intrinsics_prefix_bitwise_not_i64(i64 %a58)
%27 = call i64 @logia_intrinsics_bin_assign_i64_i64(ptr %deref57, i64 %26)
%c59 = load i64, ptr %c, align 4
%deref60 = alloca ptr, align 8
store i64 %c59, ptr %deref60, align 4
%a61 = load i64, ptr %a, align 4
%28 = call i64 @logia_intrinsics_bin_bitwise_shl_i64_i64(i64 %a61, i64 1)
%29 = call i64 @logia_intrinsics_bin_assign_i64_i64(ptr %deref60, i64 %28)
%c62 = load i64, ptr %c, align 4
%deref63 = alloca ptr, align 8
store i64 %c62, ptr %deref63, align 4
%a64 = load i64, ptr %a, align 4
%30 = call i64 @logia_intrinsics_bin_bitwise_shr_i64_i64(i64 %a64, i64 2)
%31 = call i64 @logia_intrinsics_bin_assign_i64_i64(ptr %deref63, i64 %30)
%c65 = load i64, ptr %c, align 4
%deref66 = alloca ptr, align 8
store i64 %c65, ptr %deref66, align 4
%a67 = load i64, ptr %a, align 4
%32 = call i64 @logia_intrinsics_prefix_neg_i64(i64 %a67)
%33 = call i64 @logia_intrinsics_bin_assign_i64_i64(ptr %deref66, i64 %32)
ret i64 0
}
