#include "logia/ast/vardeclstmt.h"

#include "utils.h"
#include "logia/type_inference.h"
#include "logia/ast/identifier.h"
#include "logia/ast/types/typedef.h"
#include "logia/ast/types/struct.h"
#include "logia/ast/block.h"
#include "logia/ast/binaryexpr.h"
#include "logia/backend.h"

#include "llvm/IR/Instructions.h"

namespace logia::AST
{
    VarDeclStmt::VarDeclStmt(location loc, Identifier *id, Expression *expr) : Stmt(loc), alloca_inst(nullptr)
    {
        LOGIA_VERIFY(expr != nullptr);

        this->id = id;

        this->first_usage = expr;
        this->assignment = new BinaryExpression(expr->loc, node_clone(id), Operators::BINARY_ASSIGN, expr);
        this->push_child(this->assignment); // 0
    }

    VarDeclStmt::VarDeclStmt(location loc, Identifier *id, TypeDef *type, Expression *expr) : Stmt(loc), alloca_inst(nullptr)
    {
        LOGIA_VERIFY(type != nullptr);
        LOGIA_VERIFY(expr != nullptr);

        this->id = id;

        this->first_usage = expr;
        this->assignment = new BinaryExpression(expr->loc, node_clone(id), Operators::BINARY_ASSIGN, expr);
        this->push_child(this->assignment); // 0

        this->push_child(type); // 1
    }

    const char *VarDeclStmt::get_name()
    {
        return this->get_identifier()->identifier;
    }
    Identifier *VarDeclStmt::get_identifier()
    {
        return this->id;
    }

    BinaryExpression *VarDeclStmt::get_init_expr()
    {
        return this->assignment;
    }

    std::string VarDeclStmt::to_string()
    {
        return std::format("VarDeclStmt[{}]{}", this->get_name(), Node::to_string());
    }

    std::string VarDeclStmt::to_code(size_t ident)
    {
        auto type = this->get_type();
        // this is a "defect"
        if (this->assignment != nullptr)
        {
            return std::format("var {} {}\n{}", type == nullptr ? "" : type->to_code(), this->get_identifier()->to_code(), this->assignment->to_code(ident));
        }

        return std::format("var {} {}", type == nullptr ? "" : type->to_code(), this->get_identifier()->to_code());
    }

    void VarDeclStmt::post_codegen(logia::Backend *backend)
    {
        if (this->alloca_inst != nullptr)
        {
            return;
        }
        auto type = this->get_type_decl()->get_effective_type_decl();
        LOG(SILLY, "type = {}", type->to_string());
        LOG(SILLY, "{}", this->to_string());

        // TODO this will lead to problems when the type is below the declaration!
        // to support this we need to modify how the codegen is done, so atm we continue until the time comes!
        // type->codegen(backend);

        auto name = this->get_name();

        this->alloca_inst = backend->builder->CreateAlloca(type->ir_type, 0, nullptr, name);
        backend->set_debug_loc(this->alloca_inst, this->loc);

        this->assignment->post_codegen(backend);

        return Stmt::post_codegen(backend);
    }

    void VarDeclStmt::on_after_attach()
    {
        // it's unsage to push a child here -> move to CST2AST, I't makes no sense but vector iteration will fail otherwise!
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
        if (children.size() == 2)
        {
            return this->get_child<Type>(1);
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
                auto tyd = this->first_usage->get_type_decl();
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
            this->get_init_expr()->set_type(tyd);
        }
        break;
        }
        return true;
    }

    void VarDeclStmt::_on_set_type(TypeDecl *ty)
    {
        // TODO WHY? -> get_type could be nullptr always otherwise
        if (children.size() == 1)
        {
            this->push_child(ty);
        }

        this->get_identifier()->set_type(ty);
    }
} // namespace logia::AST
