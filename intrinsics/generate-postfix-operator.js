// for completeness but this operators are not used!
const types = process.argv.slice(2);

const operators = [
{
  name: "INCREMENT",
  operator: "++",
  func: "logia_intrinsics_postfix_inc"
},
{
  name: "DECREMENT",
  operator: "--",
  func: "logia_intrinsics_postfix_dec"
}];

for (let o of operators) {
    for (let type of types) {
        if (o.operator == "++" || o.operator == "--") {
            continue;
        }
                console.log(`
extern "C"
${type} ${o.func}_${type}(${type} a) {
    return ${o.operator}a;
}`);
    }
}