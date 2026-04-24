const types = process.argv.slice(2);

const operators = require("./binary-operators.js");

for (let type of types) {
    for (let o of operators) {
        if (o.category == "bitwise" || ["%", "%=", "&=", "|=", "^=", ">", "<<=", ">>="].indexOf(o.operator) >= 0)
            continue;

        if (o.category === 'comparison') { // return bool
            console.log(`
extern "C"
bool ${o.func}_${type}_${type}(${type} a, ${type} b){
    return a ${o.operator} b;
}`);
        } else if (o.category === 'bitwise' || o.category === 'arithmetic') {
            console.log(`
extern "C"
${type} ${o.func}_${type}_${type}(${type} a, ${type} b){
    return a ${o.operator} b;
}`);
        } else if (o.category === 'assignment') {
            console.log(`
extern "C"
${type} ${o.func}_${type}_${type}(${type} *a, ${type} b){
    *a ${o.operator} b;
    return *a;
}`);
        } else if (o.category === 'logical') {
            // ignore logical as they will be implemented inside the language for code-coverage purposes
        } else {
            console.log(op);
            throw new Error("unreachable");
        }
    }
}
