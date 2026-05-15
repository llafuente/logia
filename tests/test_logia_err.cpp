// ./build-debug/bin/Debug/logia_test_suite.exe --gtest_break_on_failure
#include "ast/semantic_error.h"
#include "gtest/gtest.h"
#include "test_utils.h"
#include "logia/run.h"

const char *test_file_with_semantic_error(const char *logia_folder, const char *ir_folder, const char *obj_folder, const char *file)
{
    try
    {
        test_single_file(logia_folder, ir_folder, obj_folder, file);
        EXPECT_TRUE(false && "unreachable, file run but no error found!");
    }
    catch (logia::AST::semantic_error const &e)
    {
        std::cerr << "catch logia::AST::semantic_error" << std::endl;
        return strdup(e.what());
    }
    catch (std::exception const &e)
    {
        std::cerr << "catch std::exception" << std::endl;
        throw std::runtime_error(std::format("file: {} throw an unexpected exception:\n{}", file, e.what()));
    }

    throw std::runtime_error(std::format("file: {} don't throw", file));
}

TEST(logia_run_error_file, semantic_error_LGER030)
{
    auto msg = test_file_with_semantic_error(".\\tests\\logia-errors\\", ".\\tests\\tmp\\", ".\\tests\\tmp\\", "err-LGER030-001");
    // std::cerr << "???" << msg << "????" << std::endl;
    //  EXPECT_NE(strstr(err.what(), "semantic error"), NULL);
    //  EXPECT_NE(strstr(err.what(), "LGER030 incompatible type 'function sum () i32', expected a struct"), NULL);
    EXPECT_NE(std::string(msg).find("LGER030 incompatible type 'function sum () i32', expected a struct"), std::string::npos);
    EXPECT_NE(std::string(msg).find("err-LGER030-001.logia:8:16"), std::string::npos);
}
TEST(logia_run_error_file, semantic_error_LGER031)
{
    auto msg = test_file_with_semantic_error(".\\tests\\logia-errors\\", ".\\tests\\tmp\\", ".\\tests\\tmp\\", "err-LGER031-001");
    // std::cerr << "???" << msg << "????" << std::endl;
    //  EXPECT_NE(strstr(err.what(), "semantic error"), NULL);
    EXPECT_NE(std::string(msg).find("LGER031 Missing initializer for field 'y' at position '2' of type 'struct point {x i64, y i64}"), std::string::npos);
    EXPECT_NE(std::string(msg).find("err-LGER031-001.logia:9:18"), std::string::npos);
}
TEST(logia_run_error_file, semantic_error_LGER032)
{
    auto msg = test_file_with_semantic_error(".\\tests\\logia-errors\\", ".\\tests\\tmp\\", ".\\tests\\tmp\\", "err-LGER032-001");

    EXPECT_NE(std::string(msg).find("LGER032 lhs cannot be a constant expression"), std::string::npos);
    EXPECT_NE(std::string(msg).find("err-LGER032-001.logia:3:4"), std::string::npos);

    msg = test_file_with_semantic_error(".\\tests\\logia-errors\\", ".\\tests\\tmp\\", ".\\tests\\tmp\\", "err-LGER032-002");

    EXPECT_NE(std::string(msg).find("LGER032 lhs cannot be a constant expression"), std::string::npos);
    EXPECT_NE(std::string(msg).find("err-LGER032-002.logia:3:4"), std::string::npos);
}
TEST(logia_run_error_file, semantic_error_LGERR_ST001)
{
    {
        auto msg = test_file_with_semantic_error(".\\tests\\logia-errors\\", ".\\tests\\tmp\\", ".\\tests\\tmp\\", "err-LGERR_ST001-001");
        EXPECT_NE(std::string(msg).find("LGERR_ST001 Redeclaration of field name 'x'"), std::string::npos);
        EXPECT_NE(std::string(msg).find("err-LGERR_ST001-001.logia:3:4"), std::string::npos);
        EXPECT_NE(std::string(msg).find("err-LGERR_ST001-001.logia:5:4"), std::string::npos);
    }
    {
        auto msg = test_file_with_semantic_error(".\\tests\\logia-errors\\", ".\\tests\\tmp\\", ".\\tests\\tmp\\", "err-LGERR_ST001-002");
        EXPECT_NE(std::string(msg).find("LGERR_ST001 Redeclaration of field name 'x'"), std::string::npos);
        EXPECT_NE(std::string(msg).find("err-LGERR_ST001-002.logia:3:4"), std::string::npos);
        EXPECT_NE(std::string(msg).find("err-LGERR_ST001-002.logia:5:4"), std::string::npos);
    }
}
TEST(logia_run_error_file, semantic_error_LGERR_ST002)
{
    {
        auto msg = test_file_with_semantic_error(".\\tests\\logia-errors\\", ".\\tests\\tmp\\", ".\\tests\\tmp\\", "err-LGERR_ST002-001");
        EXPECT_NE(std::string(msg).find("LGERR_ST002 Alias target 'b' not found."), std::string::npos);
        EXPECT_NE(std::string(msg).find("err-LGERR_ST002-001.logia:5:4"), std::string::npos);
    }
}
