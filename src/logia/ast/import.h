#pragma once

#include <vector>

#include "logia/ast/node.h"
#include "logia/ast/block.h"

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
        /// @brief If true, all the imported symbols will be directly available in the parent scope, otherwise they will be available under the package name
        bool is_import_into_scope : 1 = false;

        /// @brief If true, all symbols from the package will be imported
        bool is_import_all_package : 1 = false;

        /// @brief Package to import, represented as a list of identifiers, for example "foo.bar.baz" will be ["foo", "bar", "baz"]
        std::vector<AST::Identifier *> package = {};
        /// @brief Import symbol list
        std::vector<AST::Identifier *> import_list = {};
        /// @brief The result of parsing the imported file
        ParseResult *parse_result = nullptr;
        /// @brief The target scope where the imported symbols will be registered, if nullptr the parent scope will be used
        Scope *target = nullptr;

        Import(antlr4::ParserRuleContext *rule);

        std::string to_string() override;

        /// @brief Sets the package to import
        /// @param package The package represented as a list of identifiers
        void set_package(std::vector<AST::Identifier *> package);
        /// @brief Sets the import to be directly into the parent scope
        void set_import_into_scope();
        /// @brief Sets the import to include all symbols from the package
        void set_import_all();
        /// @brief Sets the list of symbols to import
        /// @param list The list of identifiers to import
        void set_import_list(std::vector<AST::Identifier *> list);
        /// @brief Sets the target scope for the imported symbols
        /// @param target The target scope
        void set_scope_target(Scope *target);
        /// @brief Throws if the import is not valid
        void validate();
        /// @brief Parses file at this stage, once attached we can parse and copy our scope if required
        void post_attach() override;
    };
}