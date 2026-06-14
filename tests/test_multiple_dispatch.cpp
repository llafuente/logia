// ./build-debug/bin/Debug/logia_test_suite.exe --gtest_break_on_failure

#include "logia/ast/node.h"
#include "logia/ast/type.h"
#include "logia/ast/expr.h"
#include "logia/ast/constexpr.h"
#include "logia/ast/callexpr.h"
#include "logia/ast/identifier.h"

#include "logia/maybe_error.h"
#include "logia/type_system.h"
#include "logia/multiple_dispatch.h"

#include "gtest/gtest.h"
#include "gmock/gmock.h" // For matchers like HasSubstr
#include <Windows.h>

#include "test_utils.h"
#include "logia/run.h"

TEST(logia_test_multiple_dispatch, match001)
{
    LOGIA_UNIT_TEST();

    using namespace logia::multiple_dispatch;
    using namespace logia::AST;
    using namespace logia::type_system;

    auto i16_v = new IntegerLiteral(loc, "10", i16);
    auto i32_v = new IntegerLiteral(loc, "10", i32);
    auto i32_v2 = new IntegerLiteral(loc, "10", i32);
    auto i64_v = new IntegerLiteral(loc, "10", i64);

    auto xxx_fn = new Function(loc, new Identifier(loc, "xxx"), i32, false);
    xxx_fn->push_parameter(new FunctionParameter(new Identifier(loc, "a"), i32, nullptr));
    program->push_child(xxx_fn);

    auto xxx2_fn = new Function(loc, new Identifier(loc, "xxx"), i32, false);
    xxx2_fn->push_parameter(new FunctionParameter(new Identifier(loc, "a"), i32, nullptr));
    xxx2_fn->push_parameter(new FunctionParameter(new Identifier(loc, "b"), i32, i32_v2));
    program->push_child(xxx2_fn);

    // NOTE call expression locator is not tested!
    auto callexpr = new CallExpression(loc, new Identifier(loc, "?"), {i32_v});
    main_body->push_child(callexpr);

    auto callexpr2 = new CallExpression(loc, new Identifier(loc, "?"), {i16_v});
    main_body->push_child(callexpr2);

    auto callexpr3 = new CallExpression(loc, new Identifier(loc, "?"), {});
    callexpr3->push_named_argument(new Identifier(loc, "b"), i32_v);
    main_body->push_child(callexpr3);

    auto callexpr4 = new CallExpression(loc, new Identifier(loc, "?"), {});
    callexpr4->push_named_argument(new Identifier(loc, "a"), i32_v);
    callexpr4->push_named_argument(new Identifier(loc, "b"), i32_v);
    main_body->push_child(callexpr4);

    {
        // function xxx(i32 a)
        // xxx(i32)
        auto x = match(callexpr, xxx_fn, false);
        EXPECT_TRUE(!x.is_error());
    }

    {
        // function xxx(i32 a)
        // xxx(i16)
        auto x = match(callexpr2, xxx_fn, false);
        EXPECT_TRUE(x.is_error());
        EXPECT_STREQ(x.message.c_str(), "LGERR_MD001 Invalid argument type at position '1' of type 'i16', expected to match parameter 'a' of type: 'i32'\nLGERR_TS_INT002 Incompatible types 'i16' -> 'i32'. Explicit cast is required, conversion loses integer precision.");
        // std::cerr << x.message << std::endl;
    }

    {
        // function xxx(i32 a)
        // xxx(b = i32)
        auto x = match(callexpr3, xxx_fn, false);
        EXPECT_TRUE(x.is_error());
        EXPECT_STREQ(x.message.c_str(), "LGERR_MD002 Invalid argument name 'b', could not be found in the function: 'function xxx (i32 a) i32'.");
        // std::cerr << x.message << std::endl;
    }

    {
        // function xxx(i32 a)
        // xxx(a = i32, b = i32)
        auto x = match(callexpr4, xxx_fn, false);
        EXPECT_TRUE(x.is_error());
        EXPECT_STREQ(x.message.c_str(), "LGERR_MD003 Invalid argument count. 'function xxx (i32 a) i32' takes '1' arguments but '2' were given.");
        // std::cerr << x.message << std::endl;
    }

    {
        // function xxx2(i32 a, i32 b = 100)
        // xxx2(i32)
        auto x = match(callexpr, xxx2_fn, false);
        EXPECT_TRUE(!x.is_error());
        // std::cerr << x.message << std::endl;
    }

    {
        // function xxx2(i32 a, i32 b = 100)
        // xxx(i16)
        auto x = match(callexpr2, xxx2_fn, false);
        EXPECT_TRUE(x.is_error());
        EXPECT_STREQ(x.message.c_str(), "LGERR_MD001 Invalid argument type at position '1' of type 'i16', expected to match parameter 'a' of type: 'i32'\nLGERR_TS_INT002 Incompatible types 'i16' -> 'i32'. Explicit cast is required, conversion loses integer precision.");
        // std::cerr << x.message << std::endl;
    }

    {
        // function xxx2(i32 a, i32 b = 100)
        // xxx(b = i32)
        auto x = match(callexpr3, xxx2_fn, false);
        EXPECT_TRUE(x.is_error());
        EXPECT_STREQ(x.message.c_str(), "LGERR_MD004 Missing required argument 'a'. Calling 'function xxx (i32 a, i32 b = ?) i32'");
        // std::cerr << x.message << std::endl;
    }

    {
        // function xxx2(i32 a, i32 b = 100)
        // xxx(a = i32, b = i32)
        auto x = match(callexpr4, xxx2_fn, false);
        EXPECT_TRUE(!x.is_error());
        // std::cerr << x.message << std::endl;
    }

    // now we modify callexpr to match the function!
    {
        // function xxx2(i32 a, i32 b = 100)
        // xxx2(i32)
        EXPECT_EQ(callexpr->argument_count, 1);
        auto x = match(callexpr, xxx2_fn, true);
        EXPECT_TRUE(!x.is_error());
        EXPECT_EQ(callexpr->argument_count, 2);
        EXPECT_EQ(callexpr->get_argument_by_index(0)->get_value(), i32_v);
        EXPECT_EQ(callexpr->get_argument_by_index(1)->get_value(), i32_v2);

        // it's obvious, but we should be able to call it again!
        auto x2 = match(callexpr, xxx2_fn, true);
        EXPECT_TRUE(!x2.is_error());
        EXPECT_EQ(callexpr->argument_count, 2);
        EXPECT_EQ(callexpr->get_argument_by_index(0)->get_value(), i32_v);
        EXPECT_EQ(callexpr->get_argument_by_index(1)->get_value(), i32_v2);
    }

    // now we modify callexpr to match the function!
    {
        // function xxx2(i32 a, i32 b = 100)
        // xxx2(i32)
        EXPECT_EQ(callexpr4->argument_count, 2);
        auto x = match(callexpr4, xxx2_fn, true);
        EXPECT_TRUE(!x.is_error());
        EXPECT_EQ(callexpr4->argument_count, 2);
        EXPECT_EQ(callexpr4->get_argument_by_index(0)->get_value(), i32_v);
        EXPECT_EQ(callexpr4->get_argument_by_index(1)->get_value(), i32_v);

        // it's obvious, but we should be able to call it again!
        auto x2 = match(callexpr4, xxx2_fn, true);
        EXPECT_TRUE(!x2.is_error());
        EXPECT_EQ(callexpr4->argument_count, 2);
        EXPECT_EQ(callexpr4->get_argument_by_index(0)->get_value(), i32_v);
        EXPECT_EQ(callexpr4->get_argument_by_index(1)->get_value(), i32_v);
    }
}
TEST(logia_test_multiple_dispatch, find001)
{
    LOGIA_UNIT_TEST();

    using namespace logia::multiple_dispatch;
    using namespace logia::AST;
    using namespace logia::type_system;

    auto i16_v = new IntegerLiteral(loc, "10", i16);
    auto i32_v = new IntegerLiteral(loc, "10", i32);
    auto i32_v2 = new IntegerLiteral(loc, "10", i32);
    auto i64_v = new IntegerLiteral(loc, "10", i64);

    auto fn_a_i32 = new Function(loc, new Identifier(loc, "xxx"), i32, false);
    fn_a_i32->push_parameter(new FunctionParameter(new Identifier(loc, "a"), i32, nullptr));
    program->push_child(fn_a_i32);

    auto callexpr = new CallExpression(loc, new Identifier(loc, "xxx"), {i32_v});
    main_body->push_child(callexpr);

    /*
        auto fn_a_i32_b_i32 = new Function(loc, new Identifier(loc, "xxx"), i32, false);
        fn_a_i32_b_i32->push_parameter(new FunctionParameter(new Identifier(loc, "a"), i32, nullptr));
        fn_a_i32_b_i32->push_parameter(new FunctionParameter(new Identifier(loc, "b"), i32, i32_v2));
        program->push_child(fn_a_i32_b_i32);


        {
            // function xxx(i32 a)
            // function xxx(i32 a, i32)
            // xxx(i32) -> found first function!
            auto x = find(callexpr);
            EXPECT_EQ(x, fn_a_i32);
        }
    */
    auto fn_a_i64 = new Function(loc, new Identifier(loc, "xxx"), i64, false);
    fn_a_i64->push_parameter(new FunctionParameter(new Identifier(loc, "a"), i64, nullptr));
    program->push_child(fn_a_i64);

    {
        // i32 should be used
        // i64 should be discarded
        auto list = callexpr->find_candidates();
        auto result = find_one(list, callexpr);
        EXPECT_TRUE(result.is_success());
        EXPECT_EQ(result.unwrap_success(), fn_a_i32);
    }

    auto fn_a_i16 = new Function(loc, new Identifier(loc, "xxx"), i16, false);
    fn_a_i16->push_parameter(new FunctionParameter(new Identifier(loc, "a"), i16, nullptr));
    program->push_child(fn_a_i16);

    {
        // function xxx(i32 a)
        // function xxx(i64 a)
        // function xxx(i32 a, i32)
        // xxx(i32) -> found first function!
        auto list = callexpr->find_candidates();
        auto result = find_one(list, callexpr);
        EXPECT_TRUE(result.is_error());
        EXPECT_NE(std::string(result.message).find("LGERR_MD002 Ambiguous call expression, multiple candidates found"), std::string::npos);
    }

    {
        // TODO
        // callexpr->set_type_hint(i64);
        auto list = callexpr->find_candidates();
        auto result = find_one(list, callexpr);
        EXPECT_TRUE(result.is_error());
        EXPECT_NE(std::string(result.message).find("LGERR_MD002 Ambiguous call expression, multiple candidates found"), std::string::npos);
    }
}
/*
TEST(logia_run_error_file, type_system)
{
    {
        auto msg = test_file_with_semantic_error(".\\tests\\logia-errors\\", ".\\tests\\tmp\\", ".\\tests\\tmp\\", "err-LGERR_TS002-001");
        EXPECT_NE(std::string(msg).find("LGERR_TS002 Incompatible types 'i32' -> 'i64'. Explicit cast is required, conversion loses integer precision."), std::string::npos);
        EXPECT_NE(std::string(msg).find("err-LGERR_TS002-001.logia:7:4"), std::string::npos);
    }
}
    */