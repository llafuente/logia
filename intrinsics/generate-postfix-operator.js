// for completeness but this operators are not used!
const types = process.argv.slice(2);

const operators = [
    {
        name: "INCREMENT",
        operator: "++",
        func: "logia_intrinsics_postfix_inc",
        logia_fn: "__logia_postfix_inc"
    },
    {
        name: "DECREMENT",
        operator: "--",
        func: "logia_intrinsics_postfix_dec",
        logia_fn: "__logia_postfix_dec"
    }];

for (let o of operators) {
    for (let type of types) {
        if (o.operator == "++" || o.operator == "--") {
            continue;
        }
        console.log(`
extern "C"
[[clang::annotate("logia=${o.logia_fn}")]]
${type} ${o.func}_${type}([[clang::annotate("logia=ref<${type}>")]] ${type} a) {
    return ${o.operator}a;
}`);
    }
}