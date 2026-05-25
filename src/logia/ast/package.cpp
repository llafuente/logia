#include "logia/ast/package.h"
#include "logia/ast/expr.h"

#include "logia/ast/block.h"
#include "logia/ast/llvm.h"
#include "logia/ast/semantic_error.h"

namespace logia::AST
{

    Package::Package(antlr4::ParserRuleContext *rule, const char *entry_point_file, const char *file_contents) : Scope(rule), entry_point_file(entry_point_file), file_contents(file_contents) {}
    std::string Package::to_string()
    {
        return std::format("{}{}", "Package", Scope::to_string());
    }

    void Package::pre_codegen(logia::Backend *backend)
    {
    }

    Type *Package::get_type(void)
    {
        return nullptr;
    }

    llvm::Value *Package::post_codegen(logia::Backend *backend)
    {
        this->cg_value = nullptr;
        return Node::post_codegen(backend);
    }

}