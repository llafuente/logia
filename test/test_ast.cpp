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
  program->add_statement(func);
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

  auto firstArg = createSignedIntegerLiteral(func->Function.body, 17);
  auto secondArg = createSignedIntegerLiteral(func->Function.body, 21);
  auto callFuncName = createStringLiteral(strdup("logia_operator_add_i64_i64"));
  std::cout << "locator = " << callFuncName->toString() << std::endl;

  EXPECT_EQ(firstArg->parentNode, nullptr);
  EXPECT_EQ(secondArg->parentNode, nullptr);
  EXPECT_EQ(callFuncName->parentNode, nullptr);
  auto callexpr = createCallExpression(callFuncName, {firstArg, secondArg});
  EXPECT_EQ(firstArg->parentNode, callexpr);
  EXPECT_EQ(secondArg->parentNode, callexpr);
  EXPECT_EQ(callFuncName->parentNode, callexpr);

  EXPECT_EQ(callexpr->parentNode, nullptr);
  auto ret_stmt = createReturn(callexpr);
  EXPECT_EQ(callexpr->parentNode, ret_stmt);

  EXPECT_EQ(ret_stmt->parentNode, nullptr);
  func->Function.body->add_statement(ret_stmt);
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
  ast_struct_add_field(string_t, (logia::AST::Type *)program->lookup(strdup("λi64")), "capacity", nullptr);
  EXPECT_EQ(string_t->Struct.properties.size(), 1);

  EXPECT_TRUE(string_t->Struct.properties[0].isField());
  EXPECT_FALSE(string_t->Struct.properties[0].isAlias());
  EXPECT_FALSE(string_t->Struct.properties[0].isGetter());
  EXPECT_FALSE(string_t->Struct.properties[0].isSetter());

  // invalid ?
  // program->add_statement(string_t);

  logia::AST::Type *func = logia::AST::ast_create_function_type(program, strdup("main"), logia::AST::ast_get_type_by_name(program, strdup("λi32")));
  EXPECT_TRUE(func);

  ast_function_add_param(func, string_t, "first", nullptr);

  program->add_statement(func);

  auto hello_world = createStringLiteral(strdup("Hello world!"));
  auto callFuncName = createStringLiteral(strdup("logia_print_stdout"));
  func->Function.body->add_statement(createCallExpression(callFuncName, {hello_world}));

  auto exit_code_value = createSignedIntegerLiteral(func->Function.body, 0);
  func->Function.body->add_statement(createReturn(exit_code_value));
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
  ast_struct_add_field(string_t, (logia::AST::Type *)program->lookup(strdup("λi64")), "capacity", nullptr);
  EXPECT_EQ(string_t->Struct.properties.size(), 1);

  EXPECT_TRUE(string_t->Struct.properties[0].isField());
  EXPECT_FALSE(string_t->Struct.properties[0].isAlias());
  EXPECT_FALSE(string_t->Struct.properties[0].isGetter());
  EXPECT_FALSE(string_t->Struct.properties[0].isSetter());

  // invalid ?
  // program->add_statement(string_t);

  logia::AST::Type *func = logia::AST::ast_create_function_type(program, strdup("main"), logia::AST::ast_get_type_by_name(program, strdup("λi32")));
  EXPECT_TRUE(func);

  program->add_statement(func);

  auto hello_world = createStringLiteral(strdup("Hello world!"));
  auto vdecl = ast_create_var_decl(func->Function.body, strdup("hello"), string_t, hello_world);
  func->Function.body->add_statement(vdecl);
  EXPECT_EQ(hello_world->parentNode, func->Function.body->statements[0]);

  // print static string
  auto callFuncName = createStringLiteral(strdup("logia_print_stdout"));
  auto hello_world2 = createStringLiteral(strdup("Hello world!"));
  func->Function.body->add_statement(createCallExpression(callFuncName, {hello_world2}));
  EXPECT_EQ(hello_world2->parentNode, func->Function.body->statements[1]);
  EXPECT_EQ(callFuncName->parentNode, func->Function.body->statements[1]);

  // print static string from variable
  auto ident = (Expression *)ast_create_identifier(func->Function.body, strdup("hello"));
  func->Function.body->add_statement(createCallExpression(callFuncName, {ident}));
  EXPECT_EQ(ident->parentNode, func->Function.body->statements[2]);

  auto exit_code_value = createSignedIntegerLiteral(func->Function.body, 0);
  func->Function.body->add_statement(createReturn(exit_code_value));
  EXPECT_EQ(program->children.size(), 1);
  EXPECT_EQ(func->Function.body->statements.size(), 4);

  program->codegen(back, back->builder);

  back->emitTargetLLVMIR("./tmp/hellow-world-alloca.ll");

  int exit_code = back->run_jit();
  EXPECT_EQ(exit_code, 0);

  delete back;
}
