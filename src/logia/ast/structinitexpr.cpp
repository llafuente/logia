#include "logia/ast/structinitexpr.h"

#include "logia/backend.h"
#include "logia/ast/expr.h"
#include "logia/ast/identifier.h"
#include "logia/ast/constexpr.h"
#include "logia/ast/struct.h"

#include "llvm/IR/Constant.h"

namespace logia::AST
{
    //
    // StructInitializer
    //

    StructInitializer::StructInitializer(location loc) : Expression(loc) {}
    std::string StructInitializer::to_string()
    {
        return std::format("{}{}", "StructInitializer", Expression::to_string());
    }

    void StructInitializer::_set_type(Type *type)
    {
        Struct *struct_ty;
        if (!type->try_cast<Struct>(&struct_ty))
        {
            throw_semantic_error(this, std::format("LGER030 incompatible type '{}', expected a struct", type->get_repr()));
        }

        this->type = type;

        // TODO defaults!
        // search struct by name, if not found use the next position that should match the type, continue until defaults
        auto named_values = this->children;
        bool *used = new bool[struct_ty->field_count];

        // reset
        this->values = 0;
        this->children.clear(); // we will reorder children to match struct fields order, so we need to clear them first
        this->children.reserve(struct_ty->field_count * 2);

        for (auto i = 0; i < struct_ty->field_count; ++i)
        {
            used[i] = false;
            this->children.push_back(nullptr);
            this->children.push_back(nullptr);
        }

#if _DEBUG
        {
            std::string d = "";
            for (auto i = 0; i < struct_ty->field_count; ++i)
            {
                d += used[i] ? "X" : "0";
            }
            LOG(DBG, "fields = {}", d);
        }
#endif

        // map first all named values
        for (auto j = 0; j < named_values.size(); j += 2)
        {
            auto name = named_values[j];
            auto value_node = named_values[j + 1]->as<Expression>();
            if (name->is<NoOp>())
            {
                continue; // positional, will be handled later
            }
            auto value_name = name->as<Identifier>();

            auto field = struct_ty->get_field(value_name);
            used[field->index] = true;

            this->set_named_property(field->get_name(), value_node, field->index);
            // remove from named_values as it's used
            named_values.erase(named_values.begin() + j, named_values.begin() + j + 2);
            j -= 2;
        }
#if _DEBUG
        {
            std::string d = "";
            for (auto i = 0; i < struct_ty->field_count; ++i)
            {
                d += used[i] ? "X" : "0";
            }
            LOG(DBG, "fields = {}", d);
        }
#endif

        // second, map by position!
        for (auto j = 0; j < named_values.size(); j += 2)
        {
            auto value_node = named_values[j + 1]->as<Expression>();
            auto found = false;
            // search first field "not-used" and use it
            for (auto i = 0; i < struct_ty->field_count; ++i)
            {
                if (!used[i])
                {
                    used[i] = true;

                    auto field = struct_ty->get_field_by_index(i);
                    auto field_name = field->get_name();
                    this->set_named_property(field_name, value_node, field->index);
                    named_values.erase(named_values.begin() + j, named_values.begin() + j + 2);
                    j -= 2;
                    found = true;
                    break;
                }
            }
            if (!found)
            {
                throw_semantic_error(value_node, std::format("Too many initializers. Expected {}", struct_ty->field_count));
            }
        }
        assert(named_values.size() == 0);

        // third fill defaults!
        for (uint32_t i = 0; i < struct_ty->field_count; ++i)
        {
            if (!used[i])
            {
                auto field = struct_ty->get_field_by_index(i);
                auto field_name = field->get_name();
                auto field_ty = field->get_type();
                auto field_default_value = field->get_default_value();
                if (field_default_value == nullptr)
                {
                    throw_semantic_error(this, std::format("LGER031 Missing initializer for field '{}' at position '{}' of type '{}'", field_name->identifier, i + 1, struct_ty->get_repr()));
                }
                this->set_named_property(field_name, field_default_value, field->index);
            }
        }

        StructInitializer *si = nullptr;
        int constant_count = 0;
        for (auto i = 0; i < struct_ty->field_count; ++i)
        {
            auto field_ty = struct_ty->get_field_by_index(i)->get_final_type();
            auto value = this->get_value_by_index(i);
            value->set_type(field_ty); // TODO check type compatibility ?

            if (value->try_cast<StructInitializer>(&si))
            {
                if (si->is_constant)
                {
                    ++constant_count;
                }
            }
            else if (value->is<ConstExpression>())
            {
                ++constant_count;
            }
        }
        this->is_constant = constant_count == struct_ty->field_count;
        delete[] used;
    }

    void StructInitializer::add_named_property(Identifier *name, Expression *value)
    {
        this->push_child(name);
        name->skip_codegen = true;
        name->skip_type_inference = true;
        this->push_child(value);

        ++this->values;
    }

    void StructInitializer::set_named_property(Identifier *name, Expression *value, uint32_t index)
    {
        this->children[index * 2] = name;
        name->skip_codegen = true;
        name->skip_type_inference = true;
        this->children[index * 2 + 1] = value;
    }

    void StructInitializer::add_positional_property(Expression *value)
    {
        this->push_child(new NoOp());
        this->push_child(value);

        ++this->values;
    }

    Expression *StructInitializer::get_value_by_index(uint32_t index)
    {
        return this->get_child<Expression>((index * 2) + 1);
    }

    Expression *StructInitializer::get_value_by_name(const char *name)
    {
        throw_compiler_error("TODO!");
    }

    llvm::Value *StructInitializer::post_codegen(logia::Backend *backend)
    {
        if (!this->is_constant)
        {
            throw_semantic_error(this, "non-constant initialization not supported atm.");
        }

        auto &ctx = backend->context;
        const llvm::DataLayout &dl = backend->module->getDataLayout();

        auto struct_ty = this->get_type()->as<Struct>();

        auto v = std::vector<llvm::Constant *>();
        v.reserve(this->values);

        // skip first, it's the type

        for (auto field_index = 0, i = 0; i < this->children.size(); i += 2, ++field_index)
        {
            auto field_ty = struct_ty->get_field_by_index(field_index)->get_final_type();
            field_ty->codegen(backend);

            auto item = this->get_child<Expression>(i + 1);
            auto item_ty = item->get_final_type();
            item_ty->codegen(backend);

            auto ir_item_value = item->codegen(backend);
            auto cir_item_value = (llvm::Constant *)(ir_item_value);

            if (field_ty->ir_type != item_ty->ir_type)
            {
                throw_semantic_error(item, std::format("Expected type {} found type {}", field_ty->get_repr(), item_ty->get_repr()));
            }

            v.push_back(cir_item_value);
        }

        auto ir_struct_ty = (llvm::StructType *)struct_ty->codegen(backend);

        // 1) Constant initializer (replace with your child constants)
        llvm::Constant *init = llvm::ConstantStruct::get(ir_struct_ty, v);

        // 2a) If parent is a StructInitializer, means is a nested initialization
        // do not create the global variable
        auto p = this->parent_node;
        if (p->is<StructInitializer>())
        {
            this->cg_value = init;
            return Node::post_codegen(backend);
        }
        // if (p->is<Stmt>() || p->is<Block>())

        // 2b) Materialize constant in read-only global memory (memcpy source must be an address)
        auto *srcGlobal = new llvm::GlobalVariable(
            *backend->module,
            ir_struct_ty,
            true, // isConstant
            llvm::GlobalValue::PrivateLinkage,
            init,
            ".struct.init");

        auto abiAlign = llvm::Align(dl.getABITypeAlign(ir_struct_ty).value());
        // srcGlobal->setAlignment(llvm::Align(8));
        srcGlobal->setAlignment(abiAlign);

        this->cg_value = srcGlobal;
        // skip to Node -> LLVM crashes
        return Node::post_codegen(backend);
    }

    void StructInitializer::post_attach() {}

    void StructInitializer::validate() {}

    Type *StructInitializer::get_type()
    {
        return this->type; // TODO InferType
    }
} // namespace logia::AST
