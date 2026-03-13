/* Copyright (c) 2026 Luis Lafuente Morales. All rights reserved.
 * Use of this file is governed by the MIT license that
 * can be found in the LICENSE.txt file in the project root.
 */

#include <iostream>

#include "antlr4-runtime.h"
#include "LogiaParser.h"
#include "LogiaLexer.h"

#include "llvmvisitor.h"

#include <Windows.h>

#pragma execution_character_set("utf-8")

using namespace antlr4;
using namespace std;
using namespace logia;

class LogiaErrorListener : ANTLRErrorListener
{
	std::string input;
	std::string inputFile;

public:
	LogiaErrorListener(std::string inputFile, std::string input)
	{
		this->input = input;
		this->inputFile = inputFile;
	}
	void syntaxError(Recognizer *recognizer, Token *offendingSymbol, size_t line,
					 size_t column, const std::string &msg, std::exception_ptr e)
	{

		// Create a stringstream object
		// to str
		stringstream ss(this->input);

		// Temporary object to store
		// the splitted string
		string str;

		// Delimiter
		char del = '\n';

		// Splitting the str string
		// by delimiter
		int count = 1;
		auto start = max(0, line - 5);
		auto end = line + 3;
		while (getline(ss, str, del))
		{
			if (count >= start && count < end)
			{
				cout << str << std::endl;
			}
			if (count == line)
			{
				for (int i = 0; i < column; ++i)
				{
					cout << " ";
				}
				// console.log(Array(column).fill("-").join("") + "^\x1B[31m", msg, "\x1B[39m")
				cout << "^\x1B[31m" << msg << "\x1B[39m" << std::endl;
			}

			++count;
		}

		std::cout << this->inputFile << ":" << line << ":" << column;

		// console.log(recognizer.getLiteralNames())
		// console.log(recognizer.getSymbolicNames())
		std::cout << offendingSymbol->toString();

		exit(1);
	}

	// Inherited via ANTLRErrorListener
	void reportAmbiguity(Parser *recognizer, const dfa::DFA &dfa, size_t startIndex, size_t stopIndex, bool exact, const antlrcpp::BitSet &ambigAlts, atn::ATNConfigSet *configs) override
	{
	}
	void reportAttemptingFullContext(Parser *recognizer, const dfa::DFA &dfa, size_t startIndex, size_t stopIndex, const antlrcpp::BitSet &conflictingAlts, atn::ATNConfigSet *configs) override
	{
	}
	void reportContextSensitivity(Parser *recognizer, const dfa::DFA &dfa, size_t startIndex, size_t stopIndex, size_t prediction, atn::ATNConfigSet *configs) override
	{
	}
};

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

char *file_read(const char *file_path)
{
	FILE *file;
	auto err = fopen_s(&file, file_path, "rb");
	if (err)
	{
		std::stringstream ss;
		ss << "Error opening file: " << file_path;

		perror(ss.str().c_str());
		return nullptr;
	}

	// Seek to the end to determine file size
	fseek(file, 0, SEEK_END);
	auto fileSize = ftell(file);
	rewind(file);

	// Allocate memory for the file content
	char *buffer = (char *)malloc(fileSize + 2);
	if (buffer == NULL)
	{
		perror("Memory allocation failed");
		fclose(file);

		return nullptr;
	}

	// Read the file into the buffer
	fread(buffer, 1, fileSize, file);
	buffer[fileSize] = '\n';	 // Null-terminate the string
	buffer[fileSize + 1] = '\0'; // Null-terminate the string

	fclose(file);
	return buffer;
}

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

int main(int argc, const char *argv[])
{
	try
	{
		SetConsoleOutputCP(65001); // CP_UTF8

		if (argc == 1)
		{
			std::cout << "Usage: compiler.exe <file> [-package]" << std::endl;
			return 1;
		}
		auto file_path = argv[1];
		bool package = false;
		bool verbose = false;
		bool print = false;
		for (int i = 0; i < argc; ++i)
		{
			if (strcmp("--package", argv[i]) == 0)
			{
				package = true;
			}
			if (strcmp("--print", argv[i]) == 0)
			{
				print = true;
			}
			if (strcmp("--verbose", argv[i]) == 0)
			{
				verbose = true;
			}
		}

		if (verbose)
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
				<< "* print: " << (print ? "yes" : "no") << std::endl
				<< "* is package? " << (package ? "yes" : "no") << std::endl;
		}

		auto text = file_read(file_path);
		if (print)
		{
			std::cout << "File Content \n"
					  << text << std::endl;
		}

		// deprecated ANTLRFileStream* input = new ANTLRFileStream(file_path);

		// std::ifstream stream(file_path, std::ios::binary);
		// antlr4::ANTLRInputStream input(stream);

		antlr4::ANTLRInputStream input(text);

		// ANTLRInputStream input(text);

		LogiaLexer lexer(&input);
		CommonTokenStream tokens(&lexer);
		LogiaParser parser(&tokens);
		ANTLRErrorListener *elis = (ANTLRErrorListener *)new LogiaErrorListener(file_path, text);
		parser.addErrorListener(elis);

		LLVMVisitor *llvmVisitor = new LLVMVisitor();

		// LogiaParser::ProgramContext *context = parser.program();
		// llvmVisitor->visitProgram(context);
		//

		antlr4::ParserRuleContext *tree = package ? (antlr4::ParserRuleContext *)parser.packageProgram() : (antlr4::ParserRuleContext *)parser.program();
		llvmVisitor->visit(tree);

		auto s = tree->toStringTree(&parser, true);
		std::cout << s << std::endl;

		if (!package)
		{
			auto program = static_cast<LogiaParser::ProgramContext *>(tree);
			auto stmts = program->programStmsList()->programStmt();
			std::cout << "stmts[" << stmts.size() << "]" << std::endl;
			for (auto p = stmts.begin(); p < stmts.end(); ++p)
			{
				auto f = (*p)->functionDecl();
				if (f)
				{
					// std::cout << "function!! " << f->functionDef()->functionModifiers() << std::endl;
					std::cout << "function found: " << f->functionDef()->functionName()->identifier()->getText() << std::endl;
				}
			}
		}

		/*
		auto root = tree;
		auto ruleRoot = ((RuleContext*)root);
		auto program = ((LanguageParser::ProgramContext*)ruleRoot);
		std::cout << "\n**********************************************\n" << std::endl;
		std::cout << program->toStringTree() << std::endl;
		program->children;
		*/
	}
	catch (std::exception e)
	{
		std::cout << "Error: " << e.what() << std::endl;
	}

	return 0;
}
