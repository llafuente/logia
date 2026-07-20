#include "logia/ast/package.h"
#include "logia/ast/expr.h"

#include "logia/ast/block.h"
#include "logia/ast/llvm.h"
#include "logia/ast/semantic_error.h"

namespace logia::AST
{

    Package::Package(location loc, const char *entry_point_file, const char *entry_point_reldir, const char *file_contents) : File(loc, entry_point_file, entry_point_reldir, file_contents)
    {
    }

    std::string Package::to_string()
    {

        return std::format("{} {}", "Package", Scope::to_string());
    }

    void Package::pre_codegen(logia::Backend *backend)
    {
    }

    Type *Package::get_type(void)
    {
        return nullptr;
    }

    void Package::post_codegen(logia::Backend *backend)
    {
        return Node::post_codegen(backend);
    }

}