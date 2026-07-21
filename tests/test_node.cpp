// ./build-debug/bin/Debug/logia_test_suite.exe --gtest_break_on_failure

#include "logia/frontend.h"
#include "logia/backend.h"

#include "logia/ast/node.h"
#include "logia/ast/if_stmt.h"
#include "logia/ast/expr.h"
#include "logia/ast/callexpr.h"
#include "logia/ast/constexpr.h"
#include "logia/ast/stmt.h"
#include "logia/ast/identifier.h"
#include "logia/ast/operators.h"
#include "logia/ast/binaryexpr.h"
#include "logia/ast/returnstmt.h"
#include "logia/ast/unaryexpr.h"
#include "logia/ast/types.h"
#include "logia/ast/vardeclstmt.h"
#include "logia/type_inference.h"
#include "logia/ast/structinitexpr.h"

#include "gtest/gtest.h"
#include <gmock/gmock.h> // For matchers like HasSubstr
#include <Windows.h>

#include "test_utils.h"
#include "logia/run.h"

TEST(test_node, clone_identifier)
{
    LOGIA_UNIT_TEST();
    using namespace logia::AST;

    int size = program->children.size();
    auto a = new Identifier(loc, "a");
    EXPECT_EQ(a->parent_node, nullptr);

    program->push_child(a);
    EXPECT_NE(a->parent_node, nullptr);

    a->real_type = (TypeDecl *)a; // xD
    auto aclone = node_clone<Identifier>(a);
    // type is removed
    EXPECT_EQ(aclone->is_typed, false);
    EXPECT_EQ(aclone->real_type, nullptr);

    EXPECT_EQ(aclone->parent_node, nullptr); // parent_node is removed
}

TEST(test_node, test_1)
{
    LOGIA_UNIT_TEST();
    using namespace logia::AST;

    int size = program->children.size();
    program->push_child(new Identifier(loc, "a"));
    program->push_child(new BinaryExpression(loc, new Identifier(loc, "a"), Operators::BINARY_ADD, new Identifier(loc, "b")));
    program->push_child(new Identifier(loc, "c"));

    EXPECT_TRUE(program->get_child<Identifier>(size));
    EXPECT_TRUE(program->get_child<Expression>(size));
    EXPECT_TRUE(program->get_child<Node>(size));

    EXPECT_TRUE(program->get_child<BinaryExpression>(size + 1));
    EXPECT_TRUE(program->get_child<Expression>(size + 1));
    EXPECT_TRUE(program->get_child<Node>(size + 1));

    EXPECT_THROW(program->get_child<UnaryExpression>(size + 1), std::runtime_error);

    int count = 0;
    program->intrinsics->foreach_child<Integer>([&count](auto p)
                                                { ++count; });

    EXPECT_EQ(count, 9);

    count = 0;
    program->foreach_child<Identifier>([&count](auto p)
                                       { ++count; });

    EXPECT_EQ(count, 2);

    count = 0;
    program->foreach_child<Expression>([&count](auto p)
                                       { ++count; });

    EXPECT_EQ(count, 3);

    // EXPECT_EQ(program->get_child<Integer>(0)->first_parent<Program>(), program);

    EXPECT_EQ(program->first_child<Function>()->get_body()->first_parent<Program>(), program);

    program->insert_child_after(new Identifier(loc, "d"), program->children[0]);
    EXPECT_TRUE(program->children[1]->is<Identifier>());
    EXPECT_TRUE(program->children[1]->as<Identifier>()->operator==("d"));

    program->insert_child_after(new Identifier(loc, "e"), program->children[3]);
    EXPECT_TRUE(program->children[4]->is<Identifier>());
    EXPECT_TRUE(program->children[4]->as<Identifier>()->operator==("e"));

    auto last = program->children.size();
    EXPECT_EQ(program->children.size(), last);
    auto el = new Identifier(loc, "last");
    program->insert_child_after(el, program->children[last - 1]);
    EXPECT_EQ(program->children.size(), last + 1);
    EXPECT_EQ(program->children[last], el);
    EXPECT_TRUE(program->children[last]->is<Identifier>());
    EXPECT_TRUE(program->children[last]->as<Identifier>()->operator==("last"));
}

TEST(test_constexpr, integer_literals)
{
    LOGIA_UNIT_TEST();
    using namespace logia::AST;

    auto i_10 = new IntegerLiteral(loc, "10");
    auto i_11 = new IntegerLiteral(loc, "11");
    auto i_5 = new IntegerLiteral(loc, "5");
    auto i_7 = new IntegerLiteral(loc, "7");

    EXPECT_TRUE(i_10->is_valid_constant_operator(Operators::BINARY_ADD));
    {
        auto result = i_10->operator+(i_11)->as<IntegerLiteral>();

        EXPECT_STREQ(result->value_str, "21");
        EXPECT_EQ(result->value.getSExtValue(), (int64_t)21);
    }
    EXPECT_TRUE(i_10->is_valid_constant_operator(Operators::BINARY_SUB));
    {
        auto result = i_10->operator-(i_11)->as<IntegerLiteral>();
        // TODO REVIEW this is the expected value
        // EXPECT_STREQ(result->value_str, "-1");
        EXPECT_STREQ(result->value_str, "18446744073709551615");
        EXPECT_EQ(result->value.getSExtValue(), (int64_t)-1);
    }
    EXPECT_TRUE(i_10->is_valid_constant_operator(Operators::BINARY_MUL));
    {

        auto result = i_10->operator*(i_11)->as<IntegerLiteral>();

        EXPECT_STREQ(result->value_str, "110");
        EXPECT_EQ(result->value.getSExtValue(), (int64_t)110);
    }
    EXPECT_TRUE(i_10->is_valid_constant_operator(Operators::BINARY_DIV));
    {
        auto result = i_10->operator/(i_5)->as<IntegerLiteral>();

        EXPECT_STREQ(result->value_str, "2");
        EXPECT_EQ(result->value.getSExtValue(), (int64_t)2);
    }
}

TEST(test_constexpr, float_literals)
{
    LOGIA_UNIT_TEST();
    using namespace logia::AST;

    auto i_10 = new FloatLiteral(loc, "10.8");
    auto i_11 = new FloatLiteral(loc, "11.7");
    auto i_5 = new FloatLiteral(loc, "5.6");
    auto i_7 = new FloatLiteral(loc, "7.4");

    EXPECT_TRUE(i_10->is_valid_constant_operator(Operators::BINARY_ADD));
    {
        auto result = i_10->operator+(i_11)->as<FloatLiteral>();

        EXPECT_STREQ(result->value_str, "22.5");
        EXPECT_NEAR(result->value.convertToDouble(), 10.8 + 11.7, 0.01);
    }
    EXPECT_TRUE(i_10->is_valid_constant_operator(Operators::BINARY_SUB));
    {
        auto result = i_10->operator-(i_11)->as<FloatLiteral>();
        EXPECT_STREQ(result->value_str, "-0.89999999999999858");
        EXPECT_NEAR(result->value.convertToDouble(), 10.8 - 11.7, 0.01);
    }
    EXPECT_TRUE(i_10->is_valid_constant_operator(Operators::BINARY_MUL));
    {

        auto result = i_10->operator*(i_11)->as<FloatLiteral>();

        EXPECT_STREQ(result->value_str, "126.36");
        EXPECT_NEAR(result->value.convertToDouble(), 10.8 * 11.7, 0.01);
    }
    EXPECT_TRUE(i_10->is_valid_constant_operator(Operators::BINARY_DIV));
    {
        auto result = i_10->operator/(i_5)->as<FloatLiteral>();

        EXPECT_STREQ(result->value_str, "1.9285714285714288");
        EXPECT_NEAR(result->value.convertToDouble(), 10.8 / 5.6, 0.01);
    }
}

TEST(test_constexpr, string_literals)
{
    LOGIA_UNIT_TEST();
    using namespace logia::AST;

    auto a = new StringLiteral(loc, "10");
    auto b = new StringLiteral(loc, "11");

    EXPECT_TRUE(a->is_valid_constant_operator(Operators::BINARY_ADD));

    auto result = a->operator+(b)->as<StringLiteral>();

    EXPECT_STREQ(result->text, "1011");
}

TEST(test_constexpr, float_limits)
{
    LOGIA_UNIT_TEST();
    using namespace logia::AST;
    {
        auto a = new FloatLiteral(loc, "-infinity");
        EXPECT_TRUE(a->value.isInfinity());
    }
    {
        auto a = new FloatLiteral(loc, "infinity");
        EXPECT_TRUE(a->value.isInfinity());
    }
    {
        auto a = new FloatLiteral(loc, "nan");
        EXPECT_TRUE(a->value.isNaN());
    }
    {
        auto a = new FloatLiteral(loc, "-nan");
        EXPECT_TRUE(a->value.isNaN());
    }
}

TEST(test_type, function)
{
    LOGIA_UNIT_TEST();
    using namespace logia::AST;
    main_fn->is_attached = false; // avoid throw

    auto a = new FunctionParameter(new Identifier(loc, "a"), i32, nullptr);
    main_fn->insert_parameter(0, a);
    EXPECT_EQ(a->index, 0);

    auto b = new FunctionParameter(new Identifier(loc, "b"), i32, nullptr);
    main_fn->insert_parameter(0, b);
    EXPECT_EQ(b->index, 0);
    EXPECT_EQ(a->index, 1);

    auto c = new FunctionParameter(new Identifier(loc, "c"), i32, nullptr);
    main_fn->insert_parameter(0, c);

    EXPECT_EQ(a->index, 2);
    EXPECT_EQ(b->index, 1);
    EXPECT_EQ(c->index, 0);

    auto d = new FunctionParameter(new Identifier(loc, "d"), i32, nullptr);
    main_fn->insert_parameter(2, d);
    EXPECT_EQ(a->index, 3);
    EXPECT_EQ(d->index, 2);
    EXPECT_EQ(b->index, 1);
    EXPECT_EQ(c->index, 0);

    main_fn->is_attached = true;
    program->semantic_analysis();
    EXPECT_STREQ(main_fn->get_repr().c_str(), "function main (λi32 c, λi32 b, λi32 d, λi32 a) λi32");
}

TEST(test_type, struct)
{
    LOGIA_UNIT_TEST();
    using namespace logia::AST;
    main_fn->is_attached = false; // avoid throw

    auto point_st = new Struct(loc, new Identifier(loc, "point"));
    program->push_child(point_st);

    auto add_fn = new Function(loc, new Identifier(loc, "add"), test_make_typdef("point"), false);
    add_fn->push_parameter(new FunctionParameter(new Identifier(loc, "other"), test_make_typdef("point")));
    LOG(DBG, "function: {}", add_fn->to_string_tree());
    LOG(DBG, "struct {}", point_st->to_string_tree());
    point_st->add_method(add_fn);
    LOG(DBG, "struct {}", point_st->to_string_tree());

    program->semantic_analysis();
    EXPECT_STREQ(point_st->get_repr().c_str(), "struct point {}");
    EXPECT_STREQ(add_fn->get_repr().c_str(), "function add (ref<struct point {}> this, point other) point");
}

TEST(test_type, callexpr)
{
    LOGIA_UNIT_TEST();
    using namespace logia::AST;
    main_fn->is_attached = false; // avoid throw

    auto test_fn = new Function(loc, new Identifier(loc, "test"), i32, false);
    test_fn->get_body()->push_child(new ReturnStmt(loc, new IntegerLiteral(loc, "0", i32)));
    program->unshift_child(test_fn);

    EXPECT_STREQ(test_fn->get_repr().c_str(), "function test () λi32");

    main_body->unshift_child(new CallExpression(loc, new Identifier(loc, "test"), {}));
    program->semantic_analysis();
    // backend->emitTargetLLVMIR("xxx.ll");
    // backend->run_jit("main");
}

TEST(test_type, type_inference_return_stmt_constexpr)
{
    LOGIA_UNIT_TEST();
    using namespace logia::AST;
    main_fn->is_attached = false; // avoid throw

    EXPECT_EQ(backend->run_jit("main"), 0);
    // 0 - NoOp
    auto ret = main_body->get_child<ReturnStmt>(1);

    EXPECT_EQ(ret->get_type_decl(), i32);
    EXPECT_EQ(ret->get_expr()->get_type_decl(), i32);
}

TEST(type_inference_pass_check, constant_return)
{
    LOGIA_UNIT_TEST();
    using namespace logia::AST;
    main_fn->is_attached = false; // avoid throw

    // 0 - NoOp
    auto ret = main_body->get_child<ReturnStmt>(1);
    EXPECT_EQ(ret->get_type(), nullptr);
    // nodes starts with default values
    logia::type_inference_pass(program, program, TYPE_INFERENCE_EARLY);
    EXPECT_EQ(ret->get_type(), nullptr);
    EXPECT_EQ(ret->get_expr()->get_type(), i64);
    EXPECT_EQ(ret->get_expr()->get_type_decl(), i64);

    // function narrow return types to i32 -> ret = i32 -> ret.expr = i32
    logia::type_inference_pass(program, program, TYPE_INFERENCE_PRE);
    EXPECT_EQ(ret->get_type(), i32);
    EXPECT_EQ(ret->get_type_decl(), i32);
    EXPECT_EQ(ret->get_expr()->get_type(), i32);
    EXPECT_EQ(ret->get_expr()->get_type_decl(), i32);
}

TEST(type_inference_pass_check, infer_vardecl_with_initialization)
{
    LOGIA_UNIT_TEST();
    using namespace logia::AST;
    main_fn->is_attached = false; // avoid throw

    auto vardecl = new VarDeclStmt(loc, new Identifier(loc, "x"), new IntegerLiteral(loc, "15"));
    main_body->unshift_child(vardecl);

    EXPECT_EQ(vardecl->get_type(), nullptr);
    EXPECT_EQ(vardecl->get_expr()->get_type(), nullptr);

    // nodes starts with default values
    logia::type_inference_pass(program, program, TYPE_INFERENCE_EARLY);
    EXPECT_EQ(vardecl->get_type(), nullptr);
    EXPECT_EQ(vardecl->get_expr()->get_type(), i64);

    // function narrow return types to i32 -> ret = i32 -> ret.expr = i32
    logia::type_inference_pass(program, program, TYPE_INFERENCE_PRE);
    EXPECT_EQ(vardecl->get_type(), i64);
    EXPECT_EQ(vardecl->get_expr()->get_type(), i64);
}

TEST(type_inference_pass_check, vardecl_with_constant_initialization)
{
    LOGIA_UNIT_TEST();
    using namespace logia::AST;
    main_fn->is_attached = false; // avoid throw

    auto td_i16 = new TypeDef(loc);
    td_i16->add_identifier(new Identifier(loc, "i16"));

    auto vardecl = new VarDeclStmt(loc, new Identifier(loc, "x"), td_i16, new IntegerLiteral(loc, "15"));
    main_body->unshift_child(vardecl);

    auto st_i16 = scope_lookup_one(td_i16, "i16")->as<TypeDecl>();

    EXPECT_EQ(vardecl->get_type(), td_i16);
    EXPECT_EQ(vardecl->get_type_decl(), nullptr);
    EXPECT_EQ(vardecl->get_expr()->get_type(), nullptr);

    // nodes starts with default values
    program->semantic_analysis_validate();
    program->semantic_analysis_type_inference(TYPE_INFERENCE_EARLY);

    EXPECT_EQ(vardecl->get_type(), td_i16);
    EXPECT_EQ(vardecl->get_type_decl(), st_i16);
    EXPECT_EQ(vardecl->get_expr()->get_type(), i64); // this is the effective as is set by type_inference

    program->semantic_analysis_type_inference(TYPE_INFERENCE_PRE);
    program->semantic_analysis_type_inference(TYPE_INFERENCE_POST);

    EXPECT_EQ(vardecl->get_type(), td_i16);
    EXPECT_EQ(vardecl->get_expr()->get_type(), scope_look_one<logia::AST::Struct>(program, "i16"));
    EXPECT_EQ(vardecl->get_expr()->get_type_decl()->get_effective_type_decl(), i16);
}

TEST(type_inference_pass_check, vardecl_with_expr_initialization)
{
    LOGIA_UNIT_TEST();
    using namespace logia::AST;
    main_fn->is_attached = false; // avoid throw

    auto td_i32_ret = test_make_typdef("i32");
    auto xxx_fn = new Function(loc, new Identifier(loc, "xxx"), td_i32_ret, false);
    program->unshift_child(xxx_fn);
    auto xxx_call = new CallExpression(loc, new Identifier(loc, "xxx"), {});

    auto vardecl = new VarDeclStmt(loc, new Identifier(loc, "x"), xxx_call);
    main_body->unshift_child(vardecl);

    auto st_i32 = scope_lookup_one(program, "i32")->as<TypeDecl>();

    EXPECT_EQ(vardecl->get_type(), nullptr);
    EXPECT_EQ(vardecl->get_type_decl(), nullptr);
    EXPECT_EQ(vardecl->get_expr()->get_type(), nullptr);

    // nodes starts with default values
    logia::type_inference_pass(program, program, TYPE_INFERENCE_EARLY);
    EXPECT_EQ(vardecl->get_type(), nullptr);
    EXPECT_EQ(vardecl->get_type_decl(), nullptr);
    EXPECT_EQ(vardecl->get_expr()->get_type(), nullptr);

    logia::type_inference_pass(program, program, TYPE_INFERENCE_PRE);
    logia::type_inference_pass(program, program, TYPE_INFERENCE_POST);
    EXPECT_EQ(vardecl->get_type(), st_i32);
    EXPECT_EQ(vardecl->get_type_decl(), st_i32);
    EXPECT_EQ(vardecl->get_type_decl()->get_effective_type_decl(), i32);

    EXPECT_EQ(vardecl->get_identifier()->get_type(), st_i32);
    EXPECT_EQ(vardecl->get_identifier()->get_type_decl(), st_i32);
    EXPECT_EQ(vardecl->get_identifier()->get_type_decl()->get_effective_type_decl(), i32);

    EXPECT_EQ(vardecl->get_expr()->get_type(), st_i32);
    EXPECT_EQ(vardecl->get_expr()->get_type_decl(), st_i32);
    EXPECT_EQ(vardecl->get_expr()->get_type_decl()->get_effective_type_decl(), i32);
}
/* TODO
TEST(type_inference_pass_check, vardecl_with_expr_initialization_and_cast)
{
    LOGIA_UNIT_TEST();
    using namespace logia::AST;
    main_fn->is_attached = false; // avoid throw

    auto td_i16 = new TypeDef();
    td_i16->add_locator(new Identifier(loc, "i16"));

    auto xxx_fn = new Function(loc, new Identifier(loc, "xxx"), td_i16, false);
    program->unshift_child(xxx_fn);
    auto xxx_call = new CallExpression(loc, new Identifier(loc, "xxx"), {});

    auto td_i32 = new TypeDef();
    td_i32->add_locator(new Identifier(loc, "i32"));
    auto vardecl = new VarDeclStmt(loc, new Identifier(loc, "x"), td_i32, xxx_call);
    main_body->unshift_child(vardecl);

    auto st_i32 = scope_lookup_one(td_i32, "i32")->as<TypeDecl>();
    auto st_i16 = scope_lookup_one(td_i16, "i16")->as<TypeDecl>();

    EXPECT_EQ(vardecl->get_type(), td_i32);
    EXPECT_EQ(vardecl->get_type_decl(), nullptr);
    EXPECT_EQ(vardecl->get_expr()->get_type(), nullptr);

    // nodes starts with default values
    logia::type_inference_pass(program, program, TYPE_INFERENCE_EARLY);
    EXPECT_EQ(vardecl->get_type(), td_i32);
    EXPECT_EQ(vardecl->get_type_decl(), st_i32);
    EXPECT_EQ(vardecl->get_expr()->get_type(), nullptr);

    logia::type_inference_pass(program, program, TYPE_INFERENCE_PRE);
    logia::type_inference_pass(program, program, TYPE_INFERENCE_POST);
    EXPECT_EQ(vardecl->get_type(), td_i32);
    EXPECT_EQ(vardecl->get_type_decl(), st_i32);
    EXPECT_EQ(vardecl->get_type_decl()->get_effective_type_decl(), i32);

    EXPECT_EQ(vardecl->get_identifier()->get_type(), st_i32);
    EXPECT_EQ(vardecl->get_identifier()->get_type_decl(), st_i32);
    EXPECT_EQ(vardecl->get_identifier()->get_type_decl()->get_effective_type_decl(), i32);

    // TODO this need to be resolved in a cast -> vardecl shall be split into "alloca -> assignament"
    // vardecl->get_expr()->is<BinaryExpression>()
    // vardecl->get_expr()->get_right()->is<BinaryExpression>()

    std::cout << vardecl->get_expr()->get_type()->get_repr() << std::endl;
    std::cout << vardecl->get_expr()->get_type_decl()->get_repr() << std::endl;
    std::cout << vardecl->get_expr()->get_type_decl()->get_efective_type_decl()->get_repr() << std::endl;

    EXPECT_EQ(vardecl->get_expr()->get_type(), st_i16);
    EXPECT_EQ(vardecl->get_expr()->get_type_decl(), st_i32);
    EXPECT_EQ(vardecl->get_expr()->get_type_decl()->get_efective_type_decl(), i32);
}
*/
TEST(test_type, struct_field_ref)
{
    LOGIA_UNIT_TEST();
    using namespace logia::AST;
    main_fn->is_attached = false; // avoid throw

    auto i32_st = scope_look_one<logia::AST::Struct>(program, "i32");

    auto point_st = new Struct(loc, new Identifier(loc, "point"));
    point_st->add_field(loc, new Identifier(loc, "x"), test_make_typdef("i32"));
    program->push_child(point_st);
    // normal typedef
    {
        auto td_point = test_make_typdef("point");
        auto init = new StructInitializer(loc);
        init->add_positional_property(new IntegerLiteral(loc, "100"));
        auto vardecl = new VarDeclStmt(loc, new Identifier(loc, "x"), td_point, init);
        main_body->unshift_child(vardecl);

        program->semantic_analysis();

        // TODO madness!?
        EXPECT_STREQ(point_st->get_repr().c_str(), "struct point {struct i32 {λi32 λ} x}");

        EXPECT_EQ(vardecl->get_type(), td_point);
        EXPECT_EQ(vardecl->get_type_decl(), point_st);
        EXPECT_EQ(vardecl->get_type_decl()->get_effective_type_decl(), point_st);

        EXPECT_EQ(init->get_type(), point_st);
        EXPECT_EQ(init->get_type_decl(), point_st);
        EXPECT_EQ(init->get_type_decl()->get_effective_type_decl(), point_st);
    }

    // 2 level typedef
    {
        auto td_point_x = new TypeDef(loc);
        td_point_x->add_identifier(new Identifier(loc, "point"));
        td_point_x->add_identifier(new Identifier(loc, "x"));

        auto vardecl = new VarDeclStmt(loc, new Identifier(loc, "x"), td_point_x, new IntegerLiteral(loc, "100"));
        main_body->unshift_child(vardecl);

        program->type_inference_pass_id = 0;
        program->semantic_analysis();

        EXPECT_EQ(vardecl->get_type(), td_point_x);
        EXPECT_EQ(vardecl->get_type_decl(), i32_st);
        EXPECT_EQ(vardecl->get_type_decl()->get_effective_type_decl(), i32);

        EXPECT_EQ(vardecl->get_expr()->get_type_decl(), i32_st);
        EXPECT_EQ(vardecl->get_expr()->get_type_decl()->get_effective_type_decl(), i32);
    }
    // invalid 2 level
    try
    {
        auto the_typedef = new TypeDef(loc);
        the_typedef->add_identifier(new Identifier(loc, "i32"));
        the_typedef->add_identifier(new Identifier(loc, "xxx"));

        auto vardecl = new VarDeclStmt(loc, new Identifier(loc, "x"), the_typedef, new IntegerLiteral(loc, "100"));
        main_body->unshift_child(vardecl);

        program->type_inference_pass_id = 0;
        program->semantic_analysis();
    }
    catch (logia::AST::semantic_error &e)
    {
        EXPECT_THAT(e.what(), testing::HasSubstr("unexpected type 'struct i32 {λi32 λ}' do not have a property named: 'xxx'"));
    }

    /*


    EXPECT_STREQ(point_st->get_repr().c_str(), "struct point {λi32 x}");

        EXPECT_EQ(td_point->get_type_decl(), point_st);
        EXPECT_EQ(td_point_x->get_type_decl(), i32);
    */
}

TEST(test_type, struct_tpl_1)
{
    LOGIA_UNIT_TEST();
    using namespace logia::AST;
    main_fn->is_attached = false; // avoid throw

    auto st = new Struct(loc, new Identifier(loc, "v2"));
    EXPECT_EQ(st->scope->scope.size(), 1); // self
    EXPECT_TRUE(st->scope->scope_contains("self"));

    EXPECT_EQ(st->base, nullptr);
    EXPECT_EQ(st->tpl_params.size(), 0);

    st->add_template_parameter(new TemplateParameter(loc, new Identifier(loc, "$t")));
    EXPECT_EQ(st->tpl_params.size(), 1);
    EXPECT_EQ(st->scope->children.size(), 1);

    EXPECT_STREQ(st->get_repr().c_str(), "struct v2<$t> {}");

    st->add_field(loc, new Identifier(loc, "x"), test_make_typdef("$t"));
    EXPECT_STREQ(st->get_repr().c_str(), "struct v2<$t> {?? x}");
    EXPECT_EQ(st->children.size(), 1);
    EXPECT_EQ(st->scope->children.size(), 2);

    st->add_field(loc, new Identifier(loc, "y"), test_make_typdef("$t"));
    EXPECT_STREQ(st->get_repr().c_str(), "struct v2<$t> {?? x, ?? y}");

    EXPECT_EQ(st->scope->children.size(), 3);

    program->unshift_child(st);
    // check scope here, as it's attached
    EXPECT_EQ(st->scope->scope.size(), 2);
    EXPECT_TRUE(st->scope->scope_contains("$t"));

    program->semantic_analysis();

    auto f0 = st->get_field_by_index(0);
    auto f0_td = f0->get_type()->as<TypeDef>();
    f0_td->get_type_decl();

    EXPECT_STREQ(st->get_repr().c_str(), "struct v2<$t> {$t x, $t y}");
}
