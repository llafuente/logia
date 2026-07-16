#include "logia/ast/types/type.h"

#include "utils.h"
#include "logia/type_inference.h"
#include "logia/backend.h"
#include "logia/ast/constexpr.h"
#include "logia/ast/callexpr.h"
#include "logia/ast/identifier.h"
#include "logia/ast/functionblock.h"
#include "logia/ast/returnstmt.h"
#include "logia/ast/scope.h"
#include "logia/ast/types/struct.h"

#include "llvm/IR/Type.h"              // Type
#include "llvm/IR/DebugInfoMetadata.h" // dwarf

namespace logia::AST
{
    Type::Type(location loc) : Node(loc)
    {
    }
    Type::~Type()
    {
        // TODO
    }

    void Type::_on_set_type(TypeDecl *type)
    {
        // REVIEW we should do something, makes sense to treat Types as "expressions" that should be the identifier right ?
        throw_compiler_error("set_type of a type ?");
    }

    void Type::__register_type(const char *name)
    {
        // TODO REVIEW function block scope ? -> or closest block scope!?
        auto scope = this->first_parent<Scope>();
        scope->scope_set(name, this);
    }

    //
    // InferType
    //
    InferType::InferType() : Type({}) {}

    InferType::~InferType() {}

    void InferType::_on_set_type(TypeDecl *tyd)
    {
        // this->replace_self(t);
    }

    std::string InferType::to_string()
    {
        return std::format("InferType");
    }

    std::string InferType::get_repr()
    {
        return std::format("infer");
    }

    void InferType::pre_codegen(logia::Backend *backend)
    {
        throw std::runtime_error("InferType cannot be codegen!");
    }

    void InferType::post_codegen(logia::Backend *backend)
    {
        throw std::runtime_error("InferType cannot be codegen!");
    }

    void InferType::on_after_attach() {}

    void InferType::validate() {}
}
