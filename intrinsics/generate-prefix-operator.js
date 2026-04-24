const types = process.argv.slice(2);

const operators = [
  /*
  // implemented in the compiler
{
  name: "DEREFERENCE",
  operator: "&",
  func: "logia_intrinsics_deref"
},
  */
  {
    name: "NEGATION",
    operator: "-",
    func: "logia_intrinsics_prefix_neg"
  },
  {
    name: "LOGICAL_NOT",
    operator: "!",
    func: "logia_intrinsics_prefix_logical_not"
  },
  {
    name: "INCREMENT",
    operator: "++",
    func: "logia_intrinsics_prefix_inc"
  },
  {
    name: "DECREMENT",
    operator: "--",
    func: "logia_intrinsics_prefix_dec"
  },
  {
    name: "BITWISE_NOT",
    operator: "~",
    func: "logia_intrinsics_prefix_bitwise_not"
  }];

for (let o of operators) {
  for (let type of types) {
    if (o.operator == "++" || o.operator == "--") {
      continue;
    }

    if (o.operator == "!") {
      console.log(`
extern "C"
bool ${o.func}_${type}(${type} a) {
    return ${o.operator}a;
}`);
    } else {
      console.log(`
extern "C"
${type} ${o.func}_${type}(${type} a) {
    return ${o.operator}a;
}`);
    }
  }
}
