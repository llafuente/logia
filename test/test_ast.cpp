#include "ast.h"
#include "gtest/gtest.h"
#include <Windows.h>

TEST(AST_Type, createFunctionType)
{
  using namespace logia::AST;
  
  auto back = new logia::Backend();
  auto program = logia::AST::createProgram(back->context);
  // has value
  EXPECT_TRUE(logia::AST::getTypeByName(program, strdup("λi8")));
  // and is unique everytime
  EXPECT_EQ(logia::AST::getTypeByName(program, strdup("λi8")), logia::AST::getTypeByName(program, strdup("λi8")));

  logia::AST::Type *func = logia::AST::createFunctionType(program, strdup("main"), logia::AST::getTypeByName(program, strdup("λi8")));
  EXPECT_TRUE(func);
  program->add_statement(func);

  EXPECT_EQ(func->type, logia::AST::Primitives::PRIMITIVE_FUNCTION);

  // can look for main function as it's declared inside program
  EXPECT_EQ(func, logia::AST::getTypeByName(program, strdup("main")));


  EXPECT_EQ(func->Function.body->parent, program);

  EXPECT_TRUE(logia::AST::getTypeByName(func->Function.body, strdup("λi8")));
  EXPECT_TRUE(logia::AST::getTypeByName(func->Function.body, strdup("λi64")));

  auto firstArg = createSignedIntegerLiteral(func->Function.body, 17);
  auto secondArg = createSignedIntegerLiteral(func->Function.body, 21);
  auto callFuncName = createStringLiteral(program, strdup("logia_operator_add_i64_i64"));

  func->Function.body->add_statement(createReturn(createCallExpression(callFuncName, {firstArg, secondArg})));

  program->codegen(back);
  back->emitTargetLLVMIR("xxx.ll");
  back->emitTargetObjectFile("xxx.obj");
  back->emitTargetAssemblyFile("xxx.asm");
  back->emitTargetExecutable("xxx.exe");
  back->applyLLVMOptimizers();
  back->run_jit();

  delete back;

  //std::cout << "toStringTree" << std::endl;
  //std::cout << program->toStringTree();
}