#include "logia/ast/vardeclstmt.h"

#include "logia/ast/identifier.h"
#include "logia/ast/struct.h"

namespace logia::AST
{
    VarDeclStmt::VarDeclStmt(antlr4::ParserRuleContext *rule, Identifier *id, Type *type, Expression *expr) : Stmt(rule), alloca_inst(nullptr)
    {
        this->push_child(id);           // 0
        id->skip_type_inference = true; // sets the same as vardecl
        this->push_child(expr);         // 1
        if (type != nullptr)
        {
            this->set_type(type);
        }
    }

    const char *VarDeclStmt::get_name()
    {
        return this->get_identifier()->identifier;
    }
    Identifier *VarDeclStmt::get_identifier()
    {
        return this->get_child<Identifier>(0);
    }

    Expression *VarDeclStmt::get_expr()
    {
        return this->get_child<Expression>(1);
    }

    std::string VarDeclStmt::to_string()
    {
        return std::format("VarDeclStmt[{}]{}", this->get_name(), Node::to_string());
    }

    llvm::Value *VarDeclStmt::post_codegen(logia::Backend *backend)
    {
        if (this->alloca_inst != nullptr)
        {
            return this->alloca_inst;
        }

        DEBUG() << this->to_string() << std::endl;
        auto init_value = (llvm::Value *)this->get_expr()->codegen(backend);
        auto type = this->get_final_type();
        type->codegen(backend);
        auto name = this->get_name();

        this->cg_value = this->alloca_inst = backend->builder->CreateAlloca(type->ir_type, 0, nullptr, name);

        // TODO this shoul be handled by "binaryExpression"
        // that said
        // var x = xxx() <-- function xxx() big_struct
        // that big_struct has to be "stack allocated" and passed by pointer, a PIA at this stage!
        if (type->is<Struct>())
        {
            if (llvm::isa<llvm::GlobalVariable>(init_value))
            {
                auto dl = backend->module->getDataLayout();
                auto gv = llvm::dyn_cast<llvm::GlobalVariable>(init_value);
                backend->builder->CreateMemCpy(this->alloca_inst, this->alloca_inst->getAlign(), init_value, init_value->getPointerAlignment(dl), dl.getTypeAllocSize(gv->getValueType()));
            }
            else
            {
                backend->builder->CreateStore(init_value, this->alloca_inst);
            }
        }
        else
        {
            backend->builder->CreateStore(init_value, this->alloca_inst);
        }

        return Stmt::post_codegen(backend);
    }

    void VarDeclStmt::post_attach()
    {
        if (!this->is_attached)
        {
            this->is_attached = true;
            auto block = this->first_parent<Block>();
            block->scope_set(this->get_name(), this);
        }
    }

    Type *VarDeclStmt::get_type()
    {
        if (children.size() == 3)
        {
            return this->get_child<Type>(2)->get_final_type();
        }
        return nullptr;
    }
    void VarDeclStmt::_pre_type_inference()
    {
        // VarDeclStmt --> expression
        if (this->is_typed)
        {
            this->get_expr()->set_type(this->get_type());
            return Stmt::_pre_type_inference();
        }
        // expression --> VarDeclStmt
        auto type = this->get_expr()->get_final_type();
        if (type != nullptr)
        {
            this->set_type(type);
            return Stmt::_pre_type_inference();
        }
        // cannot determine type, "try later"
    }

    void VarDeclStmt::_set_type(Type *ty)
    {
        if (children.size() == 3)
        {
            this->children[2] = ty;
        }
        else
        {
            this->push_child(ty);
        }

        this->get_identifier()->set_type(ty);
    }

    LOGIA_API LOGIA_LEND VarDeclStmt *ast_create_var_decl(Identifier *id, Type *type, Expression *expr)
    {
        LOGIA_ASSERT(id);

        VarDeclStmt *variable = new VarDeclStmt(nullptr, id, type, expr);

        return variable;
    }
} // namespace logia::AST
