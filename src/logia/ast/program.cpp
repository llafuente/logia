#include "logia/ast/program.h"

#include "logia/log.h"
#include "logia/backend.h"
#include "logia/ast/node.h"
#include "logia/ast/type.h"
#include "logia/ast/expr.h"
#include "logia/ast/import.h"
#include "logia/ast/identifier.h"
#include "logia/ast/function.h"
#include "logia/ast/llvm.h"

namespace logia::AST
{
    Program::Program(location loc, const char *entry_point_file, const char *file_contents) : Scope(loc), entry_point_file(entry_point_file), file_contents(file_contents)
    {
        this->is_attached = true; // Program is obviously never attached to anything, it's the root -> manually set the flag
        intrinsics = new Scope(loc);
        this->push_child(intrinsics);

        // special case i1 (not signed!)
        auto i1 = new Integer(false, 1);
        intrinsics->push_child(i1);

        // we know declare all primitives
        // any type in the language should use those
        // it's prohibited to create type using llvm
        // everything shall be supported directly
        auto i8 = new Integer(true, 8);
        auto i16 = new Integer(true, 16);
        auto i32 = new Integer(true, 32);
        auto i64 = new Integer(true, 64);
        // TODO i128 ??
        intrinsics->push_child(i8);
        intrinsics->push_child(i16);
        intrinsics->push_child(i32);
        intrinsics->push_child(i64);

        auto u8 = new Integer(false, 8);
        auto u16 = new Integer(false, 16);
        auto u32 = new Integer(false, 32);
        auto u64 = new Integer(false, 64);
        // TODO u128 ??
        intrinsics->push_child(u8);
        intrinsics->push_child(u16);
        intrinsics->push_child(u32);
        intrinsics->push_child(u64);

        auto the_void = new Void(); // starcraft reference is mandatory :)
        intrinsics->push_child(the_void);
        auto ptr = new Pointer();
        intrinsics->push_child(ptr);

        // TODO f16?
        auto f32 = new Float(32);
        auto f64 = new Float(64);
        // TODO f128?
        intrinsics->push_child(f32);
        intrinsics->push_child(f64);

        // declare references to every primitive, to load intrintics!
        auto ref_i8 = new Ref(i8);
        auto ref_i16 = new Ref(i16);
        auto ref_i32 = new Ref(i32);
        auto ref_i64 = new Ref(i64);
        auto ref_u8 = new Ref(u8);
        auto ref_u16 = new Ref(u16);
        auto ref_u32 = new Ref(u32);
        auto ref_u64 = new Ref(u64);
        auto ref_f32 = new Ref(f32);
        auto ref_f64 = new Ref(f64);
        intrinsics->push_child(ref_i8);
        intrinsics->push_child(ref_i16);
        intrinsics->push_child(ref_i32);
        intrinsics->push_child(ref_i64);
        intrinsics->push_child(ref_u8);
        intrinsics->push_child(ref_u16);
        intrinsics->push_child(ref_u32);
        intrinsics->push_child(ref_u64);
        intrinsics->push_child(ref_f32);
        intrinsics->push_child(ref_f64);

        // aliases
        intrinsics->scope[(char *)"int"] = intrinsics->scope[(char *)"λi64"];
        intrinsics->scope[(char *)"float"] = intrinsics->scope[(char *)"λf64"];
        intrinsics->scope[(char *)"bool"] = intrinsics->scope[(char *)"λi1"];
        intrinsics->scope[(char *)"void"] = intrinsics->scope[(char *)"λvoid"];
        intrinsics->scope[(char *)"ptr"] = {ptr};
        START_INTRINSICS();
        LOG(DBG, "intrinsics before core load = {}", intrinsics->to_string());

        auto imp = new Import({});
        imp->set_import_into_scope();
        imp->set_package({new Identifier({}, "core"), new Identifier({}, "primitives")});
        imp->set_scope_target(this);

        intrinsics->push_child(imp);
        intrinsics->scope_copy_all(this);
        this->children.pop_back(); // safe to remove now

        LOG(DBG, "intrinsics = {}", intrinsics->to_string_tree());
        STOP_INTRINSICS();
    }

    void Program::codegen_primitives(logia::Backend *backend)
    {
        for (const auto it : intrinsics->children)
        {
            it->codegen(backend);
        }
    }

    Type *Program::get_type()
    {
        return nullptr;
    }

    std::string Program::to_string()
    {
        return std::format("Program.{}", Scope::to_string());
    }

    void Program::add_intrinsic(Intrinsic *fn)
    {
        intrinsics->push_child(fn);
        // REVIEW should not be necessary
        scope_set(fn->get_name(), fn);
    }

    Type *Program::get_ast_type(llvm::Type *type)
    {
        // search in the scope, not in children
        for (const auto &it : scope)
        {
            auto vec = it.second;
            if (vec[0]->is<Type>())
            {
                auto ltype = vec[0]->as<Type>();
                // DEBUG() << ltype->to_string() << std::endl;
                if (ltype->ir_type == type)
                {
                    return ltype;
                }
            }
        }

        throw_compiler_error(std::format("llvm type not found: {}", llvm_type_to_string(type)));
    }

    void Program::post_attach()
    {
        // do nothing, parentBody should be empty
    }

    void Program::pre_codegen(logia::Backend *backend)
    {
        LOG(DBG, "{}", this->to_string());
        return Scope::pre_codegen(backend);
    }

    llvm::Value *Program::post_codegen(logia::Backend *backend)
    {
        LOG(DBG, "{}", this->to_string());

        this->intrinsics->codegen(backend); // forward codegen

        auto max = this->children.size();
        for (size_t i = 0; i < max; ++i)
        {
            Node *n = this->children[i];
            LOG(DBG, "codegen.program.statement[{}] = {}", i, n->to_string());

            n->codegen(backend);
        }

        this->cg_value = nullptr; // nobody need program return type!

        // NOTE: overwrite - no override!
        // Block::post_codegen(backend);
        return Node::post_codegen(backend);
    }
}
