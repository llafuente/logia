#include "ast.h"
#include "gtest/gtest.h"
#include <Windows.h>

TEST(AST_Type, createFunctionType)
{
  using namespace logia::AST;
  
  auto back = new logia::Backend();
  auto program = logia::AST::createProgram(back->context);
  // has value
  EXPECT_TRUE(logia::AST::ast_get_type_by_name(program, strdup("λi8")));
  // and is unique everytime
  EXPECT_EQ(logia::AST::ast_get_type_by_name(program, strdup("λi8")), logia::AST::ast_get_type_by_name(program, strdup("λi8")));
  // no parent
  EXPECT_EQ(program->parentNode, nullptr);

  logia::AST::Type *func = logia::AST::createFunctionType(program, strdup("main"), logia::AST::ast_get_type_by_name(program, strdup("λi8")));
  EXPECT_TRUE(func);
  
  EXPECT_EQ(program->children.size(), 0);
  program->add_statement(func);
  EXPECT_EQ(program->children.size(), 1);

  EXPECT_EQ(func->type, logia::AST::Primitives::PRIMITIVE_FUNCTION);
  EXPECT_TRUE(strcmp(func->Function.name, "main") == 0);

  // can look for main function as it's declared inside program
  EXPECT_EQ(func, logia::AST::ast_get_type_by_name(program, strdup("main")));

  // correct parenting
  EXPECT_EQ(func->Function.body->parent, program);

  // function body is connected to program, type available
  EXPECT_TRUE(logia::AST::ast_get_type_by_name(func->Function.body, strdup("λi8")));
  EXPECT_TRUE(logia::AST::ast_get_type_by_name(func->Function.body, strdup("λi64")));


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

  program->codegen(back);
  back->emitTargetLLVMIR("xxx.ll");
  back->emitTargetObjectFile("xxx.obj");
  back->emitTargetAssemblyFile("xxx.asm");
  back->emitTargetExecutable("xxx.exe");
  back->applyLLVMOptimizers();
  int exit_code = back->run_jit();
  EXPECT_EQ(exit_code, 38);

  delete back;

  //std::cout << "toStringTree" << std::endl;
  //std::cout << program->toStringTree();
}