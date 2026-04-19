// ./build-debug/bin/Debug/logia_test_suite.exe --gtest_break_on_failure

#include "ast/node.h"
#include "ast/traverse.h"
#include "ast/if_stmt.h"
#include "ast/types.h"
#include "ast/expr.h"
#include "ast/constexpr.h"
#include "ast/stmt.h"

#include "gtest/gtest.h"
#include <Windows.h>

#include "test_utils.h"
#include "logia/run.h"

int test_single_file(const char *file)
{
    const char *argv[3];
    argv[0] = file;
    argv[1] = "--emit-llvm";
    auto len = strlen(file);
    auto llfile = std::format("{}{}", file, ".ll");
    argv[2] = llfile.c_str();

    auto ret = logia::logia_run(3, argv);

    return ret;
}

TEST(run_from_file, sum_logia)
{
    DEBUG() << std::endl
            << "TEST START" << std::endl
            << std::endl;

    // EXPECT_EQ(test_single_file(".\\test\\logia\\sum.logia"), 25);
    // EXPECT_EQ(test_single_file(".\\test\\logia\\math.logia"), 21);
    // EXPECT_EQ(test_single_file(".\\test\\logia\\struct-initializer.logia"), 21);

    // 0 means it jumped, 1 it doesn't
    // also not so true as "return 1" is removed, but maybe to check regressions
    EXPECT_EQ(test_single_file(".\\test\\logia\\function-goto.logia"), 0);

    // jump inside function
    EXPECT_EQ(test_single_file(".\\test\\logia\\function-goto2.logia"), 0);

    // EXPECT_EQ(test_single_file(".\\test\\logia\\function-blocks.logia"), 2);
    // EXPECT_EQ(test_single_file(".\\test\\logia\\struct-initializer-named.logia"), 32); // 31 means not ordering!
}
