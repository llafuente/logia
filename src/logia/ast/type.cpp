#include "logia/ast/type.h"

#include "utils.h"
#include "logia/type_inference.h"
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
    // TypeDecl
    //

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

    //
    // Integer
    //

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

    //
    // Float
    //

    Float::Float(int bits) : TypeDecl({}, Primitives::FLOATING_POINT_TY), bits(bits)
    {
        // TODO REVIEW type-system do not use: set_type atm
        this->real_type = this;
        this->is_typed = true;
    }
    Float::~Float() {}

    std::string Float::to_string()
    {
        return std::format("float[{}]{}", this->get_repr(), Node::to_string());
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

    //
    // Void
    //

    Void::Void() : TypeDecl({}, Primitives::VOID_TY)
    {
        // TODO REVIEW type-system do not use: set_type atm
        this->real_type = this;
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

        Node::pre_codegen(backend);
    }

    void Void::on_after_attach()
    {
        this->__register_type(std::format("λ{}", this->get_repr()).c_str());
    }

    void Void::validate() {}

    //
    // Pointer
    //

    Pointer::Pointer() : TypeDecl({}, Primitives::PTR_TY)
    {
        // TODO REVIEW type-system do not use: set_type atm
        this->real_type = this;
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

        Node::pre_codegen(backend);
    }

    void Pointer::on_after_attach()
    {
        this->__register_type(std::format("λ{}", this->get_repr()).c_str());
    }

    void Pointer::validate() {}

    //
    // Ref
    //

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
        return std::format("ref<{}> {}", this->get_pointee()->get_repr(), Node::to_string());
    }

    std::string Ref::get_repr()
    {
        return std::format("ref<{}>", this->get_pointee()->get_repr());
    }

    TypeDecl *Ref::get_pointee()
    {
        return this->pointee;
    }

    void Ref::pre_codegen(logia::Backend *backend)
    {
        if (this->ir_type)
        {
            return;
        }

        auto pointee = this->get_pointee();
        pointee->pre_codegen(backend);

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

    //
    // TypeDef
    //
    // REVIEW, it's a type but it's definition, need to  distinguish both ?
    TypeDef::TypeDef() : Type({})
    {
        this->has_type = true;
        this->is_typed = false;
    }
    TypeDef::~TypeDef() {}

    void TypeDef::add_locator(Identifier *name)
    {
        name->skip_codegen = true;
        name->resolve = true;
        name->resolve_unique = false;
        this->push_child(name);
    }

    Type *TypeDef::get_type()
    {
        // this could happens while parsing, not a real issue but avoid some null-access
        if (!this->is_attached)
        {
            return nullptr;
        }

        return this->real_type;
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
        std::string t;
        Identifier *ident;
        for (auto node : this->children)
        {
            if (node->try_cast(&ident))
            {
                t += (t.size() ? "." : "");
                t += ident->identifier;
            }
        }

        return t;
    }

    void TypeDef::post_codegen(logia::Backend *backend)
    {
        this->get_type()->post_codegen(backend);
        return Node::post_codegen(backend);
    }

    void TypeDef::on_after_attach()
    {
        // TODO nasty HACK this is a PIA because we need typedef so early for intrinsics...
        try
        {
            if (this->type_inference_pass_id == 0)
            {
                this->type_inference(TYPE_INFERENCE_EARLY);
            }
        }
        catch (std::exception e)
        {
        }
    }

    bool TypeDef::type_inference(size_t pass_id)
    {
        switch (pass_id)
        {
        case TYPE_INFERENCE_EARLY:
        {
            LOG(DBG, "");
            // TODO support more than one!?
            LOGIA_VERIFY(this->children.size() == 1, "TO-DO: single resolve atm");
            // search children!
            auto id = this->get_child<Identifier>(0);
            // TODO this could be removed ?
            id->type_inference(TYPE_INFERENCE_PRE);
            id->type_inference(TYPE_INFERENCE_POST);

            auto list = id->decl_candidates;
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

            // TODO REVIEW type-system do not use: set_type atm
            this->real_type = list[0]->get_type_decl();
            this->is_typed = true;
        }
        break;
        }
        return true;
    }

    void TypeDef::validate() {}

    //
    // InferType
    //
    InferType::InferType() : Type({}) {}

    InferType::~InferType() {}

    void InferType::_on_set_type(TypeDecl *t)
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
