#include "logia/compiler_error.h"
#include "logia/ast/constexpr.h"
#include "logia/ast/type.h"

#include "logia/log.h"
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

    bool IntegerLiteral::is_valid_constant_operator(Operators op)
    {
        switch (op)
        {
        case Operators::BINARY_ADD:
        case Operators::BINARY_SUB:
        case Operators::BINARY_MUL:
        case Operators::BINARY_DIV:
        case Operators::BINARY_MOD:
        case Operators::BINARY_LOGIAL_EQ:
        case Operators::BINARY_LOGIAL_NEQ:
        case Operators::BINARY_LOGIAL_LT:
        case Operators::BINARY_LOGIAL_GT:
        case Operators::BINARY_LOGIAL_LTE:
        case Operators::BINARY_LOGIAL_GTE:
        case Operators::BINARY_BITWISE_AND:
        case Operators::BINARY_BITWISE_OR:
        case Operators::BINARY_BITWISE_XOR:
        case Operators::BINARY_BITWISE_LEFT_SHIFT:
        case Operators::BINARY_BITWISE_RIGHT_SHIFT:
            return true;
        default:
            return false;
        }
    }

    ConstExpression *IntegerLiteral::operator+(ConstExpression *other)
    {
        if (!other->is<IntegerLiteral>())
        {
            throw_semantic_error(other, LGERR_CONSTEX002);
        }
        auto rhs = other->as<IntegerLiteral>();
        // Build a copy-like result (same rule/type, own buffers)
        auto result = new IntegerLiteral(this->rule, "0", this->type);

        // Align bit width/signedness before add
        llvm::APSInt lhsVal = this->value;
        llvm::APSInt rhsVal = rhs->value;

        const unsigned maxBits = std::max(lhsVal.getBitWidth(), rhsVal.getBitWidth());
        lhsVal = lhsVal.extOrTrunc(maxBits);
        rhsVal = rhsVal.extOrTrunc(maxBits);

        // Keep lhs signedness policy
        rhsVal.setIsSigned(lhsVal.isSigned());

        llvm::APInt sumRaw = lhsVal + rhsVal;
        result->value = llvm::APSInt(sumRaw, lhsVal.isUnsigned());

        // Keep textual form in sync (decimal)
        std::string text = result->value.isSigned()
                               ? std::to_string(result->value.getSExtValue())
                               : std::to_string(result->value.getZExtValue());

        if (result->value_str != nullptr)
        {
            free(result->value_str);
            result->value_str = nullptr;
        }

        result->value_str = (char *)malloc(text.size() + 1);
        std::memcpy(result->value_str, text.c_str(), text.size() + 1);

        return result;
    }

    llvm::Value *IntegerLiteral::post_codegen(logia::Backend *backend)
    {
        LOG(DBG, "{}", this->to_string());
        auto type = this->get_final_type();
        type->codegen(backend);
        auto llvm_type = type->ir_type;

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
            LOG(DBG, "{} to float {} / {}!", this->value_str, type->get_repr(), llvm_type_to_string(llvm_type));
            switch (ftype->bits)
            {
            case 16:
                throw_compiler_error("not supportted atm");
            /*
                this->cg_value = llvm::ConstantFP::get(
                    llvm_type,
                    // this->value_str
                    llvm::APFloat(static_cast<_Float16>(this->value.getSExtValue())));
                    */
            case 32:
                this->cg_value = llvm::ConstantFP::get(
                    llvm_type,
                    // this->value_str
                    llvm::APFloat(static_cast<float>(this->value.getSExtValue())));
                break;
            case 64:
                this->cg_value = llvm::ConstantFP::get(
                    llvm_type,
                    // this->value_str
                    llvm::APFloat(static_cast<double>(this->value.getSExtValue())));
                break;
            default:
                throw_compiler_error("Invalid number of bits for float type");
            }
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

    FloatLiteral::FloatLiteral(antlr4::ParserRuleContext *rule, const char *number_as_text, Type *type) : ConstExpression(rule)
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
        LOG(DBG, "{} as decimal", this->value_str);
        auto ref = llvm::StringRef(this->value_str);

        // Parse with rounding to nearest, ties to even
        llvm::Expected<llvm::APFloat::opStatus> result =
            value.convertFromString(this->value_str, llvm::APFloat::rmNearestTiesToEven);
        if (!result)
        {
            // std::unique_ptr<ErrorInfoBase> Payload = result.takeError().takePayload();
            //  Extract and print the error
            llvm::handleAllErrors(result.takeError(), [this](const llvm::ErrorInfoBase &EIB)
                                  { throw_semantic_error(this, std::format("LGERR_CEXPR004 Invalid floating point format '{}'.\nParse error: {}", this->value_str, EIB.message())); });
        }
    }

    Type *FloatLiteral::get_type()
    {
        return this->type;
    }

    std::string FloatLiteral::to_string()
    {
        return std::format("FloatLiteral[{}]{}", this->value_str, Node::to_string());
    }

    llvm::Value *FloatLiteral::post_codegen(logia::Backend *backend)
    {
        LOG(DBG, "{} to float!", this->value_str);
        auto type = this->get_final_type();

        this->cg_value = llvm::ConstantFP::get(type->ir_type, this->value);
        return ConstExpression::post_codegen(backend);
    }

    void FloatLiteral::_set_type(Type *type)
    {
        this->type = type;
    }

    //
    // StringLiteral
    //

    StringLiteral::StringLiteral(antlr4::ParserRuleContext *rule, const char *text) : ConstExpression(rule)
    {
        // assert not null, or llvm will crash without any message :S
        LOGIA_ASSERT(text != nullptr);

        this->text = strdup(text);
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

    bool StringLiteral::is_valid_constant_operator(Operators op)
    {
        switch (op)
        {
        case Operators::BINARY_ADD:
            return true;
        }
        return false;
    }

    ConstExpression *StringLiteral::operator+(ConstExpression *other)
    {
        if (!other->is<StringLiteral>())
        {
            throw_semantic_error(other, LGERR_CONSTEX002);
        }
        auto rhs = other->as<StringLiteral>();
        const size_t lhs_len = strlen(this->text);
        const size_t rhs_len = strlen(rhs->text);

        char *combined = (char *)malloc(lhs_len + rhs_len + 1);

        if (lhs_len > 0)
        {
            std::memcpy(combined, this->text, lhs_len);
        }
        if (rhs_len > 0)
        {
            std::memcpy(combined + lhs_len, rhs->text, rhs_len);
        }
        combined[lhs_len + rhs_len] = '\0';

        auto result = new StringLiteral(this->rule, combined);
        result->type = this->type;
        return result;
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
}
