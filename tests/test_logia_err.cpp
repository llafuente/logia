// ./build-debug/bin/Debug/logia_test_suite.exe --gtest_break_on_failure
#include "logia/ast/semantic_error.h"

#include <gtest/gtest.h>
#include <gmock/gmock.h> // For matchers like HasSubstr

#include "test_utils.h"
#include "logia/run.h"

TEST(logia_run_error_file, semantic_error_LGER030)
{
    auto msg = test_file_with_semantic_error(".\\tests\\logia-errors\\", ".\\tests\\tmp\\", ".\\tests\\tmp\\", "err-LGER030-001");
    // std::cerr << "???" << msg << "????" << std::endl;
    //  EXPECT_NE(strstr(err.what(), "semantic error"), NULL);
    //  EXPECT_NE(strstr(err.what(), "LGER030 incompatible type 'function sum () i32', expected a struct"), NULL);
    EXPECT_THAT(msg, testing::HasSubstr("LGER030 incompatible type 'function sum () i32', expected a struct"));
    EXPECT_THAT(msg, testing::HasSubstr("err-LGER030-001.logia:7:17"));
}
TEST(logia_run_error_file, semantic_error_LGER031)
{
    auto msg = test_file_with_semantic_error(".\\tests\\logia-errors\\", ".\\tests\\tmp\\", ".\\tests\\tmp\\", "err-LGER031-001");
    // std::cerr << "???" << msg << "????" << std::endl;
    //  EXPECT_NE(strstr(err.what(), "semantic error"), NULL);
    EXPECT_THAT(msg, testing::HasSubstr("LGER031 Missing initializer for field 'y' at position '2' of type 'struct point {λi64 x, λi64 y}"));
    EXPECT_THAT(msg, testing::HasSubstr("err-LGER031-001.logia:8:19"));
}
TEST(logia_run_error_file, semantic_error_LGER032)
{
    auto msg = test_file_with_semantic_error(".\\tests\\logia-errors\\", ".\\tests\\tmp\\", ".\\tests\\tmp\\", "err-LGERR_BINEXPR002-001");
    // std::cerr << "???" << msg << "????" << std::endl;
    EXPECT_THAT(msg, testing::HasSubstr("LGERR_BINEXPR002 lhs cannot be a constant expression"));
    EXPECT_THAT(msg, testing::HasSubstr("err-LGERR_BINEXPR002-001.logia:2:5"));

    msg = test_file_with_semantic_error(".\\tests\\logia-errors\\", ".\\tests\\tmp\\", ".\\tests\\tmp\\", "err-LGERR_BINEXPR002-002");

    EXPECT_THAT(msg, testing::HasSubstr("LGERR_BINEXPR002 lhs cannot be a constant expression"));
    EXPECT_THAT(msg, testing::HasSubstr("err-LGERR_BINEXPR002-002.logia:2:5"));
}
TEST(logia_run_error_file, semantic_error_LGERR_ST001)
{
    {
        auto msg = test_file_with_semantic_error(".\\tests\\logia-errors\\", ".\\tests\\tmp\\", ".\\tests\\tmp\\", "err-LGERR_ST001-001");
        EXPECT_THAT(msg, testing::HasSubstr("LGERR_ST001 Redeclaration of field name 'x'"));
        EXPECT_THAT(msg, testing::HasSubstr("err-LGERR_ST001-001.logia:2:5"));
        EXPECT_THAT(msg, testing::HasSubstr("err-LGERR_ST001-001.logia:4:5"));
    }
    {
        auto msg = test_file_with_semantic_error(".\\tests\\logia-errors\\", ".\\tests\\tmp\\", ".\\tests\\tmp\\", "err-LGERR_ST001-002");
        // std::cerr << "???" << msg << "????" << std::endl;
        EXPECT_THAT(msg, testing::HasSubstr("LGERR_ST001 Redeclaration of field name 'x'"));
        EXPECT_THAT(msg, testing::HasSubstr("err-LGERR_ST001-002.logia:2:5"));
        EXPECT_THAT(msg, testing::HasSubstr("err-LGERR_ST001-002.logia:4:5"));
    }
}
TEST(logia_run_error_file, semantic_error_LGERR_ST002)
{
    {
        auto msg = test_file_with_semantic_error(".\\tests\\logia-errors\\", ".\\tests\\tmp\\", ".\\tests\\tmp\\", "err-LGERR_ST002-001");
        // std::cerr << "???" << msg << "????" << std::endl;
        EXPECT_THAT(msg, testing::HasSubstr("LGERR_ST002 Alias target 'b' not found."));
        EXPECT_THAT(msg, testing::HasSubstr("err-LGERR_ST002-001.logia:4:5"));
    }
}

TEST(logia_run_error_file, semantic_error_LGERR_IF001)
{
    {
        auto msg = test_file_with_semantic_error(".\\tests\\logia-errors\\", ".\\tests\\tmp\\", ".\\tests\\tmp\\", "err-LGERR_IF001-001");
        EXPECT_THAT(msg, testing::HasSubstr("LGERR_IF001 Expected contition type to be 'bool' but found: 'λf64'"));
        EXPECT_THAT(msg, testing::HasSubstr("err-LGERR_IF001-001.logia:9:9"));
    }
}
