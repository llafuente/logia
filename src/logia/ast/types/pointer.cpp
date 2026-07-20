#include "logia/ast/types/pointer.h"

#include "logia/backend.h"

#include "llvm/IR/DerivedTypes.h"
#include "utils.h"

namespace logia::AST
{
    Pointer::Pointer() : TypeDecl({}, Primitives::PTR_TY)
    {
    }

    Pointer::~Pointer() {}

    std::string Pointer::to_string()
    {
        return std::format("ptr{}", Node::to_string());
    }

    std::string Pointer::to_code(size_t ident)
    {
        return this->get_repr();
    }

    std::string Pointer::get_repr()
    {
        return std::format("{}", "ptr");
    }

    void Pointer::pre_codegen(logia::Backend *backend)
    {
        this->ir_type = llvm::PointerType::get(backend->context, 0);
        LOGIA_VERIFY(this->ir_type != nullptr);

        if (backend->debug)
        {
            auto avoid = this->di_type = backend->dbuilder->createUnspecifiedType("void");
            this->di_type = backend->dbuilder->createPointerType(
                avoid,        // DIType *PointeeTy, Pointee type
                64,           // uint64_t SizeInBits, Pointer size in bits
                0,            // uint32_t AlignInBits, Alignment in bit
                std::nullopt, // std::optional<unsigned> DWARFAddressSpace
                "void*"       // StringRef Name, Optional name
            );
            LOGIA_VERIFY(this->di_type != nullptr);
        }

        Node::pre_codegen(backend);
    }

    void Pointer::on_after_attach()
    {
        this->__register_type(std::format("λ{}", this->get_repr()).c_str());
    }

    void Pointer::validate() {}
}