#include "logia/ast/semantic_error.h"

#include "logia/ast/node.h"
#include "logia/ast/program.h"
#include "logia/ast/import.h"

#include <format>
#include <stacktrace>

namespace logia::AST
{

    semantic_error::semantic_error(Node *node, const std::string &message, const std::string &trace, const char *function, const char *file, int line) : std::runtime_error(format_message(node, message, trace, function, file, line)) {}
    // semantic_error::semantic_error(const char *message, const char *function, const char *file, int line): std::runtime_error(format_message(message, function, file, line)) {}

    std::string semantic_error::format_message(Node *node, const std::string &message, const std::string &trace, const char *function, const char *file, int line)
    {
        std::string code_location;
        if (node->rule == nullptr)
        {
            code_location = std::format("at <unknown>:0:0", message);
        }
        else
        {
            // TODO handle error at import
            // TODO extract code snippet!!
            auto program = node->first_parent<Program>();

            auto err_line = node->rule->start->getLine();
            auto err_start_column = node->rule->start->getCharPositionInLine();
            auto err_stop_column = std::max<int>(err_start_column + 1, node->rule->stop->getCharPositionInLine());

            auto start_line = std::max<size_t>(0, err_line - 2);
            auto end_line = err_line + 2;

            char snippet[1024];
            const char *text = node->first_parent<Program>()->file_contents;
            size_t src = 0;
            size_t dst = 0;
            size_t line = 0;
            char c;
            while ((c = text[src++]) != '\0')
            {
                if (line >= start_line && line <= end_line)
                {
                    snippet[dst++] = c;
                }

                if (c == '\n')
                {
                    if (line == err_line)
                    {
                        for (size_t j = 0; j < err_start_column; ++j)
                        {
                            snippet[dst++] = ' ';
                        }
                        for (size_t j = err_start_column; j < err_stop_column; ++j)
                        {
                            snippet[dst++] = '^';
                        }
                        snippet[dst++] = '\n';
                    }
                    ++line;
                }
            }
            snippet[dst++] = '\0';

            code_location = std::format("at {}:{}:{}\n{}", program->entry_point_file, err_line + 1, err_start_column, snippet);
        }

        return std::format("semantic error:\n    \033[31m{}\033[0m\nat: {}\nExpcetion thrown at {} {}:{}\n\nstacktrace:\n{}", message, code_location, function, file, line, trace);
    }
}
/*
    semantic_error::semantic_error(antlr4::ParserRuleContext *rule, const std::string &message)
        : std::runtime_error(format_message(rule, message))
    {
    }

    std::string semantic_error::format_message(antlr4::ParserRuleContext *rule, const std::string &message)
    {
        if (!rule)
        {
            return std::format("{} [at <unknown>:0:0]", message);
        }

        auto start = rule->getStart();
        if (!start)
        {
            return std::format("{} [at <unknown>:0:0]", message);
        }

        std::string source = "<unknown>";
        if (auto tokenSource = start->getTokenSource())
        {
            if (auto input = tokenSource->getInputStream())
            {
                source = input->getSourceName();
            }
        }

        return std::format("{} [at {}:{}:{}]", message, source, start->getLine(), start->getCharPositionInLine());
    }

    [[noreturn]] void throw_parser_context_exception(antlr4::ParserRuleContext *rule, const std::string &message)
    {
        throw ParserRuleContextException(rule, message);
    }

    [[noreturn]] void throw_node_exception(const Node *node, const std::string &message)
    {
        throw_parser_context_exception(node ? node->rule : nullptr, message);
    }
}
*/