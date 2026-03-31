// node generate-floating-operators.js > floating-operators.cpp

const fs = require('fs');
const path = require('path');
const process = require('process');

const operators = JSON.parse(fs.readFileSync(path.join(__dirname, 'operators.json'), 'utf-8'));

// list of floating-point types
const types = [
    'f32',
    'f64',
];

// operators to name
const operation = {
    '+': 'add',
    '-': 'sub',
    '*': 'mul',
    '/': 'div',
    '==': 'eq',
    '!=': 'neq',
    '<': 'lt',
    '>': 'gt',
    '<=': 'lte',
    '>=': 'gte',
    '&&': 'and',
    '||': 'or',
    '!': 'not',
    '++': 'inc',
    '--': 'dec',
    '=': 'assign',
    '+=': 'add_assign',
    '-=': 'sub_assign',
    '*=': 'mul_assign',
    '/=': 'div_assign',
    '? :': 'ternary',
};
const invalid_ops = ['%', '%=', '&', '&=', '|', '|=', '^', '^=', '~', '<<', '<<=', '>>', '>>='];

types.forEach(type => {
    operators.operators.forEach(op => {
        if (invalid_ops.includes(op.operator)) {
            return;
        }

        const operation_name = operation[op.operator];
        if (!operation_name) {
            console.log(`Unknown operator: ${op.operator}`);
            process.exit(1);
        }

        if (op.type === 'binary') {
            if (op.category === 'comparison' || op.category === 'logical') {
                console.log(`bool logia_intrinsics_bin_${operation_name}_${type}(${type} a, ${type} b){
return a ${op.operator} b;
}`);
            } else if (op.category === 'arithmetic' || op.category === 'assignment') {
                console.log(`${type} logia_intrinsics_bin_${operation_name}_${type}(${type} a, ${type} b){
return a ${op.operator} b;
}`);
            }
        } else if (op.type === 'pre-unary') {
            if (op.operator === '!') {
                console.log(`bool logia_intrinsics_pre_una_${operation_name}_${type}(${type} a) {
return ${op.operator}a;
}`);
            } else {
                console.log(`${type} logia_intrinsics_pre_una_${operation_name}_${type}(${type} a) {
return ${op.operator}a;
}`);
            }
        } else if (op.type === 'post-unary') {
            console.log(`${type} logia_intrinsics_post_una_${operation_name}_${type}(${type} a) {
return a${op.operator};
}`);
        } else if (op.type === 'ternary') {
            console.log(`${type} logia_intrinsics_ter_${operation_name}_${type}(${type} a, ${type} b, ${type} c){
return a ? b : c;
}`);
        }
    });
});
