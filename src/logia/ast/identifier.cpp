#include "logia/ast/identifier.h"
#include "logia/ast/vardeclstmt.h"
#include "logia/ast/block.h"
#include "logia/ast/function.h"

namespace logia::AST
{
    Identifier::Identifier(antlr4::ParserRuleContext *rule, LOGIA_CLONE const char *identifier) : Expression(rule)
    {
        this->identifier = strdup(identifier);
    }
    std::string Identifier::to_string()
    {
        return std::format("Identifier[{}]{}", this->identifier, Node::to_string());
    }

    llvm::Value *Identifier::post_codegen(logia::Backend *backend)
    {
        LOG(DBG, "{}", this->to_string());

        auto decl = this->first_parent<Scope>()->lookup<Node>(this->identifier);
        if (decl->is<VarDeclStmt>())
        {
            this->cg_value = decl->as<VarDeclStmt>()->alloca_inst;
            return Expression::post_codegen(backend);
        }
        if (decl->is<FunctionParameter>())
        {
            this->cg_value = decl->as<FunctionParameter>()->alloca_inst;
            return Expression::post_codegen(backend);
        }
        // TODO function? -> function pointer
        throw_compiler_error(std::format("{}{}", "Identifier found but type not handled yet {}!", decl->to_string(), typeid(decl).name()));
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
        return this->first_parent<Block>()->lookup<VarDeclStmt>(this->identifier);
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
        if (this->identifier == nullptr || strlen(this->identifier) == 0)
        {
            return nullptr;
        }

        auto scope = this->first_parent<Scope>();
        return scope->lookup<Node>(this->identifier);
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
            auto ty = this->resolve()->get_final_type();
            if (ty == nullptr)
            {
                LOG(WRN, "skip._pre_type_inference (target no type) {}", this->to_string());
                return; // skip for later!
            }
            this->set_type(ty);
        }
        Node::_pre_type_inference();
    }

    LOGIA_API Identifier *ast_create_identifier(LOGIA_CLONE const char *name)
    {
        LOGIA_VERIFY(name != nullptr);

        return new Identifier(nullptr, strdup(name));
    }

} // namespace logia::AST
