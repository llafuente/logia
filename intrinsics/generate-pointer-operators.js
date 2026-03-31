// node generate-pointer-operators.js > pointer-operators.cpp

const fs = require('fs');
const path = require('path');
const process = require('process');

const operators = JSON.parse(fs.readFileSync(path.join(__dirname, 'operators.json'), 'utf-8'));

const pointerType = 'ptr';
const offsetType = 'i64';
const diffType = 'i64';

// operators to name
const operation = {
    '+': 'add',
    '-': 'sub',
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
    '? :': 'ternary',
};

const invalid_ops = [
    '*', '*=',
    '/', '/=',
    '%', '%=',
    '&', '&=',
    '|', '|=',
    '^', '^=',
    '~',
    '<<', '<<=',
    '>>', '>>=',
];

function emit(text) {
    console.log(text);
}

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
        if (op.operator === '+') {
            emit(`${pointerType} logia_intrinsics_bin_${operation_name}_${pointerType}_${offsetType}(${pointerType} a, ${offsetType} b){
return a + b;
}`);
            emit(`${pointerType} logia_intrinsics_bin_${operation_name}_${offsetType}_${pointerType}(${offsetType} a, ${pointerType} b){
return a + b;
}`);
        } else if (op.operator === '-') {
            emit(`${pointerType} logia_intrinsics_bin_${operation_name}_${pointerType}_${offsetType}(${pointerType} a, ${offsetType} b){
return a - b;
}`);
            emit(`${diffType} logia_intrinsics_bin_${operation_name}_${pointerType}_${pointerType}(${pointerType} a, ${pointerType} b){
return a - b;
}`);
        } else if (op.category === 'comparison' || op.category === 'logical') {
            emit(`bool logia_intrinsics_bin_${operation_name}_${pointerType}(${pointerType} a, ${pointerType} b){
return a ${op.operator} b;
}`);
        } else if (op.operator === '=') {
            emit(`${pointerType} logia_intrinsics_bin_${operation_name}_${pointerType}(${pointerType} a, ${pointerType} b){
return a = b;
}`);
        } else if (op.operator === '+=') {
            emit(`${pointerType} logia_intrinsics_bin_${operation_name}_${pointerType}_${offsetType}(${pointerType} a, ${offsetType} b){
return a += b;
}`);
        } else if (op.operator === '-=') {
            emit(`${pointerType} logia_intrinsics_bin_${operation_name}_${pointerType}_${offsetType}(${pointerType} a, ${offsetType} b){
return a -= b;
}`);
        }
    } else if (op.type === 'pre-unary') {
        if (op.operator === '!') {
            emit(`bool logia_intrinsics_pre_una_${operation_name}_${pointerType}(${pointerType} a) {
return !a;
}`);
        } else if (op.operator === '++' || op.operator === '--') {
            emit(`${pointerType} logia_intrinsics_pre_una_${operation_name}_${pointerType}(${pointerType} a) {
return ${op.operator}a;
}`);
        }
    } else if (op.type === 'post-unary') {
        if (op.operator === '++' || op.operator === '--') {
            emit(`${pointerType} logia_intrinsics_post_una_${operation_name}_${pointerType}(${pointerType} a) {
return a${op.operator};
}`);
        }
    } else if (op.type === 'ternary') {
        emit(`${pointerType} logia_intrinsics_ter_${operation_name}_${pointerType}(${pointerType} a, ${pointerType} b, ${pointerType} c){
return a ? b : c;
}`);
    }
});
