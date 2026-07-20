#include "logia/ast/functionblock.h"

#include "logia/ast/types/type.h"
#include "logia/ast/types/function.h"
#include "logia/backend.h"

namespace logia::AST
{
    //
    // FunctionBlock
    //

    FunctionBlock::FunctionBlock(location loc, Identifier *name) : Block(loc, name) {}

    std::string FunctionBlock::to_string()
    {
        return std::format("{}{}", "Function", Block::to_string());
    }

    std::string FunctionBlock::to_code(size_t ident)
    {
        return Block::to_code(size_t ident));
    }

    void FunctionBlock::pre_codegen(logia::Backend *backend)
    {
        if (is_inserted)
        {
            throw_compiler_error("why do you call this ?");
            return;
        }
        is_inserted = true;

        Block::pre_codegen(backend); // pre call to populate ir_basicblock
        auto function = this->first_parent<Function>();
        function->ir_func->insert(function->ir_func->end(), this->ir_basicblock);
    }

    void FunctionBlock::post_codegen(logia::Backend *backend)
    {
        LOG(DBG, "");
        backend->builder->SetInsertPoint(this->ir_basicblock);
        this->parent_node->as<Function>()->codegen_parameters(backend);

        Block::post_codegen_children(backend);

        return Node::post_codegen(backend);
    }

    void FunctionBlock::on_after_attach()
    {
        // skip block!
        Scope::on_after_attach();
    }

    void FunctionBlock::validate()
    {
        // skip block!
        Scope::validate();
    }

}