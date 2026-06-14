#include "logia/ast/type.h"

#include "utils.h"
#include "logia/backend.h"
#include "logia/ast/constexpr.h"
#include "logia/ast/callexpr.h"
#include "logia/ast/identifier.h"
#include "logia/ast/functionblock.h"
#include "logia/ast/returnstmt.h"
#include "logia/ast/scope.h"

#include "llvm/IR/Type.h"              // Type
#include "llvm/IR/DebugInfoMetadata.h" // dwarf

namespace logia::AST
{
    LOGIA_API LOGIA_LEND char *ast_primitives_to_string(Primitives prim)
    {
        switch (prim)
        {
        case Primitives::VOID_TY:
            return _strdup("void");
        case Primitives::INTEGER_TY:
            return _strdup("int");
        case Primitives::FLOATING_POINT_TY:
            return _strdup("float");
        case Primitives::PTR_TY:
            return _strdup("ptr");
        case Primitives::STRUCT_TY:
            return _strdup("struct");
        case Primitives::FUNCTION_TY:
            return _strdup("function");
        default:
            throw std::exception("unreachable");
        }
    }

    //
    // Type
    //

    Type::Type(location loc, Primitives prim) : Node(loc)
    {
        this->skip_type_inference = true; // we are a type ourselves!
        this->primitive = prim;
    }
    Type::~Type()
    {
        // TODO
    }

    std::string Type::to_string()
    {
        return std::format("Type[{}]{}", ast_primitives_to_string(this->primitive), Node::to_string());
    }

    std::string Type::get_repr()
    {
        return "not-def";
    }

    Type *Type::get_type()
    {
        return this;
    }

    Type *Type::get_pointer_to()
    {
        // return scope_lookup_all(this, "ptr").unwrap_success()[0]->as<Type>();
        return new Ref(this); // <-- TODO this should be a factory
    }

    void Type::_set_type(Type *type)
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

    llvm::Value *Type::post_codegen(logia::Backend *backend)
    {
        LOG(DBG, "{}", this->to_string());
        // cache, because type are unique and we will be visiting this a lot
        if (this->ir_type)
        {
            this->cg_value = (llvm::Value *)this->ir_type;
            return Node::post_codegen(backend);
        }

        // TODO
        throw std::exception(TOSTRING(__FUNCTION__) "to-do");
    }

    //
    // Integer
    //

    Integer::Integer(bool is_signed, int bits) : Type({}, Primitives::INTEGER_TY), is_signed(is_signed), bits(bits)
    {
        this->is_typed = true;
    }
    Integer::~Integer() {}

    std::string Integer::to_string()
    {
        return std::format("Type[{}]{}", this->get_repr(), Node::to_string());
    }
    std::string Integer::get_repr()
    {
        return std::format("{}{}", (this->is_signed || this->bits == 1 ? "i" : "u"), this->bits);
    }

    void Integer::pre_codegen(logia::Backend *backend)
    {
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

        LOGIA_VERIFY(this->ir_type != nullptr);
        if (backend->debug)
        {
            LOGIA_VERIFY(this->di_type != nullptr);
        }

        this->cg_value = (llvm::Value *)this->ir_type;
        Node::pre_codegen(backend);
    }

    void Integer::post_attach()
    {
        this->__register_type(std::format("λ{}", this->get_repr()).c_str());
    }

    void Integer::validate() {}

    //
    // Float
    //

    Float::Float(int bits) : Type({}, Primitives::FLOATING_POINT_TY), bits(bits)
    {
        this->is_typed = true;
    }
    Float::~Float() {}

    std::string Float::to_string()
    {
        return std::format("Type[{}]{}", this->get_repr(), Node::to_string());
    }
    std::string Float::get_repr()
    {
        return std::format("f{}", this->bits);
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

        this->cg_value = (llvm::Value *)this->ir_type;
        Node::pre_codegen(backend);
    }

    void Float::post_attach()
    {
        this->__register_type(std::format("λ{}", this->get_repr()).c_str());
    }

    void Float::validate() {}

    //
    // Void
    //

    Void::Void() : Type({}, Primitives::VOID_TY)
    {
        this->is_typed = true;
    }
    Void::~Void() {}

    std::string Void::to_string()
    {
        return std::format("void{}", Node::to_string());
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

        this->cg_value = (llvm::Value *)this->ir_type;
        Node::pre_codegen(backend);
    }

    void Void::post_attach()
    {
        this->__register_type(std::format("λ{}", this->get_repr()).c_str());
    }

    void Void::validate() {}

    //
    // Pointer
    //

    Pointer::Pointer() : Type({}, Primitives::PTR_TY)
    {
        this->is_typed = true;
    }
    Pointer::~Pointer() {}

    std::string Pointer::to_string()
    {
        return std::format("ptr{}", Node::to_string());
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

        this->cg_value = (llvm::Value *)this->ir_type;
        Node::pre_codegen(backend);
    }

    void Pointer::post_attach()
    {
        this->__register_type(std::format("λ{}", this->get_repr()).c_str());
    }

    void Pointer::validate() {}

    //
    // Ref
    //

    Ref::Ref(Type *pointee) : Pointer()
    {
        this->is_typed = true;
        this->pointee = pointee;
        this->push_child(pointee);
    }

    Ref::~Ref() {}

    std::string Ref::to_string()
    {
        return std::format("ref<{}> {}", this->get_pointee()->get_repr(), Node::to_string());
    }

    std::string Ref::get_repr()
    {
        return std::format("ref<{}>", this->get_pointee()->get_repr());
    }

    Type *Ref::get_pointee()
    {
        return this->pointee;
    }

    void Ref::pre_codegen(logia::Backend *backend)
    {
        this->ir_type = llvm::PointerType::get(backend->context, 0);
        LOGIA_VERIFY(this->ir_type != nullptr);

        if (backend->debug)
        {
            this->di_type = backend->dbuilder->createPointerType(
                this->get_pointee()->di_type, // DIType *PointeeTy, Pointee type
                64,                           // uint64_t SizeInBits, Pointer size in bits
                0,                            // uint32_t AlignInBits, Alignment in bit
                std::nullopt,                 // std::optional<unsigned> DWARFAddressSpace
                "void*"                       // StringRef Name, Optional name
            );
            LOGIA_VERIFY(this->di_type != nullptr);
        }

        this->cg_value = (llvm::Value *)this->ir_type;
        Node::pre_codegen(backend);
    }

    void Ref::post_attach()
    {
        // Type::post_attach(); <-- skip pointer/Type as we don't want to re-register!
        scope_set(this, this->get_repr().c_str(), this, true);
    }

    void Ref::validate() {}

    //
    // TypeDef
    //
    // REVIEW, it's a type but it's definition, need to  distinguish both ?
    TypeDef::TypeDef() : Type({}, Primitives::NONE)
    {
        this->is_typed = false;
    }
    TypeDef::~TypeDef() {}

    void TypeDef::add_locator(Identifier *name)
    {
        name->skip_codegen = true;
        this->push_child(name);
    }

    Type *TypeDef::get_type()
    {
        if (this->type == nullptr)
        {
            // TODO support more than one!?
            LOGIA_VERIFY(this->children.size() == 1, "TO-DO: single resolve atm");
            // search children!
            auto id = this->get_child<Identifier>(0);
            auto result = scope_lookup_all(this, id->identifier);

            if (result.is_error())
            {
                throw_semantic_error(this, result.message);
            }
            auto list = result.unwrap_success();
            if (list.size() == 0)
            {
                throw_semantic_error(this, std::format(LGERR_ID001, id->identifier));
            }
            if (list.size() > 1)
            {
                std::string debug_candidates = "";
                int i = 1;
                for (const auto &node : list)
                {
                    debug_candidates += std::format("Candidate {} declared {}\n", i++, node->loc.get_debug_location(1, 1));
                    ++i;
                }
                throw_semantic_error(this, std::format(LGERR_ID002, list.size(), id->identifier, debug_candidates));
            }

            this->type = list[0]->get_type();
            this->is_typed = true;
        }
        return this->type;
    }
    std::string TypeDef::to_string()
    {
        if (this->children.size())
        {
            auto id = (Identifier *)this->children[0];
            return std::format("TypeDef[{}]{}", id->identifier, Node::to_string());
        }
        return std::format("TypeDef[?]{}", Node::to_string());
    }

    std::string TypeDef::get_repr()
    {
        if (is_attached)
        {
            return this->get_final_type()->get_repr();
        }
        return "not-attached-yet";
    }

    llvm::Value *TypeDef::post_codegen(logia::Backend *backend)
    {
        this->cg_value = this->get_type()->codegen(backend);
        return Node::post_codegen(backend);
    }

    Node *TypeDef::resolve()
    {
        LOGIA_VERIFY(this->children.size() == 1, "TO-DO: single resolve atm");
        return this->children[0]->resolve();
    }

    void TypeDef::post_attach() {}
    void TypeDef::validate() {}

    //
    // InferType
    //
    InferType::InferType() : Type({}, Primitives::NONE) {}

    InferType::~InferType() {}

    void InferType::_set_type(Type *t)
    {
        this->replace_self(t);
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

    llvm::Value *InferType::post_codegen(logia::Backend *backend)
    {
        throw std::runtime_error("InferType cannot be codegen!");
    }

    void InferType::post_attach() {}

    void InferType::validate() {}

}
