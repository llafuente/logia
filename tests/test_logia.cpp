// ./build-debug/bin/Debug/logia_test_suite.exe --gtest_break_on_failure

#include "ast/node.h"
#include "ast/traverse.h"
#include "ast/if_stmt.h"
#include "ast/expr.h"
#include "ast/constexpr.h"
#include "ast/stmt.h"

#include "gtest/gtest.h"
#include <Windows.h>

#include "test_utils.h"
#include "logia/run.h"

int test_single_file(const char *logia_folder, const char *ir_folder, const char *obj_folder, const char *file)
{
    std::cout << std::endl
              << "logia file      : " << logia_folder << file << ".logia" << std::endl
              << "current  ll file: " << ir_folder << file << ".ll" << std::endl
              << "expected ll file: " << ".\\tests\\expected-ir\\" << file << ".ll" << std::endl;

    bool debug = true;

    int argc = debug ? 7 : 5;
    //++argc;
    const char **argv = (const char **)malloc(sizeof(char *) * argc);
    int arg = 0;
    auto logia_file = std::format("{}{}{}", logia_folder, file, ".logia");
    argv[arg++] = logia_file.c_str();

    argv[arg++] = "--emit-llvm";
    auto llfile = std::format("{}{}{}", ir_folder, file, ".ll");
    argv[arg++] = llfile.c_str();

    argv[arg++] = "--emit-obj";
    auto objfile = std::format("{}{}{}", obj_folder, file, ".o");
    argv[arg++] = objfile.c_str();

    if (debug)
    {
        argv[arg++] = "--debug";
        argv[arg++] = "--coverage";
    }
    // argv[arg++] = "--verbose";

    auto ret = logia::logia_run(argc, argv);
    free(argv);
    return ret;
}

TEST(run_from_file, sum_logia)
{
    DEBUG() << std::endl
            << "TEST START" << std::endl
            << std::endl;

    EXPECT_EQ(test_single_file(".\\tests\\logia\\", ".\\tests\\tmp\\", ".\\tests\\tmp\\", "helloworld"), 0);

    EXPECT_EQ(test_single_file(".\\tests\\logia\\", ".\\tests\\tmp\\", ".\\tests\\tmp\\", "binary-expr-add-constants"), 25);
    EXPECT_EQ(test_single_file(".\\tests\\logia\\", ".\\tests\\tmp\\", ".\\tests\\tmp\\", "binary-expr-add"), 21);

    EXPECT_EQ(test_single_file(".\\tests\\logia\\", ".\\tests\\tmp\\", ".\\tests\\tmp\\", "struct-initializer"), 11 + 13);
    EXPECT_EQ(test_single_file(".\\tests\\logia\\", ".\\tests\\tmp\\", ".\\tests\\tmp\\", "struct-initializer2"), 13 + 17);
    //! EXPECT_EQ(test_single_file(".\\tests\\logia\\struct-initializer-named.logia"), 32); // 31 means not ordering!

    EXPECT_EQ(test_single_file(".\\tests\\logia\\", ".\\tests\\tmp\\", ".\\tests\\tmp\\", "function-blocks"), 5);
    EXPECT_EQ(test_single_file(".\\tests\\logia\\", ".\\tests\\tmp\\", ".\\tests\\tmp\\", "function-parameters"), 25);

    // 0 means it jumped, 1 it doesn't
    // also not so true as "return 1" is removed, but maybe to check regressions
    EXPECT_EQ(test_single_file(".\\tests\\logia\\", ".\\tests\\tmp\\", ".\\tests\\tmp\\", "goto-stmt-dead-code"), 0);

    // 3 tests -> 3
    EXPECT_EQ(test_single_file(".\\tests\\logia\\", ".\\tests\\tmp\\", ".\\tests\\tmp\\", "if-stmt"), 3);

    EXPECT_EQ(test_single_file(".\\tests\\logia\\", ".\\tests\\tmp\\", ".\\tests\\tmp\\", "expr-all-operator-i64"), 0);

    EXPECT_EQ(test_single_file(".\\tests\\logia\\", ".\\tests\\tmp\\", ".\\tests\\tmp\\", "numeric-vardecl"), 15);
}
