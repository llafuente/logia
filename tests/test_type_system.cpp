// ./build-debug/bin/Debug/logia_test_suite.exe --gtest_break_on_failure

#include "logia/ast/node.h"
#include "logia/ast/type.h"
#include "logia/ast/expr.h"
#include "logia/ast/identifier.h"
#include "logia/ast/constexpr.h"
#include "logia/ast/struct.h"

#include "logia/maybe_error.h"
#include "logia/type_system.h"

#include <gtest/gtest.h>
#include <gmock/gmock.h> // For matchers like HasSubstr
#include <Windows.h>

#include "test_utils.h"
#include "logia/run.h"

TEST(logia_test_type_system, start)
{
    // NOTE use primitives, not real but test APIs
    using namespace logia::AST;
    using namespace logia::type_system;

    location loc = {"", 0, 0, 0, 0, 0, 0, ""};

    auto i32 = new logia::AST::Integer(true, 32);
    auto u16 = new logia::AST::Integer(false, 16);
    auto u32 = new logia::AST::Integer(false, 32);
    auto u64 = new logia::AST::Integer(false, 64);
    auto f32 = new logia::AST::Float(32);
    auto i64 = new logia::AST::Integer(true, 64);
    auto f64 = new logia::AST::Float(64);

    auto u32_ref = new logia::AST::Ref(u32);
    auto u64_ref = new logia::AST::Ref(u64);
    auto u16_ref = new logia::AST::Ref(u16);
    auto i32_ref = new logia::AST::Ref(i32);
    auto i64_ref = new logia::AST::Ref(i64);

    {
        EXPECT_EQ(type_is_compatible(i64, i64_ref).unwrap_success(), (type_compatibility)((uint32_t)type_compatibility::YES | (uint32_t)type_compatibility::AUTOCAST_REF));
        EXPECT_EQ(type_is_compatible(i64_ref, i64).unwrap_success(), (type_compatibility)((uint32_t)type_compatibility::YES | (uint32_t)type_compatibility::AUTOCAST_DEREF));
    }
    {
        auto err = type_is_compatible(i64, i32_ref);
        EXPECT_EQ(err.is_error(), true);
        EXPECT_STREQ(err.message.c_str(), "LGERR_TS_PTR001 Incompatible types 'λi64' -> 'ref<λi32>'. Pointee types are incompatible.");
    }
    {
        EXPECT_EQ(type_is_compatible(i64, i32).unwrap_success(), type_compatibility::AUTOCAST_CAST);
        EXPECT_EQ(type_is_compatible(f64, f32).unwrap_success(), type_compatibility::AUTOCAST_CAST);
    }
    {
        EXPECT_EQ(type_is_compatible(i32, i32).unwrap_success(), (type_compatibility)((uint32_t)type_compatibility::YES | (uint32_t)type_compatibility::LAYOUT_COMPATIBLE));
        EXPECT_EQ(type_is_compatible(f32, f32).unwrap_success(), (type_compatibility)((uint32_t)type_compatibility::YES | (uint32_t)type_compatibility::LAYOUT_COMPATIBLE));
    }

    {
        auto err = type_is_compatible(i32, u32);
        EXPECT_TRUE(err.is_error());
        EXPECT_STREQ(err.message.c_str(), "LGERR_TS_INT001 Incompatible types 'λi32' -> 'λu32'. Explicit cast is required, conversion changes signedness.");
    }
    {
        auto err = type_is_compatible(u32, i32);
        EXPECT_TRUE(err.is_error());
        EXPECT_STREQ(err.message.c_str(), "LGERR_TS_INT001 Incompatible types 'λu32' -> 'λi32'. Explicit cast is required, conversion changes signedness.");
    }
    {
        auto err = type_is_compatible(i32, i64);
        EXPECT_TRUE(err.is_error());
        EXPECT_STREQ(err.message.c_str(), "LGERR_TS_INT002 Incompatible types 'λi32' -> 'λi64'. Explicit cast is required, conversion loses integer precision.");
    }
    {
        auto err = type_is_compatible(f32, f64);
        EXPECT_TRUE(err.is_error());
        EXPECT_STREQ(err.message.c_str(), "LGERR_TS_FLT001 Incompatible types 'λf32' -> 'λf64'. Explicit cast is required, conversion loses floating-point precision.");
    }

    {
        auto err = type_is_compatible(i32_ref, i64_ref);
        EXPECT_TRUE(err.is_error());
        EXPECT_STREQ(err.message.c_str(), "LGERR_TS_PTR001 Incompatible types 'ref<λi32>' -> 'ref<λi64>'. Pointee types are incompatible.\nLGERR_TS_INT002 Incompatible types 'λi32' -> 'λi64'. Explicit cast is required, conversion loses integer precision.");
    }
    {
        auto err = type_is_compatible(i64_ref, i32_ref);
        EXPECT_TRUE(err.is_error());
        EXPECT_STREQ(err.message.c_str(), "LGERR_TS_PTR001 Incompatible types 'ref<λi64>' -> 'ref<λi32>'. Pointee types are incompatible.");
    }

    // {x,y,z}
    auto st1_i32 = new logia::AST::Struct(loc, new logia::AST::Identifier(loc, "st1_i32"));
    st1_i32->add_field(loc, new logia::AST::Identifier(loc, "x"), i32);
    st1_i32->add_field(loc, new logia::AST::Identifier(loc, "y"), i32);
    st1_i32->add_field(loc, new logia::AST::Identifier(loc, "z"), i32);

    // {x,y,(z)} <-- compatible at code level!
    auto st2_i32 = new logia::AST::Struct(loc, new logia::AST::Identifier(loc, "st2_i32"));
    st2_i32->add_field(loc, new logia::AST::Identifier(loc, "x"), i32);
    st2_i32->add_field(loc, new logia::AST::Identifier(loc, "y"), i32);
    st2_i32->add_alias(loc, new logia::AST::Identifier(loc, "z"), new logia::AST::Identifier(loc, "y"), nullptr);

    // {x,y,g} <-- incompatible at code level! - compatible at layout!
    auto st3_i32 = new logia::AST::Struct(loc, new logia::AST::Identifier(loc, "st3_i32"));
    st3_i32->add_field(loc, new logia::AST::Identifier(loc, "x"), i32);
    st3_i32->add_field(loc, new logia::AST::Identifier(loc, "y"), i32);
    st3_i32->add_field(loc, new logia::AST::Identifier(loc, "g"), i32);

    // {x,y,g} <-- incompatible at code level! - compatible at layout!
    auto st4_i32 = new logia::AST::Struct(loc, new logia::AST::Identifier(loc, "st3_i32"));
    st4_i32->add_field(loc, new logia::AST::Identifier(loc, "x"), i32);
    st4_i32->add_field(loc, new logia::AST::Identifier(loc, "y"), f32);
    st4_i32->add_field(loc, new logia::AST::Identifier(loc, "z"), i32);

    {
        auto err = type_is_compatible(st1_i32, st1_i32);
        EXPECT_FALSE(err.is_error());
    }
    {
        auto err = type_is_compatible(st2_i32, st2_i32);
        EXPECT_FALSE(err.is_error());
    }
    {
        auto err = type_is_compatible(st3_i32, st3_i32);
        EXPECT_FALSE(err.is_error());
    }
    {
        auto err = type_is_compatible(st4_i32, st4_i32);
        EXPECT_FALSE(err.is_error());
    }

    {
        auto err = type_is_compatible(st1_i32, st2_i32);
        EXPECT_TRUE(err.is_error());
        EXPECT_STREQ(err.message.c_str(), "LGERR_TS_ST001 Incompatible types 'struct st1_i32 {λi32 x, λi32 y, λi32 z}' -> 'struct st2_i32 {λi32 x, λi32 y}'. Types has different fields count.");
    }

    {
        // NOTE this may change in the future, as this may be CODE_INCOMPATIBLE
        auto err = type_is_compatible(st1_i32, st3_i32);
        EXPECT_TRUE(!err.is_error());
        EXPECT_STREQ(err.message.c_str(), "");
    }
    {
        auto err = type_is_compatible(st1_i32, st4_i32);
        EXPECT_TRUE(err.is_error());
        EXPECT_STREQ(err.message.c_str(), "LGERR_TS_ST002 Incompatible types 'struct st1_i32 {λi32 x, λi32 y, λi32 z}' -> 'struct st3_i32 {λi32 x, λf32 y, λi32 z}'. Incompatible field at position: '1'.\nLGERR_TS001 Incompatible types 'λi32' -> 'λf32'. Types should have the same primitive.");
    }
}

TEST(logia_run_error_file, type_system)
{
    {
        auto msg = test_file_with_semantic_error(".\\tests\\logia-errors\\", ".\\tests\\tmp\\", ".\\tests\\tmp\\", "err-LGERR_TS_INT002-001");
        // std::cerr << msg;
        EXPECT_THAT(msg, testing::HasSubstr("LGERR_TS_INT002 Incompatible types 'λi32' -> 'λi64'. Explicit cast is required, conversion loses integer precision."));
        EXPECT_THAT(msg, testing::HasSubstr("err-LGERR_TS_INT002-001.logia:6:5"));
    }
}

TEST(logia_test_type_system, function)
{
    // NOTE use primitives directly in this test, not real but it's just test APIs
    using namespace logia::AST;
    using namespace logia::type_system;

    location loc = {"", 0, 0, 0, 0, 0, 0, ""};
    auto i16 = new logia::AST::Integer(true, 16);
    auto i32 = new logia::AST::Integer(true, 32);
    auto f32 = new logia::AST::Float(32);
    auto the_void = new logia::AST::Void();

    auto fn_a = new Function(loc, new Identifier(loc, "a"), the_void, false);
    auto fn_b = new Function(loc, new Identifier(loc, "b"), the_void, false);
    auto fn_c = new Function(loc, new Identifier(loc, "c"), the_void, false);
    {
        auto err = type_is_compatible(fn_a, fn_b);
        EXPECT_FALSE(err.is_error());
    }
    fn_a->push_parameter(new FunctionParameter(new Identifier(loc, "a"), i32));
    {
        auto err = type_is_compatible(fn_a, fn_b);
        EXPECT_TRUE(err.is_error());
        EXPECT_STREQ(err.message.c_str(), "LGERR_TS_FN001 Incompatible types 'function a (λi32 a) void' -> 'function b () void'. Parameter count mismatch.");
    }
    fn_b->push_parameter(new FunctionParameter(new Identifier(loc, "b"), i16));
    {
        auto err = type_is_compatible(fn_a, fn_b);
        EXPECT_TRUE(err.is_error());
        EXPECT_STREQ(err.message.c_str(), "LGERR_TS_FN003 Incompatible types 'function a (λi32 a) void' -> 'function b (λi16 b) void'. Argument 1 type mismatch.");
    }
    fn_c->push_parameter(new FunctionParameter(new Identifier(loc, "c"), f32));
    {
        auto err = type_is_compatible(fn_a, fn_c);
        EXPECT_TRUE(err.is_error());
        EXPECT_STREQ(err.message.c_str(), "LGERR_TS_FN003 Incompatible types 'function a (λi32 a) void' -> 'function c (λf32 c) void'. Argument 1 type mismatch.\nLGERR_TS001 Incompatible types 'λi32' -> 'λf32'. Types should have the same primitive.");
    }
}