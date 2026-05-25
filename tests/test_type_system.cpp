// ./build-debug/bin/Debug/logia_test_suite.exe --gtest_break_on_failure

#include "logia/ast/node.h"
#include "logia/ast/type.h"
#include "logia/ast/expr.h"
#include "logia/ast/identifier.h"
#include "logia/ast/constexpr.h"
#include "logia/ast/struct.h"

#include "logia/maybe_error.h"
#include "logia/type_system.h"

#include "gtest/gtest.h"
#include <Windows.h>

#include "test_utils.h"
#include "logia/run.h"

TEST(logia_test_type_system, start)
{
    using namespace logia::AST;
    using namespace logia::type_system;

    auto rule = new antlr4::ParserRuleContext();
    rule->start = new antlr4::CommonToken(102);
    rule->stop = new antlr4::CommonToken(102);
    //    rule->start = antlr4::TokenFactory<antlr4::Token>.create(std::pair<TokenSource *, CharStream *> source, size_t type, const std::string &text, size_t channel, size_t start, size_t stop, size_t line, size_t charPositionInLine)

    auto i32 = new logia::AST::Integer(true, 32);
    auto u32 = new logia::AST::Integer(false, 32);
    auto f32 = new logia::AST::Float(32);
    auto i64 = new logia::AST::Integer(true, 64);
    auto f64 = new logia::AST::Float(64);

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
        EXPECT_STREQ(err.message.c_str(), "LGERR_TS001 Incompatible types 'i32' -> 'u32'. Explicit cast is required, conversion changes signedness.");
    }
    {
        auto err = type_is_compatible(u32, i32);
        EXPECT_TRUE(err.is_error());
        EXPECT_STREQ(err.message.c_str(), "LGERR_TS001 Incompatible types 'u32' -> 'i32'. Explicit cast is required, conversion changes signedness.");
    }
    {
        auto err = type_is_compatible(i32, i64);
        EXPECT_TRUE(err.is_error());
        EXPECT_STREQ(err.message.c_str(), "LGERR_TS002 Incompatible types 'i32' -> 'i64'. Explicit cast is required, conversion loses integer precision.");
    }
    {
        auto err = type_is_compatible(f32, f64);
        EXPECT_TRUE(err.is_error());
        EXPECT_STREQ(err.message.c_str(), "LGERR_TS003 Incompatible types 'f32' -> 'f64'. Explicit cast is required, conversion loses floating-point precision.");
    }

    // {x,y,z}
    auto st1_i32 = new logia::AST::Struct(rule, new logia::AST::Identifier(rule, "st1_i32"));
    st1_i32->add_field(rule, new logia::AST::Identifier(rule, "x"), i32);
    st1_i32->add_field(rule, new logia::AST::Identifier(rule, "y"), i32);
    st1_i32->add_field(rule, new logia::AST::Identifier(rule, "z"), i32);

    // {x,y,(z)} <-- compatible at code level!
    auto st2_i32 = new logia::AST::Struct(rule, new logia::AST::Identifier(rule, "st2_i32"));
    st2_i32->add_field(rule, new logia::AST::Identifier(rule, "x"), i32);
    st2_i32->add_field(rule, new logia::AST::Identifier(rule, "y"), i32);
    st2_i32->add_alias(rule, new logia::AST::Identifier(rule, "z"), new logia::AST::Identifier(rule, "y"), nullptr);

    // {x,y,g} <-- incompatible at code level! - compatible at layout!
    auto st3_i32 = new logia::AST::Struct(rule, new logia::AST::Identifier(rule, "st3_i32"));
    st3_i32->add_field(rule, new logia::AST::Identifier(rule, "x"), i32);
    st3_i32->add_field(rule, new logia::AST::Identifier(rule, "y"), i32);
    st3_i32->add_field(rule, new logia::AST::Identifier(rule, "g"), i32);

    // {x,y,g} <-- incompatible at code level! - compatible at layout!
    auto st4_i32 = new logia::AST::Struct(rule, new logia::AST::Identifier(rule, "st3_i32"));
    st4_i32->add_field(rule, new logia::AST::Identifier(rule, "x"), i32);
    st4_i32->add_field(rule, new logia::AST::Identifier(rule, "y"), f32);
    st4_i32->add_field(rule, new logia::AST::Identifier(rule, "z"), i32);

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
        EXPECT_STREQ(err.message.c_str(), "LGERR_TS004 Incompatible types 'struct st1_i32 {x i32, y i32, z i32}' -> 'struct st2_i32 {x i32, y i32}'. Types has different fields count.");
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
        EXPECT_STREQ(err.message.c_str(), "LGERR_TS005 Incompatible types 'struct st1_i32 {x i32, y i32, z i32}' -> 'struct st3_i32 {x i32, y f32, z i32}'. Incompatible field at position: '1'.\nLGERR_TS002 Incompatible types 'i32' -> 'f32'. Types should have the same primitive.");
    }
}

TEST(logia_run_error_file, type_system)
{
    {
        auto msg = test_file_with_semantic_error(".\\tests\\logia-errors\\", ".\\tests\\tmp\\", ".\\tests\\tmp\\", "err-LGERR_TS002-001");
        EXPECT_NE(std::string(msg).find("LGERR_TS002 Incompatible types 'i32' -> 'i64'. Explicit cast is required, conversion loses integer precision."), std::string::npos);
        EXPECT_NE(std::string(msg).find("err-LGERR_TS002-001.logia:7:4"), std::string::npos);
    }
}