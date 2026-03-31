// node generate-integer-operators.js > integer-operators.cpp

const fs = require('fs');
const path = require('path');
const process = require('process');

const operators = JSON.parse(fs.readFileSync(path.join(__dirname, 'operators.json'), 'utf-8'));

// list of all types in c
const types = [
    'i8',
    'i16',
    'i32',
    'i64',
    'u8',
    'u16',
    'u32',
    'u64',
];

// operators to name
const operation = {
    '+': 'add',
    '-': 'sub',
    '*': 'mul',
    '/': 'div',
    '%': 'mod',
    '==': 'eq',
    '!=': 'neq',
    '<': 'lt',
    '>': 'gt',
    '<=': 'lte',
    '>=': 'gte',
    '&&': 'and',
    '||': 'or',
    '!': 'not',
    '<<': 'shl',
    '>>': 'shr',
    '++': 'inc',
    '--': 'dec',
    '&': 'bitand',
    '|': 'bitor',
    '^': 'bitxor',
    '~': 'bitnot',
    '=': 'assign',
    '+=': 'add_assign',
    '-=': 'sub_assign',
    '*=': 'mul_assign',
    '/=': 'div_assign',
    '%=': 'mod_assign',
    '&=': 'bitand_assign',
    '|=': 'bitor_assign',
    '^=': 'bitxor_assign',
    '<<=': 'shl_assign',
    '>>=': 'shr_assign',
    '? :': 'ternary',
}
//types.slice(0, 1).forEach(type => {
types.forEach(type => {
    operators.operators.forEach(op => {
        operation_name = operation[op.operator];
        if (!operation_name) {
            console.log(`Unknown operator: ${op.operator}`);
            process.exit(1);
        }

        if (op.type === 'binary') {
            if(op.category === 'comparison') {
                console.log(`bool logia_intrinsics_bin_${operation_name}_${type}(${type} a, ${type} b){
return a ${op.operator} b;
}`);
            } else if (op.category === 'bitwise' || op.category === 'arithmetic' || op.category === 'assignment' ){
                console.log(`${type} logia_intrinsics_bin_${operation_name}_${type}(${type} a, ${type} b){
return a ${op.operator} b;
}`);
            }
        } else if (op.type === 'pre-unary') {
            console.log(`${type} logia_intrinsics_pre_una_${operation_name}_${type}(${type} a) {
return ${op.operator}a;
}`);
        } else if (op.type === 'post-unary') {
            console.log(`${type} logia_intrinsics_post_una_${operation_name}_${type}(${type} a) {
return a${op.operator};
}`);
        } else if (op.type === 'ternary') {
            console.log(`${type} logia_intrinsics_ter_${operation_name}_${type}(${type} a, ${type} b, ${type} c);\n`);
        }


        //console.log(`${type} logia_intrinsics_${operation_name}_${type}(${type} a, ${type} b);\n`);
    });
});

