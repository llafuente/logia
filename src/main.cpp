/* Copyright (c) 2026 Luis Lafuente Morales. All rights reserved.
 * Use of this file is governed by the MIT license that
 * can be found in the LICENSE.txt file in the project root.
 */

#include <iostream>

#include "antlr4-runtime.h"
#include "LogiaParser.h"
#include "LogiaLexer.h"

#include "llvmvisitor.h"
#include "compiler.h"

#include <Windows.h>

#pragma execution_character_set("utf-8")

#include "logia/help.h"
#include "logia/run.h"

// file structure
// help
// commands
// main

int main(int argc, const char *argv[])
{
	SetConsoleOutputCP(65001); // CP_UTF8

	logia_init_log((char *)"./logia.log");

	try
	{
		if (argc == 1)
		{
			logia::print_usage(nullptr);
			return 1;
		}

		auto command = argv[1];
		if (strcmp(command, "run") == 0)
		{
			return logia::logia_run(argc - 2, argv + 2);
		}

		std::cout << "unkown command: \"" << command << "\"" << std::endl;
		logia::print_usage(nullptr);
		return -1;
	}
	catch (std::exception e)
	{
		std::cout << "Error: " << e.what() << std::endl;
	}

	return -1;
}
