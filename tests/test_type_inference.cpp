// ./build-debug/bin/Debug/logia_test_suite.exe --gtest_break_on_failure
#include "logia/frontend.h"
#include "logia/backend.h"
#include "ast/node.h"
#include "ast/traverse.h"
#include "ast/if_stmt.h"
#include "ast/type.h"
#include "ast/expr.h"
#include "ast/constexpr.h"
#include "ast/stmt.h"
#include "ast/program.h"
#include "logia/type_inference.h"

#include "gtest/gtest.h"
#include <Windows.h>

#include "test_utils.h"

// hello world example
TEST(AST, type_inference)
{
  LOGIA_BACKEND_START();
  using namespace logia::AST;
  {
    auto ten = new IntegerLiteral(nullptr, "10", new InferType());
    auto eleven = new IntegerLiteral(nullptr, "11", new InferType());
    auto bin = new BinaryExpression(nullptr, ten, Operators::BINARY_ADD, eleven);
    program->push_child(bin);
  }
  logia::type_inference_program(program);

  delete back;
}
