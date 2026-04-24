#include "ast/stmt.h"
#include "ast/traverse.h"

namespace logia::AST
{
    //
    // Stmt
    //
    Stmt::Stmt(antlr4::ParserRuleContext *rule) : Node(rule) {}

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
            this->cg_value = backend->builder->CreateRet(expr->codegen(backend));
        }

        return Stmt::post_codegen(backend);
    }

    //
    //
    //
    VarDeclStmt::VarDeclStmt(antlr4::ParserRuleContext *rule, Identifier *id, Type *type, Expression *expr) : Stmt(rule), alloca_inst(nullptr)
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
        this->alloca_inst = backend->builder->CreateAlloca(init_value->getType(), 0, nullptr);
        backend->builder->CreateStore(init_value, this->alloca_inst);

        this->cg_value = this->alloca_inst;
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
            block->set(this->get_name(), this);
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
        if (is_typed)
        {
            // TODO determine type if possible
            // TODO what we do when we cant ? push somewhere and back later ?
            auto t = this->get_type();
            if (t->is<Struct>())
            {
                // if rhs is struct initializer -> set_type
                auto expr = this->get_expr();
                if (expr != nullptr && expr->is<StructInitializer>())
                {
                    expr->as<StructInitializer>()->set_type(t);
                }
            }
        }
        return true;
    }

    void VarDeclStmt::post_type_inference()
    {
        if (!this->is_typed)
        {
            auto expr = this->get_expr();
            if (expr == nullptr)
            {
                throw std::runtime_error("type guessing not implemented yet!");
            }

            // override type with initializer
            this->children[1] = this->get_expr()->get_type();
        }
    }
}
