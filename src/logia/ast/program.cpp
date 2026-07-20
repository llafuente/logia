#include "logia/ast/program.h"

#include "logia/log.h"
#include "logia/type_inference.h"
#include "logia/backend.h"
#include "logia/ast/node.h"
#include "logia/ast/types/type.h"
#include "logia/ast/types/integer.h"
#include "logia/ast/types/float.h"
#include "logia/ast/types/pointer.h"
#include "logia/ast/types/void.h"
#include "logia/ast/types/ref.h"
#include "logia/ast/types/vec.h"
#include "logia/ast/expr.h"
#include "logia/ast/import.h"
#include "logia/ast/identifier.h"
#include "logia/ast/types/function.h"
#include "logia/ast/llvm.h"
#include "logia/ast/constexpr.h"

#include "utils.h"

namespace logia::AST
{
    File::File(location loc, const char *entry_point_file, const char *entry_point_reldir, const char *file_contents) : Scope(loc)
    {
        this->entry_point_file = _strdup(entry_point_file);
        this->entry_point_reldir = _strdup(entry_point_reldir);
        // do not duplicate this because ParseResult will led this "leak"
        // this->file_contents = _strdup(file_contents);
        this->file_contents = file_contents;
    }

    std::string File::to_code(size_t ident)
    {
        // accumulate all children code by newlines
        return std::accumulate(this->children.begin(), this->children.end(), std::string(""), [](std::string acc, Node *child)
                               { return acc + child->to_code() + "\n"; });
    }

    Program::Program(location loc, const char *entry_point_file, const char *entry_point_reldir, const char *file_contents) : File(loc, entry_point_file, entry_point_reldir, file_contents)
    {
        this->backend = std::make_unique<logia::Backend>(this);

        this->is_attached = true; // Program is obviously never attached to anything, it's the root -> manually set the flag
        intrinsics = new Scope(loc);
        this->push_child(intrinsics);

        std::vector<TypeDecl *> primitives = {};

        // special case i1 (not signed!)
        auto i1 = new Integer(false, 1);
        bool_type = i1;
        intrinsics->push_child(i1);
        primitives.push_back(i1);

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
        primitives.push_back(i8);
        primitives.push_back(i16);
        primitives.push_back(i32);
        primitives.push_back(i64);

        auto u8 = new Integer(false, 8);
        auto u16 = new Integer(false, 16);
        auto u32 = new Integer(false, 32);
        auto u64 = new Integer(false, 64);
        // TODO u128 ??
        intrinsics->push_child(u8);
        intrinsics->push_child(u16);
        intrinsics->push_child(u32);
        intrinsics->push_child(u64);
        primitives.push_back(u8);
        primitives.push_back(u16);
        primitives.push_back(u32);
        primitives.push_back(u64);

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
        primitives.push_back(f32);
        primitives.push_back(f64);

        // declare references to every primitive, to load intrintics!
        for (auto prim : primitives)
        {
            auto ref_prim = new Ref(prim);
            intrinsics->push_child(ref_prim);
        }

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

        this->false_value = new IntegerLiteral({}, "0", i1);
        this->true_value = new IntegerLiteral({}, "1", i1);

        LOG(DBG, "intrinsics = {}", intrinsics->to_string_tree());
        STOP_INTRINSICS();
    }

    void Program::codegen_primitives()
    {
        for (const auto it : intrinsics->children)
        {
            it->pre_codegen(this->backend.get());
            it->post_codegen(this->backend.get());
        }
        this->false_value->pre_codegen(this->backend.get());
        this->true_value->pre_codegen(this->backend.get());
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

    TypeDecl *Program::get_ast_type(llvm::Type *type)
    {
        // search in the scope, not in children
        for (const auto &it : scope)
        {
            auto vec = it.second;
            if (vec[0]->is<Type>())
            {
                auto ltype = vec[0]->as<TypeDecl>();
                // DEBUG() << ltype->to_string() << std::endl;
                if (ltype->ir_type == type)
                {
                    return ltype;
                }
            }
        }

        throw_compiler_error(std::format("llvm type not found: {}", llvm_type_to_string(type)));
    }

    void Program::on_after_attach()
    {
        // do nothing, parentBody should be empty
    }

    void Program::pre_codegen(logia::Backend *backend)
    {
        LOG(DBG, "{}", this->to_string());
        return Scope::pre_codegen(backend);
    }

    void Program::post_codegen(logia::Backend *backend)
    {
        LOG(DBG, "{}", this->to_string());

        this->intrinsics->post_codegen(backend); // forward codegen

        auto max = this->children.size();
        for (size_t i = 0; i < max; ++i)
        {
            Node *n = this->children[i];
            LOG(DBG, "codegen.program.statement[{}] = {}", i, n->to_string());

            n->post_codegen(backend);
        }

        // NOTE: overwrite - no override!
        // Block::post_codegen(backend);
        return Node::post_codegen(backend);
    }

    void Program::codegen(logia::Backend *backend)
    {
        this->pre_codegen(backend);
        this->foreach_descendant([backend](AST::Node *node, auto deep) -> bool
                                 {
                                                LOG(DBG, "pre_codegen {}", node->to_string());
                                                if (node->skip_codegen) { return true;}
                                                // guard against multiple pre_codegen, some nodes may require to pre_codegen others in a specific order
                                                if (!node->is_pre_codegen) {
                node->pre_codegen(backend);
                                                }
                if (!node->is_pre_codegen)
                {
                    throw_compiler_error(std::format("is_pre_codegen is not set: {}", node->to_string()));
                }
                return true; });
        // NOTE this allow to see the program "skeleton"
        // at this point we should not have generated any instruction, just functions and variables!
        // backend->module->dump();
        this->post_codegen(backend);
    }

    void Program::load_intrinsics()
    {
        // re-entry protection
        if (this->loaded_intrinsics == true)
        {
            return;
        }

        START_INTRINSICS();
        //  to find logia type from LLVM Type we need to codegen our types first!
        this->codegen_primitives();

        LOGIA_VERIFY(this->backend != nullptr, "call set_backend before semantic_analysis. Intrinsics should be available!");
        this->backend->load_intrinsics();
        STOP_INTRINSICS();
        this->loaded_intrinsics = true;
    }

    void Program::semantic_analysis_validate()
    {
        LOGIA_VERIFY(this->loaded_intrinsics == true, "call load_intrinsics() before!");

        // re-entry protection
        if (this->is_validated)
        {
            LOG(INF, "validated");
            return;
        }

        // NOTE this pass is in pre-order because most of the error are for users to handle them
        // and people are not compilers and their minds work top-to-bottom, left-to-right
        // while post-order is possible it will give error in reverse order and it will be a mess

        // auto all_nodes = this->program->get_post_descendant();
        auto all_nodes = this->get_pre_descendant();
        LOG(INF, "validating {} nodes", all_nodes.size());
        for (auto node : all_nodes)
        {
            LOG(SILLY, "validate {}", node->to_string());
            node->validate();
            node->is_validated = true;
        }
    }

    void Program::semantic_analysis_type_inference(size_t pass_id)
    {
        LOGIA_VERIFY(this->loaded_intrinsics == true, "call load_intrinsics() before!");
        LOGIA_VERIFY(this->is_validated == true, "call semantic_analysis_validate() before!");

        // no re-entry protection atm, for testing purposes
        LOG(INF, "start type inference: {}", pass_id);
        type_inference_program(this, pass_id);
    }

    void Program::semantic_analysis()
    {
        this->load_intrinsics();
        this->semantic_analysis_validate();
        this->semantic_analysis_type_inference();
    }

    void Program::dump()
    {
        // dump all info like crazy!
        std::cout << this->intrinsics->to_string_tree() << std::endl;
        std::cout << this->to_string_tree() << std::endl;
    }

}
