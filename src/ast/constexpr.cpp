#include "ast/constexpr.h"
#include "ast/type.h"
#include "utils.h"

namespace logia::AST
{
    //
    // ConstExpression
    //

    ConstExpression::ConstExpression(antlr4::ParserRuleContext *rule, ast_types type) : Expression(rule, (ast_types)(type | ast_types::CONST)) {}
    std::string ConstExpression::to_string()
    {
        return std::format("ConstExpression ({:p})", static_cast<void *>(this));
    }

    //
    // IntegerLiteral
    //

    IntegerLiteral::IntegerLiteral(antlr4::ParserRuleContext *rule, Type *type, int64_t value) : ConstExpression(rule, ast_types::INTEGER_LITERAL)
    {
        LOGIA_ASSERT(type);
        this->uvalue = 0;

        this->ivalue = value;
        this->push_child(type);
    }
    IntegerLiteral::IntegerLiteral(antlr4::ParserRuleContext *rule, Type *type, uint64_t value) : ConstExpression(rule, ast_types::INTEGER_LITERAL)
    {
        LOGIA_ASSERT(type);
        this->ivalue = 0;

        this->uvalue = value;
        this->push_child(type);
    }
    Type *IntegerLiteral::get_type()
    {
        return (Type *)this->children[0];
    }

    std::string IntegerLiteral::to_string()
    {
        return std::format("IntegerLiteral[{}] {}/{} ({:p})", this->get_type()->to_string(), this->ivalue, this->uvalue, static_cast<void *>(this));
    }

    llvm::Value *IntegerLiteral::codegen(logia::Backend *codegen, llvm::IRBuilder<> *builder)
    {
        DEBUG() << this->to_string() << std::endl;
        return llvm::ConstantInt::get((llvm::Type *)this->get_type()->codegen(codegen, builder), llvm::APInt(this->get_type()->Integer.bits, this->ivalue, this->get_type()->Integer.is_signed));
    }

    //
    // FloatLiteral
    //

    FloatLiteral::FloatLiteral(antlr4::ParserRuleContext *rule, Type *type, double value) : ConstExpression(rule, ast_types::FLOAT_LITERAL)
    {
        this->value = value;
        this->push_child(type);
    }

    Type *FloatLiteral::get_type()
    {
        return (Type *)this->children[0];
    }

    std::string FloatLiteral::to_string()
    {
        return std::format("FloatLiteral[{}] {} ({:p})", this->get_type()->to_string(), this->value, static_cast<void *>(this));
    }

    llvm::Value *FloatLiteral::codegen(logia::Backend *codegen, llvm::IRBuilder<> *builder)
    {
        DEBUG() << this->to_string() << std::endl;
        throw std::runtime_error(__FUNCTION__ "todo");
    }

    //
    // StringLiteral
    //

    StringLiteral::StringLiteral(antlr4::ParserRuleContext *rule, char *text) : ConstExpression(rule, ast_types::STRING_LITERAL)
    {
        this->text = text;
    }

    Type *StringLiteral::get_type()
    {
        // TODO
        return nullptr;
    }

    std::string StringLiteral::to_string()
    {
        return std::format("StringLiteral[{}] ({:p})", this->text, static_cast<void *>(this));
    }

    llvm::Value *StringLiteral::codegen(logia::Backend *codegen, llvm::IRBuilder<> *builder)
    {
        DEBUG() << this->to_string() << std::endl;
        // NOTE module is required or 0xc0000005
        // !getType()->isVoidTy() && "Cannot assign a name to void values!"??
        return builder->CreateGlobalString(this->text, ".str", 0, codegen->module.get(), true);
        /*
                llvm::Constant *strConst = llvm::ConstantDataArray::getString(codegen->context, this->text, true);

                // Create a global variable to hold the string
                llvm::GlobalVariable *gvar = new llvm::GlobalVariable(
                    codegen->module,
                    strConst->getType(),
                    true, // isConstant
                    llvm::GlobalValue::PrivateLinkage,
                    strConst,
                    ".str");

                gvar->setAlignment(llvm::Align(1));

                return gvar;
        */
    }

    LOGIA_API LOGIA_LEND StringLiteral *ast_create_string_lit(char *text)
    {
        // TODO review remove parentNode from constructor, is a leaf right?
        return new StringLiteral(nullptr, text);
    }
    LOGIA_API LOGIA_LEND FloatLiteral *ast_create_float_lit(Block *body, double value)
    {
        return new FloatLiteral(nullptr, (Type *)body->lookup(strdup("λf64")), value);
    }
    LOGIA_API LOGIA_LEND IntegerLiteral *ast_create_int_lit(Block *body, int64_t value)
    {
        return new IntegerLiteral(nullptr, (Type *)body->lookup(strdup("λi64")), value);
    }
    LOGIA_API LOGIA_LEND IntegerLiteral *ast_create_uint_lit(Block *body, uint64_t value)
    {
        return new IntegerLiteral(nullptr, (Type *)body->lookup(strdup("λu64")), value);
    }

}