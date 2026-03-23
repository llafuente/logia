#include "ast.h"
#include "gtest/gtest.h"
#include <Windows.h>
#include "test_utils.h"

// hello world example
TEST(AST_Type, ast_create_program)
{
  using namespace logia::AST;

  auto back = new logia::Backend();
  auto program = logia::AST::ast_create_program(back->context);
  EXPECT_EQ(program->parentNode, nullptr);
  EXPECT_EQ(program->statements.size(), 0);
  // This shall be updated as we add more primitives
  EXPECT_EQ(program->scope.size(), 14);

  // has value
  EXPECT_TRUE(logia::AST::ast_get_type_by_name(program, strdup("λi8")));
  // and is the same everytime
  EXPECT_EQ(logia::AST::ast_get_type_by_name(program, strdup("λi8")), logia::AST::ast_get_type_by_name(program, strdup("λi8")));

  delete program;
}

TEST(AST_Type, ast_create_function_type)
{
  using namespace logia::AST;

  auto back = new logia::Backend();
  back->load_intrinsics();
  auto program = ast_create_program(back->context);

  Type *func = ast_create_function_type(program, strdup("main"), ast_get_type_by_name(program, strdup("λi8")));
  EXPECT_TRUE(func);

  EXPECT_EQ(program->children.size(), 0);
  program->push_statement(func);
  EXPECT_EQ(program->children.size(), 1);

  EXPECT_EQ(func->type, Primitives::PRIMITIVE_FUNCTION);
  EXPECT_TRUE(strcmp(func->Function.name, "main") == 0);

  // can look for main function as it's declared inside program
  EXPECT_EQ(func, ast_get_type_by_name(program, strdup("main")));

  // correct parenting
  EXPECT_EQ(func->Function.body->parent, program);

  // function body is connected to program, type available
  EXPECT_TRUE(ast_get_type_by_name(func->Function.body, strdup("λi8")));
  EXPECT_TRUE(ast_get_type_by_name(func->Function.body, strdup("λi64")));

  auto firstArg = ast_create_int_lit(func->Function.body, 17);
  auto secondArg = ast_create_int_lit(func->Function.body, 21);
  auto callFuncName = ast_create_string_literal(strdup("logia_operator_add_i64_i64"));
  std::cout << "locator = " << callFuncName->toString() << std::endl;

  EXPECT_EQ(firstArg->parentNode, nullptr);
  EXPECT_EQ(secondArg->parentNode, nullptr);
  EXPECT_EQ(callFuncName->parentNode, nullptr);
  auto callexpr = ast_create_call_expr(callFuncName, {firstArg, secondArg});
  EXPECT_EQ(firstArg->parentNode, callexpr);
  EXPECT_EQ(secondArg->parentNode, callexpr);
  EXPECT_EQ(callFuncName->parentNode, callexpr);

  EXPECT_EQ(callexpr->parentNode, nullptr);
  auto ret_stmt = ast_create_return(callexpr);
  EXPECT_EQ(callexpr->parentNode, ret_stmt);

  EXPECT_EQ(ret_stmt->parentNode, nullptr);
  func->Function.body->push_statement(ret_stmt);
  EXPECT_EQ(ret_stmt->parentNode, func->Function.body);
  EXPECT_EQ(func->Function.body->parentNode, func);

  program->codegen(back, back->builder);
  back->emitTargetLLVMIR("./tmp/maincall.ll");
  back->emitTargetObjectFile("./tmp/maincall.obj");
  back->emitTargetAssemblyFile("./tmp/maincall.asm");
  back->emitTargetExecutable("./tmp/maincall.exe");
  back->applyLLVMOptimizers();
  int exit_code = back->run_jit();
  EXPECT_EQ(exit_code, 38);

  delete back;

  // std::cout << "toStringTree" << std::endl;
  // std::cout << program->toStringTree();
}

// hello world example
TEST(AST_Type, ast_create_struct_type)
{
  using namespace logia::AST;

  auto back = new logia::Backend();
  back->load_intrinsics();
  auto program = logia::AST::ast_create_program(back->context);
  EXPECT_EQ(program->parentNode, nullptr);
  EXPECT_EQ(program->statements.size(), 0);

  auto string_t = ast_create_struct_type(program, strdup("string"));
  EXPECT_EQ(string_t->Struct.properties.size(), 0);
  ast_struct_add_field(string_t, (logia::AST::Type *)program->lookup(strdup("λi64")), strdup("capacity"), nullptr);
  EXPECT_EQ(string_t->Struct.properties.size(), 1);

  EXPECT_TRUE(string_t->Struct.properties[0].isField());
  EXPECT_FALSE(string_t->Struct.properties[0].isAlias());
  EXPECT_FALSE(string_t->Struct.properties[0].isGetter());
  EXPECT_FALSE(string_t->Struct.properties[0].isSetter());

  // invalid ?
  // program->add_statement(string_t);

  logia::AST::Type *func = logia::AST::ast_create_function_type(program, strdup("main"), logia::AST::ast_get_type_by_name(program, strdup("λi32")));
  EXPECT_TRUE(func);

  ast_function_add_param(func, string_t, strdup("first"), nullptr);

  program->push_statement(func);

  auto hello_world = ast_create_string_literal(strdup("Hello world!"));
  auto callFuncName = ast_create_string_literal(strdup("logia_print_stdout"));
  func->Function.body->push_statement(ast_create_call_expr(callFuncName, {hello_world}));

  auto exit_code_value = ast_create_int_lit(func->Function.body, 0);
  func->Function.body->push_statement(ast_create_return(exit_code_value));
  EXPECT_EQ(program->children.size(), 1);
  EXPECT_EQ(func->Function.body->statements.size(), 2);

  program->codegen(back, back->builder);

  back->emitTargetLLVMIR("./tmp/struct.ll");

  // NOTE works, but for an unkown reason yet, we can't jit again.
  int exit_code = back->run_jit();
  /*
  // capture stdout is not working on JIT, but works with simple fprint/std::cout...
  // need more inverstigation, maybe it's something inside ORC
  if (start_stdout_capture())
  {
    int exit_code = back->run_jit();
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
  using namespace logia::AST;

  auto back = new logia::Backend();
  back->load_intrinsics();
  auto program = logia::AST::ast_create_program(back->context);

  auto string_t = ast_create_struct_type(program, strdup("string"));
  EXPECT_EQ(string_t->Struct.properties.size(), 0);
  ast_struct_add_field(string_t, (logia::AST::Type *)program->lookup(strdup("λi64")), strdup("capacity"), nullptr);
  EXPECT_EQ(string_t->Struct.properties.size(), 1);

  EXPECT_TRUE(string_t->Struct.properties[0].isField());
  EXPECT_FALSE(string_t->Struct.properties[0].isAlias());
  EXPECT_FALSE(string_t->Struct.properties[0].isGetter());
  EXPECT_FALSE(string_t->Struct.properties[0].isSetter());

  // invalid ?
  // program->add_statement(string_t);

  logia::AST::Type *func = logia::AST::ast_create_function_type(program, strdup("main"), logia::AST::ast_get_type_by_name(program, strdup("λi32")));
  EXPECT_TRUE(func);

  program->push_statement(func);

  auto hello_world = ast_create_string_literal(strdup("Hello world!"));
  auto vdecl = ast_create_var_decl(func->Function.body, strdup("hello"), string_t, hello_world);
  func->Function.body->push_statement(vdecl);
  EXPECT_EQ(hello_world->parentNode, func->Function.body->statements[0]);

  // print static string
  auto callFuncName = ast_create_string_literal(strdup("logia_print_stdout"));
  auto hello_world2 = ast_create_string_literal(strdup("Hello world!"));
  func->Function.body->push_statement(ast_create_call_expr(callFuncName, {hello_world2}));
  EXPECT_EQ(hello_world2->parentNode, func->Function.body->statements[1]);
  EXPECT_EQ(callFuncName->parentNode, func->Function.body->statements[1]);

  // print static string from variable
  auto ident = (Expression *)ast_create_identifier(func->Function.body, strdup("hello"));
  func->Function.body->push_statement(ast_create_call_expr(callFuncName, {ident}));
  EXPECT_EQ(ident->parentNode, func->Function.body->statements[2]);

  auto exit_code_value = ast_create_int_lit(func->Function.body, 0);
  func->Function.body->push_statement(ast_create_return(exit_code_value));
  EXPECT_EQ(program->children.size(), 1);
  EXPECT_EQ(func->Function.body->statements.size(), 4);

  program->codegen(back, back->builder);

  back->emitTargetLLVMIR("./tmp/hellow-world-alloca.ll");

  int exit_code = back->run_jit();
  EXPECT_EQ(exit_code, 0);

  delete back;
}

#define TEST_INIT_MAIN()                                                                                                                \
  logia::Backend *back;                                                                                                                 \
  logia::AST::Program *program;                                                                                                         \
  logia::AST::Type *main_fn;                                                                                                            \
  logia::AST::Body *main_body;                                                                                                          \
  do                                                                                                                                    \
  {                                                                                                                                     \
    back = new logia::Backend();                                                                                                        \
    back->load_intrinsics();                                                                                                            \
    program = logia::AST::ast_create_program(back->context);                                                                            \
    main_fn = logia::AST::ast_create_function_type(program, strdup("main"), logia::AST::ast_get_type_by_name(program, strdup("λi32"))); \
    EXPECT_TRUE(main_fn);                                                                                                               \
    program->push_statement(main_fn);                                                                                                   \
    main_body = main_fn->Function.body;                                                                                                 \
  } while (0)

// sum 15+20 as variables
TEST(AST_Type, ast_create_var_decl2)
{
  TEST_INIT_MAIN();

  using namespace logia::AST;

  auto str_a = strdup("a");
  {
    auto value_a = ast_create_int_lit(program, 11);
    auto vdecl_a = ast_create_var_decl(main_body, str_a, value_a->type, value_a);
    main_body->push_statement(vdecl_a);
  }

  auto str_b = strdup("b");
  {
    auto value_b = ast_create_int_lit(program, 12);
    auto vdecl_b = ast_create_var_decl(main_body, str_b, value_b->type, value_b);
    main_body->push_statement(vdecl_b);
  }

  // print static string from variable
  {
    auto ident_a = (Expression *)ast_create_identifier(main_body, str_a);
    auto ident_b = (Expression *)ast_create_identifier(main_body, str_b);

    auto callFuncName = ast_create_string_literal(strdup("logia_operator_add_i64_i64"));
    auto sum_expr = ast_create_call_expr(callFuncName, {ident_a, ident_b});

    main_fn->Function.body->push_statement(ast_create_return(sum_expr));
  }

  program->codegen(back, back->builder);

  back->emitTargetLLVMIR("./tmp/alloca-integer-sum.ll");

  int exit_code = back->run_jit();
  EXPECT_EQ(exit_code, 11 + 12);

  delete back;
}

extern "C" int logia_compiler_to_jit_test()
{
  return 101;
}

// expose compiler functions to logia
TEST(AST_Type, logia_compiler_to_jit_test)
{
  TEST_INIT_MAIN();

  // this is how you create an intrinsic to use it comptime
  back->add_intrinsic((void *)(&logia_compiler_to_jit_test), strdup("logia_compiler_to_jit_test"));
  ast_create_instrinsic(program, strdup("logia_compiler_to_jit_test"), logia::AST::ast_get_type_by_name(program, strdup("λi32")));

  using namespace logia::AST;

  {
    auto callFuncName = ast_create_string_literal(strdup("logia_compiler_to_jit_test"));
    auto sum_expr = ast_create_call_expr(callFuncName, {});

    main_fn->Function.body->push_statement(ast_create_return(sum_expr));
  }

  program->codegen(back, back->builder);

  back->emitTargetLLVMIR("./tmp/logia_compiler_to_jit_test.ll");

  int exit_code = back->run_jit();
  EXPECT_EQ(exit_code, 101);

  delete back;
}
