#include "logia/ast/stmt.h"

#include "utils.h"
#include "logia/type_inference.h"
#include "logia/backend.h"
#include "logia/ast/llvm.h"
#include "logia/ast/identifier.h"
#include "logia/ast/block.h"
#include "logia/ast/function.h"
#include "logia/ast/semantic_error.h"

namespace logia::AST
{
    //
    // Stmt
    //
    Stmt::Stmt(location loc) : Node(loc) { this->has_type = false; }

    std::string Stmt::to_string()
    {
        return std::format("Statement{}", Node::to_string());
    }

    Type *Stmt::get_type()
    {
        return this->real_type;
    }

    ///
    /// GotoStmt
    ///

    GotoStmt::GotoStmt(location loc, Identifier *name) : Stmt(loc)
    {
        // we don't have type and don't need to calc anything!
        this->type_inference_pass_id = TYPE_INFERENCE_MAX;

        this->push_child(name);
        name->has_type = false; // there is no "block" type
        name->skip_codegen = true;
        name->type_inference_pass_id = TYPE_INFERENCE_MAX;
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

    void GotoStmt::post_codegen(logia::Backend *backend)
    {
        LOG(DBG, "{}", this->to_string());
        // find label and jump to it
        // label shall be inside the current function
        auto func = this->first_parent<Function>();
        Block *block = nullptr;
        auto ident = this->get_identifier();

        auto result = scope_lookup_all(this, ident->identifier);
        if (result.is_error())
        {
            throw_semantic_error(this, result.message);
        }
        auto list = result.unwrap_success();
        if (list.size() == 0)
        {
            throw_semantic_error(this, std::format(LGERR_GT001, ident->identifier));
        }
        if (list.size() > 1)
        {
            throw_semantic_error(this, std::format(LGERR_GT002, ident->identifier, "to-do!"));
        }

        if (!list[0]->try_cast<Block>(&block))
        {
            throw_semantic_error(this, std::format(LGERR_GT003, ident->identifier, list[0]->loc.get_debug_location()));
        }
        block->pre_codegen(backend);
        LOGIA_VERIFY(block->ir_basicblock != nullptr); // should be generated in pre_codegen!
        auto inst = backend->builder->CreateBr(block->ir_basicblock);
        backend->set_debug_loc(inst, this->loc);
        return Stmt::post_codegen(backend);
    }

    void GotoStmt::on_after_attach() {}

    void GotoStmt::validate() {}

    void GotoStmt::_on_set_type(TypeDecl *ty) {}
}
