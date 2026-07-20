#include "logia/ast/vardeclstmt.h"

#include "utils.h"
#include "logia/type_inference.h"
#include "logia/ast/identifier.h"
#include "logia/ast/types/typedef.h"
#include "logia/ast/types/struct.h"
#include "logia/ast/block.h"
#include "logia/backend.h"

#include "llvm/IR/Instructions.h"

namespace logia::AST
{
    VarDeclStmt::VarDeclStmt(location loc, Identifier *id, Expression *expr) : Stmt(loc), alloca_inst(nullptr)
    {
        LOGIA_VERIFY(expr != nullptr);

        this->push_child(id); // 0
        // we will set the type based on expr / type
        id->type_inference_pass_id = TYPE_INFERENCE_MAX;
        this->push_child(expr); // 1
    }

    VarDeclStmt::VarDeclStmt(location loc, Identifier *id, TypeDef *type, Expression *expr) : Stmt(loc), alloca_inst(nullptr)
    {
        LOGIA_VERIFY(type != nullptr);
        LOGIA_VERIFY(expr != nullptr);

        this->push_child(id); // 0
        // we will set the type based on expr / type
        id->type_inference_pass_id = TYPE_INFERENCE_MAX;
        this->push_child(expr); // 1

        this->push_child(type); // 2
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

    std::string VarDeclStmt::to_code(size_t ident)
    {
        auto type = this->get_type();
        auto expr = this->get_expr();

        return std::format("var {} {} = {}", type == nullptr ? "" : type->to_code(), this->get_identifier()->to_code(), expr ? expr->to_code() : "");
    }

    void VarDeclStmt::post_codegen(logia::Backend *backend)
    {
        if (this->alloca_inst != nullptr)
        {
            return;
        }
        auto type = this->get_type_decl()->get_effective_type_decl();
        auto expr = this->get_expr();
        LOG(SILLY, "type = {}", type->to_string());
        LOG(SILLY, "expr = {}", expr->to_string());
        LOG(SILLY, "{}", this->to_string());

        // TODO this will lead to problems when the type is below the declaration!
        // to support this we need to modify how the codegen is done, so atm we continue until the time comes!
        // type->codegen(backend);

        auto name = this->get_name();
        auto init_value = expr->get_codegen_value(backend);

        this->alloca_inst = backend->builder->CreateAlloca(type->ir_type, 0, nullptr, name);
        backend->set_debug_loc(this->alloca_inst, this->loc);

        // TODO this should be handled by "binaryExpression" ?
        // that said ->
        // var x = xxx() <-- function xxx() big_struct
        // that big_struct has to be "stack allocated" and passed by pointer, a PIA at this stage!
        if (type->is<Struct>())
        {
            if (llvm::isa<llvm::GlobalVariable>(init_value))
            {
                LOG(DBG, "struct store from global variable");
                auto dl = backend->module->getDataLayout();
                auto gv = llvm::dyn_cast<llvm::GlobalVariable>(init_value);
                backend->builder->CreateMemCpy(this->alloca_inst, this->alloca_inst->getAlign(), init_value, init_value->getPointerAlignment(dl), dl.getTypeAllocSize(gv->getValueType()));
            }
            else
            {
                LOG(DBG, "struct store from value");
                backend->builder->CreateStore(init_value, this->alloca_inst);
            }
        }
        else
        {
            LOG(DBG, "function/primitive store");
            backend->builder->CreateStore(init_value, this->alloca_inst);
        }
        return Stmt::post_codegen(backend);
    }

    void VarDeclStmt::on_after_attach()
    {
    }

    void VarDeclStmt::validate()
    {
        auto result = scope_lookup_all(this, this->get_name());
        if (result.is_success())
        {
            auto list = result.unwrap_success();
            if (list.size())
            {
                // ups! collisions!
                int i = 1;
                std::string redeclarations;
                for (const auto &redeclaration : list)
                {
                    redeclarations += std::format("declared {} here:\n{}\n", i++, redeclaration->loc.get_debug_location(0, 0));
                }

                throw_semantic_error(this, std::format(LGERR_VDECL001, this->get_name(), this->loc.get_debug_location(3, 1), redeclarations));
            }
        }
        // ok, nobody has our id -> register!
        logia::AST::scope_set(this, this->get_name(), this, true);
    }

    Type *VarDeclStmt::get_type()
    {
        if (children.size() == 3)
        {
            return this->get_child<Type>(2);
        }
        return nullptr;
    }

    bool VarDeclStmt::type_inference(size_t pass_id)
    {
        switch (pass_id)
        {
        case TYPE_INFERENCE_PRE:
        {
            auto ty = this->get_type();
            // if I don't have a type -> my type is in the initialization!
            if (ty == nullptr)
            {
                auto tyd = this->get_expr()->get_type_decl();
                if (tyd != nullptr)
                {
                    this->set_type(tyd);
                    return true;
                }
                return false;
            }
            // my type is not ready? -> wait
            if (ty->type_inference_pass_id < TYPE_INFERENCE_PRE)
            {
                return false;
            }
            // set and foward to the initialization
            auto tyd = ty->get_type_decl();
            if (tyd == nullptr)
            {
                return false;
            }
            this->set_type(tyd);
            this->get_expr()->set_type(tyd);

            /*
            // expression --> VarDeclStmt
            auto type = this->get_expr()->get_final_type();
            if (type != nullptr)
            {
                this->set_type(type);
                return Stmt::_pre_type_inference();
            }
            // cannot determine type, "try later"
            */
        }
        break;
        }
        return true;
    }

    void VarDeclStmt::_on_set_type(TypeDecl *ty)
    {
        // TODO WHY? -> get_type could be nullptr always otherwise
        if (children.size() < 3)
        {
            this->push_child(ty);
        }

        this->get_identifier()->set_type(ty);
    }
} // namespace logia::AST
