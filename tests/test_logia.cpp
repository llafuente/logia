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

TEST(logia_run_file, helloworld)
{
    EXPECT_EQ(test_single_file(".\\tests\\logia\\", ".\\tests\\tmp\\", ".\\tests\\tmp\\", "helloworld"), 0);
}
TEST(logia_run_file, primitives)
{
    // EXPECT_EQ(test_single_file(".\\tests\\logia\\primitives\\", ".\\tests\\tmp\\", ".\\tests\\tmp\\", "math"), 7);
    EXPECT_EQ(test_single_file(".\\tests\\logia\\primitives\\", ".\\tests\\tmp\\", ".\\tests\\tmp\\", "integer-limits"), 0);
    EXPECT_EQ(test_single_file(".\\tests\\logia\\primitives\\", ".\\tests\\tmp\\", ".\\tests\\tmp\\", "integer-reprs"), 0);
    EXPECT_EQ(test_single_file(".\\tests\\logia\\primitives\\", ".\\tests\\tmp\\", ".\\tests\\tmp\\", "integer-to-float"), 0);
    EXPECT_EQ(test_single_file(".\\tests\\logia\\primitives\\", ".\\tests\\tmp\\", ".\\tests\\tmp\\", "integer-vardecl"), 15);
    EXPECT_EQ(test_single_file(".\\tests\\logia\\primitives\\", ".\\tests\\tmp\\", ".\\tests\\tmp\\", "float"), 0);
}

TEST(logia_run_file, binary_expr)
{
    EXPECT_EQ(test_single_file(".\\tests\\logia\\expr\\", ".\\tests\\tmp\\", ".\\tests\\tmp\\", "binary-expr-add"), 21);
    // require constexpr atm!
    // EXPECT_EQ(test_single_file(".\\tests\\logia\\expr\\", ".\\tests\\tmp\\", ".\\tests\\tmp\\", "binary-expr-add-constants"), 100);
    EXPECT_EQ(test_single_file(".\\tests\\logia\\expr\\", ".\\tests\\tmp\\", ".\\tests\\tmp\\", "binary-expr-logical-and"), 1);
    EXPECT_EQ(test_single_file(".\\tests\\logia\\expr\\", ".\\tests\\tmp\\", ".\\tests\\tmp\\", "binary-expr-logical-or"), 3);
    EXPECT_EQ(test_single_file(".\\tests\\logia\\expr\\", ".\\tests\\tmp\\", ".\\tests\\tmp\\", "expr-add-all-types"), 0);
}

TEST(logia_run_file, struct)
{

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

    // EXPECT_EQ(test_single_file(".\\tests\\logia\\", ".\\tests\\tmp\\", ".\\tests\\tmp\\", "struct-operators"), 64);
}
TEST(logia_run_file, function)
{
    EXPECT_EQ(test_single_file(".\\tests\\logia\\", ".\\tests\\tmp\\", ".\\tests\\tmp\\", "function-parameters"), 25);
    EXPECT_EQ(test_single_file(".\\tests\\logia\\", ".\\tests\\tmp\\", ".\\tests\\tmp\\", "function-blocks"), 11111);
}
TEST(logia_run_file, cast)
{
    EXPECT_EQ(test_single_file(".\\tests\\logia\\", ".\\tests\\tmp\\", ".\\tests\\tmp\\", "cast-expr"), 0);
}

//
// blocks
//

TEST(logia_run_file, block_err_LGERR_BLK001)
{
    auto msg = test_file_with_semantic_error(".\\tests\\logia\\block\\", ".\\tests\\tmp\\", ".\\tests\\tmp\\", "err-LGERR_BLK001");
    // std::cerr << "???" << msg << "????" << std::endl;
    EXPECT_NE(std::string(msg).find("LGERR_BLK001 Identifier 'name' in use:"), std::string::npos);
    EXPECT_NE(std::string(msg).find("err-LGERR_BLK001.logia:7:10"), std::string::npos);
}

TEST(logia_run_file, block_err_LGERR_BLK002)
{
    auto msg = test_file_with_semantic_error(".\\tests\\logia\\block\\", ".\\tests\\tmp\\", ".\\tests\\tmp\\", "err-LGERR_BLK002");
    // std::cerr << "???" << msg << "????" << std::endl;
    EXPECT_NE(std::string(msg).find("LGERR_BLK001 Identifier 'name' in use:"), std::string::npos);
    EXPECT_NE(std::string(msg).find("err-LGERR_BLK002.logia:5:10"), std::string::npos);
}

//
// goto
//

TEST(logia_run_file, goto)
{
    // 0 means it jumped, 1 it doesn't
    // also not so true as "return 1" is removed, but maybe to check regressions
    EXPECT_EQ(test_single_file(".\\tests\\logia\\goto\\", ".\\tests\\tmp\\", ".\\tests\\tmp\\", "goto-stmt-dead-code"), 0);
}

TEST(logia_run_error_file, goto_semantic_error_LGERR_GT001)
{
    auto msg = test_file_with_semantic_error(".\\tests\\logia\\goto\\", ".\\tests\\tmp\\", ".\\tests\\tmp\\", "err-LGERR_GT001");
    // std::cerr << "???" << msg << "????" << std::endl;
    EXPECT_NE(std::string(msg).find("LGERR_GT001 use of undeclared or unreachable label 'a'"), std::string::npos);
    EXPECT_NE(std::string(msg).find("err-LGERR_GT001.logia:3:4"), std::string::npos);
}
TEST(logia_run_error_file, goto_semantic_error_LGERR_GT002)
{
    auto msg = test_file_with_semantic_error(".\\tests\\logia\\goto\\", ".\\tests\\tmp\\", ".\\tests\\tmp\\", "err-LGERR_GT002");
    // std::cerr << "???" << msg << "????" << std::endl;
    EXPECT_NE(std::string(msg).find("LGERR_GT002 Identifier 'a' has multiple matches: to-do!"), std::string::npos);
    EXPECT_NE(std::string(msg).find("err-LGERR_GT002.logia:12:4"), std::string::npos);
}
TEST(logia_run_error_file, goto_semantic_error_LGERR_GT003)
{
    auto msg = test_file_with_semantic_error(".\\tests\\logia\\goto\\", ".\\tests\\tmp\\", ".\\tests\\tmp\\", "err-LGERR_GT003");
    // std::cerr << "???" << msg << "????" << std::endl;
    EXPECT_NE(std::string(msg).find("LGERR_GT003 expected label 'a' to reference a block, but found:"), std::string::npos);
    EXPECT_NE(std::string(msg).find("err-LGERR_GT003.logia:7:4"), std::string::npos);
}

//
// vardecl
//
TEST(logia_run_error_file, vardecl_semantic_error_LGERR_VDECL001)
{
    auto msg = test_file_with_semantic_error(".\\tests\\logia\\vardecl\\", ".\\tests\\tmp\\", ".\\tests\\tmp\\", "err-LGERR_VDECL001");
    // std::cerr << "???" << msg << "????" << std::endl;
    EXPECT_NE(std::string(msg).find("LGERR_VDECL001 Redeclaration/Identifier 'x' in use:"), std::string::npos);
    EXPECT_NE(std::string(msg).find("err-LGERR_VDECL001.logia:3:4"), std::string::npos);
}

//
// if
//

TEST(logia_run_file, if)
{
    // 3 tests -> 3
    EXPECT_EQ(test_single_file(".\\tests\\logia\\", ".\\tests\\tmp\\", ".\\tests\\tmp\\", "if-stmt"), 3);
    EXPECT_EQ(test_single_file(".\\tests\\logia\\", ".\\tests\\tmp\\", ".\\tests\\tmp\\", "if-stmt-2"), 1101);
}
TEST(logia_run_file, expr)
{
    EXPECT_EQ(test_single_file(".\\tests\\logia\\", ".\\tests\\tmp\\", ".\\tests\\tmp\\", "expr-all-operator-i64"), -100);
}
