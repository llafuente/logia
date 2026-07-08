#include "logia/ast/expr.h"

#include "utils.h"
#include "logia/log.h"
#include "logia/backend.h"
#include "logia/type_system.h"
#include "logia/ast/constexpr.h"
#include "logia/ast/callexpr.h"
#include "logia/ast/llvm.h"

#include "llvm/IR/Constant.h"

namespace logia::AST
{
    //
    // Expression
    //

    Expression::Expression(location loc) : Node(loc) {}
    Expression::Expression(const Expression &other) : Node(other) {}

    std::string Expression::to_string()
    {
        return std::format("Expression{}", Node::to_string());
    }

    maybe_semantic_error Expression::can_execute()
    {
        return make_semantic_error(LGERR_CONSTEX000, this);
    }
    ConstExpression *Expression::execute()
    {
        throw_compiler_error("need to be implemented!");
    }

    void Expression::post_codegen(logia::Backend *backend)
    {
        LOGIA_VERIFY(this->cg_value != nullptr, "An expression should codegen a value!");

        return Node::post_codegen(backend);
    }

    llvm::Value *Expression::get_codegen_value(logia::Backend *backend)
    {
        // do not allow to call get_codegen_value in pre_codegen pass, makes no sense!
        LOGIA_VERIFY(this->is_pre_codegen == true);

        if (!this->is_post_codegen)
        {
            this->post_codegen(backend);
        }

        LOGIA_VERIFY(this->cg_value != nullptr);
        return this->cg_value;
    }

    void Expression::set_codegen_value(logia::Backend *backend, llvm::Value *value)
    {
        LOGIA_VERIFY(this->cg_value == nullptr);
        this->cg_value = value;
        if (backend != nullptr)
        {
            backend->set_debug_loc((llvm::Instruction *)this->cg_value, this->loc);
        }
    }

}