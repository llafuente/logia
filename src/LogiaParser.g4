parser grammar LogiaParser;
options { tokenVocab=LogiaLexer; }
//file: spec\compiler\compilation-phases.md

//file: spec\compiler\compiler-configuration.md
preprocessorSetStatement
  : '#' 'set' identifier ('.' identifier)* '=' mayBeConstant
  ;

//file: spec\compiler\project-structure.md

//file: spec\compiler\the-source-file.md

//file: spec\features.md

//file: spec\general\conformance.md

//file: spec\general\glossary.md

//file: spec\general\normative-references.md

//file: spec\general\scope.md

//file: spec\generic-programming.md

//file: spec\keywords.md

keywords
  // Type system
  : 'type'
  | 'struct'
  | 'noalign'
  | 'lean'
  | 'enum'
  | 'mask'
  | 'extends'
  | 'implements'
  | 'interface'
  | 'is'
  | 'any'
  | 'self'
  | 'hoist'
  | 'readonly'
  | 'instanceof'
  | 'static'
  // constants
  | 'true'
  | 'false'
  | 'null'
  // variables
  | 'var'
  | 'const'
  | 'global'
  | 'package'
  // functions
  | 'function'
  | 'pure'
  | 'operator'
  | 'return'
  | 'alias'
  | 'get'
  | 'set'
  | 'autocast'
  | 'defer'
  | 'out'
  | 'override'
  | 'overwrite'
  // Control flow
  | 'if'
  | 'else'
  | 'switch'
  | 'case'
  | 'fallthrough'
  | 'goto'
  | 'loop'
  | 'where'
  | 'foreach'
  | 'for'
  | 'do'
  | 'while'
  | 'until'
  | 'continue'
  | 'restart'
  | 'break'
  | 'in'
  | 'default'
  // memory
  | 'clone'
  | 'new'
  | 'delete'
  | 'lend'
  | 'own'
  | 'uninitialized'
  | 'at'
  | 'cast'
  // error handling
  | 'try'
  | 'retry'
  | 'catch'
  | 'finally'
  | 'throw'
  // Operators
  | 'not'
  | 'and'
  | 'or'
  // package system
  | 'import'
  | 'as'
  // unit-test
  | 'test'
  | 'mock'
  ;

//file: spec\language\control-flow.md
selectionStmts
  : ifStmt
  | switchStmt
  | gotoStmt
  | loopStmt
  | forStmt
  | foreachStmt
  | whileStmt
  | doWhileStmt
  | continueStmt
  | restartStmt
  | breakStmt
  | fallthroughStmt
  ;

ifSelectionStmt
  // REVIEW syntax require block here ?
  : 'if' expression blockStmt
  ;

elseSelectionStmt
  : 'else' blockStmt
  ;

ifStmt
  : ifSelectionStmt ('else' ifSelectionStmt)* elseSelectionStmt?
  ;

switchCaseStmt
  // REVIEW syntax require block here ? also required colon ?
  : 'case' expressionList ':' functionBodyStmtList
  ;
switchDefaultStmt
  : 'default' ':' functionBodyStmtList
  ;

switchStmt
  : 'switch' expression '{' (endOfStmt? switchCaseStmt)+ (endOfStmt? switchDefaultStmt)? '}'
  ;

gotoStmt
  : 'goto' identifier
  ;

loopStmt
  : 'loop'
    ((value=identifier 'in') | (key=identifier ',' value=identifier 'in'))?
    loop_limit_expr=expression
    ('where' where_expr=expression)?
    (('until' until_expr=expression) | ('while' while_expr=expression))?
    loop_body=blockStmt
  ;

forCondition
    : (blockVariableDeclStmt | expression)? ';' forExpression? ';' forExpression?
    ;

forExpression
  : assignmentExpr (',' assignmentExpr)*
  ;

forStmt
  : 'for' '(' forCondition ')' blockStmt
  ;

foreachStmt
  : 'foreach'
    ((value=identifier 'in') | (key=identifier ',' value=identifier 'in'))?
    foreach_expr=expression
    foreach_body=blockStmt
  ;

whileStmt
  : 'while' expression blockStmt
  ;

doWhileStmt
  : 'do' blockStmt ('while' | 'until') expression
  ;

continueStmt
  : 'continue' (identifier | numberLiteral)?
  ;

restartStmt
  : 'restart' ( identifier | numberLiteral )?
  ;

breakStmt
  : 'break' ( identifier | numberLiteral )?
  ;

fallthroughStmt
  : 'fallthrough'
  ;

//file: spec\language\error-handling.md

errorHandlingStmts
  : tryBlock catchBlock* finallyBlock?
  | 'throw' expression?
  ;

errorHandlingExprs
  : 'try' conditionalExpr ('catch' conditionalExpr)?
  // TODO how do we rename the exception ?
  // catch x: expr
  // catch expr as x
  // try name {} catch name is x {}
  | 'catch' conditionalExpr ('as' name=identifier)? blockStmt
  ;

tryBlock
	: 'try' blockStmt
	;

catchBlock
	: 'catch' (
      typeDefinition identifier
      | '(' typeDefinition identifier ')'
      | postfixExpr
    )? blockStmt
	;

finallyBlock
	: 'finally' blockStmt
	;


retryUntilWhileStmt
  : 'retry' keyName=identifier? (',' expectionName=identifier)? ('while' expression | 'until' expression) blockStmt
  ;

//file: spec\language\expressions.md

/*
expressions
*/

constant
    : 'true'
    | 'false'
    | 'null'
    | 'default'
    | stringLiteral
    | numberLiteral
    | identifier
    | preprocessorExpr
    | regularExpressionLiteral
    // TODO REVIEW FOLLOWING constants ?
    // | 'type'                  # typeLiteralExpr
    // | 'struct'                 # structTypeLit
    // | 'cast'                  # castLiteralExpr
    // | 'new'                   # newValueExpr
    // | 'mask'                   # maskTypeLit
    // | 'enum'                   # enumTypeLit
    // | 'function'               # functionTypeLit
    ;

regularExpressionLiteral: RegularExpressionLiteral;

// this is a construct to easy syntax parsing
// some can be constant or not, but it's up to the compiler to find out, not the parser
mayBeConstant
    : constant
    | arrayInitializer
    | structConstantInitializer
    | structInitializer
    ;

groupExpr
  : '(' expr=expression ')'
  ;

primaryExpr
    : mayBeConstant
    | groupExpr
    | typeDefinition
    ;

identifierName
    : identifier
    | keywords // TODO atm we use every keyword but makes no sense
    ;

postfixExpr
    : expr1=postfixExpr ('![' | '?[' | '[') expression ']'
    | expr2=postfixExpr ('!.' | '?.' | '.') identifierName templateId?
    // TODO slice operator
    | expr3=postfixExpr '[' expression ':' expression ']'
    | expr4=postfixExpr '.' '.' primaryExpr
    // function call
    | expr5=postfixExpr '(' arguments=argumentExprList? ')'
    //| postfixExpr '.' '#' identifier '(' preprocessorMacroCallArgumentList? ')'
    | expr6='#' // reserved this inthe for later use!
    | expr7=postfixExpr ( '++' | '--' )+
    | primaryExpr
    ;

namedArgument
    : name=identifier '=' expr=conditionalExpr
    ;

positionalArgument
    : conditionalExpr| anonymousfunctionDecl
    ;


argumentExprList
  : (namedArgument | positionalArgument) (',' (namedArgument | positionalArgument))*
  // TODO
  // send as object
  // expand from struct / variable
  // use spread operator? ...b ...{}
  // a(@b) a(@{a: 1, b: 2})
  ;


// NOTE:  there is no sizeof operator, it's a property of each type
unaryExpr
    // defined at memory-management
    : unaryNewExpression
    // defined at memory-management
    | unaryDeleteExpression
    // defined at memory-management
    | unaryCloneExpression
    // defined at unit-test
    | unaryMockExpr
    // NOTE multiple inc/dec makes no sense!
    | op=unaryOperators operand=postfixExpr
    | operand=postfixExpr
    ;

unaryOperators
    // TODO REVIEW '@' | '&' | '*'
    :  '+' | '-' | '~' | '!' | 'not' | '++' |  '--'
    ;

castExpr
    :   'cast' left=unaryExpr // TODO, syntax not  final
    |   left=unaryExpr
    ;

multiplicativeExpr
    :   left=multiplicativeExpr op=multiplicativeOperators right=castExpr
    |   right=castExpr
    ;

multiplicativeOperators
    : '*'
    | '/'
    | '%'
    ;

additiveExpr
    : left=additiveExpr op=additiveOperators right=multiplicativeExpr
    | right=multiplicativeExpr
    ;

additiveOperators
    : '+'
    | '-'
    ;

shiftExpr
    : left=shiftExpr op=shiftOperators right=additiveExpr
    | right=additiveExpr
    ;

shiftOperators
    : '<' '<'
    | '>' '>'
    ;

relationalExpr
    : left=relationalExpr op=relational_operators right=shiftExpr
    | right=shiftExpr
    ;

relational_operators
    : '<'
    | '>'
    | '<='
    | '>='
    ;

equalityExpr
    : left=equalityExpr op=equality_operators right=relationalExpr
    | right=relationalExpr
    ;

equality_operators: type_equality_operators | value_equality_operators;

type_equality_operators
  : 'is'
  | 'extends'
  | 'implements'
  | 'instanceof'
  ;

value_equality_operators
    // memory equality (pointer comparation)
    : '==='
    // memory inequality (pointer comparation)
    | '!=='
    // floating point equality: abs(left - right) < epsilon
    | '~='
    // value equality
    | '=='
    // value inequality
    | '!='
    | '<' '>'
    ;

andExpr
    :   left=equalityExpr ( op='&' right=andExpr)?
    ;

exclusiveOrExpr
    :   left=andExpr (op='^' right=exclusiveOrExpr)?
    ;

inclusiveOrExpr
    :   left=exclusiveOrExpr (op='|' right=inclusiveOrExpr)?
    ;

logicalAndExpr
    :   left=inclusiveOrExpr (op=('&&' | 'and') right=logicalAndExpr)?
    ;

logicalOrExpr
    :   left=logicalAndExpr (op=('||' |'or') right=logicalOrExpr)?
    ;

conditionalExpr
    :   condition=logicalOrExpr ('?' true_expr=expression ':' false_expr=conditionalExpr)?
    ;

assignmentExpr
    //:   conditionalExpr
    : errorHandlingExprs
    | conditionalExpr
    | left=unaryExpr op=assignment_operator right=assignmentExpr
    ;

assignment_operator
    :   '=' | '*=' | '/=' | '%=' | '+=' | '-=' | '<<=' | '>>=' | '&=' | '^=' | '|='
    ;

expression
    // NOTE comma operator is removed on purpose.
    : assignmentExpr
    ;

expressionList
    : expression (',' expression)*
    ;

rhsExpr
  : errorHandlingExprs
  | conditionalExpr
  | tokenizeExpr
  | anonymousfunctionDecl
  ;
// NOTE comma is not an operator!
operators
  : assignment_operator
  | relational_operators
  | value_equality_operators
  | '||'
  | '&&'
  | '|'
  | '^'
  | '&'
  | '<' '<' | '>' '>'
  | '-' | '+'
  | multiplicativeOperators
  | '<' | '>'
  | unaryOperators
  | '--' | '++'
  ;


//file: spec\language\functions.md

returnStmt
  : 'return' expr=rhsExpr?
  ;

deferStmt
  : 'defer' rhsExpr
  ;

functionDecl
  : (functionDef | anonymousFunctionDef) blockStmt
  ;

anonymousfunctionDecl
  : anonymousFunctionDef blockStmt
  ;

functionModifiers
  : 'pure'
  | 'override'
  | 'overwrite'
  | 'readonly' // struct-readonly
  | 'inline'
  ;

functionReturnTypeModifiers
  : 'lend'
  // | 'own'
  | 'uninitialized'
  ;

anonymousFunctionDef
  : functionModifiers* 'function' templateDefinition? '(' functionParameterList? ')' functionReturnTypeModifiers* typeDefinition?
  ;

// list of valid functions names, may include keywords
functionName
  : identifier
  | 'autocast'
  ;

functionDef
  : function_mod=functionModifiers* 'function' name=functionName template_def=templateDefinition? '(' functionParameterList? ')' return_type_modifiers=functionReturnTypeModifiers* return_type=typeDefinition?
  ;

memoryFunctionDecl
  : memoryFunctionDef blockStmt
  ;

memoryFunctionDef
  : ('override' | 'overwrite')* ('new'|'delete'|'clone') '(' functionParameterList? ')'
  ;

operatorFunctionDecl
  : functionModifiers* operatorFunctionDef blockStmt
  ;

operatorFunctionDef
  : 'operator' overloadableOperators templateDefinition? '(' functionParameterList? ')' functionReturnTypeModifiers* typeDefinition?
  ;

overloadableOperators
  // Binary arithmetic operators
  : '+'
  | '-'
  | '*'
  | '/'
  | '^'
  | '%'
  // pointer get address
  | '@'
  // shift
  | '<' '<'
  | '>' '>'
  // AND
  | '&'
  // OR
  | '|'
  // XOR
  | '^'
  // Assignament operators
  | '='
  | '+='
  | '-='
  | '*='
  | '/='
  | '^='
  | '~='
  // Array subscript operator
  | '[' ']' '='
  | '[' ']'
  // Member access
  | '.'
  | '?.'
  // Comparison operators
  | '<'
  | '>'
  | '<='
  | '>='
  | '=='
  | '!='
  // function call
  | '(' ')'
  // unary
  | '~'
  | '!'
  ;


functionBodyStmtList
  : functionBodyStmt+
  ;

// label + single stmt
// label + block
labeledStatement
  : identifier ':' (functionBodyStmt | endOfStmt* blockStmt)
  ;

globalImportVar
  : 'global' identifier (',' identifier)* endOfStmt
  ;

blockStmt
  : '{' functionBodyStmtList? '}'
  ;

functionBodyStmt
  : (labeledStatement
  | globalImportVar
  | blockStmt
  | comments
  | aliasDeclStmt
  | typeDecl
  | functionDecl
  | selectionStmts
  // function exclusive
  | returnStmt
  | deferStmt
  | blockVariableDeclStmt
  | errorHandlingStmts
  | retryUntilWhileStmt
  // preprocessor
  | preprocessorStmts
  // expression at the bottom to fix some preprocessor issues
  | expression)? endOfStmt
  ;

functionParameterList
  : functionParameter (',' functionParameter)*
  ;

functionParametersTypeModifiers
  : 'lend'
  | 'own'
  | 'uninitialized' // why ? <-- real usage ?
  | 'autocast'
  | 'out'
  ;

functionParameter
  : functionParametersTypeModifiers* typeDefinition identifier ('=' rhsExpr)?
  ;

//file: spec\language\identifiers.md

identifier: Identifier;

dollarIdentifier
  : '$' identifier
  ;

dollarIdentifierList
  : dollarIdentifier (',' dollarIdentifier)*
  ;

//file: spec\language\introspection.md

//file: spec\language\literals.md
stringLiteral
  : STRING_LITERAL
  ;

numberLiteral
  : ('-')? Number
  ;

anyNonNewLine
  : ',' // NON_NEW_LINE+
  ;

//file: spec\language\program.md

// main program entry point!
program
  : preprocessors=preprocessorProgramStmtList? imports=importStmtList? statements=programStmsList? EOF
  ;

programStmsList
  : programStmt+
  ;

programStmt
  : comments endOfStmt
  | preprocessorDecl endOfStmt
  | preprocessorStmts endOfStmt
  | functionDecl endOfStmt
  | operatorFunctionDecl endOfStmt
  // program exclusive!
  | typeDecl endOfStmt
  | globalVariableDeclStmt endOfStmt
  | fileVariableDeclStmt endOfStmt
  | testStmt endOfStmt
  | endOfStmt
  ;

comments
  : SINGLE_LINE_COMMENT
  | BLOCK_COMMENT
  ;

endOfStmt: (comments? (NEWLINE_WIN_TK | NEWLINE_LINUX_TK | SEMICOLON_TK))+;

ws: (comments | NEWLINE_WIN_TK | NEWLINE_LINUX_TK)+;


//file: spec\language\reflection.md

//file: spec\language\regular-expressions.md

//file: spec\language\tokens.md

//file: spec\language\type-system.md
primitive
  : 'self' | 'any' | 'type'
  ;

typeModifiers
  : 'readonly'
  | 'lend'
  | 'own'
  | 'uninitialized'
  ;

type
  : typeModifiers* (primitive | dollarIdentifier | identifier)
  ;

templateDefinition
  : '<' templateParameter (',' templateParameter)* '>'
  ;

templateId
  : '<' templateArgument (',' templateArgument)* '>'
  ;

templateArgument
  : typeDefinition
  | dollarIdentifier
  ;

// TODO semantic error if a tempalte is inside a template...
templateParameter
  : typeDefinition ( templateIs | templateExtends | templateImplements )*
  ;

templateIs
  : 'is' (typeDefinition | 'struct' | 'enum' | 'mask' | 'function')
  ;

templateExtends
  : 'extends' (primitive | identifier | templateTypeDef)
  ;

templateImplements
  : 'implements' (identifier | templateTypeDef)
  ;

typeDefinitionList
  : typeDefinition (',' typeDefinition)*
  ;

typeDefinition
  : typeModifiers* typeLocator (optional='?')?
  ;

typeLocator
  : loc1=typeLocator '.' (typeLocator | 'type')
  | loc2=typeLocator templateId
  | loc3=typeLocator '[' argumentExprList? ']'
  | loc4=stringLiteral
  | loc5=primitive
  | loc6=dollarIdentifier
  | loc7=identifier
  ;


templateTypeDef
  : type templateId
  ;

aggregateTypeAndDecl
    :   typeDefinition (('&' | 'and') aggregateTypeAndDecl)*
    ;

aggregateTypeOrDecl
    :   aggregateTypeAndDecl (('|' |'or') aggregateTypeOrDecl)*
    ;

aggregateTypeDecl
  : aggregateTypeOrDecl
  ;

aliasTypeDecl
  : typeDefinition
  ;

typeDecl
  // types that support templates
  : 'type' name=identifier templateDefinition? '=' (structTypeDecl | interfaceTypeDecl | anonymousFunctionDef | aggregateTypeDecl | aliasTypeDecl)
  // types that DON'T support templates
  | 'type' name=identifier '=' (enumTypeDecl | maskTypeDecl)
  ;

//file: spec\language\types\array.md
arrayItem
  : rhsExpr
  ;

arrayItemList
  : arrayItem (',' arrayItemList)?
  ;

arrayInitializer
  : '[' arrayItemList? ','? ']'
  ;

// TODO
arrayConstantInitializer
  : '[' arrayItemList ','? ']'
  ;

//file: spec\language\types\date.md

//file: spec\language\types\enumerated.md
enumTypeDecl
  : 'enum' primitive? '{' endOfStmt? enumeratorList? '}'
  ;

enumeratorList
  : (enumerator endOfStmt)+
  | comments
  ;

enumerator
  : name=identifier ('=' value=logicalOrExpr)?
  ;

maskTypeDecl
  : 'mask' primitive? '{' endOfStmt? maskEnumeratorList? '}'
  ;

maskEnumeratorList
  : (maskEnumerator endOfStmt)+
  | comments
  ;

maskEnumerator
  : identifier ('=' logicalOrExpr)
  ;

//file: spec\language\types\error.md

//file: spec\language\types\interface.md
interfaceTypeDecl
  : 'interface' (typeExtendsDecl)* '{' endOfStmt? interfaceProperty* '}'
  ;

interfacePropertyDecl
  // TODO keep assignament ? it clash with the redefined one ?
  // TODO constrains to not initialize again ?
  : (structPropertyModifiers)* typeDefinition identifier ('=' (constant | arrayConstantInitializer | structConstantInitializer))?
  | propertyAlias
  | functionDef
  | memoryFunctionDef
  | operatorFunctionDef
  | structGetterDef
  | structSetterDef
  ;

interfaceProperty
  : interfacePropertyDecl endOfStmt
  | comments endOfStmt
  ;

//file: spec\language\types\numeric-arithmetic.md

//file: spec\language\types\path.md

//file: spec\language\types\pointers.md

//file: spec\language\types\string.md

//file: spec\language\types\structured.md
structTypeDecl
  : ('noalign' | 'lean')* 'struct' (typeExtendsDecl | typeImplementsDecl)* '{' structProperty* '}'
  ;

typeExtendsDecl
  : 'extends' typeDefinition
  ;

typeImplementsDecl
  : 'implements' typeDefinition
  ;

structProperty
  : (comments | structPropertyDecl)? endOfStmt
  ;

structPropertyDecl
  //: (structPropertyModifiers)* typeDefinition identifierName ('=' (constant | arrayConstantInitializer | structConstantInitializer))?
  : (structPropertyModifiers)* typeDefinition identifierName ('=' rhsExpr)?
  // TODO REVIEW aliasing operator?
  | propertyAlias
  | functionDecl
  | memoryFunctionDecl
  | operatorFunctionDecl
  | structGetterDecl
  | structSetterDecl
  ;

// TODO do not repeat at parser level ?
structPropertyModifiers
  : 'own'
  | 'hoist'
  | 'readonly'
  | 'static'
  ;

propertyAlias
  : 'alias' identifier identifier
  ;

structGetterDecl
  : structGetterDef blockStmt
  ;

structGetterDef
  : 'get' typeDefinition identifierName
  ;

structSetterDecl
  : structSetterDef blockStmt
  ;

structSetterDef
  : 'set' identifierName '(' typeDefinition identifier ')'
  ;

structInitializer
  : typeDefinition? '{' structProperyInitializerList? '}' #    cStructInitializer
  | typeDefinition? '{' jsonInitializerList '}'                           # jsonStructInitializer
  ;

structProperyInitializerList
  : structProperyInitializer (',' structProperyInitializer)*
  | endOfStmt+
  ;

structProperyInitializer
  : (locator=typeLocator ('='|':'))? value=rhsExpr
  ;

jsonInitializerList
  : jsonInitializerPair (',' jsonInitializerPair)* ','?
  ;

jsonInitializerPair
  : stringLiteral ':' constant
  ;

structConstantInitializer
  : '{' structProperyInitializerList? '}'
  ;

//file: spec\language\unit-testing.md
testStmt
  // TODO functionLocator?
  : 'test' (stringLiteral | identifier) testBlockStatement
  ;

testBlockStatement
  : '{' testBodyStmtList? '}'
  ;

testBodyStmtList
  : (testStmt | functionBodyStmt)+
  ;


unaryMockExpr
  // TODO functionLocator?
  : 'mock' postfixExpr
  ;

//file: spec\language\variables.md
globalVariableDeclStmt
  // infer variable with initialization
  : 'global' fileVariableDeclStmt
  ;

packageVariableDeclStmt
  // infer variable with initialization
  : 'package' fileVariableDeclStmt
  ;

// infer variable declaration w/out initialization
inferVariableDeclStmt
  : ('var' | 'const' | 'readonly') identifier ('=' rhsExpr)?
  ;

// typed variable declaration and initialization
typedVariableDeclStmt
  : ('var' | 'const' | 'readonly') typeDefinition identifier ('(' argumentExprList? ')' | '=' rhsExpr)?
  ;

fileVariableDeclStmt
  : inferVariableDeclStmt
  | typedVariableDeclStmt
  ;

blockVariableDeclStmt
  : inferVariableDeclStmt
  | typedVariableDeclStmt
  ;

aliasDeclStmt
  : 'alias' identifier '=' identifier
  ;

//file: spec\memory-management.md
allocator
  : 'at' identifier
  ;

defaultNewExpression
  : typeDefinition? allocator?
  ;

initializedNewExpression
  : typeDefinition? ('(' argumentExprList? ')')+ allocator?
  ;

arrayNewExpression
  : typeDefinition? ('[' rhsExpr ']')+ ('(' argumentExprList? ')')? allocator?
  ;

unaryNewExpression
  : 'new' ( defaultNewExpression | initializedNewExpression | arrayNewExpression );

unaryDeleteExpression
  : 'delete' postfixExpr
  ;

unaryCloneExpression
  : 'clone' postfixExpr;

//file: spec\meta\syntax.logia

//file: spec\package-system.md
// package entry point!
packageProgram
  : (comments endOfStmt?)* packageDefinitionStmt importStmtList? preprocessorProgramStmtList? packageStmsList? EOF
  ;


packageStmsList
  : packageStmts+
  ;

packageStmts
  : comments endOfStmt
  | preprocessorDecl endOfStmt
  | preprocessorStmts endOfStmt
  | functionDecl endOfStmt
  | operatorFunctionDecl endOfStmt
  // program exclusive!
  | typeDecl endOfStmt
  | packageVariableDeclStmt endOfStmt
  | fileVariableDeclStmt endOfStmt
  | testStmt endOfStmt
  | endOfStmt
  ;



packageDefinitionStmt
  : 'package' name=packageName version=stringLiteral endOfStmt
  ;


packageName
  : packageName ('.' (identifier | '*'))
  | identifier
  ;

importStmt
  : 'import' location=packageName version=stringLiteral? ('as' name=identifier)? endOfStmt
  | endOfStmt
  ;

importStmtList
  : importStmt+
  ;

//file: spec\preprocessor-and-metaprogramming.md

preprocessorProgramStmtList
  : preprocessorProgramStmt+
  ;

preprocessorProgramStmt
  : preprocessorSetStatement
  | endOfStmt
  ;

preprocessorExpr
  : preprocessorStr
  | preprocessorEcho
  | preprocessorCallExpr
  // PROPOSSAL: | preprocessorRepeatExpr
  ;

preprocessorStr
  : '##' identifier '#' // TODO why can't we use identifierUp here ?
  ;

preprocessorEcho
  : '#' (identifier | 'function') '#'
  ;

preprocessorCallExpr
  // TODO eos sensible
  : '#' postfixExpr ('(' preprocessorCallArgumentsList? ')')? '{' tokenList '}'
  | '#' postfixExpr ('(' preprocessorCallArgumentsList? ')')
  ;

preprocessorCallArgumentsList
  : tokenListNoComma (',' tokenListNoComma)*
  ;

tokenListNoComma
  : (groupTokens | isolatedTokenListNoComma)+  tokenListNoComma?
  ;

tokenList
  : (groupTokens | isolatedTokenList)+ tokenList?
  ;

// inside a group "," is allowed
groupTokens
  : '(' tokenList?  ')'
  | '{' tokenList? '}'
  ;

isolatedTokenListNoComma
  : ~(',' | '{' | '}' | '(' | ')')
  ;

isolatedTokenList
  : ~('{' | '}' | '(' | ')')
  ;

preprocessorStmts
  : preprocessorIfStmt
  | preprocessorLoopStmt
  ;

tokenizeExpr
  : 'tokenize' '{' tokenList '}'
  ;



preprocessorIfStmt
  : '#' ifStmt
  ;


preprocessorDecl
  : '#' 'function' identifier '(' preprocessorMacroArgumentList? ')' typeDefinition preprocessorBody
  ;

preprocessorMacroArgumentList
  : typeDefinition identifier (',' preprocessorMacroArgumentList)*
  ;

preprocessorBody
  : endOfStmt? blockStmt
  ;


forargsStmt
  : '#forargs' identifier ',' identifier blockStmt
  ;


preprocessorLoopStmt
  : '#' 'loop' identifier ',' identifier 'in' expression blockStmt
  ;


execStmt
  : '#exec' anyNonNewLine
  ;


identifierList
    : identifier? (',' identifier)*
    ;

preprocessorRepeatExpr
  : '#repeat' '(' identifierList ')'
  ;
