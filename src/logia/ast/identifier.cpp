#include "logia/ast/identifier.h"

#include "utils.h"
#include "logia/ast/vardeclstmt.h"
#include "logia/ast/block.h"
#include "logia/ast/function.h"
#include "logia/ast/semantic_error.h"

#include "llvm/IR/Instructions.h"
#include "llvm/IR/DerivedTypes.h"

namespace logia::AST
{
    Identifier::Identifier(location loc, LOGIA_CLONE const char *identifier) : Expression(loc)
    {
        this->identifier = _strdup(identifier);
    }
    std::string Identifier::to_string()
    {
        return std::format("Identifier[{}]{}", this->identifier, Node::to_string());
    }

    llvm::Value *Identifier::post_codegen(logia::Backend *backend)
    {
        LOGIA_VERIFY(this->decl != nullptr);

        if (this->decl->is<VarDeclStmt>())
        {
            LOG(DBG, "{} points to a var declaration {}", (void *)this->decl, this->to_string());
            this->cg_value = this->decl->as<VarDeclStmt>()->alloca_inst;
            return Expression::post_codegen(backend);
        }
        if (this->decl->is<FunctionParameter>())
        {
            LOG(DBG, "{} points to a function parameter {}", (void *)this->decl, this->to_string());
            this->cg_value = this->decl->as<FunctionParameter>()->alloca_inst;
            return Expression::post_codegen(backend);
        }
        if (this->decl->is<Function>())
        {
            LOG(DBG, "{} points to a function {}", (void *)this->decl, this->to_string());
            // return the function itself
            this->cg_value = (llvm::Value *)this->decl->as<Function>()->ir_func;
            // NOTE do not use Expression::post_codegen because it will set debug information on the real function -> wrong and also SEH
            return Node::post_codegen(backend);
        }
        LOG(DBG, "{} points to {}", this->to_string(), this->decl->to_string());
        // TODO function? -> function pointer
        throw_compiler_error(std::format("{}{}", "Identifier found but type not handled yet {}!", this->decl->to_string(), typeid(this->decl).name()));
    }
    bool Identifier::operator==(const char *id)
    {
        return strcmp(this->identifier, id) == 0;
    }
    bool Identifier::operator==(const Identifier *id)
    {
        return strcmp(this->identifier, id->identifier) == 0;
    }

    VarDeclStmt *Identifier::get_var_decl()
    {
        return this->decl == nullptr ? nullptr : (this->decl->is<VarDeclStmt>() ? nullptr : this->decl->as<VarDeclStmt>());
    }

    Function *Identifier::get_function_decl()
    {
        throw_compiler_error("todo");
    }

    void Identifier::post_attach()
    {
        Expression::post_attach();
        // TODO
    }

    Type *Identifier::get_type()
    {
        return this->type == nullptr ? nullptr : this->type;
    }

    void Identifier::_set_type(Type *t)
    {
        this->type = t;
    }

    Node *Identifier::resolve()
    {
        if (this->is_empty())
        {
            return nullptr;
        }

        this->pre_type_inference();
        return this->decl;
    }

    bool Identifier::is_empty()
    {
        return this->identifier == nullptr || strlen(this->identifier) == 0;
    }

    void Identifier::_pre_type_inference()
    {
        if (is_empty())
        {
            throw_compiler_error("try to type an empty identifier!");
        }
        // this means my parent will type_inference this node!
        if (!this->is_typed)
        {
            auto err = scope_lookup_all(this, this->identifier);
            if (err.is_error())
            {
                throw_semantic_error(this, err.message);
            }
            auto list = err.unwrap_success();
            // even success could be a problem :)
            if (list.size() == 0)
            {
                throw_semantic_error(this, std::format(LGERR_ID001, this->identifier));
            }
            if (list.size() > 1)
            {
                std::string debug_candidates = "";
                int i = 1;
                for (const auto &node : list)
                {
                    debug_candidates += std::format("Candidate {} declared {}\n", i++, node->loc.get_debug_location(0, 0));
                    ++i;
                }
                throw_semantic_error(this, std::format(LGERR_ID002, list.size(), this->identifier, debug_candidates));
            }

            this->set_declaration(list[0]);
            // just one is ok!
            auto ty = this->decl->get_final_type();
            if (ty == nullptr)
            {
                LOG(WRN, "skip._pre_type_inference (target no type) {}", this->to_string());
                return; // skip for later!
            }
            this->set_type(ty);
        }
        Node::_pre_type_inference();
    }

    void Identifier::set_declaration(Node *node)
    {
        this->decl = node;
    }

    LOGIA_API Identifier *ast_create_identifier(LOGIA_CLONE const char *name)
    {
        LOGIA_VERIFY(name != nullptr);

        return new Identifier({}, _strdup(name));
    }

} // namespace logia::AST
