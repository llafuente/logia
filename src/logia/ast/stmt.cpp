#include "logia/ast/stmt.h"

#include "logia/backend.h"
#include "logia/ast/llvm.h"
#include "logia/ast/identifier.h"
#include "logia/ast/block.h"
#include "logia/ast/function.h"

namespace logia::AST
{
    //
    // Stmt
    //
    Stmt::Stmt(antlr4::ParserRuleContext *rule) : Node(rule) { this->has_type = false; }

    std::string Stmt::to_string()
    {
        return std::format("Statement{}", Node::to_string());
    }

    Type *Stmt::get_type()
    {
        return nullptr;
    }

    llvm::Value *Stmt::post_codegen(logia::Backend *backend)
    {
        if (this->cg_value != nullptr)
        {
            LOG(DBG, "{}", this->to_string());
            backend->set_debug_loc((llvm::Instruction *)this->cg_value, this->rule);
        }
        return Node::post_codegen(backend);
    }

    ///
    /// GotoStmt
    ///

    GotoStmt::GotoStmt(antlr4::ParserRuleContext *rule, Identifier *id) : Stmt(rule)
    {
        this->skip_type_inference = true; // we don't have type and don't need to calc anything!

        this->push_child(id);
        id->has_type = false; // there is no "block" type
        id->skip_codegen = true;
        id->skip_type_inference = true;
    }

    LOGIA_API LOGIA_LEND GotoStmt *ast_create_goto_stmt(Identifier *id)
    {
        return new GotoStmt(nullptr, id);
    }

    Identifier *GotoStmt::get_identifier()
    {
        return this->get_child<Identifier>(0);
    }

    const char *GotoStmt::get_name()
    {
        return this->get_identifier()->identifier;
    }

    std::string GotoStmt::to_string()
    {
        return std::format("GotoStmt[{}]{}", this->get_name(), Node::to_string());
    }

    llvm::Value *GotoStmt::post_codegen(logia::Backend *backend)
    {
        LOG(DBG, "{}", this->to_string());
        // find label and jump to it
        // label shall be inside the current function
        auto func = this->first_parent<Function>();
        Block *block = nullptr;
        auto id = this->get_name();
        if (func->get_body()->try_look<Block>(id, &block))
        {
            block->codegen(backend);
            this->cg_value = backend->builder->CreateBr(block->ir_basicblock);
            return Stmt::post_codegen(backend);
        }

        throw_semantic_error(this, std::format("LGERR_GT001 use of undeclared or unreachable label '{}' ", id));
    }

    void GotoStmt::_set_type(Type *ty) {}
}
