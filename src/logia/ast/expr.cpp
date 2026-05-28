#include "logia/ast/expr.h"
#include "logia/ast/constexpr.h"
#include "logia/ast/callexpr.h"
#include "logia/ast/llvm.h"

#include "logia/log.h"

#include "llvm/IR/Constant.h"

#include "logia/type_system.h"

namespace logia::AST
{
    //
    // Expression
    //

    Expression::Expression(antlr4::ParserRuleContext *rule) : Node(rule) {}
    std::string Expression::to_string()
    {
        return std::format("Expression{}", Node::to_string());
    }

    llvm::Value *Expression::post_codegen(logia::Backend *backend)
    {
        if (this->cg_value != nullptr)
        {
            LOG(DBG, "{}", this->to_string());
            backend->set_debug_loc((llvm::Instruction *)this->cg_value, this->rule);
        }
        return Node::post_codegen(backend);
    }

    //
    // utils
    //
    char ast_binary_operator_to_string_buffer[256];
    const char *ast_postfix_unary_operator_to_string(Operators op, Type *operand)
    {
        switch (op)
        {
        case Operators::PREFIX_INCREMENT:
            sprintf(ast_binary_operator_to_string_buffer, "logia_intrinsics_postfix_inc_%s", operand->get_repr().c_str());
            return ast_binary_operator_to_string_buffer;
        case Operators::POSTFIX_DECREMENT:
            sprintf(ast_binary_operator_to_string_buffer, "logia_intrinsics_postfix_dec_%s", operand->get_repr().c_str());
            return ast_binary_operator_to_string_buffer;
        default:
            throw std::runtime_error("Unknown postfix unary operator");
        }
    }
    const char *ast_prefix_unary_operator_to_string(Operators op, Type *operand)
    {
        switch (op)
        {
        case Operators::PREFIX_DEREFERENCE:
            sprintf(ast_binary_operator_to_string_buffer, "logia_intrinsics_deref_%s", operand->get_repr().c_str());
            return ast_binary_operator_to_string_buffer;
        case Operators::PREFIX_INCREMENT:
            sprintf(ast_binary_operator_to_string_buffer, "logia_intrinsics_prefix_inc_%s", operand->get_repr().c_str());
            return ast_binary_operator_to_string_buffer;
        case Operators::PREFIX_DECREMENT:
            sprintf(ast_binary_operator_to_string_buffer, "logia_intrinsics_prefix_dec_%s", operand->get_repr().c_str());
            return ast_binary_operator_to_string_buffer;
        case Operators::PREFIX_NEGATION:
            sprintf(ast_binary_operator_to_string_buffer, "logia_intrinsics_prefix_neg_%s", operand->get_repr().c_str());
            return ast_binary_operator_to_string_buffer;
        case Operators::PREFIX_BITWISE_NOT:
            sprintf(ast_binary_operator_to_string_buffer, "logia_intrinsics_prefix_bitwise_not_%s", operand->get_repr().c_str());
            return ast_binary_operator_to_string_buffer;
        case Operators::PREFIX_LOGICAL_NOT:
            sprintf(ast_binary_operator_to_string_buffer, "logia_intrinsics_prefix_logical_not_%s", operand->get_repr().c_str());
            return ast_binary_operator_to_string_buffer;
        default:
            throw std::runtime_error("Unknown prefix unary operator");
        }
    }

    const char *ast_binary_operator_to_string(Operators op, Type *left, Type *right)
    {
        switch (op)
        {
        case Operators::BINARY_ASSIGN:
            sprintf(ast_binary_operator_to_string_buffer, "logia_intrinsics_bin_assign_%s_%s", left->get_repr().c_str(), right->get_repr().c_str());
            return ast_binary_operator_to_string_buffer;
        case Operators::BINARY_ADD_ASSIGN:
            sprintf(ast_binary_operator_to_string_buffer, "logia_intrinsics_bin_add_assign_%s_%s", left->get_repr().c_str(), right->get_repr().c_str());
            return ast_binary_operator_to_string_buffer;
        case Operators::BINARY_SUB_ASSIGN:
            sprintf(ast_binary_operator_to_string_buffer, "logia_intrinsics_bin_sub_assign_%s_%s", left->get_repr().c_str(), right->get_repr().c_str());
            return ast_binary_operator_to_string_buffer;
        case Operators::BINARY_DIV_ASSIGN:
            sprintf(ast_binary_operator_to_string_buffer, "logia_intrinsics_bin_div_assign_%s_%s", left->get_repr().c_str(), right->get_repr().c_str());
            return ast_binary_operator_to_string_buffer;
        case Operators::BINARY_MUL_ASSIGN:
            sprintf(ast_binary_operator_to_string_buffer, "logia_intrinsics_bin_mul_assign_%s_%s", left->get_repr().c_str(), right->get_repr().c_str());
            return ast_binary_operator_to_string_buffer;
        case Operators::BINARY_ADD:
            sprintf(ast_binary_operator_to_string_buffer, "logia_intrinsics_bin_add_%s_%s", left->get_repr().c_str(), right->get_repr().c_str());
            return ast_binary_operator_to_string_buffer;
        case Operators::BINARY_SUB:
            sprintf(ast_binary_operator_to_string_buffer, "logia_intrinsics_bin_sub_%s_%s", left->get_repr().c_str(), right->get_repr().c_str());
            return ast_binary_operator_to_string_buffer;
        case Operators::BINARY_MUL:
            sprintf(ast_binary_operator_to_string_buffer, "logia_intrinsics_bin_mul_%s_%s", left->get_repr().c_str(), right->get_repr().c_str());
            return ast_binary_operator_to_string_buffer;
        case Operators::BINARY_DIV:
            sprintf(ast_binary_operator_to_string_buffer, "logia_intrinsics_bin_div_%s_%s", left->get_repr().c_str(), right->get_repr().c_str());
            return ast_binary_operator_to_string_buffer;
        case Operators::BINARY_MOD:
            sprintf(ast_binary_operator_to_string_buffer, "logia_intrinsics_bin_mod_%s_%s", left->get_repr().c_str(), right->get_repr().c_str());
            return ast_binary_operator_to_string_buffer;

        case Operators::BINARY_LOGIAL_EQ:
            sprintf(ast_binary_operator_to_string_buffer, "logia_intrinsics_bin_eq_%s_%s", left->get_repr().c_str(), right->get_repr().c_str());
            return ast_binary_operator_to_string_buffer;
        case Operators::BINARY_LOGIAL_NEQ:
            sprintf(ast_binary_operator_to_string_buffer, "logia_intrinsics_bin_neq_%s_%s", left->get_repr().c_str(), right->get_repr().c_str());
            return ast_binary_operator_to_string_buffer;
        case Operators::BINARY_LOGIAL_LT:
            sprintf(ast_binary_operator_to_string_buffer, "logia_intrinsics_bin_lt_%s_%s", left->get_repr().c_str(), right->get_repr().c_str());
            return ast_binary_operator_to_string_buffer;
        case Operators::BINARY_LOGIAL_GT:
            sprintf(ast_binary_operator_to_string_buffer, "logia_intrinsics_bin_gt_%s_%s", left->get_repr().c_str(), right->get_repr().c_str());
            return ast_binary_operator_to_string_buffer;
        case Operators::BINARY_LOGIAL_LTE:
            sprintf(ast_binary_operator_to_string_buffer, "logia_intrinsics_bin_lte_%s_%s", left->get_repr().c_str(), right->get_repr().c_str());
            return ast_binary_operator_to_string_buffer;
        case Operators::BINARY_LOGIAL_GTE:
            sprintf(ast_binary_operator_to_string_buffer, "logia_intrinsics_bin_gte_%s_%s", left->get_repr().c_str(), right->get_repr().c_str());
            return ast_binary_operator_to_string_buffer;
        case Operators::BINARY_LOGICAL_AND:
            sprintf(ast_binary_operator_to_string_buffer, "logia_intrinsics_bin_logical_and_%s_%s", left->get_repr().c_str(), right->get_repr().c_str());
            return ast_binary_operator_to_string_buffer;
        case Operators::BINARY_LOGICAL_OR:
            sprintf(ast_binary_operator_to_string_buffer, "logia_intrinsics_bin_logical_or_%s_%s", left->get_repr().c_str(), right->get_repr().c_str());
            return ast_binary_operator_to_string_buffer;

        case Operators::BINARY_BITWISE_OR:
            sprintf(ast_binary_operator_to_string_buffer, "logia_intrinsics_bin_bitwise_or_%s_%s", left->get_repr().c_str(), right->get_repr().c_str());
            return ast_binary_operator_to_string_buffer;
        case Operators::BINARY_BITWISE_AND:
            sprintf(ast_binary_operator_to_string_buffer, "logia_intrinsics_bin_bitwise_and_%s_%s", left->get_repr().c_str(), right->get_repr().c_str());
            return ast_binary_operator_to_string_buffer;
        case Operators::BINARY_BITWISE_XOR:
            sprintf(ast_binary_operator_to_string_buffer, "logia_intrinsics_bin_bitwise_xor_%s_%s", left->get_repr().c_str(), right->get_repr().c_str());
            return ast_binary_operator_to_string_buffer;
        case Operators::BINARY_BITWISE_LEFT_SHIFT:
            sprintf(ast_binary_operator_to_string_buffer, "logia_intrinsics_bin_bitwise_shl_%s_%s", left->get_repr().c_str(), right->get_repr().c_str());
            return ast_binary_operator_to_string_buffer;
        case Operators::BINARY_BITWISE_RIGHT_SHIFT:
            sprintf(ast_binary_operator_to_string_buffer, "logia_intrinsics_bin_bitwise_shr_%s_%s", left->get_repr().c_str(), right->get_repr().c_str());
            return ast_binary_operator_to_string_buffer;
        default:
            throw std::runtime_error("Unknown binary operator");
        }
    }
}