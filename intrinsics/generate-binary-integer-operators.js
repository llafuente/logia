const types = process.argv.slice(2);

const operators = require("./binary-operators.js");

for (let type of types) {
    for (let o of operators) {
        if (o.category === "comparison") { // return bool
            console.log(`
extern "C"
[[clang::annotate("logia=${o.logia_fn}/λi1/λ${type}/λ${type}")]]
bool ${o.func}_${type}_${type}(${type} a, ${type} b){
    return a ${o.operator} b;
}`);
        } else if (o.category === "bitwise" || o.category === "arithmetic") {
            console.log(`
extern "C"
[[clang::annotate("logia=${o.logia_fn}/λ${type}/λ${type}/λ${type}")]]
${type} ${o.func}_${type}_${type}(${type} a, ${type} b){
    return a ${o.operator} b;
}`);
        } else if (o.category === "assignment") {
            console.log(`
extern "C"
[[clang::annotate("logia=${o.logia_fn}/λ${type}/ref<λ${type}>/λ${type}")]]
${type} ${o.func}_${type}_${type}(${type} *a, ${type} b){
    *a ${o.operator} b;
    return *a;
}`);
        } else if (o.category === "logical") {
            // ignore logical as they will be implemented inside the language for code-coverage purposes
        } else {
            console.log(o);
            throw new Error("unreachable");
        }
    }
}
