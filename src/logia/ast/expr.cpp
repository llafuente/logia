#include "logia/ast/expr.h"

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

    llvm::Value *Expression::post_codegen(logia::Backend *backend)
    {
        if (this->cg_value != nullptr)
        {
            LOG(DBG, "{}", this->to_string());
            backend->set_debug_loc((llvm::Instruction *)this->cg_value, this->loc);
        }
        return Node::post_codegen(backend);
    }
}