// ./build-debug/bin/Debug/logia_test_suite.exe --gtest_break_on_failure

#include "ast/node.h"
#include "ast/traverse.h"
#include "ast/if_stmt.h"
#include "ast/types.h"
#include "ast/expr.h"
#include "ast/constexpr.h"
#include "ast/stmt.h"

#include "gtest/gtest.h"
#include <Windows.h>

#include "test_utils.h"

void check_all_attached(logia::AST::Program *prg)
{
  logia::AST::ast_traverse(prg, [](logia::AST::Node *n) -> bool
                           {
                            DEBUG() << n->to_string() << std::endl;
      if ((n->type & logia::AST::ast_types::PROGRAM) == 0) {
        EXPECT_TRUE(n->is_attached) << std::format(" not attached {}", n->to_string());
    }
      return true; });
}

// hello world example
TEST(AST_Type, ast_create_program)
{
  DEBUG() << std::endl
          << "TEST START" << std::endl
          << std::endl;

  using namespace logia::AST;
  auto back = new logia::Backend();
  auto program = back->program;

  EXPECT_EQ(program->parent_node, nullptr);
  EXPECT_EQ(program->children.size(), 10);
  // This shall be updated as we add more primitives
  EXPECT_EQ(program->scope.size(), 16);

  std::cout << program->to_string() << std::endl;
  std::cout << program->children[0]->get_type()->get_repr();

  EXPECT_EQ(strcmp(program->children[0]->get_type()->get_repr().c_str(), "i8"), 0);
  EXPECT_EQ(strcmp(program->children[1]->get_type()->get_repr().c_str(), "i16"), 0);

  // has value
  EXPECT_TRUE(logia::AST::ast_get_type_by_name(program, strdup("λi8")));
  // and is the same everytime
  EXPECT_EQ(logia::AST::ast_get_type_by_name(program, strdup("λi8")), logia::AST::ast_get_type_by_name(program, strdup("λi8")));

  check_all_attached(program);
  delete back;
}

TEST(AST_Type, ast_create_function_type)
{
  DEBUG() << std::endl
          << "TEST START" << std::endl
          << std::endl;

  using namespace logia::AST;

  auto back = new logia::Backend();
  auto program = back->program;
  auto start_program_children = program->children.size();
  back->load_intrinsics();

  auto func = ast_create_function_type(ast_create_identifier("main"), ast_get_type_by_name(program, strdup("λi64")));
  EXPECT_TRUE(func);

  auto items_before = program->scope.size();
  program->push_child(func);
  EXPECT_EQ(program->children.size(), start_program_children + 1);
  EXPECT_EQ(program->scope.size(), items_before + 1);

  EXPECT_EQ(func->primitive, Primitives::FUNCTION_TY);
  EXPECT_TRUE(strcmp(func->get_name(), "main") == 0);

  // can look for main function as it's declared inside program
  EXPECT_EQ(func, ast_get_type_by_name(program, strdup("main")));

  // check parenting
  EXPECT_EQ(func->get_identifier()->parent_node, func);
  EXPECT_EQ(func->get_return_type()->parent_node, func);
  EXPECT_EQ(func->get_body()->parent_node, func);
  EXPECT_EQ(func->get_body()->parent, program);

  // function body is connected to program, type available
  EXPECT_TRUE(ast_get_type_by_name(func->get_body(), strdup("λi8")));
  EXPECT_TRUE(ast_get_type_by_name(func->get_body(), strdup("λi64")));

  auto firstArg = ast_create_int_lit(func->get_body(), "17");
  auto secondArg = ast_create_int_lit(func->get_body(), "21");
  auto callFuncName = ast_create_identifier("logia_intrinsics_bin_add_i64_i64");

  EXPECT_EQ(firstArg->parent_node, nullptr);
  EXPECT_EQ(secondArg->parent_node, nullptr);
  EXPECT_EQ(callFuncName->parent_node, nullptr);
  auto callexpr = ast_create_call_expr(callFuncName, {firstArg, secondArg});
  EXPECT_EQ(callexpr->get_arguments().size(), 2);
  EXPECT_EQ(firstArg->parent_node, callexpr);
  EXPECT_EQ(secondArg->parent_node, callexpr);
  EXPECT_EQ(callFuncName->parent_node, callexpr);

  EXPECT_EQ(callexpr->parent_node, nullptr);
  auto ret_stmt = ast_create_return(callexpr);
  EXPECT_EQ(callexpr->parent_node, ret_stmt);

  EXPECT_EQ(ret_stmt->parent_node, nullptr);
  func->get_body()->push_child(ret_stmt);
  EXPECT_EQ(ret_stmt->parent_node, func->get_body());
  EXPECT_EQ(func->get_body()->parent_node, func);

  EXPECT_EQ(func->get_identifier()->parent_node, func);
  check_all_attached(program);

  back->emitTargetLLVMIR("./tmp/maincall.ll");
  back->emitTargetObjectFile("./tmp/maincall.obj");
  back->emitTargetAssemblyFile("./tmp/maincall.asm");

  EXPECT_THROW(back->emitTargetExecutable("./tmp/maincall.exe"), std::runtime_error);

  back->applyLLVMOptimizers();
  int exit_code = back->run_jit("main");
  EXPECT_EQ(exit_code, 38);

  delete back;
}

// hello world example
TEST(AST_Type, ast_create_struct_type)
{
  DEBUG() << std::endl
          << "TEST START" << std::endl
          << std::endl;

  using namespace logia::AST;

  auto back = new logia::Backend();
  auto program = back->program;
  auto start_program_children = program->children.size();
  back->load_intrinsics();

  auto string_t = ast_create_struct_type(ast_create_identifier("string"));
  EXPECT_EQ(string_t->fields.size(), 0);
  string_t->add_field((logia::AST::Type *)program->lookup(strdup("λi64")), ast_create_identifier("capacity"), nullptr);
  EXPECT_EQ(string_t->fields.size(), 1);
  EXPECT_EQ(string_t->get_field_index(ast_create_identifier("capacity")), 0);

  string_t->add_field((logia::AST::Type *)program->lookup(strdup("λi64")), ast_create_identifier("length"), nullptr);
  EXPECT_EQ(string_t->fields.size(), 2);
  EXPECT_EQ(string_t->get_field_index(ast_create_identifier("length")), 1);

  string_t->add_field((logia::AST::Type *)program->lookup(strdup("λptr")), ast_create_identifier("value"), nullptr);
  EXPECT_EQ(string_t->fields.size(), 3);
  EXPECT_EQ(string_t->get_field_index(ast_create_identifier("value")), 2);

  EXPECT_EQ(string_t->get_field_index(ast_create_identifier("xxx")), -1);

  // invalid ?
  // program->add_statement(string_t);

  auto func = logia::AST::ast_create_function_type(ast_create_identifier("main"), logia::AST::ast_get_type_by_name(program, strdup("λi32")));
  EXPECT_TRUE(func);

  func->add_param(string_t, ast_create_identifier("first"), nullptr);

  program->push_child(func);

  auto hello_world = ast_create_string_lit(strdup("Hello world!"));
  auto callFuncName = ast_create_identifier("logia_print_stdout");
  func->get_body()->push_child(ast_create_call_expr(callFuncName, {hello_world}));

  auto exit_code_value = ast_create_int_lit(func->get_body(), "0");
  func->get_body()->push_child(ast_create_return(exit_code_value));
  EXPECT_EQ(program->children.size(), start_program_children + 1);
  EXPECT_EQ(func->get_body()->children.size(), 2);

  check_all_attached(program);

  program->codegen(back, back->builder);

  back->emitTargetLLVMIR("./tmp/struct.ll");

  // NOTE works, but for an unkown reason yet, we can't jit again.
  int exit_code = back->run_jit("main");
  /*
  // capture stdout is not working on JIT, but works with simple fprint/std::cout...
  // need more inverstigation, maybe it's something inside ORC
  if (start_stdout_capture())
  {
    int exit_code = back->run_jit("main");
    // fprintf(stdout, "Hello world!");
    // std::cout << "Hello world!";

    auto str_stdout = end_stdout_capture();

    // EXPECT_EQ(exit_code, 0);
    std::cout << "output!!! \n"
              << str_stdout << std::endl;
    EXPECT_TRUE(strcmp(str_stdout, "Hello world!") == 0);
    free(str_stdout);
  }
  else
  {
    EXPECT_FALSE(true);
  }
  */

  delete back;
}

// hello world example
TEST(AST_Type, ast_create_var_decl)
{
  DEBUG() << std::endl
          << "TEST START" << std::endl
          << std::endl;

  using namespace logia::AST;

  auto back = new logia::Backend();
  auto program = back->program;
  auto start_program_children = program->children.size();
  back->load_intrinsics();

  auto string_t = ast_create_struct_type(ast_create_identifier("string"));
  EXPECT_EQ(string_t->fields.size(), 0);
  string_t->add_field((logia::AST::Type *)program->lookup(strdup("λi64")), ast_create_identifier("capacity"), nullptr);
  string_t->add_field((logia::AST::Type *)program->lookup(strdup("λi64")), ast_create_identifier("length"), nullptr);
  string_t->add_field((logia::AST::Type *)program->lookup(strdup("λptr")), ast_create_identifier("value"), nullptr);
  EXPECT_EQ(string_t->fields.size(), 3);

  EXPECT_EQ(string_t->aliases.size(), 0);
  string_t->add_alias(ast_create_identifier("cap"), ast_create_identifier("capacity"));
  EXPECT_EQ(string_t->aliases.size(), 1);
  string_t->add_alias(ast_create_identifier("len"), ast_create_identifier("length"));
  EXPECT_EQ(string_t->aliases.size(), 2);

  EXPECT_EQ(string_t->get_alias_to(ast_create_identifier("cap")), string_t->aliases[0].to);
  EXPECT_EQ(string_t->get_alias_to(ast_create_identifier("xxx")), nullptr);
  EXPECT_EQ(string_t->get_field_index(ast_create_identifier("cap")), 0);
  EXPECT_EQ(string_t->get_field_index(ast_create_identifier("len")), 1);

  // invalid ?
  // program->add_statement(string_t);

  auto func = logia::AST::ast_create_function_type(ast_create_identifier("main"), logia::AST::ast_get_type_by_name(program, strdup("λi32")));
  EXPECT_TRUE(func);

  program->push_child(func);

  EXPECT_EQ(func->get_body()->scope.size(), 0);
  auto hello_world = ast_create_string_lit(strdup("Hello world!"));
  auto vdecl = ast_create_var_decl(ast_create_identifier("hello"), string_t, hello_world);
  func->get_body()->push_child(vdecl);
  EXPECT_EQ(hello_world->parent_node, vdecl);
  EXPECT_EQ(vdecl->parent_node, func->get_body());
  EXPECT_EQ(func->get_body()->scope.size(), 1);

  // print static string
  auto callFuncName = ast_create_identifier("logia_print_stdout");
  auto hello_world2 = ast_create_string_lit("Hello world!");
  func->get_body()->push_child(ast_create_call_expr(callFuncName, {hello_world2}));
  EXPECT_EQ(hello_world2->parent_node, func->get_body()->children[1]);
  EXPECT_EQ(callFuncName->parent_node, func->get_body()->children[1]);

  // print static string from variable
  auto callFuncName2 = ast_create_identifier("logia_print_stdout");
  auto x = (Expression *)ast_create_identifier("hello");
  func->get_body()->push_child(ast_create_call_expr(callFuncName2, {x}));
  EXPECT_EQ(x->parent_node, func->get_body()->children[2]);

  auto exit_code_value = ast_create_int_lit(func->get_body(), "0");
  func->get_body()->push_child(ast_create_return(exit_code_value));

  EXPECT_EQ(program->children.size(), start_program_children + 1);
  EXPECT_EQ(func->get_body()->children.size(), 4);

  check_all_attached(program);

  program->codegen(back, back->builder);

  string_t->codegen(back, back->builder);

  back->emitTargetLLVMIR("./tmp/hellow-world-alloca.ll");

  int exit_code = back->run_jit("main");
  EXPECT_EQ(exit_code, 0);

  delete back;
}

#define LOGIA_BACKEND_START()                                                                                                                                     \
  logia::Backend *back;                                                                                                                                           \
  logia::AST::Program *program;                                                                                                                                   \
  logia::AST::Function *main_fn;                                                                                                                                  \
  logia::AST::Block *main_body;                                                                                                                                   \
  do                                                                                                                                                              \
  {                                                                                                                                                               \
    back = new logia::Backend();                                                                                                                                  \
    program = back->program;                                                                                                                                      \
    back->load_intrinsics();                                                                                                                                      \
    main_fn = logia::AST::ast_create_function_type(logia::AST::ast_create_identifier(strdup("main")), logia::AST::ast_get_type_by_name(program, strdup("λi64"))); \
    EXPECT_TRUE(main_fn);                                                                                                                                         \
    program->push_child(main_fn);                                                                                                                                 \
    main_body = main_fn->get_body();                                                                                                                              \
  } while (0)

#define LOGIA_BACKEND_END() \
  do                        \
  {                         \
    delete back;            \
  } while (false)

// sum 15+20 as variables
TEST(AST_Type, ast_create_var_decl2)
{
  DEBUG() << std::endl
          << "TEST START" << std::endl
          << std::endl;

  LOGIA_BACKEND_START();
  using namespace logia::AST;

  auto str_a = "a";
  {
    auto value_a = ast_create_int_lit(program, "11");
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

  check_all_attached(program);

  program->codegen(back, back->builder);

  back->emitTargetLLVMIR("./tmp/alloca-integer-sum.ll");

  int exit_code = back->run_jit("main");
  EXPECT_EQ(exit_code, 11 + 12);

  LOGIA_BACKEND_END();
}

extern "C" int logia_compiler_to_jit_test()
{
  return 101;
}

// expose compiler functions to logia
TEST(AST_Type, logia_compiler_to_jit_test)
{
  DEBUG() << std::endl
          << "TEST START" << std::endl
          << std::endl;

  LOGIA_BACKEND_START();
  using namespace logia::AST;

  // this is how you create an intrinsic to use it comptime
  back->add_intrinsic((void *)(&logia_compiler_to_jit_test), strdup("logia_compiler_to_jit_test"));
  ast_create_instrinsic(program, ast_create_identifier("logia_compiler_to_jit_test"), ast_get_type_by_name(program, strdup("λi32")));

  {
    auto callFuncName = ast_create_identifier("logia_compiler_to_jit_test");
    auto sum_expr = ast_create_call_expr(callFuncName, {});

    main_fn->get_body()->push_child(ast_create_return(sum_expr));
  }

  check_all_attached(program);

  program->codegen(back, back->builder);

  back->emitTargetLLVMIR("./tmp/logia_compiler_to_jit_test.ll");

  int exit_code = back->run_jit("main");
  EXPECT_EQ(exit_code, 101);

  LOGIA_BACKEND_END();
}

// expose compiler functions to logia
TEST(ast_create_if2, t1)
{
  DEBUG() << std::endl
          << "TEST START" << std::endl
          << std::endl;

  LOGIA_BACKEND_START();
  using namespace logia::AST;

  auto callFuncName = ast_create_identifier("logia_intrinsics_bin_eq_i64_i64");
  auto condition = ast_create_call_expr(callFuncName, {ast_create_int_lit(program, "11"), ast_create_int_lit(program, "11")});

  auto ifstmt = ast_create_if(condition);
  main_body->push_child(ifstmt);
  EXPECT_EQ(ifstmt->get_then()->parent_node, ifstmt);
  ifstmt->get_then()->push_child(ast_create_return(ast_create_int_lit(program, "1")));
  ifstmt->get_else()->push_child(ast_create_return(ast_create_int_lit(program, "0")));

  check_all_attached(program);

  program->codegen(back, back->builder);
  back->emitTargetLLVMIR("./tmp/logia_compiler_if2.ll");

  int exit_code = back->run_jit("main");
  EXPECT_EQ(exit_code, 1);

  LOGIA_BACKEND_END();
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
  DEBUG() << std::endl
          << "TEST START" << std::endl
          << std::endl;

  LOGIA_BACKEND_START();
  using namespace logia::AST;

  auto eqeq = ast_create_identifier("logia_intrinsics_bin_eq_i64_i64");

  auto vdecl = ast_create_var_decl(ast_create_identifier("tmp"), (logia::AST::Type *)program->lookup(strdup("λi64")), ast_create_int_lit(program, "0"));
  main_body->push_child(vdecl);

  auto condition = ast_create_call_expr(eqeq, {ast_create_int_lit(program, "11"), ast_create_int_lit(program, "11")});

  auto ifstmt = ast_create_if(condition);
  main_body->push_child(ifstmt);
  EXPECT_EQ(ifstmt->get_then()->parent_node, ifstmt);
  {
    auto fn_add_name = ast_create_identifier("logia_intrinsics_bin_add_i64_i64");
    auto add = ast_create_call_expr(fn_add_name, {ast_create_identifier("tmp"), ast_create_int_lit(program, "1")});
    auto fn_assignament_name = ast_create_string_lit(strdup("logia_operator_assign_i64_i64"));
    auto assignament = ast_create_binary_expr(ast_create_identifier("tmp"), BinaryOperator::ASSIGN, add);
    ifstmt->get_then()->push_child(assignament);
  }
  {
    auto fn_add_name = ast_create_identifier("logia_intrinsics_bin_add_i64_i64");
    auto add = ast_create_call_expr(fn_add_name, {ast_create_identifier("tmp"), ast_create_int_lit(program, "2")});
    auto fn_assignament_name = ast_create_string_lit(strdup("logia_operator_assign_i64_i64"));
    auto assignament = ast_create_binary_expr(ast_create_identifier("tmp"), BinaryOperator::ASSIGN, add);
    ifstmt->get_else()->push_child(assignament);
  }

  main_body->push_child(ast_create_return(ast_create_identifier("tmp")));

  EXPECT_NE(main_body->children[2]->parent_node, nullptr);
  EXPECT_NE(main_body->parent_node, nullptr);
  EXPECT_NE(main_fn->parent_node, nullptr);

  check_all_attached(program);

  program->codegen(back, back->builder);
  back->emitTargetLLVMIR("./tmp/logia_compiler_if3.ll");

  int exit_code = back->run_jit("main");
  EXPECT_EQ(exit_code, 1);

  LOGIA_BACKEND_END();
}
