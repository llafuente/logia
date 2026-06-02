// ./build-debug/bin/Debug/logia_test_suite.exe --gtest_break_on_failure

#include "logia/log.h"
#include "logia/frontend.h"
#include "logia/backend.h"
#include "logia/ast/node.h"
#include "logia/ast/if_stmt.h"
#include "logia/ast/type.h"
#include "logia/ast/expr.h"
#include "logia/ast/constexpr.h"
#include "logia/ast/stmt.h"
#include "logia/ast/program.h"
#include "logia/ast/operators.h"
#include "logia/ast/identifier.h"
#include "logia/ast/callexpr.h"
#include "logia/ast/returnstmt.h"
#include "logia/ast/vardeclstmt.h"
#include "logia/ast/binaryexpr.h"

#include "gtest/gtest.h"
#include <Windows.h>

#include "test_utils.h"

void expect_all_attached(logia::AST::Program *prg)
{
  prg->foreach_descendant([](auto n, int deep) -> bool
                          {
                            LOG(DBG, "{}", n->to_string());
      if (!n->is<logia::AST::Program>()) {
        EXPECT_TRUE(n->is_attached) << std::format(" not attached {}", n->to_string());
    }
      return true; });
}

// hello world example
TEST(logia_ast, create)
{
  LOGIA_UNIT_TEST();
  using namespace logia::AST;

  EXPECT_EQ(program->parent_node, nullptr);

  // has value
  EXPECT_TRUE(program->look<Type>("λi8"));
  // and is the same everytime
  EXPECT_EQ(program->look<Type>("λi8"), program->look<Type>("λi8"));

  expect_all_attached(program);
  LOGIA_UNIT_TEST_END();
}
/* @llafuente atm we need casting to enable this test!
TEST(logia_ast, create_function_type)
{
  LOGIA_UNIT_TEST();
  using namespace logia::AST;

  EXPECT_TRUE(main_fn);

  EXPECT_EQ(main_fn->primitive, Primitives::FUNCTION_TY);
  EXPECT_TRUE(strcmp(main_fn->get_name(), "main") == 0);

  // can look for main function as it's declared inside program
  EXPECT_EQ(main_fn, program->look<Type>("main"));

  // check parenting
  EXPECT_EQ(main_fn->get_identifier()->parent_node, main_fn);
  EXPECT_EQ(main_fn->get_return_type()->parent_node, main_fn);
  EXPECT_EQ(main_fn->get_body()->parent_node, main_fn);
  EXPECT_EQ(main_fn->get_body()->parentScope, program);

  // function body is connected to program, type available
  EXPECT_TRUE(main_fn->get_body()->lookup<Type>("λi8"));
  EXPECT_TRUE(main_fn->get_body()->lookup<Type>("λi64"));

  auto firstArg = new IntegerLiteral(nullptr, "17");
  auto secondArg = new IntegerLiteral(nullptr, "21");
  auto callFuncName = ast_create_identifier("logia_intrinsics_bin_add_i64_i64");

  EXPECT_EQ(firstArg->parent_node, nullptr);
  EXPECT_EQ(secondArg->parent_node, nullptr);
  EXPECT_EQ(callFuncName->parent_node, nullptr);
  auto callexpr = ast_create_call_expr(callFuncName, {firstArg, secondArg});
  EXPECT_EQ(callexpr->get_arguments().size(), 2);
  EXPECT_EQ(firstArg->parent_node->parent_node, callexpr);
  EXPECT_EQ(secondArg->parent_node->parent_node, callexpr);
  EXPECT_EQ(callFuncName->parent_node, callexpr);

  EXPECT_EQ(callexpr->parent_node, nullptr);
  auto ret_stmt = ast_create_return(callexpr);
  EXPECT_EQ(callexpr->parent_node, ret_stmt);

  EXPECT_EQ(ret_stmt->parent_node, nullptr);
  main_fn->get_body()->push_child(ret_stmt);
  EXPECT_EQ(ret_stmt->parent_node, main_fn->get_body());
  EXPECT_EQ(main_fn->get_body()->parent_node, main_fn);

  EXPECT_EQ(main_fn->get_identifier()->parent_node, main_fn);
  expect_all_attached(program);

  backend->emitTargetLLVMIR("./tmp/create_function_type.ll");
  backend->emitTargetObjectFile("./tmp/create_function_type.obj");
  backend->emitTargetAssemblyFile("./tmp/create_function_type.asm");

  EXPECT_THROW(backend->emitTargetExecutable("./tmp/create_function_type.exe"), std::runtime_error);

  backend->applyLLVMOptimizers();
  int exit_code = backend->run_jit("main");
  EXPECT_EQ(exit_code, 38);

  LOGIA_UNIT_TEST_END();
}

// hello world example
TEST(logia_ast, create_struct_type)
{
  LOGIA_UNIT_TEST();

  using namespace logia::AST;
  auto start_program_children = program->children.size();

  auto string_t = ast_create_struct_type(ast_create_identifier("string"));
  EXPECT_EQ(string_t->field_count, 0);
  string_t->add_field(nullptr, ast_create_identifier("capacity"), program->lookup<Type>("λi64"), nullptr, "");
  EXPECT_EQ(string_t->field_count, 1);
  EXPECT_EQ(string_t->get_field("capacity")->index, 0);

  string_t->add_field(nullptr, ast_create_identifier("length"), program->lookup<Type>("λi64"), nullptr, "");
  EXPECT_EQ(string_t->field_count, 2);
  EXPECT_EQ(string_t->get_field("length")->index, 1);

  string_t->add_field(nullptr, ast_create_identifier("value"), program->lookup<Type>("λptr"), nullptr, "");
  EXPECT_EQ(string_t->field_count, 3);
  EXPECT_EQ(string_t->get_field("value")->index, 2);

  EXPECT_EQ(string_t->get_field("xxx"), nullptr);
  program->push_child(string_t);

  // invalid ?
  // program->add_statement(string_t);

  auto func = program->look<Function>("main");
  EXPECT_TRUE(func);

  auto hello_world = ast_create_string_lit(strdup("Hello world!"));
  auto callFuncName = ast_create_identifier("logia_print_stdout");
  func->get_body()->push_child(ast_create_call_expr(callFuncName, {hello_world}));

  auto exit_code_value = new IntegerLiteral(nullptr, "0");
  func->get_body()->push_child(ast_create_return(exit_code_value));
  EXPECT_EQ(program->children.size(), start_program_children + 1);
  EXPECT_EQ(func->get_body()->children.size(), 2);

  expect_all_attached(program);

  backend->emitTargetLLVMIR("./tmp/struct.ll");

  // NOTE works, but for an unkown reason yet, we can't jit again.
  int exit_code = backend->run_jit("main");

  LOGIA_UNIT_TEST_END();
}

// hello world example
TEST(logia_ast, create_var_decl)
{
  LOGIA_UNIT_TEST();

  using namespace logia::AST;

  auto start_program_children = program->children.size();

  auto string_t = ast_create_struct_type(ast_create_identifier("string"));
  EXPECT_EQ(string_t->field_count, 0);
  string_t->add_field(nullptr, ast_create_identifier("capacity"), program->lookup<Type>("λi64"), nullptr, "");
  string_t->add_field(nullptr, ast_create_identifier("length"), program->lookup<Type>("λi64"), nullptr, "");
  string_t->add_field(nullptr, ast_create_identifier("value"), program->lookup<Type>("λptr"), nullptr, "");
  EXPECT_EQ(string_t->field_count, 3);

  EXPECT_EQ(string_t->alias_count, 0);
  auto aliased_capacity = ast_create_identifier("capacity");
  string_t->add_alias(nullptr, ast_create_identifier("cap"), aliased_capacity, "");
  EXPECT_EQ(string_t->alias_count, 1);
  string_t->add_alias(nullptr, ast_create_identifier("len"), ast_create_identifier("length"), "");
  EXPECT_EQ(string_t->alias_count, 2);

  EXPECT_EQ(string_t->get_alias_to("cap"), aliased_capacity);
  EXPECT_EQ(string_t->get_alias_to("xxx"), nullptr);
  EXPECT_EQ(string_t->get_field("cap")->index, 0);
  EXPECT_EQ(string_t->get_field("len")->index, 1);

  // invalid ?
  // program->add_statement(string_t);

  auto func = logia::AST::ast_create_function_type(ast_create_identifier("main"), program->look<Type>("λi32"));
  EXPECT_TRUE(func);

  program->push_child(func);

  EXPECT_EQ(func->get_body()->scope.size(), 0);
  auto hello_world = ast_create_string_lit(strdup("Hello world!"));
  auto vdecl = ast_create_var_decl(ast_create_identifier("hello"), string_t, hello_world);
  func->get_body()->push_child(vdecl);
  EXPECT_EQ(hello_world->parent_node, vdecl);
  EXPECT_EQ(vdecl->parent_node, func->get_body());
  EXPECT_EQ(func->get_body()->scope.size(), 2); // TODO REVIEW WHY?!

  // print static string
  auto callFuncName = ast_create_identifier("logia_print_stdout");
  auto hello_world2 = ast_create_string_lit("Hello world!");
  func->get_body()->push_child(ast_create_call_expr(callFuncName, {hello_world2}));
  EXPECT_EQ(hello_world2->parent_node->parent_node, func->get_body()->children[1]);
  EXPECT_EQ(callFuncName->parent_node, func->get_body()->children[1]);

  // print static string from variable
  auto callFuncName2 = ast_create_identifier("logia_print_stdout");
  auto x = (Expression *)ast_create_identifier("hello");
  func->get_body()->push_child(ast_create_call_expr(callFuncName2, {x}));
  EXPECT_EQ(x->parent_node->parent_node, func->get_body()->children[2]);

  auto exit_code_value = ast_create_int_lit(func->get_body(), "0");
  func->get_body()->push_child(ast_create_return(exit_code_value));

  EXPECT_EQ(program->children.size(), start_program_children + 1);
  EXPECT_EQ(func->get_body()->children.size(), 4);

  expect_all_attached(program);

  backend->emitTargetLLVMIR("./tmp/hellow-world-alloca.ll");

  int exit_code = backend->run_jit("main");
  EXPECT_EQ(exit_code, 0);

  LOGIA_UNIT_TEST_END();
}

// sum 15+20 as variables
TEST(AST_Type, ast_create_var_decl2)
{
  LOGIA_UNIT_TEST();
  using namespace logia::AST;

  auto str_a = "a";
  {
    auto value_a = new IntegerLiteral(rule, "11", i64);
    auto vdecl_a = ast_create_var_decl(ast_create_identifier(str_a), value_a->get_type(), value_a);
    main_body->push_child(vdecl_a);
  }

  auto str_b = "b";
  {
    auto value_b = ast_create_int_lit(program, "12");
    auto vdecl_b = ast_create_var_decl(ast_create_identifier(str_b), value_b->get_type(), value_b);
    main_body->push_child(vdecl_b);
  }

  // print static string from variable
  {
    auto ident_a = (Expression *)ast_create_identifier(str_a);
    auto ident_b = (Expression *)ast_create_identifier(str_b);

    auto callFuncName = ast_create_identifier("logia_intrinsics_bin_add_i64_i64");
    auto sum_expr = ast_create_call_expr(callFuncName, {ident_a, ident_b});

    main_fn->get_body()->push_child(ast_create_return(sum_expr));
  }

  expect_all_attached(program);

  backend->emitTargetLLVMIR("./tmp/alloca-integer-sum.ll");

  int exit_code = backend->run_jit("main");
  EXPECT_EQ(exit_code, 11 + 12);

  LOGIA_UNIT_TEST_END();
}

extern "C" int logia_compiler_to_jit_test()
{
  return 101;
}

// expose compiler functions to logia
TEST(logia_ast, logia_compiler_to_jit_test)
{
  LOGIA_UNIT_TEST();
  using namespace logia::AST;

  // this is how you create an intrinsic to use it comptime
  backend->add_intrinsic((void *)(&logia_compiler_to_jit_test), strdup("logia_compiler_to_jit_test"));
  ast_create_instrinsic(program, ast_create_identifier("logia_compiler_to_jit_test"), program->look<Type>("λi32"));

  {
    auto callFuncName = ast_create_identifier("logia_compiler_to_jit_test");
    auto sum_expr = ast_create_call_expr(callFuncName, {});

    main_fn->get_body()->push_child(ast_create_return(sum_expr));
  }

  expect_all_attached(program);

  backend->emitTargetLLVMIR("./tmp/logia_compiler_to_jit_test.ll");

  int exit_code = backend->run_jit("main");
  EXPECT_EQ(exit_code, 101);

  LOGIA_UNIT_TEST_END();
}
*/
// expose compiler functions to logia
TEST(ast_create_if2, t1)
{
  LOGIA_UNIT_TEST();
  using namespace logia::AST;

  auto callFuncName = ast_create_identifier("logia_intrinsics_bin_eq_i64_i64");
  auto condition = ast_create_call_expr(callFuncName, {new IntegerLiteral(rule, "11", i64), new IntegerLiteral(rule, "11", i64)});

  auto ifstmt = ast_create_if(condition);
  main_body->push_child(ifstmt);
  EXPECT_EQ(ifstmt->get_then()->parent_node, ifstmt);
  ifstmt->get_then()->push_child(ast_create_return(new IntegerLiteral(rule, "1", i64)));
  ifstmt->get_else()->push_child(ast_create_return(new IntegerLiteral(rule, "0", i64)));

  expect_all_attached(program);

  backend->emitTargetLLVMIR("./tmp/logia_compiler_if2.ll");

  int exit_code = backend->run_jit("main");
  EXPECT_EQ(exit_code, 1);

  LOGIA_UNIT_TEST_END();
}

// expose compiler functions to logia

/*
function main() i64 {
  var tmp = 0
  if (11 == 11) {
    tmp = tmp + 1
  } else {
    tmp = tmp + 2
  }
  #assert tmp == 1
}

*/
TEST(ast_create_if3, t1)
{
  LOGIA_UNIT_TEST();
  using namespace logia::AST;

  ast_create_instrinsic(program, ast_create_identifier("logia_intrinsics_bin_add_i64_i64"), program->lookup<Type>("λi64"));
  ast_create_instrinsic(program, ast_create_identifier("logia_intrinsics_bin_eq_i64_i64"), program->lookup<Type>("λi1"));

  auto eqeq = ast_create_identifier("logia_intrinsics_bin_eq_i64_i64");

  auto vdecl = ast_create_var_decl(ast_create_identifier("tmp"), program->lookup<Type>("λi64"), new IntegerLiteral(rule, "0", i64));
  main_body->push_child(vdecl);

  auto condition = ast_create_call_expr(eqeq, {new IntegerLiteral(rule, "11", i64), new IntegerLiteral(rule, "11", i64)});

  auto ifstmt = ast_create_if(condition);
  main_body->push_child(ifstmt);
  EXPECT_EQ(ifstmt->get_then()->parent_node, ifstmt);
  {
    auto fn_add_name = ast_create_identifier("logia_intrinsics_bin_add_i64_i64");
    auto add = ast_create_call_expr(fn_add_name, {ast_create_identifier("tmp"), new IntegerLiteral(rule, "1", i64)});
    auto fn_assignament_name = new StringLiteral(rule, "logia_operator_assign_i64_i64");
    auto assignament = ast_create_binary_expr(ast_create_identifier("tmp"), Operators::BINARY_ASSIGN, add);
    ifstmt->get_then()->push_child(assignament);
  }
  {
    auto fn_add_name = ast_create_identifier("logia_intrinsics_bin_add_i64_i64");
    auto add = ast_create_call_expr(fn_add_name, {ast_create_identifier("tmp"), new IntegerLiteral(rule, "2", i64)});
    auto fn_assignament_name = new StringLiteral(rule, "logia_operator_assign_i64_i64");
    auto assignament = ast_create_binary_expr(ast_create_identifier("tmp"), Operators::BINARY_ASSIGN, add);
    ifstmt->get_else()->push_child(assignament);
  }

  main_body->push_child(ast_create_return(ast_create_identifier("tmp")));

  EXPECT_NE(main_body->children[2]->parent_node, nullptr);
  EXPECT_NE(main_body->parent_node, nullptr);
  EXPECT_NE(main_fn->parent_node, nullptr);

  expect_all_attached(program);

  backend->emitTargetLLVMIR("./tmp/logia_compiler_if3.ll");

  int exit_code = backend->run_jit("main");
  EXPECT_EQ(exit_code, 1);

  LOGIA_UNIT_TEST_END();
}
