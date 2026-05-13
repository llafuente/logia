#include "ast/stmt.h"
#include "ast/traverse.h"
#include "ast/llvm.h"

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
            DEBUG() << this->to_string() << std::endl;
            backend->set_debug_loc((llvm::Instruction *)this->cg_value, this->rule);
        }
        return Node::post_codegen(backend);
    }

    //
    // ReturnStmt
    //

    ReturnStmt::ReturnStmt(antlr4::ParserRuleContext *rule, Expression *expr) : Stmt(rule)
    {
        this->push_child(expr);
    }

    std::string ReturnStmt::to_string()
    {
        return std::format("ReturnStmt{}", Node::to_string());
    }

    Expression *ReturnStmt::get_expr()
    {
        return (Expression *)this->children[0];
    }

    llvm::Value *ReturnStmt::post_codegen(logia::Backend *backend)
    {
        DEBUG() << this->to_string() << std::endl;
        auto expr = this->get_expr();
        if (!expr)
        {
            this->cg_value = backend->builder->CreateRetVoid();
        }
        else
        {
            auto value = llvm_load_if_required(expr->codegen(backend), backend);
            this->cg_value = backend->builder->CreateRet(value);
        }

        return Stmt::post_codegen(backend);
    }

    //
    // VarDeclStmt
    //
    VarDeclStmt::VarDeclStmt(antlr4::ParserRuleContext *rule, Identifier *id, Type *type, Expression *expr) : Stmt(rule), alloca_inst(nullptr)
    {
        this->push_child(id);
        if (type == nullptr)
        {
            this->push_child(new InferType());
        }
        else
        {
            this->is_typed = true;
            this->push_child(type);
        }
        this->push_child(expr);
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
        return this->get_child<Expression>(2);
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

        if (type->is<Struct>())
        {
            auto dl = backend->module->getDataLayout();
            auto gv = llvm::dyn_cast<llvm::GlobalVariable>(init_value);
            backend->builder->CreateMemCpy(this->alloca_inst, this->alloca_inst->getAlign(), init_value, init_value->getPointerAlignment(dl), dl.getTypeAllocSize(gv->getValueType()));
        }
        else
        {
            backend->builder->CreateStore(init_value, this->alloca_inst);
        }

        return Stmt::post_codegen(backend);
    }

    //
    // GotoStmt
    //

    GotoStmt::GotoStmt(antlr4::ParserRuleContext *rule, Identifier *id) : Stmt(rule)
    {
        this->push_child(id);
    }

    //
    //
    //
    LOGIA_API LOGIA_LEND GotoStmt *ast_create_goto_stmt(Identifier *id)
    {
        return new GotoStmt(nullptr, id);
    }

    ///
    /// toString
    ///
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

    ///
    /// codegen
    ///

    llvm::Value *GotoStmt::post_codegen(logia::Backend *backend)
    {
        DEBUG() << this->to_string() << std::endl;
        // find label and jump to it
        // label shall be inside the current function
        auto func = this->first_parent<Function>();
        Block *block = nullptr;
        if (func->get_body()->try_look<Block>(this->get_name(), &block))
        {
            block->codegen(backend);
            this->cg_value = backend->builder->CreateBr(block->llvm_basicblock);
            return Stmt::post_codegen(backend);
        }

        throw std::runtime_error(std::string("Expected a block: ") + this->to_string());
    }

    //
    // ast creation
    //
    LOGIA_API LOGIA_LEND ReturnStmt *ast_create_return(Expression *ret)
    {
        auto stmt = new ReturnStmt(nullptr, ret);
        return stmt;
    }

    LOGIA_API LOGIA_LEND VarDeclStmt *ast_create_var_decl(Identifier *id, Type *type, Expression *expr)
    {
        LOGIA_ASSERT(id);

        VarDeclStmt *variable = new VarDeclStmt(nullptr, id, type, expr);

        return variable;
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
        return this->get_child<Type>(1)->get_final_type();
    }
    void VarDeclStmt::set_type(Type *ty)
    {
        if (this->is_typed)
        {
            throw_compiler_error("already has a type!");
        }
        // TODO assume [1] is InferType ?
        this->children[1]->set_type(ty);
        this->is_typed = true;
    }
}
