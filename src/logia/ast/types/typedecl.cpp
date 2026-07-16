#include "logia/ast/types/typedecl.h"

#include "logia/ast/types/ref.h"
#include "logia/backend.h"
#include "utils.h"

namespace logia::AST
{
    TypeDecl::TypeDecl(location loc, Primitives prim) : Type(loc)
    {
        this->real_type = this;
        this->primitive = prim;
    }

    TypeDecl::~TypeDecl() {}

    TypeDecl *TypeDecl::get_effective_type_decl()
    {
        return this;
    }

    TypeDecl *TypeDecl::get_reference_to()
    {
        // return scope_lookup_all(this, "ptr").unwrap_success()[0]->as<Type>();
        return new Ref(this); // <-- TODO this should be a factory
    }

    std::string TypeDecl::to_string()
    {
        return std::format("TypeDecl[{}]{}", ast_primitives_to_string(this->primitive), Node::to_string());
    }

    void TypeDecl::pre_codegen(logia::Backend *backend)
    {
        LOGIA_VERIFY(this->ir_type != nullptr);
        LOGIA_VERIFY(backend->debug ? this->di_type != nullptr : true);
        Type::pre_codegen(backend);
    }
}