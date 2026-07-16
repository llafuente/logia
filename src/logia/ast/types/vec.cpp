#include "logia/ast/types/vec.h"

#include "utils.h"
#include "logia/backend.h"

#include "llvm/IR/DerivedTypes.h"

namespace logia::AST
{

    Vec::Vec(TypeDecl *pointee, size_t size) : Ref(pointee), size(size) {}
    Vec::~Vec() {}

    std::string Vec::to_string()
    {
        return std::format("vec<{}, {}> {}", this->get_pointee()->get_repr(), this->size, Node::to_string());
    }

    std::string Vec::get_repr()
    {
        return std::format("vec<{}, {}>", this->get_pointee()->get_repr(), this->size);
    }

    void Vec::pre_codegen(logia::Backend *backend)
    {
        if (this->ir_type)
        {
            return;
        }

        auto pointee = this->get_pointee();
        pointee->pre_codegen(backend);
        LOGIA_VERIFY(pointee->ir_type != nullptr);

        // create an array type with the given size and pointee type
        this->ir_type = llvm::ArrayType::get(pointee->ir_type, this->size);
        LOGIA_VERIFY(this->ir_type != nullptr);

        auto &dl = backend->module->getDataLayout();

        auto elemSizeBits = dl.getTypeAllocSizeInBits(pointee->ir_type);
        auto elemAlignBits = dl.getABITypeAlign(pointee->ir_type).value() * 8;
        // for unbounded array it will be 0
        auto totalSizeBits = elemSizeBits * this->size;

        auto subrange = backend->dbuilder->getOrCreateSubrange(0, this->size);
        auto subscripts = backend->dbuilder->getOrCreateArray({subrange});

        if (backend->debug)
        {
            // https://llvm.org/docs/SourceLevelDebugging.html#array-types
            this->di_type = backend->dbuilder->createArrayType(
                totalSizeBits,    // SizeInBits of whole array
                elemAlignBits,    // AlignInBits
                pointee->di_type, // Element DI type
                subscripts        // DINodeArray with one subrange
            );
            LOGIA_VERIFY(this->di_type != nullptr);
        }
        // skip Ref and Pointer
        Node::pre_codegen(backend);
    }

    void Vec::on_after_attach() {}

    void Vec::validate() {}
}