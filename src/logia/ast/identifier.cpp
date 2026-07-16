#include "logia/ast/identifier.h"

#include "utils.h"
#include "logia/ast/vardeclstmt.h"
#include "logia/ast/block.h"
#include "logia/ast/callexpr.h"
#include "logia/ast/types/function.h"
#include "logia/ast/semantic_error.h"
#include "logia/type_inference.h"

#include "llvm/IR/Instructions.h"
#include "llvm/IR/DerivedTypes.h"

namespace logia::AST
{
    Identifier::Identifier(location loc, LOGIA_CLONE const char *identifier) : Expression(loc)
    {
        this->identifier = _strdup(identifier);
    }
    // copy constructor
    Identifier::Identifier(const Identifier &other) : Expression(other)
    {
        this->resolve = other.resolve;
        this->resolve_unique = other.resolve_unique;
        this->identifier = _strdup(other.identifier);
    }

    std::string Identifier::to_string()
    {
        // flags
        std::string flags = "";
        if (this->resolve)
        {
            flags += (flags.length() ? "," : "");
            flags += std::format("resolve");

            flags += (flags.length() ? "," : "");
            flags += std::format("resolved({})", this->decl_candidates.size());
        }
        flags += (flags.length() ? "," : "");
        flags += this->resolve_unique ? "unique" : "multiple";

        return std::format("Identifier[{}]{}{}", this->identifier, flags, Node::to_string());
    }

    void Identifier::post_codegen(logia::Backend *backend)
    {
        LOGIA_VERIFY(this->decl != nullptr);

        if (this->decl->is<VarDeclStmt>())
        {
            LOG(DBG, "{} points to a var declaration {}", (void *)this->decl, this->to_string());
            this->set_codegen_value(backend, this->decl->as<VarDeclStmt>()->alloca_inst);
            return Expression::post_codegen(backend);
        }
        if (this->decl->is<FunctionParameter>())
        {
            LOG(DBG, "{} points to a function parameter {}", (void *)this->decl, this->to_string());
            this->set_codegen_value(backend, this->decl->as<FunctionParameter>()->alloca_inst);
            return Expression::post_codegen(backend);
        }
        if (this->decl->is<Function>())
        {
            LOG(DBG, "{} points to a function {}", (void *)this->decl, this->to_string());
            // return the function itself - it's considered a constant so no debug information!
            this->set_codegen_value(nullptr, this->decl->as<Function>()->ir_func);
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

    void Identifier::on_after_attach() {}

    void Identifier::validate() {}

    void Identifier::_on_set_type(TypeDecl *t)
    {
    }

    bool Identifier::is_empty()
    {
        return this->identifier == nullptr || strlen(this->identifier) == 0;
    }

    std::vector<Node *> Identifier::scope_search(bool allow_empty, bool allow_multiple)
    {
        auto err = scope_lookup_all(this, this->identifier);
        if (err.is_error())
        {
            throw_semantic_error(this, err.message);
        }
        this->decl_candidates = err.unwrap_success();

        if (!allow_empty && this->decl_candidates.size() == 0)
        {
            throw_semantic_error(this, std::format(LGERR_ID001, this->identifier));
        }

        if (!allow_multiple && this->decl_candidates.size() > 1)
        {
            std::string debug_candidates = "";
            int i = 1;
            for (const auto &node : this->decl_candidates)
            {
                debug_candidates += std::format("Candidate {} declared {}\n", i++, node->loc.get_debug_location(0, 0));
                ++i;
            }
            throw_semantic_error(this, std::format(LGERR_ID002, this->decl_candidates.size(), this->identifier, debug_candidates));
        }
        return this->decl_candidates;
    }

    bool Identifier::type_inference(size_t pass_id)
    {
        switch (pass_id)
        {
        case TYPE_INFERENCE_PRE:
        {
            if (is_empty())
            {
                throw_compiler_error("try to type an empty identifier!");
            }
            if (!this->resolve)
            {
                LOG(DBG, "skip pre_type_inference resolve = false");
                return true;
            }

            this->scope_search(false, !resolve_unique);

            // just one -> go!
            if (this->decl_candidates.size() == 1)
            {
                // just one candidate, we can resolve it right now
                this->set_declaration(this->decl_candidates[0]);
                auto tyd = this->decl->get_type_decl();
                if (tyd == nullptr)
                {
                    return false;
                }

                this->set_type(tyd);
            }
            else
            {
                // my parent will type me and shoyuld be a CallExpression
                LOGIA_VERIFY(this->parent_node->is<CallExpression>() == true);
            }
        }
        break;
        }

        return true;
    }

    void Identifier::set_declaration(Node *node)
    {
        this->decl = node;
    }

} // namespace logia::AST
