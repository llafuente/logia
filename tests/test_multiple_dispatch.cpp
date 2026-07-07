// ./build-debug/bin/Debug/logia_test_suite.exe --gtest_break_on_failure

#include "logia/ast/node.h"
#include "logia/ast/type.h"
#include "logia/ast/expr.h"
#include "logia/ast/constexpr.h"
#include "logia/ast/callexpr.h"
#include "logia/ast/identifier.h"

#include "logia/maybe_error.h"
#include "logia/type_system.h"
#include "logia/type_inference.h"
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
    program->unshift_child(xxx_fn);

    auto xxx2_fn = new Function(loc, new Identifier(loc, "xxx"), i32, false);
    xxx2_fn->push_parameter(new FunctionParameter(new Identifier(loc, "a"), i32, nullptr));
    xxx2_fn->push_parameter(new FunctionParameter(new Identifier(loc, "b"), i32, i32_v2));
    program->unshift_child(xxx2_fn);

    logia::type_inference_program(program);

    CallExpression *prev_callexpr = nullptr;
    {
        auto callexpr = new CallExpression(loc, new Identifier(loc, "xxx"), {i32_v});
        prev_callexpr = callexpr;
        // now one by one we type each node!
        main_body->unshift_child(callexpr); // attach before type_inference! "Cannot search from a detached node"

        logia::type_inference_node(program, callexpr->get_locator());
        logia::type_inference_node(program, callexpr->get_argument_by_index(0));
        {
            // function xxx(i32 a)
            // xxx(i32)
            auto x = match(callexpr, xxx_fn, false);
            EXPECT_TRUE(!x.is_error());
        }

        {
            // function xxx(i32 a, i32 b = ?)
            // xxx(i32)
            auto x = match(callexpr, xxx2_fn, false);
            EXPECT_TRUE(!x.is_error());
        }
        // after match we should have all arguments named
        EXPECT_EQ(callexpr->get_argument_by_name("a"), nullptr);
        EXPECT_EQ(callexpr->get_argument_by_name("b"), nullptr);
        auto x = match(callexpr, xxx2_fn, true);
        EXPECT_NE(callexpr->get_argument_by_name("a"), nullptr);
        EXPECT_NE(callexpr->get_argument_by_name("b"), nullptr);
    }

    {

        // xxx(i16)
        auto callexpr = new CallExpression(loc, new Identifier(loc, "xxx"), {i16_v});
        prev_callexpr->replace_self(callexpr);
        prev_callexpr = callexpr;

        logia::type_inference_node(program, callexpr->get_locator());
        logia::type_inference_node(program, callexpr->get_argument_by_index(0));

        // function xxx(i32 a)
        auto x = match(callexpr, xxx_fn, false);
        EXPECT_TRUE(x.is_error());
        EXPECT_STREQ(x.message.c_str(), "LGERR_MD001 Invalid argument type at position '1' of type 'i16', expected to match parameter 'a' of type: 'i32'\nLGERR_TS_INT002 Incompatible types 'i16' -> 'i32'. Explicit cast is required, conversion loses integer precision.");
        // std::cerr << x.message << std::endl;
    }

    {
        auto callexpr = new CallExpression(loc, new Identifier(loc, "xxx"), {});
        prev_callexpr->replace_self(callexpr);
        prev_callexpr = callexpr;

        callexpr->push_named_argument(new Identifier(loc, "b"), i32_v);
        {
            // function xxx(i32 a)
            auto x = match(callexpr, xxx_fn, false);
            EXPECT_TRUE(x.is_error());
            EXPECT_STREQ(x.message.c_str(), "LGERR_MD002 Invalid argument name 'b', could not be found in the function: 'function xxx (i32 a) i32'.");
        }
        {
            // function xxx(i32 a, i32 b)
            auto x = match(callexpr, xxx2_fn, false);
            EXPECT_TRUE(x.is_error());
            EXPECT_STREQ(x.message.c_str(), "LGERR_MD004 Missing required argument 'a'. Calling 'function xxx (i32 a, i32 b = ?) i32'");
        }
    }
    {
        auto callexpr = new CallExpression(loc, new Identifier(loc, "xxx"), {});
        prev_callexpr->replace_self(callexpr);
        prev_callexpr = callexpr;

        callexpr->push_named_argument(new Identifier(loc, "a"), i32_v);
        callexpr->push_named_argument(new Identifier(loc, "b"), i32_v);

        {
            // function xxx(i32 a)
            auto x = match(callexpr, xxx_fn, false);
            EXPECT_TRUE(x.is_error());
            EXPECT_STREQ(x.message.c_str(), "LGERR_MD003 Invalid argument count. 'function xxx (i32 a) i32' takes '1' arguments but '2' were given.");
        }
        {
            // function xxx(i32 a, i32 b)
            auto x = match(callexpr, xxx2_fn, false);
            EXPECT_TRUE(x.is_success());
        }
    }
}

TEST(logia_test_multiple_dispatch, find001)
{
    LOGIA_UNIT_TEST();

    using namespace logia::multiple_dispatch;
    using namespace logia::AST;
    using namespace logia::type_system;

    auto i16_v = new IntegerLiteral(loc, "10", i16);
    EXPECT_EQ(i16_v->get_type(), nullptr); // even if we sent the type, type_inference is not started and it's not a typedecl!
    auto i32_v = new IntegerLiteral(loc, "10", i32);
    auto i32_v2 = new IntegerLiteral(loc, "10", i32);
    auto i64_v = new IntegerLiteral(loc, "10", i64);

    // function xxx(i32 a) i32
    auto fn_a_i32 = new Function(loc, new Identifier(loc, "xxx"), i32, false);
    fn_a_i32->push_parameter(new FunctionParameter(new Identifier(loc, "a"), i32, nullptr));
    program->unshift_child(fn_a_i32);

    // function xxx(i64 a) i64
    auto fn_a_i64 = new Function(loc, new Identifier(loc, "xxx"), i64, false);
    fn_a_i64->push_parameter(new FunctionParameter(new Identifier(loc, "a"), i64, nullptr));
    program->push_child(fn_a_i64);
    /*
    // function xxx(i32 a, i32 b = 10) i32
    auto fn_a_i32_b_i32 = new Function(loc, new Identifier(loc, "xxx"), i32, false);
    fn_a_i32_b_i32->push_parameter(new FunctionParameter(new Identifier(loc, "a"), i32, nullptr));
    fn_a_i32_b_i32->push_parameter(new FunctionParameter(new Identifier(loc, "b"), i32, i32_v2));
    program->push_child(fn_a_i32_b_i32);
    */
    auto callexpr = new CallExpression(loc, new Identifier(loc, "xxx"), {i32_v});
    main_body->unshift_child(callexpr);

    logia::type_inference_program(program);
    // untype!
    logia::type_inference_untype(callexpr);
    logia::type_inference_untype(callexpr->get_locator());

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