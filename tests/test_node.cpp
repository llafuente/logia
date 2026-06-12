// ./build-debug/bin/Debug/logia_test_suite.exe --gtest_break_on_failure

#include "logia/frontend.h"
#include "logia/backend.h"

#include "logia/ast/node.h"
#include "logia/ast/if_stmt.h"
#include "logia/ast/expr.h"
#include "logia/ast/constexpr.h"
#include "logia/ast/stmt.h"
#include "logia/ast/identifier.h"
#include "logia/ast/operators.h"
#include "logia/ast/binaryexpr.h"
#include "logia/ast/unaryexpr.h"
#include "logia/ast/struct.h"

#include "gtest/gtest.h"
#include <Windows.h>

#include "test_utils.h"
#include "logia/run.h"

TEST(test_node, test_1)
{
    LOGIA_UNIT_TEST();
    using namespace logia::AST;

    int size = program->children.size();
    program->push_child(ast_create_identifier("a"));
    program->push_child(ast_create_binary_expr(ast_create_identifier("a"), Operators::BINARY_ADD, ast_create_identifier("b")));
    program->push_child(ast_create_identifier("c"));

    EXPECT_TRUE(program->get_child<Identifier>(size));
    EXPECT_TRUE(program->get_child<Expression>(size));
    EXPECT_TRUE(program->get_child<Node>(size));

    EXPECT_TRUE(program->get_child<BinaryExpression>(size + 1));
    EXPECT_TRUE(program->get_child<Expression>(size + 1));
    EXPECT_TRUE(program->get_child<Node>(size + 1));

    EXPECT_THROW(program->get_child<UnaryExpression>(size + 1), std::runtime_error);

    int count = 0;
    program->intrinsics->foreach_child<Integer>([&count](auto p)
                                                { ++count; });

    EXPECT_EQ(count, 9);

    count = 0;
    program->foreach_child<Identifier>([&count](auto p)
                                       { ++count; });

    EXPECT_EQ(count, 2);

    count = 0;
    program->foreach_child<Expression>([&count](auto p)
                                       { ++count; });

    EXPECT_EQ(count, 3);

    // EXPECT_EQ(program->get_child<Integer>(0)->first_parent<Program>(), program);

    EXPECT_EQ(program->first_child<Function>()->get_body()->first_parent<Program>(), program);

    LOGIA_UNIT_TEST_END();
}

TEST(test_constexpr, integer_literals)
{
    LOGIA_UNIT_TEST();
    using namespace logia::AST;

    auto i_10 = new IntegerLiteral(loc, "10");
    auto i_11 = new IntegerLiteral(loc, "11");
    auto i_5 = new IntegerLiteral(loc, "5");
    auto i_7 = new IntegerLiteral(loc, "7");

    EXPECT_TRUE(i_10->is_valid_constant_operator(Operators::BINARY_ADD));
    {
        auto result = i_10->operator+(i_11)->as<IntegerLiteral>();

        EXPECT_STREQ(result->value_str, "21");
        EXPECT_EQ(result->value.getSExtValue(), (int64_t)21);
    }
    EXPECT_TRUE(i_10->is_valid_constant_operator(Operators::BINARY_SUB));
    {
        auto result = i_10->operator-(i_11)->as<IntegerLiteral>();
        // TODO REVIEW this is the expected value
        // EXPECT_STREQ(result->value_str, "-1");
        EXPECT_STREQ(result->value_str, "18446744073709551615");
        EXPECT_EQ(result->value.getSExtValue(), (int64_t)-1);
    }
    EXPECT_TRUE(i_10->is_valid_constant_operator(Operators::BINARY_MUL));
    {

        auto result = i_10->operator*(i_11)->as<IntegerLiteral>();

        EXPECT_STREQ(result->value_str, "110");
        EXPECT_EQ(result->value.getSExtValue(), (int64_t)110);
    }
    EXPECT_TRUE(i_10->is_valid_constant_operator(Operators::BINARY_DIV));
    {
        auto result = i_10->operator/(i_5)->as<IntegerLiteral>();

        EXPECT_STREQ(result->value_str, "2");
        EXPECT_EQ(result->value.getSExtValue(), (int64_t)2);
    }

    LOGIA_UNIT_TEST_END();
}

TEST(test_constexpr, float_literals)
{
    LOGIA_UNIT_TEST();
    using namespace logia::AST;

    auto i_10 = new FloatLiteral(loc, "10.8");
    auto i_11 = new FloatLiteral(loc, "11.7");
    auto i_5 = new FloatLiteral(loc, "5.6");
    auto i_7 = new FloatLiteral(loc, "7.4");

    EXPECT_TRUE(i_10->is_valid_constant_operator(Operators::BINARY_ADD));
    {
        auto result = i_10->operator+(i_11)->as<FloatLiteral>();

        EXPECT_STREQ(result->value_str, "22.5");
        EXPECT_NEAR(result->value.convertToDouble(), 10.8 + 11.7, 0.01);
    }
    EXPECT_TRUE(i_10->is_valid_constant_operator(Operators::BINARY_SUB));
    {
        auto result = i_10->operator-(i_11)->as<FloatLiteral>();
        EXPECT_STREQ(result->value_str, "-0.89999999999999858");
        EXPECT_NEAR(result->value.convertToDouble(), 10.8 - 11.7, 0.01);
    }
    EXPECT_TRUE(i_10->is_valid_constant_operator(Operators::BINARY_MUL));
    {

        auto result = i_10->operator*(i_11)->as<FloatLiteral>();

        EXPECT_STREQ(result->value_str, "126.36");
        EXPECT_NEAR(result->value.convertToDouble(), 10.8 * 11.7, 0.01);
    }
    EXPECT_TRUE(i_10->is_valid_constant_operator(Operators::BINARY_DIV));
    {
        auto result = i_10->operator/(i_5)->as<FloatLiteral>();

        EXPECT_STREQ(result->value_str, "1.9285714285714288");
        EXPECT_NEAR(result->value.convertToDouble(), 10.8 / 5.6, 0.01);
    }

    LOGIA_UNIT_TEST_END();
}

TEST(test_constexpr, string_literals)
{
    LOGIA_UNIT_TEST();
    using namespace logia::AST;

    auto a = new StringLiteral(loc, "10");
    auto b = new StringLiteral(loc, "11");

    EXPECT_TRUE(a->is_valid_constant_operator(Operators::BINARY_ADD));

    auto result = a->operator+(b)->as<StringLiteral>();

    EXPECT_STREQ(result->text, "1011");

    LOGIA_UNIT_TEST_END();
}

TEST(test_constexpr, float_limits)
{
    LOGIA_UNIT_TEST();
    using namespace logia::AST;
    {
        auto a = new FloatLiteral(loc, "-infinity");
        EXPECT_TRUE(a->value.isInfinity());
    }
    {
        auto a = new FloatLiteral(loc, "infinity");
        EXPECT_TRUE(a->value.isInfinity());
    }
    {
        auto a = new FloatLiteral(loc, "nan");
        EXPECT_TRUE(a->value.isNaN());
    }
    {
        auto a = new FloatLiteral(loc, "-nan");
        EXPECT_TRUE(a->value.isNaN());
    }

    LOGIA_UNIT_TEST_END();
}

TEST(test_type, function)
{
    LOGIA_UNIT_TEST();
    using namespace logia::AST;
    main_fn->is_attached = false; // avoid throw

    auto a = new FunctionParameter(new Identifier(loc, "a"), i32, nullptr);
    main_fn->insert_parameter(0, a);
    EXPECT_EQ(a->index, 0);

    auto b = new FunctionParameter(new Identifier(loc, "b"), i32, nullptr);
    main_fn->insert_parameter(0, b);
    EXPECT_EQ(b->index, 0);
    EXPECT_EQ(a->index, 1);

    auto c = new FunctionParameter(new Identifier(loc, "c"), i32, nullptr);
    main_fn->insert_parameter(0, c);

    EXPECT_EQ(a->index, 2);
    EXPECT_EQ(b->index, 1);
    EXPECT_EQ(c->index, 0);

    auto d = new FunctionParameter(new Identifier(loc, "d"), i32, nullptr);
    main_fn->insert_parameter(2, d);
    EXPECT_EQ(a->index, 3);
    EXPECT_EQ(d->index, 2);
    EXPECT_EQ(b->index, 1);
    EXPECT_EQ(c->index, 0);

    main_fn->is_attached = true;
    EXPECT_STREQ(main_fn->get_repr().c_str(), "function main (i32 c, i32 b, i32 d, i32 a) i32");

    LOGIA_UNIT_TEST_END();
}
