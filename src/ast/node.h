#pragma once

#include <vector>

#include "ast/types.h"
#include "utils.h"
#include "backend.h"

#include "antlr4-runtime.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/Value.h"

struct ::logia::Backend;

namespace logia::AST
{
    struct Type;

    // Homogenous AST node type
    struct LOGIA_EXPORT Node
    {
    public:
        unsigned char is_attached : 1 = false;
        unsigned char is_codegen : 1 = false;

        /**
         * TODO
         */
        antlr4::ParserRuleContext *rule = nullptr;
        /**
         * Node type to be able to cast from node back to the real type
         */
        ast_types type = (ast_types)0;
        /**
         * parent node to traverse to root
         */
        Node *parent_node = nullptr;
        /**
         * Avoid children modification (from API uknow :)
         */
        bool freezed = false;
        /**
         * My beatiful children, and some not so beatiful.
         */
        std::vector<Node *> children = {}; // normalized list of children

        Node(antlr4::ParserRuleContext *rule, ast_types type);

        // TODO
        // std::string getText() { return this->rule->getText(); }
        /**
         * Adds child at the end
         */
        void push_child(Node *child);

        /**
         * Adds a child at start
         */
        void unshift_child(Node *child);

        void _has_to_notify_attached(Node *child);

        void __notify_attached();

        std::string to_string_tree(std::string padding = "");
        /**
         * print essential information nto debug
         */
        virtual std::string to_string() = 0;
        /**
         * AST -> LLVM
         */
        virtual llvm::Value *codegen(logia::Backend *codegen, llvm::IRBuilder<> *builder) = 0;

        virtual Type *get_type() = 0;
        virtual void on_after_attach();
    };
}