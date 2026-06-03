#include "logia/ast/functionblock.h"

#include "logia/ast/type.h"
#include "logia/ast/function.h"
#include "logia/backend.h"

namespace logia::AST
{
    //
    // FunctionBlock
    //

    FunctionBlock::FunctionBlock(antlr4::ParserRuleContext *rule, Identifier *name) : Block(rule, name) {}
    std::string FunctionBlock::to_string()
    {
        return std::format("{}{}", "Function", Block::to_string());
    }

    void FunctionBlock::pre_codegen(logia::Backend *backend)
    {
        if (is_inserted)
        {
            return;
        }
        is_inserted = true;

        Block::pre_codegen(backend);
        auto function = this->first_parent<Function>();
        function->ir_func->insert(function->ir_func->end(), this->ir_basicblock);
        backend->builder->SetInsertPoint(this->ir_basicblock);

        this->parent_node->as<Function>()->codegen_parameters(backend);
    }

    llvm::Value *FunctionBlock::post_codegen(logia::Backend *backend)
    {
        backend->builder->SetInsertPoint(this->ir_basicblock);

        Block::codegen_children(backend);

        this->cg_value = this->ir_basicblock;
        return Node::post_codegen(backend);
    }

    void FunctionBlock::post_attach()
    {
        Scope::post_attach();
    }

}