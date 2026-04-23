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

int test_single_file(const char *file)
{
    bool debug = true;

    int argc = debug ? 7 : 5;
    //++argc;
    const char **argv = (const char **)malloc(sizeof(char *) * argc);
    int arg = 0;
    argv[arg++] = file;

    argv[arg++] = "--emit-llvm";
    auto llfile = std::format("{}{}", file, ".ll");
    argv[arg++] = llfile.c_str();

    argv[arg++] = "--emit-obj";
    auto objfile = std::format("{}{}", file, ".o");
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

    EXPECT_EQ(test_single_file(".\\test\\logia\\sum.logia"), 25);
    EXPECT_EQ(test_single_file(".\\test\\logia\\math.logia"), 21);
    EXPECT_EQ(test_single_file(".\\test\\logia\\struct-initializer.logia"), 21);

    EXPECT_EQ(test_single_file(".\\test\\logia\\function-blocks.logia"), 5);
    EXPECT_EQ(test_single_file(".\\test\\logia\\function-parameters.logia"), 25);

    // 0 means it jumped, 1 it doesn't
    // also not so true as "return 1" is removed, but maybe to check regressions
    EXPECT_EQ(test_single_file(".\\test\\logia\\function-goto.logia"), 0);

    // jump inside function
    // EXPECT_EQ(test_single_file(".\\test\\logia\\function-goto2.logia"), 0);

    // 3 tests -> 3
    EXPECT_EQ(test_single_file(".\\test\\logia\\if.logia"), 3);

    //! EXPECT_EQ(test_single_file(".\\test\\logia\\struct-initializer-named.logia"), 32); // 31 means not ordering!
}
