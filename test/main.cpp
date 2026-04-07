#include "gtest/gtest.h"
#include <Windows.h>
#include "utils.h"

int main(int argc, char **argv)
{
  SetConsoleOutputCP(65001); // CP_UTF8

  logia_init_log((char *)"./logialog.log");

  testing::InitGoogleTest(&argc, argv);

  // filter test in code, usefull to debug only one test
  // testing::GTEST_FLAG(filter) = "*if3*";

  return RUN_ALL_TESTS();
}