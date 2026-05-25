#include "logia/compiler_error.h"
#include "logia/ast/constexpr.h"
#include "logia/ast/type.h"
#include "utils.h"

namespace logia::AST
{
    //
    // ConstExpression
    //

    ConstExpression::ConstExpression(antlr4::ParserRuleContext *rule) : Expression(rule)
    {
        this->is_constant = true;
    }
    std::string ConstExpression::to_string()
    {
        return std::format("ConstExpression{}", Node::to_string());
    }

    llvm::Value *ConstExpression::post_codegen(logia::Backend *backend)
    {
        // skip expression, as we couldn't generate debug information, it crash!
        return Node::post_codegen(backend);
    }

    //
    // IntegerLiteral
    //

    IntegerLiteral::IntegerLiteral(antlr4::ParserRuleContext *rule, const char *number_as_text, Type *type) : ConstExpression(rule)
    {
        LOGIA_ASSERT(number_as_text);
        // TODO number literals with dashes need to be cleaned right ?
        // TODO 0x???
        // TODO 0b???
        this->number_str = strdup(number_as_text);
        if (type != nullptr)
        {
            this->set_type(type);
        }
    }

    Type *IntegerLiteral::get_type()
    {
        return this->type;
    }

    // ?? https://learn.microsoft.com/en-us/cpp/c-runtime-library/reference/atoi64-atoi64-l-wtoi64-wtoi64-l?view=msvc-170

    uint64_t IntegerLiteral::as_unsigned()
    {
        char *end = nullptr;
        uint64_t result = strtoull(this->number_str, &end, 10);

        // Check for conversion errors
        if (errno == ERANGE)
        {
            throw std::runtime_error("Error: number out of 64-bit range.");
            return 1;
        }
        if (end == this->number_str)
        {
            throw std::runtime_error("Error: no digits found.");
        }
        return result;
    }

    int64_t IntegerLiteral::as_signed()
    {
        char *end = nullptr;
        int64_t result = strtoll(this->number_str, &end, 10);

        // Check for conversion errors
        if (errno == ERANGE)
        {
            throw std::runtime_error("Error: number out of 64-bit range.");
            return 1;
        }
        if (end == this->number_str)
        {
            throw std::runtime_error("Error: no digits found.");
        }
        return result;
    }

    std::string IntegerLiteral::to_string()
    {
        // TODO review format
        return std::format("IntegerLiteral {}/{}{}", this->number_str, this->as_signed(), Node::to_string());
    }

    void IntegerLiteral::_set_type(Type *type)
    {
        this->type = type;
    }

    llvm::Value *IntegerLiteral::post_codegen(logia::Backend *backend)
    {
        DEBUG() << this->to_string() << std::endl;
        auto type = this->get_final_type();
        auto llvm_type = (llvm::Type *)this->get_final_type()->codegen(backend);

        if (type->is<Integer>())
        {
            auto itype = type->as<Integer>();
            // TODO add support for octal
            // TODO add support for hexadecimal
            this->cg_value = llvm::ConstantInt::get(llvm_type, llvm::APInt(itype->bits, this->number_str, 10));
        }
        else if (type->is<Float>())
        {
            auto ftype = type->as<Float>();
            this->cg_value = llvm::ConstantFP::get(
                llvm_type,
                this->number_str
                // llvm::APFloat(ftype->bits, this->number_str) // APFloat from float
            );
        }
        else
        {
            throw_compiler_error(std::format("Unexpected type {}, could not generate an integer/floating point literal from it", type->get_repr()));
        }

        return ConstExpression::post_codegen(backend);
    }

    //
    // FloatLiteral
    //

    FloatLiteral::FloatLiteral(antlr4::ParserRuleContext *rule, double value, Type *type) : ConstExpression(rule)
    {
        this->value = value;
        if (type != nullptr)
        {
            this->set_type(type);
        }
    }

    Type *FloatLiteral::get_type()
    {
        return this->type;
    }

    std::string FloatLiteral::to_string()
    {
        return std::format("FloatLiteral[{}] {}{}", this->get_type()->to_string(), this->value, Node::to_string());
    }

    llvm::Value *FloatLiteral::post_codegen(logia::Backend *backend)
    {
        DEBUG() << this->to_string() << std::endl;
        this->cg_value = nullptr;
        throw_compiler_error("TODO!");
        return ConstExpression::post_codegen(backend);
    }

    void FloatLiteral::_set_type(Type *type)
    {
        this->type = type;
    }

    //
    // StringLiteral
    //

    StringLiteral::StringLiteral(antlr4::ParserRuleContext *rule, char *text) : ConstExpression(rule)
    {
        this->text = text;
    }

    Type *StringLiteral::get_type()
    {
        return this->type;
    }

    std::string StringLiteral::to_string()
    {
        return std::format("StringLiteral[{}]{}", this->text, Node::to_string());
    }

    void StringLiteral::_set_type(Type *t)
    {
        this->type = t;
    }

    void StringLiteral::_pre_type_inference()
    {
        // this->set_type(this->first_parent<Scope>()->lookup<Type>("cstring"));
        this->set_type(this->first_parent<Scope>()->lookup<Type>("ptr"));

        ConstExpression::_pre_type_inference();
    }

    llvm::Value *StringLiteral::post_codegen(logia::Backend *backend)
    {
        DEBUG() << this->to_string() << std::endl;
        // NOTE module is required or 0xc0000005
        // !getType()->isVoidTy() && "Cannot assign a name to void values!"??
        this->cg_value = backend->builder->CreateGlobalString(this->text, ".str", 0, backend->module.get(), true);
        return ConstExpression::post_codegen(backend);

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

    LOGIA_API LOGIA_LEND StringLiteral *ast_create_string_lit(LOGIA_CLONE const char *text)
    {
        return new StringLiteral(nullptr, strdup(text));
    }
    LOGIA_API LOGIA_LEND FloatLiteral *ast_create_float_lit(Block *body, double value)
    {
        return new FloatLiteral(nullptr, value, body->lookup<Type>("λf64"));
    }
    LOGIA_API LOGIA_LEND IntegerLiteral *ast_create_int_lit(Block *body, const char *numberstr)
    {
        return new IntegerLiteral(nullptr, numberstr, body->lookup<Type>("λi64"));
    }
    LOGIA_API LOGIA_LEND IntegerLiteral *ast_create_uint_lit(Block *body, const char *numberstr)
    {
        return new IntegerLiteral(nullptr, numberstr, body->lookup<Type>("λu64"));
    }

}