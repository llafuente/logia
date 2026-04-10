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

using namespace std;

class EmptyNodeRemover : antlr4::tree::ParseTreeWalker
{

public:
	virtual void exitRule(antlr4::tree::ParseTreeListener *listener, antlr4::tree::ParseTree *r)
	{
		auto x = r->getTreeType();

		std::cout << (int)r->getTreeType() << std::endl;

		// Check if the node is empty and remove it
		if (r->children.empty())
		{
			r->parent->children.pop_back();
		}
	}
};

/*
std::string file_read(const std::string& filePath) {
	std::ifstream file(filePath, std::ios::in | std::ios::binary);
	if (!file) throw std::ios_base::failure("Failed to open file");
	return std::string((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
}
*/

// UTF-8 console
// do not include windows! world explodes!
// #include <windows.h>
#pragma execution_character_set("utf-8")

void print_usage(std::string *command)
{

	if (!command)
	{
		std::cout << "Usage: logia.exe command [options]" << std::endl;
	}

	std::cout << "  check <file> [options]" << std::endl;
	std::cout << "    checks syntax and semantics of given file" << std::endl;
	std::cout << "    [options]" << std::endl;
	std::cout << "      --json prints output as json" << std::endl;
	std::cout << "  build <file> <output> [options]" << std::endl;
	std::cout << "    [options]" << std::endl;
	std::cout << "    --emit-llvm prints llvm intermediary representation" << std::endl;
	std::cout << "  run <file>" << std::endl;
	std::cout << "    JIT compiles and runs main function" << std::endl;
	std::cout << "  test <file.logia|file.clogia> [filter] [options]" << std::endl;
	std::cout << "    JIT compiles and runs all tests found in given file" << std::endl;
	std::cout << "    if file is a logia-compiler-test it will check process output match the expected" << std::endl;
	std::cout << "    [filter] only executes tests that contains given filter (string|regex)" << std::endl;
	std::cout << "    [options]" << std::endl;
	std::cout << "      --all-test Run all test found in all compiled files" << std::endl;
	std::cout << "      --update Updates test output" << std::endl;
}

int main(int argc, const char *argv[])
{
	SetConsoleOutputCP(65001); // CP_UTF8

	logia_init_log((char *)"./logia.log");
	logia::Compiler *logia_compiler = new logia::Compiler();

	try
	{

		if (argc == 1)
		{
			print_usage(nullptr);
			return 1;
		}
		auto command = argv[1];
		const char *file_path;
		bool check = true;
		bool build = false;

		bool run_main = false;
		bool run_test = false;

		file_path = argv[2];
		if (strcmp(command, "check") == 0)
		{
		}
		else if (strcmp(command, "build") == 0)
		{
			build = true;
		}
		else if (strcmp(command, "run") == 0)
		{
			build = true;
			run_main = true;
		}
		else if (strcmp(command, "test") == 0)
		{
			build = true;
			run_test = true;
		}
		else
		{
			std::cout << "unkown command: \"" << command << "\"" << std::endl;
			print_usage(nullptr);
			return 1;
		}

		// parse common options

		bool print_file_contents = false;
		bool emit_llvm = false;
		for (int i = 1; i < argc; ++i)
		{
			if (strcmp("--package", argv[i]) == 0)
			{
				logia_compiler->is_program = false;
			}
			else if (strcmp("--print", argv[i]) == 0)
			{
				print_file_contents = true;
			}
			else if (strcmp("--verbose", argv[i]) == 0)
			{
				logia_compiler->verbose = true;
			}
			else if (strcmp("--emit-llvm", argv[i]) == 0)
			{
				emit_llvm = true;
			}
		}

		if (logia_compiler->verbose)
		{
			TCHAR cwd[MAX_PATH];
			GetCurrentDirectory(MAX_PATH, cwd);

			std::cout
				<< "Configuration:" << std::endl;
			// << "* cwd: " << std::filesystem::current_path() << std::endl
			std::wcout
				<< "* cwd: " << cwd << std::endl;
			std::cout
				<< "* file: " << file_path << std::endl
				<< "* print: " << (print_file_contents ? "yes" : "no") << std::endl
				<< "* program: " << (logia_compiler->is_program ? "yes" : "no") << std::endl;
		}

		logia_compiler->read(file_path);
		if (print_file_contents)
		{
			std::cout << "File Contents:" << std::endl
					  << logia_compiler->text << std::endl;
		}
		antlr4::ParserRuleContext *tree = logia_compiler->parse();

		logia_compiler->print_cst();
		logia_compiler->build_ast();
		logia_compiler->print_ast();

		if (run_main)
		{
			logia_compiler->backend->run_jit("main");
		}
		if (run_test)
		{
			throw std::runtime_error(__FUNCTION__ "todo");
			// TODO
			// create main_test, that will gather all tests from file
			logia_compiler->backend->run_jit("main_test");
		}
		if (emit_llvm)
		{
			// std::cout << logia_compiler->module->llvm()
		}
	}
	catch (std::exception e)
	{
		std::cout << "Error: " << e.what() << std::endl;
	}
	std::cout << "bye bye!: " << std::endl;
	delete logia_compiler;

	return 0;
}
