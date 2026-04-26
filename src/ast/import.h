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

    // Design
    // A program do not need Backend information, but a logia Program does!
    // When creating a program we fill primitives and a bit later intrinsics,
    // those instrinsics need a llvm type to logia type conversion and that require
    // backend in the early stage. Not the end of the world, just leave the note to know

    /// @brief Root of the AST, contains all the top level declarations and statements
    class Import : public Scope
    {
    public:
        // if list is empty -> imply is_import_into_scope ?
        bool is_import_into_scope : 1 = false;

        bool is_import_all_package : 1 = false;

        std::vector<AST::Identifier *> package;
        std::vector<AST::Identifier *> import_list;

        // resolved from package
        const char *entry_point_file;

        Import(antlr4::ParserRuleContext *rule);

        std::string to_string() override;

        void set_package(std::vector<AST::Identifier *> package);
        void set_import_into_scope();
        void set_import_all();
        void set_import_list(std::vector<AST::Identifier *> list);

        Type *get_type() override;
        void post_attach() override;
    };
}