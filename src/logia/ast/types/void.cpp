#include "logia/ast/types/void.h"

#include "logia/backend.h"
#include "utils.h"

#include "llvm/IR/Type.h"

namespace logia::AST
{
    Void::Void() : TypeDecl({}, Primitives::VOID_TY)
    {
    }

    Void::~Void() {}

    std::string Void::to_string()
    {
        return std::format("void{}", Node::to_string());
    }

    std::string Void::to_code(size_t ident)
    {
        return this->get_repr();
    }

    std::string Void::get_repr()
    {
        return std::format("{}", "void");
    }

    void Void::pre_codegen(logia::Backend *backend)
    {
        this->ir_type = llvm::Type::getVoidTy(backend->context);
        LOGIA_VERIFY(this->ir_type != nullptr);

        if (backend->debug)
        {
            this->di_type = backend->dbuilder->createUnspecifiedType("void");
            LOGIA_VERIFY(this->di_type != nullptr);
        }

        Node::pre_codegen(backend);
    }

    void Void::on_after_attach()
    {
        this->__register_type(std::format("λ{}", this->get_repr()).c_str());
    }

    void Void::validate() {}
}