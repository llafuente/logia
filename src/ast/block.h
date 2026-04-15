#pragma once

#include "ast/node.h"

namespace logia::AST
{

    struct Block : public Node
    {
    public:
        // NOTE: about cpp
        // std::unordered_map<char*, Node*> scope; --> wrong char* is not the expected type, no "=="
        // std::unordered_map<string, Node*> scope; --> misc errors
        std::unordered_map<std::string_view, Node *> scope;
        // back pointer to fast access
        Block *parent = nullptr;
        // BasicBlock will be populated at create_llvm_block and cached
        // NOTE BasicBlock needs to be attached before codegen into them
        llvm::BasicBlock *llvm_basicblock = nullptr;
        // TODO remove body as we can reverse the tree and search it!
        Block(antlr4::ParserRuleContext *rule);
        /**
         * Register a name in the scope
         */
        void set(const char *name, Node *node);

        Node *lookup(const char *name);

        std::string to_string() override;
        /**
         * Creates named or returns cached LLVM BasicBlock
         *
         * NOTE: BasicBlocks needs to be created and attached before codegen inside them or raise SEH / parenting issues
         */
        llvm::BasicBlock *create_llvm_block(logia::Backend *codegen, char *name);
        llvm::Value *codegen(logia::Backend *codegen, llvm::IRBuilder<> *builder) override;
        Type *get_type() override;
        void post_attach() override;
    };

    /**
     * Creates a body (function body/block scope)
     */
    LOGIA_API LOGIA_LEND Block *ast_create_block();

}