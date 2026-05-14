#pragma once

#include <vector>

#include "ast/node.h"
#include "ast/block.h"

#include "utils.h"

#include "antlr4-runtime.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/Value.h"
namespace logia
{
    struct Backend;
    struct ParseResult;
}

namespace logia::AST
{
    struct Type;
    struct Identifier;

    /// @brief Imports a source file
    class Import : public Scope
    {
    public:
        // if list is empty -> imply is_import_into_scope ?
        bool is_import_into_scope : 1 = false;

        bool is_import_all_package : 1 = false;

        std::vector<AST::Identifier *> package;
        std::vector<AST::Identifier *> import_list;

        ParseResult *parse_result;
        Scope *target;

        Import(antlr4::ParserRuleContext *rule);

        std::string to_string() override;

        void set_package(std::vector<AST::Identifier *> package);
        void set_import_into_scope();
        void set_import_all();
        void set_import_list(std::vector<AST::Identifier *> list);
        void set_scope_target(Scope *target);

        /// @brief Parses file at this stage, once attached we can parse and copy our scope if required
        void post_attach() override;
    };
}