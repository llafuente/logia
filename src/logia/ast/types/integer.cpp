#include "logia/ast/types/integer.h"

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
    Integer::Integer(bool is_signed, int bits) : TypeDecl({}, Primitives::INTEGER_TY), is_signed(is_signed), bits(bits)
    {
        // TODO REVIEW type-system do not use: set_type atm
        this->real_type = this;
        this->is_typed = true;
    }
    Integer::~Integer() {}

    std::string Integer::to_string()
    {
        return std::format("integer[{}]{}", this->get_repr(), Node::to_string());
    }
    std::string Integer::get_repr()
    {
        return std::format("λ{}{}", (this->is_signed || this->bits == 1 ? "i" : "u"), this->bits);
    }

    void Integer::pre_codegen(logia::Backend *backend)
    {
        LOG(SILLY, "");
        switch (this->bits)
        {
        case 1:
            this->ir_type = llvm::Type::getInt1Ty(backend->context);
            if (backend->debug)
            {
                this->di_type = backend->dbuilder->createBasicType("i1", this->bits, llvm::dwarf::DW_ATE_boolean);
            }
            break;
        case 8:
            this->ir_type = llvm::Type::getInt8Ty(backend->context);
            if (backend->debug)
            {
                this->di_type = backend->dbuilder->createBasicType("i8", this->bits, this->is_signed ? llvm::dwarf::DW_ATE_signed : llvm::dwarf::DW_ATE_unsigned);
            }
            break;
        case 16:
            this->ir_type = llvm::Type::getInt16Ty(backend->context);
            if (backend->debug)
            {
                this->di_type = backend->dbuilder->createBasicType("i16", this->bits, this->is_signed ? llvm::dwarf::DW_ATE_signed : llvm::dwarf::DW_ATE_unsigned);
            }
            break;
        case 32:
            this->ir_type = llvm::Type::getInt32Ty(backend->context);
            if (backend->debug)
            {
                this->di_type = backend->dbuilder->createBasicType("i32", this->bits, this->is_signed ? llvm::dwarf::DW_ATE_signed : llvm::dwarf::DW_ATE_unsigned);
            }
            break;
        case 64:
            this->ir_type = llvm::Type::getInt64Ty(backend->context);
            if (backend->debug)
            {
                this->di_type = backend->dbuilder->createBasicType("i64", this->bits, this->is_signed ? llvm::dwarf::DW_ATE_signed : llvm::dwarf::DW_ATE_unsigned);
            }
            break;
        case 128:
            this->ir_type = llvm::Type::getInt128Ty(backend->context);
            if (backend->debug)
            {
                this->di_type = backend->dbuilder->createBasicType("i128", this->bits, this->is_signed ? llvm::dwarf::DW_ATE_signed : llvm::dwarf::DW_ATE_unsigned);
            }
            break;
        default:
            throw std::runtime_error("Not supported number of bits");
        }

        TypeDecl::pre_codegen(backend);
    }

    void Integer::on_after_attach()
    {
        this->__register_type(std::format("{}", this->get_repr()).c_str());
    }

    void Integer::validate() {}
}