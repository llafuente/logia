#include "ast/stmt.h"
#include "ast/traverse.h"

namespace logia::AST
{
    //
    // Stmt
    //
    Stmt::Stmt(antlr4::ParserRuleContext *rule, ast_types type) : Node(rule, (ast_types)(type | ast_types::STMT)) {}
    std::string Stmt::to_string()
    {
        return "Statement";
    }
    Type *Stmt::get_type()
    {
        return nullptr;
    }

    //
    // ReturnStmt
    //

    ReturnStmt::ReturnStmt(antlr4::ParserRuleContext *rule, Expression *expr) : Stmt(rule, ast_types::RETURN_STMT)
    {
        this->push_child(expr);
    }

    std::string ReturnStmt::to_string()
    {
        return "ReturnStmt";
    }

    Expression *ReturnStmt::get_expr()
    {
        return (Expression *)this->children[0];
    }

    llvm::Value *ReturnStmt::codegen(logia::Backend *codegen, llvm::IRBuilder<> *builder)
    {
        DEBUG() << this->to_string() << std::endl;
        auto expr = this->get_expr();
        if (!expr)
        {
            return builder->CreateRetVoid();
            // return llvm::ReturnInst::Create(codegen->context);
        }
        // return llvm::ReturnInst::Create(codegen->context, this->expr->codegen(codegen, builder));
        return builder->CreateRet(expr->codegen(codegen, builder));
    }

    //
    //
    //
    VarDeclStmt::VarDeclStmt(antlr4::ParserRuleContext *rule, Identifier *id, Type *type, Expression *expr) : Stmt(rule, ast_types::VAR_DECL_STMT), ir(nullptr)
    {
        this->push_child(id);
        if (type == nullptr)
        {
            this->push_child(new NoOp());
        }
        else
        {
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
        return (Identifier *)this->children[0];
    }

    Expression *VarDeclStmt::get_expr()
    {
        return (Expression *)this->children[1];
    }

    std::string VarDeclStmt::to_string()
    {
        return std::format("VarDeclStmt[{}] ({:p})", this->get_name(), static_cast<void *>(this));
    }

    llvm::Value *VarDeclStmt::codegen(logia::Backend *codegen, llvm::IRBuilder<> *builder)
    {
        if (this->ir != nullptr)
        {
            return this->ir;
        }

        DEBUG() << this->to_string() << std::endl;
        auto value = (llvm::Value *)this->get_expr()->codegen(codegen, builder);

        // TODO Type should be handled before ?
        // this->ir = builder->CreateAlloca((llvm::Type*) this->type->codegen(codegen, builder), 0, value);
        // this->ir = builder->CreateAlloca(value->getType(), 0, value);
        this->ir = builder->CreateAlloca(value->getType(), 0, nullptr);
        builder->CreateStore(value, this->ir);

        return this->ir;
    }

    //
    // GotoStmt
    //

    GotoStmt::GotoStmt(antlr4::ParserRuleContext *rule, char *name) : Stmt(rule, ast_types::GOTO_STMT)
    {
        this->name = name;
    }

    //
    //
    //
    LOGIA_API LOGIA_LEND GotoStmt *ast_create_goto_stmt(Block *body, char *name)
    {
        return new GotoStmt(nullptr, name);
    }

    ///
    /// toString
    ///

    // utils
    std::string __itoa(int i)
    {
        char buffer[36];
        return std::string(itoa(i, buffer, 10));
    }

    std::string GotoStmt::to_string()
    {
        return std::format("GotoStmt[{}] ({:p})", this->name, static_cast<void *>(this));
    }

    ///
    /// codegen
    ///

    llvm::Value *GotoStmt::codegen(logia::Backend *codegen, llvm::IRBuilder<> *builder)
    {
        DEBUG() << this->to_string() << std::endl;
        // find label and jump to it
        // function shall be inside the closest function
        auto label = (Block *)ast_find_closest_parent(this, (ast_types)(ast_types::FUNCTION | ast_types::BODY));
        if (!label)
        {
            throw std::runtime_error(std::string("goto statement has no parent function or body: ") + this->to_string());
        }
        // TODO generate before or wait until generated to continue ?
        return builder->CreateBr(label->llvm_basicblock);
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

    void VarDeclStmt::on_after_attach()
    {
        if (!this->is_attached)
        {
            this->is_attached = true;
            auto parentBody = (Block *)ast_find_closest_parent(this, ast_types::BODY);
            LOGIA_ASSERT(parentBody);

            parentBody->set(this->get_name(), this);
        }
    }
    Type *VarDeclStmt::get_type()
    {
        // replace NoOp ?
        // TODO REVIEW this may be too soon...
        if (((int)children[1]->type & (int)ast_types::TYPE) != 0)
        {
            // NoOp
            this->children[1] = this->get_expr()->get_type();
        }

        return this->children[1]->get_type();
    }

    bool VarDeclStmt::pre_type_inference()
    {
        // TODO determine type if possible
        // TODO what we do when we cant ? push somewhere and back later ?
        return true;
    }
}
