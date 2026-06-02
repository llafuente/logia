// ./build-debug/bin/Debug/logia_test_suite.exe --gtest_break_on_failure

#include "logia/ast/node.h"
#include "logia/ast/if_stmt.h"
#include "logia/ast/expr.h"
#include "logia/ast/constexpr.h"
#include "logia/ast/stmt.h"

#include "gtest/gtest.h"
#include <Windows.h>

#include "test_utils.h"
#include "logia/run.h"

TEST(logia_run_file, basic_staff)
{
    EXPECT_EQ(test_single_file(".\\tests\\logia\\", ".\\tests\\tmp\\", ".\\tests\\tmp\\", "helloworld"), 0);

    EXPECT_EQ(test_single_file(".\\tests\\logia\\primitives\\", ".\\tests\\tmp\\", ".\\tests\\tmp\\", "integer-limits"), 0);
    EXPECT_EQ(test_single_file(".\\tests\\logia\\primitives\\", ".\\tests\\tmp\\", ".\\tests\\tmp\\", "integer-reprs"), 0);
    EXPECT_EQ(test_single_file(".\\tests\\logia\\primitives\\", ".\\tests\\tmp\\", ".\\tests\\tmp\\", "integer-to-float"), 0);
    EXPECT_EQ(test_single_file(".\\tests\\logia\\primitives\\", ".\\tests\\tmp\\", ".\\tests\\tmp\\", "integer-vardecl"), 15);
    EXPECT_EQ(test_single_file(".\\tests\\logia\\primitives\\", ".\\tests\\tmp\\", ".\\tests\\tmp\\", "float"), 0);

    EXPECT_EQ(test_single_file(".\\tests\\logia\\", ".\\tests\\tmp\\", ".\\tests\\tmp\\", "binary-expr-add-constants"), 25);
    EXPECT_EQ(test_single_file(".\\tests\\logia\\", ".\\tests\\tmp\\", ".\\tests\\tmp\\", "binary-expr-add"), 21);

    EXPECT_EQ(test_single_file(".\\tests\\logia\\", ".\\tests\\tmp\\", ".\\tests\\tmp\\", "struct-initializer"), 11 + 13);
    EXPECT_EQ(test_single_file(".\\tests\\logia\\", ".\\tests\\tmp\\", ".\\tests\\tmp\\", "struct-initializer2"), 13 + 17);
    EXPECT_EQ(test_single_file(".\\tests\\logia\\", ".\\tests\\tmp\\", ".\\tests\\tmp\\", "struct-initializer3"), 3);
    EXPECT_EQ(test_single_file(".\\tests\\logia\\", ".\\tests\\tmp\\", ".\\tests\\tmp\\", "struct-initializer4"), 2);

    EXPECT_EQ(test_single_file(".\\tests\\logia\\", ".\\tests\\tmp\\", ".\\tests\\tmp\\", "struct-initializer-defaults1"), 75 + 1);
    EXPECT_EQ(test_single_file(".\\tests\\logia\\", ".\\tests\\tmp\\", ".\\tests\\tmp\\", "struct-initializer-defaults2"), 99 + 1);
    EXPECT_EQ(test_single_file(".\\tests\\logia\\", ".\\tests\\tmp\\", ".\\tests\\tmp\\", "struct-initializer-defaults3"), 75 + 99);
    EXPECT_EQ(test_single_file(".\\tests\\logia\\", ".\\tests\\tmp\\", ".\\tests\\tmp\\", "struct-initializer-named"), 32); // 31 means not ordering!
    EXPECT_EQ(test_single_file(".\\tests\\logia\\", ".\\tests\\tmp\\", ".\\tests\\tmp\\", "struct-initializer-alias"), 4);
    EXPECT_EQ(test_single_file(".\\tests\\logia\\", ".\\tests\\tmp\\", ".\\tests\\tmp\\", "struct-all-primitives"), 0);

    EXPECT_EQ(test_single_file(".\\tests\\logia\\", ".\\tests\\tmp\\", ".\\tests\\tmp\\", "function-parameters"), 25);
    EXPECT_EQ(test_single_file(".\\tests\\logia\\", ".\\tests\\tmp\\", ".\\tests\\tmp\\", "function-blocks"), 5);

    EXPECT_EQ(test_single_file(".\\tests\\logia\\", ".\\tests\\tmp\\", ".\\tests\\tmp\\", "cast-expr"), 0);

    // 0 means it jumped, 1 it doesn't
    // also not so true as "return 1" is removed, but maybe to check regressions
    EXPECT_EQ(test_single_file(".\\tests\\logia\\", ".\\tests\\tmp\\", ".\\tests\\tmp\\", "goto-stmt-dead-code"), 0);

    // 3 tests -> 3
    EXPECT_EQ(test_single_file(".\\tests\\logia\\", ".\\tests\\tmp\\", ".\\tests\\tmp\\", "if-stmt"), 3);
    EXPECT_EQ(test_single_file(".\\tests\\logia\\", ".\\tests\\tmp\\", ".\\tests\\tmp\\", "if-stmt-2"), 1101);

    EXPECT_EQ(test_single_file(".\\tests\\logia\\", ".\\tests\\tmp\\", ".\\tests\\tmp\\", "expr-all-operator-i64"), 0);

    // EXPECT_EQ(test_single_file(".\\tests\\logia\\", ".\\tests\\tmp\\", ".\\tests\\tmp\\", "struct-operators"), 64);
}
