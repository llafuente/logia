#include "logia/ast/types/float.h"

#include "logia/ast/types/typedecl.h"
#include "logia/backend.h"
#include "utils.h"

#include "llvm/IR/Type.h"

namespace logia::AST
{
    Float::Float(int bits) : TypeDecl({}, Primitives::FLOATING_POINT_TY), bits(bits)
    {
    }

    Float::~Float() {}

    std::string Float::to_string()
    {
        return std::format("float[{}]{}", this->get_repr(), Node::to_string());
    }

    std::string Float::to_code(size_t ident)
    {
        return this->get_repr();
    }

    std::string Float::get_repr()
    {
        return std::format("λf{}", this->bits);
    }

    void Float::pre_codegen(logia::Backend *backend)
    {
        switch (this->bits)
        {
        case 16:
            this->ir_type = llvm::Type::getHalfTy(backend->context);
            if (backend->debug)
            {
                this->di_type = backend->dbuilder->createBasicType("f16", this->bits, llvm::dwarf::DW_ATE_float);
            }
            break;
        case 32:
            this->ir_type = llvm::Type::getFloatTy(backend->context);
            if (backend->debug)
            {
                this->di_type = backend->dbuilder->createBasicType("f32", this->bits, llvm::dwarf::DW_ATE_float);
            }
            break;
        case 64:
            this->ir_type = llvm::Type::getDoubleTy(backend->context);
            if (backend->debug)
            {
                this->di_type = backend->dbuilder->createBasicType("f64", this->bits, llvm::dwarf::DW_ATE_float);
            }
            break;
        case 128:
            this->ir_type = llvm::Type::getFP128Ty(backend->context);
            if (backend->debug)
            {
                this->di_type = backend->dbuilder->createBasicType("f128", this->bits, llvm::dwarf::DW_ATE_float);
            }
            break;
        default:
            throw std::runtime_error("Not supported number of bits");
        }

        LOGIA_VERIFY(this->ir_type != nullptr);
        if (backend->debug)
        {
            LOGIA_VERIFY(this->di_type != nullptr);
        }

        Node::pre_codegen(backend);
    }

    void Float::on_after_attach()
    {
        this->__register_type(std::format("{}", this->get_repr()).c_str());
    }

    void Float::validate() {}
}