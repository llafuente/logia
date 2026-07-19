#include "logia/ast/types/ref.h"

#include "logia/backend.h"
#include "logia/ast/scope.h"
#include "utils.h"

#include "llvm/IR/DerivedTypes.h"

namespace logia::AST
{
    Ref::Ref() : Pointer() {}

    Ref::Ref(TypeDecl *pointee) : Pointer()
    {
        if (!pointee->is<TypeDecl>())
        {
            throw_compiler_error("wtf! not a typedecl!");
        }
        this->pointee = pointee;
    }

    Ref::~Ref() {}

    std::string Ref::to_string()
    {
        return std::format("{} {}", this->get_repr(), Node::to_string());
    }

    std::string Ref::get_repr()
    {
        if (this->pointee == nullptr)
        {
            return std::format("ref<null>");
        }
        return std::format("ref<{}>", this->get_pointee()->get_repr());
    }

    TypeDecl *Ref::get_pointee()
    {
        return this->pointee;
    }

    void Ref::pre_codegen(logia::Backend *backend)
    {
        if (this->pointee == nullptr)
        {
            throw_compiler_error("At codegen ref is expected to have pointee defined! Ref should not be used as opaque pointer.");
        }

        if (this->ir_type)
        {
            return;
        }

        auto pointee = this->get_pointee();
        pointee->pre_codegen(backend);
        LOGIA_VERIFY(pointee->ir_type != nullptr);

        this->ir_type = this->ir_ptype = llvm::PointerType::get(backend->context, 0);
        LOGIA_VERIFY(this->ir_type != nullptr);

        if (backend->debug)
        {
            this->di_type = backend->dbuilder->createPointerType(
                pointee->di_type,                       // DIType *PointeeTy, Pointee type
                backend->getPointerSizeInBits(),        // uint64_t SizeInBits, Pointer size in bits
                0,                                      // uint32_t AlignInBits, Alignment in bit
                std::nullopt,                           // std::optional<unsigned> DWARFAddressSpace
                std::format("{}*", pointee->get_repr()) // StringRef Name, Optional name
            );
            LOGIA_VERIFY(this->di_type != nullptr);
        }

        Node::pre_codegen(backend);
    }

    void Ref::on_after_attach()
    {
        // Type::on_after_attach(); <-- skip pointer/Type as we don't want to re-register!
        scope_set(this, this->get_repr().c_str(), this, true);
    }

    void Ref::validate() {}
}