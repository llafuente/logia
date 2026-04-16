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
    VarDeclStmt::VarDeclStmt(antlr4::ParserRuleContext *rule, Identifier *id, Type *type, Expression *expr) : Stmt(rule, ast_types::VAR_DECL_STMT), alloca(nullptr)
    {
        this->push_child(id);
        if (type == nullptr)
        {
            this->push_child(new NoOp());
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
        return std::format("VarDeclStmt[{}] ({:p})", this->get_name(), static_cast<void *>(this));
    }

    llvm::Value *VarDeclStmt::codegen(logia::Backend *codegen, llvm::IRBuilder<> *builder)
    {
        if (this->alloca != nullptr)
        {
            return this->alloca;
        }

        DEBUG() << this->to_string() << std::endl;
        auto init_value = (llvm::Value *)this->get_expr()->codegen(codegen, builder);
        auto type = this->get_type();
        /*
        if (type->is<Struct>())
        {
            auto structTy = (llvm::StructType *)this->get_type()->codegen(codegen, builder);
            // struct path
            // 3) Destination stack allocation
            llvm::Value *dstAlloca = builder->CreateAlloca(structTy, nullptr, "myStruct");

            const llvm::DataLayout &dl = codegen->module->getDataLayout();
            auto *i8PtrTy = builder->getIntPtrTy(dl);

            llvm::Value *dstI8 = builder->CreateBitCast(dstAlloca, i8PtrTy);
            llvm::Value *srcI8 = builder->CreateBitCast(init_value, i8PtrTy);

            // 4) memcpy
            auto abiAlign = llvm::Align(dl.getABITypeAlign(structTy).value());
            uint64_t size = dl.getTypeAllocSize(structTy);
            builder->CreateMemCpy(
                dstI8, llvm::MaybeAlign(abiAlign),
                srcI8, llvm::MaybeAlign(abiAlign),
                size);

            return dstAlloca; // pointer to initialized struct
        }
        */

        // other path

        // TODO Type should be handled before ?
        // this->ir = builder->CreateAlloca((llvm::Type*) this->type->codegen(codegen, builder), 0, value);
        // this->ir = builder->CreateAlloca(value->getType(), 0, value);
        this->alloca = builder->CreateAlloca(init_value->getType(), 0, nullptr);
        builder->CreateStore(init_value, this->alloca);

        return this->alloca;
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

    void VarDeclStmt::post_attach()
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
        if (this->is_child<NoOp>(1))
        {
            // NoOp
            throw std::runtime_error("????");
        }

        return ast_resolve_type(this->get_child<Type>(1));
    }

    bool VarDeclStmt::pre_type_inference()
    {
        if (is_typed) {
            // TODO determine type if possible
            // TODO what we do when we cant ? push somewhere and back later ?
            auto t = this->get_type();
            if (t->is<Struct>()) {
                // if rhs is struct initializer -> set_type
                auto expr = this->get_expr();
                if (expr != nullptr && expr->is<StructInitializer>()) {
                    expr->as<StructInitializer>()->set_type(t);
                }
            }
        }
        return true;
    }

    void VarDeclStmt::post_type_inference() {
        if (!this->is_typed) {
            auto expr = this->get_expr();
            if (expr == nullptr) {
                throw std::runtime_error("type guessing not implemented yet!");
            }

            // override type with initializer
            this->children[1] = this->get_expr()->get_type();
        }
    }
}
