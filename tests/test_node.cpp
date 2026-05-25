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
#include "logia/ast/postfixunaryexpr.h"

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

    EXPECT_THROW(program->get_child<PostfixUnaryExpression>(size + 1), std::runtime_error);

    int count = 0;
    program->intrinsics->foreach_child<Integer>([&count](auto p)
                                                { ++count; });

    EXPECT_EQ(count, 11);

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
