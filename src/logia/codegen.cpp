#include "logia/codegen.h"
#include "logia/ast/node.h"
#include "logia/ast/type.h"

namespace logia
{
    using namespace logia::AST;

    void codegen(Program *program, Backend *backend)
    {

        auto all_nodes = program->get_post_descendant();
        for (auto node : all_nodes)
        {
            node->pre_codegen(backend);
#if _DEBUG
            if (!node->is_pre_codegen)
            {
                throw_compiler_error("is_pre_codegen shoud be set");
            }
#endif
        }

        for (auto node : all_nodes)
        {
            node->post_codegen(backend);
#if _DEBUG
            if (!node->is_post_codegen)
            {
                throw_compiler_error("is_post_codegen shoud be set");
            }
#endif
        }

        // Struct *st;
        // Function *func;
        // if (expr->try_cast<StructInitializer>(&sinit))
    }
}