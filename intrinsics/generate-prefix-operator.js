const types = process.argv.slice(2);

[/*
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
    func: "logia_intrinsics_prefix_neg",
    logia_fn: "__logia_prefix_neg"
  },
  {
    name: "BITWISE_NOT",
    operator: "~",
    func: "logia_intrinsics_prefix_bitwise_not",
    logia_fn: "__logia_prefix_bitwise_not"
  }
].forEach((o) => {
  for (let type of types) {
    console.log(`
extern "C"
[[clang::annotate("logia=${o.logia_fn}")]]
${type} ${o.func}_${type}([[clang::annotate("logia=λ${type}")]] ${type} a) {
    return ${o.operator}a;
}`);
  }
});

[
  {
    name: "LOGICAL_NOT",
    operator: "!",
    func: "logia_intrinsics_prefix_logical_not",
    logia_fn: "__logia_prefix_bitwise_not"
  }
].forEach((o) => {
  for (let type of types) {
    console.log(`
extern "C"
[[clang::annotate("logia=${o.logia_fn}")]]
bool ${o.func}_${type}([[clang::annotate("logia=λ${type}")]] ${type} a) {
    return ${o.operator}a;
}`);
  }
});

[
  {
    name: "INCREMENT",
    operator: "++",
    func: "logia_intrinsics_prefix_inc",
    logia_fn: "__logia_prefix_inc"
  },
  {
    name: "DECREMENT",
    operator: "--",
    func: "logia_intrinsics_prefix_dec",
    logia_fn: "__logia_prefix_dec"
  }].forEach((o) => {
    for (let type of types) {
      console.log(`
extern "C"
[[clang::annotate("logia=${o.logia_fn}")]]
${type} ${o.func}_${type}([[clang::annotate("logia=ref<λ${type}>")]] ${type}* a) {
    return ${o.operator}(*a);
}`);
    }
  });