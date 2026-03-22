#include "ast.h"
#include "gtest/gtest.h"
#include <Windows.h>

int main(int argc, char **argv)
{
  SetConsoleOutputCP(65001); // CP_UTF8
  
  logia_init_log((char*)"./logialog.txt");

  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}