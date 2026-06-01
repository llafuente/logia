#include "logia/compiler_error.h"
#include "logia/ast/constexpr.h"
#include "logia/ast/type.h"

#include "logia/log.h"
#include "utils.h"

#include "llvm/ADT/APInt.h"
#include "llvm/ADT/APSInt.h"

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
        LOGIA_ASSERT(number_as_text == nullptr);
        // TODO number literals with dashes need to be cleaned right ?

        // alloc one more space for sign and negate without any allocation
        auto length = strlen(number_as_text);
        this->value_str = (char *)malloc(length + 2); // 1 for sign, 1 for null
        strncpy(this->value_str, number_as_text, length);
        this->value_str[length] = '\0'; // Ensure null termination

        if (type != nullptr)
        {
            this->set_type(type);
        }

        // parse
        // NOTE: here value_str should be well-formed, or llvm will abort the process :S
        auto canBeNonDec = strlen(this->value_str) > 2;
        if (canBeNonDec && this->value_str[0] == '0' && this->value_str[1] == 'b')
        {
            // binary
            LOG(DBG, "{} as binary", value_str);
            this->value = llvm::APSInt(llvm::APInt(64, value_str + 2, 2), true);
        }
        else if (canBeNonDec && this->value_str[0] == '0' && this->value_str[1] == 'o')
        {
            // octal
            LOG(DBG, "{} as octal", value_str);
            this->value = llvm::APSInt(llvm::APInt(64, value_str + 2, 8), true);
        }
        else if (canBeNonDec && this->value_str[0] == '0' && this->value_str[1] == 'x')
        {
            // hexadecimal
            LOG(DBG, "{} as hexadecimal", value_str);
            this->value = llvm::APSInt(llvm::APInt(64, value_str + 2, 16), true);
            // this->value = llvm::APInt(64, "1F", 16);
        }
        else
        {
            LOG(DBG, "{} as decimal", value_str);
            this->value = llvm::APSInt(llvm::APInt(64, value_str, 10), true);
        }
    }

    Type *IntegerLiteral::get_type()
    {
        return this->type;
    }

    void IntegerLiteral::negate()
    {
        if (this->value_str[0] == '-')
        {
            memmove(this->value_str, this->value_str + 1, strlen(this->value_str) + 1);
        }
        else
        {
            memmove(this->value_str + 1, this->value_str, strlen(this->value_str) + 1);
            this->value_str[0] = '-';
        }
    }

    std::string IntegerLiteral::to_string()
    {
        // TODO review format
        return std::format("IntegerLiteral [{}]{}", this->value_str, Node::to_string());
    }

    void IntegerLiteral::_set_type(Type *type)
    {
        this->type = type;
    }

    llvm::Value *IntegerLiteral::post_codegen(logia::Backend *backend)
    {
        LOG(DBG, "{}", this->to_string());
        auto type = this->get_final_type();
        auto llvm_type = (llvm::Type *)this->get_final_type()->codegen(backend);

        if (type->is<Integer>())
        {
            auto itype = type->as<Integer>();

            LOG(DBG, "signed? {}", itype->is_signed);

            // check first as unsigned, limit should not be reached
            auto required_bits = value.getActiveBits();
            LOG(DBG, "{} activebits = {} expected = {}", this->value_str, required_bits, itype->bits);

            if (required_bits > itype->bits)
            {
                throw_semantic_error(this, std::format("LGERR_CEXPR002a Integer literal '{}' is too big for type '{}' required at least {} bits", this->value_str, type->get_repr(), required_bits));
            }

            // now apply signdness, negate, and truncate in this ORDER!
            value.setIsSigned(itype->is_signed);
            if (this->value_str[0] == '-')
            {
                // negate!
                value.negate();
                // value = -value; // signed negation
            }
            value = itype->is_signed ? value.truncSSat(itype->bits) : value.trunc(itype->bits);

            required_bits = value.getActiveBits();
            LOG(DBG, "{} activebits = {} expected = {}", this->value_str, required_bits, itype->bits);

            if (required_bits > itype->bits)
            {
                throw_semantic_error(this, std::format("LGERR_CEXPR002b Integer literal '{}' is too big for type '{}' required at least {} bits", this->value_str, type->get_repr(), required_bits));
            }

            this->cg_value = llvm::ConstantInt::get(llvm_type, value);
        }
        else if (type->is<Float>())
        {
            auto ftype = type->as<Float>();
            this->cg_value = llvm::ConstantFP::get(
                llvm_type,
                this->value_str
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
        LOG(DBG, "{}", this->to_string());
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
        LOG(DBG, "{}", this->to_string());
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
